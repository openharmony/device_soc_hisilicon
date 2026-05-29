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

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_COMMON_SAMPLE_COMM_VENC_BODY_PART01_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_COMMON_SAMPLE_COMM_VENC_BODY_PART01_H_


#line 1 "sample_comm_venc.c"
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <poll.h>
#include <sys/time.h>
#include <sys/select.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>
#include <signal.h>
#include <sys/prctl.h>
#include <limits.h>

#include "sample_comm.h"
#include "ot_osal.h"

#ifdef CONFIG_OT_HEIF_SUPPORT
#include "heif_format.h"
#endif
#include "ot_osal_user.h"

#define TEMP_BUF_LEN 8
#define MAX_THM_SIZE (64 * 1024)
#define QPMAP_BUF_NUM 8
#define VENC_QPMAP_MAX_CHN 2

#define SAMPLE_RETURN_CONTINUE  1
#define SAMPLE_RETURN_BREAK     2
#define SAMPLE_RETURN_NULL      3
#define SAMPLE_RETURN_GOTO      4
#define SAMPLE_RETURN_FAILURE   (-1)

typedef struct {
    td_u32 qpmap_size[VENC_QPMAP_MAX_CHN];
    td_phys_addr_t qpmap_phys_addr[VENC_QPMAP_MAX_CHN][QPMAP_BUF_NUM];
    td_void *qpmap_vir_addr[VENC_QPMAP_MAX_CHN][QPMAP_BUF_NUM];

    td_u32 skip_weight_size[VENC_QPMAP_MAX_CHN];
    td_phys_addr_t skip_weight_phys_addr[VENC_QPMAP_MAX_CHN][QPMAP_BUF_NUM];
    td_void *skip_weight_vir_addr[VENC_QPMAP_MAX_CHN][QPMAP_BUF_NUM];
} sample_comm_venc_frame_proc_info;

typedef struct {
    FILE *file[OT_VENC_MAX_CHN_NUM];
    td_s32 venc_fd[OT_VENC_MAX_CHN_NUM];
    td_s32 maxfd;
    td_u32 picture_cnt[OT_VENC_MAX_CHN_NUM];
    td_char file_name[OT_VENC_MAX_CHN_NUM][FILE_NAME_LEN];
    td_char real_file_name[OT_VENC_MAX_CHN_NUM][PATH_MAX];
    ot_venc_chn venc_chn;
    td_char file_postfix[10]; /* 10 :file_postfix number */
    td_s32 chn_total;
    td_bool save_heif;
} sample_comm_venc_stream_proc_info;

