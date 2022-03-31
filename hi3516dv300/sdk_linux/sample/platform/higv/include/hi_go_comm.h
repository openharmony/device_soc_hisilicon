/*
 * Copyright (c) 2022 HiSilicon (Shanghai) Technologies CO., LIMITED.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef HI_GO_COMM_H
#define HI_GO_COMM_H

#include "hi_type.h"
#include "hi_go_errno.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/* Maximum number of colors in the palette, */
#define MAX_PALETTE_COLOR_SIZE 256

/* Color value, */
typedef HI_U32 HI_COLOR;

/* Palette, */
typedef HI_COLOR HI_PALETTE[MAX_PALETTE_COLOR_SIZE];

typedef enum {
    HIGO_IMGTYPE_JPEG = 0,
    /* jpeg picture, */
    HIGO_IMGTYPE_GIF,
    /* gif picture, */
    HIGO_IMGTYPE_BMP,
    /* bmp picture, */
    HIGO_IMGTYPE_PNG,
    /* png picture, */
    HIGO_IMGTYPE_RLE,
    /* rle picture, */
    HIGO_IMGTPYE_BUTT
} HIGO_IMGTYPE_E;

/* Rectangle, */
typedef struct {
    HI_S32 x, y;

    HI_S32 w, h;
} HI_RECT;

typedef HI_RECT HI_POINT;

/* Region, */
typedef struct {
    HI_S32 l;
    HI_S32 t;
    HI_S32 r;
    HI_S32 b;
} HI_REGION;

/* Mode of adjusting the window z-order, */
typedef enum {
    HIGO_ZORDER_MOVETOP = 0,
    /* *<Move to the top, */
    HIGO_ZORDER_MOVEUP,
    /* *<Move upwards, */
    HIGO_ZORDER_MOVEBOTTOM,
    /* *<Move to the bottom, */
    HIGO_ZORDER_MOVEDOWN,
    /* *<Move downwards, */
    HIGO_ZORDER_BUTT
} HIGO_ZORDER_E;

#define HIGO_INVALID_HANDLE 0x0

/*
 * \brief Initializes the HiGo.
 * \attention \n
 * Before using the HiGo, you must call this application programming interface (API) to initialize the HiGo. The HiGo
 * includes the graphic device (Gdev) module, decoder, Winc module, bit block transfer (Bliter) module, and surface
 * module.
 *
 * \param N/A.
 * \retval ::HI_SUCCESS
 * \retval ::HI_FAILURE
 * \retval ::HIGO_ERR_DEPEND_TDE
 * \retval ::HIGO_ERR_INITFAILED
 * \see \n
 * ::HI_GO_Deinit
 */
HI_S32 HI_GO_Init(HI_VOID);

/*
 * \brief Deinitializes the HiGo.
 * \attention \n
 * If the HiGo is not used, you need to call this API to release resources.
 * ʹHiGoʱҪøýӿڣͷԴ CNend \param N/A.
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HIGO_ERR_DEINITFAILED
 * \see \n
 * ::HI_GO_Init
 */
HI_S32 HI_GO_Deinit(HI_VOID);

/*
 * \brief Initializes the extended library of the HiGo.
 * \attention \n
 * If the extended library of the HiGo is not used, you need to call this API to release resources, especially the
 * resources of the cursor and text modules.
 *
 * \param N/A.
 * \retval ::HI_SUCCESS
 * \retval ::HI_FAILURE
 * \retval ::HIGO_ERR_DEPEND_TDE
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HIGO_ERR_DEINITFAILED
 * \see \n
 * ::HI_GO_DeInitExt
 */
HI_S32 HI_GO_InitExt(HI_VOID);

/*
 * \brief Deinitializes the extended library of the HiGo.
 * \attention \n
 * If the HiGo is not used, you need to call this API to release resources.
 * ʹHiGoչ⹦ʱҪøýӿڣͷԴҪCURSORTEXTģ顣CNend \param N/A.
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HIGO_ERR_DEINITFAILED
 * \see \n
 * ::HI_GO_InitExt
 */
HI_S32 HI_GO_DeInitExt(HI_VOID);

/*
 * \brief Obtains the version information.
 * \attention \n
 * N/A.
 * \param[out] ppVersion Output address of the version information string. The value cannot be empty.
 * 汾ϢַַΪ CNend \param[out] ppBuildTime Build Output address of the time string.
 * The value cannot be empty.
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_NULLPTR
 * \see \n
 * N/A.
 */
HI_S32 HI_GO_GetVersion(HI_CHAR **ppVersion, HI_CHAR **ppBuildTime);

/*
 * \brief Converts the data on a surface into a .bmp picture for output.
 * SurfaceеתBMPʽͼCNend \attention \n The output file is
 * [year]-[date]-[hour]-[min]-[second]-[ms].bmp.
 * The output picture must be a 16-bit bitmap.
 * It is recommended that you call HI_GO_EncodeToFile rather than HI_GO_Surface2Bmp.
 * ʹHI_GO_EncodeToFileӿ˽ӿ CNend
 * \param[in] Surface Data to be captured.
 * \param[in] pRect Pointer to a rectangle. If this parameter is not set, it indicates the entire surface.
 * ָ,ΪձʾsurfaceCNend
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_NULLPTR
 * \retval ::HIGO_ERR_INVPARAM
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_INVSURFACESIZE
 * \retval ::HIGO_ERR_INVSURFACEPF
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HIGO_ERR_NOMEM
 */
HI_S32 HI_GO_Surface2Bmp(HI_HANDLE Surface, const HI_RECT *pRect);

/*
 * \brief Enables the memory management module.
 * \attention \n
 * The memory management module is disabled by default. Before using the memory statistics function, you must call this
 * API to enable the memory management module.
 *
 * \param[in] bEnable Whether to enable the memory statistics function.
 * \retval ::HI_SUCCESS
 */
HI_S32 HI_GO_EnableMemMng(HI_BOOL bEnable);

/*
 * \brief Obtains the enable status of the memory management module.
 * \attention \n
 * \param[out] pbEnable Whether to obtain the enable status of the memory statistics
 * function.
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_NULLPTR
 */
HI_S32 HI_GO_GetMemMngStatus(HI_BOOL *pbEnable);

/*
 * \brief Outputs the general information about the system memory.
 * \attention \n
 * \param N/A.
 * \retval ::HI_SUCCESS
 */
HI_S32 HI_GO_SysMemQuene(HI_VOID);

/*
 * \brief Outputs the general information about the media memory zone (MMZ).
 * \attention \n
 * \param N/A.
 * \retval ::HI_SUCCESS
 */
HI_S32 HI_GO_MMZMemQuene(HI_VOID);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* HI_GO_COMM_H */
