/*
 * Copyright (c) 2025 HiSilicon (Shanghai) Technologies Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "display_gralloc.h"
#include <inttypes.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <securec.h>
#include "display_gralloc_internal.h"

#define MEM_USAGE_MASK (HBM_USE_MEM_MMZ | HBM_USE_MEM_SHARE | HBM_USE_MEM_MMZ_CACHE)

typedef struct {
    uint32_t numPlanes;
    uint32_t radio[MAX_PLANES];
} PlaneLayoutInfo;

typedef struct {
    uint32_t format;
    uint32_t bitsPerPixel; // bits per pixel for first plane
    const PlaneLayoutInfo *planes;
} FormatInfo;

static const PlaneLayoutInfo YUV420SP_LAYOUT = {
    .numPlanes = 2,
    .radio = { 4, 2 },
};

static const PlaneLayoutInfo YUV420_P_LAYOUT = {
    .numPlanes = 3,
    .radio = { 4, 1, 1 },
};

static const FormatInfo *GetFormatInfo(uint32_t format)
{
    static const FormatInfo fmtInfos[] = {
        {PIXEL_FMT_RGBX_8888,  32, NULL},  {PIXEL_FMT_RGBA_8888, 32,  NULL},
        {PIXEL_FMT_BGRX_8888,  32, NULL},  {PIXEL_FMT_BGRA_8888, 32,  NULL},
        {PIXEL_FMT_RGB_888,    24, NULL},  {PIXEL_FMT_BGR_565,   16,  NULL},
        {PIXEL_FMT_RGBA_5551,  16, NULL},  {PIXEL_FMT_RGB_565,   16,  NULL},
        {PIXEL_FMT_BGRX_4444,  16, NULL},  {PIXEL_FMT_BGRA_4444, 16,  NULL},
        {PIXEL_FMT_RGBA_4444,  16, NULL},  {PIXEL_FMT_RGBX_4444, 16,  NULL},
        {PIXEL_FMT_BGRX_5551,  16, NULL},  {PIXEL_FMT_BGRA_5551, 16,  NULL},
        {PIXEL_FMT_YCBCR_420_SP, 8, &YUV420SP_LAYOUT}, {PIXEL_FMT_YCRCB_420_SP, 8, &YUV420SP_LAYOUT},
        {PIXEL_FMT_YCBCR_420_P, 8, &YUV420_P_LAYOUT}, {PIXEL_FMT_YCRCB_420_P, 8, &YUV420_P_LAYOUT},
    };

    for (uint32_t i = 0; i < sizeof(fmtInfos) / sizeof(FormatInfo); i++) {
        if (fmtInfos[i].format == format) {
            return &fmtInfos[i];
        }
    }
    HDF_LOGE("the format can not support %d %d", format, PIXEL_FMT_RGBA_8888);
    return NULL;
}

static uint32_t AdjustStrideFromFormat(uint32_t format, uint32_t width)
{
    const FormatInfo *fmtInfo = GetFormatInfo(format);

    if ((fmtInfo != NULL) && (fmtInfo->planes != NULL)) {
        uint32_t sum = fmtInfo->planes->radio[0];
        for (uint32_t i = 1; (i < fmtInfo->planes->numPlanes) && (i < MAX_PLANES); i++) {
            sum += fmtInfo->planes->radio[i];
        }
        if (sum > 0) {
            return DIV_ROUND_UP((width * sum), fmtInfo->planes->radio[0]);
        }
    }
    return width;
}

static int32_t InitBufferHandle(PriBufferHandle* buffer, const AllocInfo* info)
{
    if ((info->usage & HBM_USE_ASSIGN_SIZE) == HBM_USE_ASSIGN_SIZE) {
        if (info->expectedSize <= 0) {
            HDF_LOGE("%s: alloc info is invalid, size is %d", __func__, info->expectedSize);
            return DISPLAY_FAILURE;
        }
        buffer->hdl.size = info->expectedSize >= INT32_MAX ? INT32_MAX : info->expectedSize;
    } else {
        const FormatInfo *fmtInfo = GetFormatInfo(info->format);
        if (fmtInfo == NULL) {
            HDF_LOGE("%s: can not get format information : %d", __func__, buffer->hdl.format);
            return DISPLAY_FAILURE;
        }

        int32_t stride = ALIGN_UP((int32_t)AdjustStrideFromFormat(info->format, info->width), WIDTH_ALIGN) *
            fmtInfo->bitsPerPixel / BITS_PER_BYTE;
        int32_t size = stride * ALIGN_UP(info->height, HEIGHT_ALIGN);
        buffer->hdl.width = info->width > INT32_MAX ? INT32_MAX : info->width;
        buffer->hdl.stride = stride;
        buffer->hdl.height = info->height > INT32_MAX ? INT32_MAX : info->height;
        buffer->hdl.size = size;
    }
    if ((buffer->hdl.size > MAX_MALLOC_SIZE) || (buffer->hdl.size == 0)) {
        HDF_LOGE("%s: size is invalid %d ", __func__, buffer->hdl.size);
        return DISPLAY_FAILURE;
    }

    buffer->hdl.usage = info->usage;
    buffer->hdl.fd = -1;
    buffer->shmid = INVALID_SHMID;
    buffer->hdl.format = info->format;
    buffer->hdl.reserveInts = (uint32_t)((sizeof(PriBufferHandle) - sizeof(BufferHandle) -
        buffer->hdl.reserveFds * sizeof(uint32_t)) / sizeof(uint32_t));
    return DISPLAY_SUCCESS;
}

static int32_t AllocShm(BufferHandle *buffer)
{
    static int32_t key = SHM_START_KEY;
    int32_t shmid;

    uint32_t flag = IPC_CREAT | IPC_EXCL | DEFAULT_READ_WRITE_PERMISSIONS;
    int32_t flagInt32 = flag > INT32_MAX ? INT32_MAX : (int32_t)flag;
    while ((shmid = shmget(key, buffer->size, flagInt32)) < 0) {
        if (errno != EEXIST) {
            HDF_LOGE("%s: fail to alloc the shared memory, errno = %d", __func__, errno);
            return DISPLAY_FAILURE;
        }
        key++;
        if (key >= SHM_MAX_KEY) {
            key = SHM_START_KEY;
        }
    }
    void *pBase = shmat(shmid, NULL, 0);
    if (pBase == ((void *)-1)) {
        HDF_LOGE("%s: Fail to attach the shared memory, errno = %d", __func__, errno);
        if (shmctl(shmid, IPC_RMID, 0) == -1) {
            HDF_LOGE("%s: Fail to free shmid, errno = %d", __func__, errno);
        }
        return DISPLAY_FAILURE;
    }
    buffer->virAddr = pBase;
    buffer->fd = key;
    PriBufferHandle* buf = (PriBufferHandle*)buffer;
    buf->shmid = shmid;
    key++;
    if (key >= SHM_MAX_KEY) {
        key = SHM_START_KEY;
    }
    return DISPLAY_SUCCESS;
}

static void FreeShm(BufferHandle *buffer)
{
    CHECK_NULLPOINTER_RETURN(buffer->virAddr);
    if (shmdt(buffer->virAddr) == -1) {
        HDF_LOGE("%s: Fail to free shared memory, errno = %d", __func__, errno);
    }
    PriBufferHandle* buf = (PriBufferHandle*)buffer;
    if (shmctl(buf->shmid, IPC_RMID, 0) == -1) {
        HDF_LOGE("%s: Fail to free shmid, errno = %d", __func__, errno);
    }
}

static void *MmapShm(BufferHandle *buffer)
{
    int32_t shmid;
#ifndef __LITEOS__
    uint32_t shmflg = IPC_EXCL;
#else
    uint32_t shmflg = IPC_CREAT;
#endif
    shmid = shmget(buffer->fd, buffer->size, shmflg | DEFAULT_READ_WRITE_PERMISSIONS);
    if (shmid < 0) {
        HDF_LOGE("%s: Fail to mmap the shared memory, errno = %d", __func__, errno);
        return NULL;
    }
    void *pBase = shmat(shmid, NULL, 0);
    if (pBase == ((void *)-1)) {
        HDF_LOGE("%s: Fail to attach the shared memory, errno = %d", __func__, errno);
        return NULL;
    }
    PriBufferHandle* buf = (PriBufferHandle*)buffer;
    buf->shmid = shmid;
    buffer->virAddr = pBase;
    return pBase;
}

static int32_t UnmapShm(BufferHandle *buffer)
{
    if (shmdt(buffer->virAddr) == -1) {
        HDF_LOGE("%s: Fail to unmap shared memory errno =  %d", __func__, errno);
        return DISPLAY_FAILURE;
    }
    return DISPLAY_SUCCESS;
}

static int32_t AllocMmz(BufferHandle *buffer)
{
    int32_t ret;
    td_phys_addr_t phyAddr = 0;
    void *virAddr = NULL;

    switch (buffer->usage & MEM_USAGE_MASK) {
        case HBM_USE_MEM_MMZ_CACHE:
            ret = ss_mpi_sys_mmz_alloc_cached(&phyAddr, &virAddr, NULL, NULL, buffer->size);
            break;
        case HBM_USE_MEM_MMZ:
            ret = ss_mpi_sys_mmz_alloc(&phyAddr, &virAddr, NULL, NULL, buffer->size);
            break;
        default:
            HDF_LOGE("%s: not support memory usage: 0x%" PRIx64 "", __func__, buffer->usage);
            return DISPLAY_NOT_SUPPORT;
    }
    if (ret != DISPLAY_SUCCESS) {
        HDF_LOGE("%s: mmzalloc failure, usage = 0x%" PRIx64 ", ret 0x%x", __func__,
            buffer->usage, ret);
        return DISPLAY_FAILURE;
    }

    ret = ShareMemory(phyAddr, buffer);
    if (ret != DISPLAY_SUCCESS) {
        return DISPLAY_FAILURE;
    }

    (void)memset_s(virAddr, buffer->size, 0x0, buffer->size);
    buffer->phyAddr = (uint64_t)phyAddr;
    buffer->virAddr = virAddr;
    return DISPLAY_SUCCESS;
}

static int32_t AllocMem(const AllocInfo* info, BufferHandle **buffer)
{
    int32_t ret;
    BufferHandle *bufferHdl = NULL;
    DISPLAY_CHK_RETURN((buffer == NULL), DISPLAY_NULL_PTR, HDF_LOGE("%s: in buffer is null", __func__));
    DISPLAY_CHK_RETURN((info == NULL), DISPLAY_NULL_PTR, HDF_LOGE("%s: in info is null", __func__));
    PriBufferHandle* priBuffer = calloc(1, sizeof(PriBufferHandle));
    DISPLAY_CHK_RETURN((priBuffer == NULL), DISPLAY_NULL_PTR, HDF_LOGE("%s: can not calloc errno : %d",
        __func__, errno));
    ret = InitBufferHandle(priBuffer, info);
    DISPLAY_CHK_GOTO((ret != DISPLAY_SUCCESS), out, HDF_LOGE("%s: can not init buffe handle", __func__));
    bufferHdl = &priBuffer->hdl;

    switch (bufferHdl->usage & MEM_USAGE_MASK) {
        case HBM_USE_MEM_MMZ_CACHE:
        case HBM_USE_MEM_MMZ:
            ret = AllocMmz(bufferHdl);
            break;
        case HBM_USE_MEM_SHARE:
            ret = AllocShm(bufferHdl);
            break;
        default:
            HDF_LOGE("%s: not support memory usage: 0x%" PRIx64 "", __func__, bufferHdl->usage);
            ret = DISPLAY_NOT_SUPPORT;
    }

out:
    if ((ret != DISPLAY_SUCCESS) && (priBuffer != NULL)) {
        free(priBuffer);
        priBuffer = NULL;
        bufferHdl = NULL;
    }
    *buffer = bufferHdl;
    return ret;
}

static void FreeMem(BufferHandle *buffer)
{
    int32_t ret;
    CHECK_NULLPOINTER_RETURN(buffer);
    if ((buffer->size > MAX_MALLOC_SIZE) || (buffer->size == 0)) {
        HDF_LOGE("%s: size is invalid, buffer->size = %d", __func__, buffer->size);
        return;
    }
    switch (buffer->usage & MEM_USAGE_MASK) {
        case HBM_USE_MEM_MMZ_CACHE:
        case HBM_USE_MEM_MMZ:
            ret = UnShareMemory(buffer);
            if (ret != DISPLAY_SUCCESS) {
                HDF_LOGE("%s: UnShareMemory failure, ret 0x%x", __func__, ret);
                return;
            }
            ret = ss_mpi_sys_mmz_free(buffer->phyAddr, buffer->virAddr);
            if (ret != DISPLAY_SUCCESS) {
                HDF_LOGE("%s: ss_mpi_sys_mmz_free failure, ret 0x%x", __func__, ret);
            }
            break;
        case HBM_USE_MEM_SHARE:
            FreeShm(buffer);
            break;
        default:
            HDF_LOGE("%s: not support memory usage: 0x%" PRIx64 "", __func__, buffer->usage);
            break;
    }
    free(buffer);
}

static void *Mmap(BufferHandle *buffer)
{
    CHECK_NULLPOINTER_RETURN_VALUE(buffer, NULL);
    if ((buffer->size > MAX_MALLOC_SIZE) || (buffer->size == 0)) {
        HDF_LOGE("%s: size is invalid, buffer->size = %d", __func__, buffer->size);
        return NULL;
    }
    switch (buffer->usage & MEM_USAGE_MASK) {
        case HBM_USE_MEM_MMZ_CACHE:
        case HBM_USE_MEM_MMZ:
            return ss_mpi_sys_mmap(buffer->phyAddr, buffer->size);
        case HBM_USE_MEM_SHARE:
            return MmapShm(buffer);
        default:
            HDF_LOGE("%s: not support memory usage: 0x%" PRIx64 "", __func__, buffer->usage);
            break;
    }
    return NULL;
}

static void *MmapCache(BufferHandle *buffer)
{
    CHECK_NULLPOINTER_RETURN_VALUE(buffer, NULL);
    if ((buffer->size > MAX_MALLOC_SIZE) || (buffer->size == 0)) {
        HDF_LOGE("%s: size is invalid, buffer->size = %d", __func__, buffer->size);
        return NULL;
    }
    if ((buffer->usage & MEM_USAGE_MASK) == HBM_USE_MEM_MMZ_CACHE) {
        return ss_mpi_sys_mmap_cached((uint64_t)buffer->phyAddr, buffer->size);
    } else {
        HDF_LOGE("%s: buffer usage error, buffer->usage = 0x%" PRIx64 "", __func__, buffer->usage);
        return NULL;
    }
}

static int32_t Unmap(BufferHandle *buffer)
{
    int32_t ret;
    CHECK_NULLPOINTER_RETURN_VALUE(buffer, DISPLAY_NULL_PTR);
    CHECK_NULLPOINTER_RETURN_VALUE(buffer->virAddr, DISPLAY_NULL_PTR);
    if ((buffer->size > MAX_MALLOC_SIZE) || (buffer->size == 0)) {
        HDF_LOGE("%s: size is invalid, buffer->size = %d", __func__, buffer->size);
        return DISPLAY_FAILURE;
    }
    switch (buffer->usage & MEM_USAGE_MASK) {
        case HBM_USE_MEM_MMZ_CACHE:
        case HBM_USE_MEM_MMZ:
            ret = ss_mpi_sys_munmap(buffer->virAddr, buffer->size);
            break;
        case  HBM_USE_MEM_SHARE:
            ret = UnmapShm(buffer);
            break;
        default:
            HDF_LOGE("%s: not support memory usage: 0x%" PRIx64 "", __func__, buffer->usage);
            ret = DISPLAY_FAILURE;
            break;
    }
    return ret;
}

static int32_t FlushCache(BufferHandle *buffer)
{
    CHECK_NULLPOINTER_RETURN_VALUE(buffer, DISPLAY_NULL_PTR);
    CHECK_NULLPOINTER_RETURN_VALUE(buffer->virAddr, DISPLAY_NULL_PTR);
    if ((buffer->size > MAX_MALLOC_SIZE) || (buffer->size == 0)) {
        HDF_LOGE("%s: size is invalid, buffer->size = %d", __func__, buffer->size);
        return DISPLAY_FAILURE;
    }
    if ((buffer->usage & MEM_USAGE_MASK) == HBM_USE_MEM_MMZ_CACHE) {
        int32_t ret = ss_mpi_sys_flush_cache(buffer->phyAddr, buffer->virAddr, buffer->size);
        if (ret != DISPLAY_SUCCESS) {
            HDF_LOGE("%s: ss_mpi_sys_flush_cache failure, ret 0x%x", __func__, ret);
            return DISPLAY_FAILURE;
        }
    } else {
        HDF_LOGE("%s: buffer usage error, usage = 0x%" PRIx64"", __func__, buffer->usage);
        return DISPLAY_FAILURE;
    }
    return DISPLAY_SUCCESS;
}

static int32_t FlushMCache(BufferHandle *buffer)
{
    CHECK_NULLPOINTER_RETURN_VALUE(buffer, DISPLAY_NULL_PTR);
    CHECK_NULLPOINTER_RETURN_VALUE(buffer->virAddr, DISPLAY_NULL_PTR);
    if ((buffer->size > MAX_MALLOC_SIZE) || (buffer->size == 0)) {
        HDF_LOGE("%s: size is invalid, buffer->size = %d", __func__, buffer->size);
        return DISPLAY_FAILURE;
    }
    int32_t ret = ss_mpi_sys_flush_cache(buffer->phyAddr, buffer->virAddr, buffer->size);
    if (ret != DISPLAY_SUCCESS) {
        HDF_LOGE("%s: ss_mpi_sys_flush_cache failure, ret 0x%x", __func__, ret);
        return DISPLAY_FAILURE;
    }
    return DISPLAY_SUCCESS;
}

int32_t GrallocInitialize(GrallocFuncs **funcs)
{
    CHECK_NULLPOINTER_RETURN_VALUE(funcs, DISPLAY_NULL_PTR);
    GrallocFuncs *gFuncs = (GrallocFuncs *)malloc(sizeof(GrallocFuncs));
    if (gFuncs == NULL) {
        HDF_LOGE("%s: gFuncs is null", __func__);
        return DISPLAY_NULL_PTR;
    }
    (void)memset_s(gFuncs, sizeof(GrallocFuncs), 0, sizeof(GrallocFuncs));
    gFuncs->AllocMem = AllocMem;
    gFuncs->FreeMem = FreeMem;
    gFuncs->Mmap = Mmap;
    gFuncs->MmapCache = MmapCache;
    gFuncs->Unmap = Unmap;
    gFuncs->FlushCache = FlushCache;
    gFuncs->FlushMCache = FlushMCache;
    *funcs = gFuncs;
    HDF_LOGI("%s: gralloc initialize success", __func__);
    return DISPLAY_SUCCESS;
}

int32_t GrallocUninitialize(GrallocFuncs *funcs)
{
    CHECK_NULLPOINTER_RETURN_VALUE(funcs, DISPLAY_NULL_PTR);
    free(funcs);
    HDF_LOGI("%s: gralloc uninitialize success", __func__);
    return DISPLAY_SUCCESS;
}