static td_s32 sample_comm_venc_get_realpath(const td_char *src_path, td_char *dst_path, size_t dst_len)
{
    td_char resolved_path[PATH_MAX] = {0};

    if (src_path == TD_NULL || dst_path == TD_NULL || dst_len < PATH_MAX || strlen(src_path) >= PATH_MAX) {
        return TD_FAILURE;
    }

    if (realpath(src_path, resolved_path) == TD_NULL) {
        return TD_FAILURE;
    }

    if (strncpy_s(dst_path, dst_len, resolved_path, PATH_MAX - 1) != EOK) {
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

const td_u8 g_soi[2] = { 0xFF, 0xD8 }; /* 2 is a number */
const td_u8 g_eoi[2] = { 0xFF, 0xD9 }; /* 2 is a number */

static pthread_t g_venc_pid;
static pthread_t g_venc_qpmap_pid;
static sample_venc_getstream_para g_para = {
    .thread_start = TD_FALSE,
    .cnt = 0,
    .save_heif = TD_FALSE
};

static sample_venc_qpmap_sendframe_para g_qpmap_send_frame_para;
static pthread_t g_venc_rateauto_pid;
static sample_venc_rateauto_para g_venc_rateauto_frame_param;
static pthread_t g_venc_roimap_pid;
static sample_venc_roimap_frame_para g_roimap_frame_param;

td_s32 g_snap_cnt = 0;
td_char *g_dst_buf = TD_NULL;

#ifdef __READ_ALL_FILE__
static td_s32 read_jpg_file_pos(FILE *fp_jpg, td_char *psz_file, td_s32 size, td_s32 *startpos, td_s32 *endpos)
{
    td_s32 i = 0;
    td_s32 bufpos = 0;
    td_s32 tempbuf[TEMP_BUF_LEN] = { 0 };
    td_s32 startflag[2] = { 0xff, 0xd8 }; /* 2 is array size */
    td_s32 endflag[2] = { 0xff, 0xd9 }; /* 2 is array size */

    if (fread(psz_file, size, 1, fp_jpg) <= 0) {
        (td_void)fclose(fp_jpg);
        printf("fread jpeg src fail!\n");
        return TD_FAILURE;
    }

    if (fclose(fp_jpg) != 0) {
        return TD_FAILURE;
    }
    td_u16 thm_len;
    thm_len = (psz_file[0x4] << 0x8) + psz_file[0x5];
    while (i < size) {
        tempbuf[bufpos] = psz_file[i++];

        if (bufpos > 0 && memcmp(tempbuf + bufpos - 1, startflag, sizeof(startflag)) == 0) {
            *startpos = i - 0x2;
            if (*startpos < 0) {
                *startpos = 0;
            }
        }

        if (bufpos > 0 && memcmp(tempbuf + bufpos - 1, endflag, sizeof(endflag)) == 0) {
            *endpos = i;
            break;
        }

        bufpos++;

        if (bufpos == (TEMP_BUF_LEN - 1)) {
            if (tempbuf[bufpos - 1] != 0xFF) {
                bufpos = 0;
            }
        } else if (bufpos > (TEMP_BUF_LEN - 1)) {
            bufpos = 0;
        }
    }

    if (*endpos - *startpos <= 0) {
        printf("get .thm 11 fail!\n");
        return TD_FAILURE;
    }

    if (*endpos - *startpos >= size) {
        printf("NO DCF info, get .thm 22 fail!\n");
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

static td_s32 cpy_jpg_file_by_pos(td_char *psz_file, td_s32 startpos, td_s32 endpos)
{
    td_char *temp = psz_file + startpos;
    td_char *c_dst_buf = TD_NULL;
    td_s32 thm_size;

    if (startpos < 0 || endpos <= startpos || MAX_THM_SIZE < (endpos - startpos)) {
        printf("thm is too large than MAX_THM_SIZE, get .thm 33 fail!\n");
        return TD_FAILURE;
    }

    thm_size = endpos - startpos;
    c_dst_buf = (td_char *)malloc(thm_size);
    if (c_dst_buf == TD_NULL) {
        printf("memory malloc fail!\n");
        return TD_FAILURE;
    }

    if (memcpy_s(c_dst_buf, thm_size, temp, thm_size) != EOK) {
        printf("call memcpy_s error\n");
        free(c_dst_buf);
        return TD_FAILURE;
    }

    g_dst_buf = c_dst_buf;

    return TD_SUCCESS;
}

static td_s32 file_trans_get_thm_from_jpg(td_char *jpg_path, td_u8 len, td_u32 *dst_size)
{
    td_s32 ret = TD_FAILURE;
    FILE *fp_jpg = TD_NULL;
    td_s32 startpos = 0;
    td_s32 endpos = 0;
    td_char *psz_file = TD_NULL;
    td_s32 fd;
    struct stat stat_info = {0};
    td_char real_path[PATH_MAX] = {0};

    if ((len > FILE_NAME_LEN) || sample_comm_venc_get_realpath(jpg_path, real_path, sizeof(real_path)) != TD_SUCCESS) {
        printf("file %s error!\n", jpg_path);
        return TD_FAILURE;
    }

    fp_jpg = fopen(real_path, "rb");
    if (fp_jpg == TD_NULL) {
        printf("file %s not exist!\n", real_path);
        return TD_FAILURE;
    } else {
        fd = fileno(fp_jpg);
        fchmod(fd, S_IRUSR | S_IWUSR);
        fstat(fd, &stat_info);
        psz_file = (td_char *)malloc(stat_info.size);
        if ((psz_file == TD_NULL) || (stat_info.size < 6)) { /* 6: algo num */
            fclose(fp_jpg);
            printf("memory malloc fail!\n");
            return TD_FAILURE;
        }

        ret = read_jpg_file_pos(fp_jpg, psz_file, stat_info.size, &startpos, &endpos);
        if (ret != TD_SUCCESS) {
            free(psz_file);
            printf("read_jpg_file_pos fail!\n");
            return TD_FAILURE;
        }
    }

    ret = cpy_jpg_file_by_pos(psz_file, startpos, endpos)
    if (ret != TD_SUCCESS) {
        free(psz_file);
        printf("cpy_jpg_file_by_pos fail!\n");
        return TD_FAILURE;
    }

    *dst_size = endpos - startpos;
    free(psz_file);

    return TD_SUCCESS;
}

#else

static td_void file_trans_set_tembuf(FILE *fp_jpg, struct stat *stat_info, td_s32 *startpos, td_s32 *endpos)
{
    td_s32 tempbuf[TEMP_BUF_LEN] = { 0 };
    td_s32 fd;
    td_s32 ret;
    td_s32 bufpos = 0;
    td_s32 startflag[2] = { 0xff, 0xd8 }; /* 2 is a number */
    td_s32 endflag[2] = { 0xff, 0xd9 }; /* 2 is a number */
    td_bool start_match = TD_FALSE;

    fd = fileno(fp_jpg);
    fchmod(fd, S_IRUSR | S_IWUSR);
    fstat(fd, stat_info);

    while (!feof(fp_jpg)) {
        tempbuf[bufpos] = getc(fp_jpg);
        if (bufpos > 0) {
            if (memcmp(tempbuf + bufpos - 1, startflag, sizeof(startflag)) == 0) {
                *startpos = ((ftell(fp_jpg) - 2) < 0) ? 0 : (ftell(fp_jpg) - 2); /* 2 is a number 2 is a number */
                start_match = TD_TRUE;
            }

            ret = memcmp(tempbuf + bufpos - 1, endflag, sizeof(endflag));
            if ((ret == 0) && (start_match == TD_TRUE)) {
                *endpos = ftell(fp_jpg);
                break;
            } else if ((ret == 0) && (start_match != TD_TRUE)) {
                *endpos = ftell(fp_jpg);
            }
        }
        bufpos++;

        if (bufpos == (TEMP_BUF_LEN - 1)) {
            if (tempbuf[bufpos - 1] != 0xFF) {
                bufpos = 0;
            }
        } else if (bufpos > (TEMP_BUF_LEN - 1)) {
            if (tempbuf[bufpos - 1] == 0xFF) {
                tempbuf[0] = 0xFF;
                bufpos = 1;
            } else {
                bufpos = 0;
            }
        }
    }
}

static td_s32 file_trans_check_thm_range(td_s32 startpos, td_s32 endpos, const struct stat *stat_info)
{
    if (endpos - startpos <= 0) {
        printf("get .thm 11 fail!\n");
        return TD_FAILURE;
    }

    if (endpos - startpos > MAX_THM_SIZE) {
        printf("thm is too large than MAX_THM_SIZE, get .thm 22 fail!\n");
        return TD_FAILURE;
    }

    if (endpos - startpos >= stat_info->st_size) {
        printf("NO DCF info, get .thm 33 fail!\n");
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

static td_s32 file_trans_read_thm(FILE *fp_jpg, td_s32 startpos, td_s32 endpos, td_u32 *dst_size,
    td_char **dst_buf)
{
    td_s32 thm_size;

    if (startpos < 0 || endpos <= startpos || endpos - startpos > MAX_THM_SIZE) {
        printf("thm size invalid!\n");
        return TD_FAILURE;
    }

    thm_size = endpos - startpos;
    *dst_buf = (td_char *)malloc(thm_size);
    if (*dst_buf == TD_NULL) {
        printf("memory malloc fail!\n");
        return TD_FAILURE;
    }

    if (fseek(fp_jpg, (long)startpos, SEEK_SET) != 0) {
        free(*dst_buf);
        return TD_FAILURE;
    }
    *dst_size = fread(*dst_buf, 1, thm_size, fp_jpg);
    if (*dst_size != (td_u32)thm_size) {
        free(*dst_buf);
        printf("fread fail!\n");
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

static td_s32 file_trans_get_thm_from_jpg(td_char *jpg_path, td_u8 len, td_u32 *dst_size)
{
    td_s32 startpos = 0;
    td_s32 endpos = 0;
    struct stat stat_info = {0};
    FILE *fp_jpg = TD_NULL;
    td_char real_path[PATH_MAX] = {0};
    td_char *c_dst_buf = TD_NULL;

    if ((len > FILE_NAME_LEN) || sample_comm_venc_get_realpath(jpg_path, real_path, sizeof(real_path)) != TD_SUCCESS) {
        printf("file %s error!\n", jpg_path);
        return TD_FAILURE;
    }

    fp_jpg = fopen(real_path, "rb");
    if (fp_jpg == TD_NULL) {
        printf("file %s not exist!\n", real_path);
        return TD_FAILURE;
    } else {
        file_trans_set_tembuf(fp_jpg, &stat_info, &startpos, &endpos);
    }

    if (file_trans_check_thm_range(startpos, endpos, &stat_info) != TD_SUCCESS) {
        fclose(fp_jpg);
        return TD_FAILURE;
    }

    if (file_trans_read_thm(fp_jpg, startpos, endpos, dst_size, &c_dst_buf) != TD_SUCCESS) {
        fclose(fp_jpg);
        return TD_FAILURE;
    }

    g_dst_buf = c_dst_buf;
    if (fclose(fp_jpg) != 0) {
        free(c_dst_buf);
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}
#endif

/* set venc memory location */
td_s32 sample_comm_venc_mem_config(td_void)
{
    td_s32 i;
    td_s32 ret;
    ot_mpp_chn mpp_chn_venc;

    /* group, venc max chn is 64 */
    for (i = 0; i < 64; i++) {
        td_char *pc_mmz_name = TD_NULL;
        mpp_chn_venc.mod_id = OT_ID_VENC;
        mpp_chn_venc.dev_id = 0;
        mpp_chn_venc.chn_id = i;

        /* venc */
        ret = ss_mpi_sys_set_mem_cfg(&mpp_chn_venc, pc_mmz_name);
        if (ret != TD_SUCCESS) {
            sample_print("ss_mpi_sys_set_mem_config with %#x!\n", ret);
            return TD_FAILURE;
        }
    }
    return TD_SUCCESS;
}

/* get file postfix according palyload_type. */
td_s32 sample_comm_venc_get_file_postfix(ot_payload_type payload, td_char *file_postfix, td_u8 len)
{
    if (payload == OT_PT_H264) {
        if (strcpy_s(file_postfix, len, ".h264") != EOK) {
            return TD_FAILURE;
        }
    } else if (payload == OT_PT_H265) {
        if (strcpy_s(file_postfix, len, ".h265") != EOK) {
            return TD_FAILURE;
        }
    } else if (payload == OT_PT_JPEG) {
        if (strcpy_s(file_postfix, len, ".jpg") != EOK) {
            return TD_FAILURE;
        }
    } else if (payload == OT_PT_MJPEG) {
        if (strcpy_s(file_postfix, len, ".mjp") != EOK) {
            return TD_FAILURE;
        }


#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_COMMON_SAMPLE_COMM_VENC_BODY_PART01_H_ */
