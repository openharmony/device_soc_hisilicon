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
#ifndef HI_GO_ENCODER_H
#define HI_GO_ENCODER_H

#include "hi_go_comm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif  /* __cplusplus */
#endif  /* __cplusplus */

/* Encoder attributes */
typedef struct {
    HIGO_IMGTYPE_E ExpectType; /*
                                * Type of the encoded picture
                                *
                                */
    HI_U32 QualityLevel;       /*
                                * The quality level ranges from 1 to 99. The higher the level, the better the quality,
                                * and the greater the encoded picture or occupied memory. The QualityLevel parameter
                                * is valid for .jpeg pictures only.
                                *
                                */
} HIGO_ENC_ATTR_S;

/*
 * \brief Initializes the encoder.
 * \attention \n
 * When ::HI_GO_Init is called, this application programming interface (API) is also called.
 *
 * \param  N/A
 * \retval ::HI_SUCCESS
 * \retval ::HI_FAILURE
 * \retval ::HIGO_ERR_DEPEND_TDE
 * \see \n
 * ::HI_GO_Init \n
 * ::HI_GO_DeinitDecoder
 */
HI_S32 HI_GO_InitEncoder(HI_VOID);

/*
 * \brief Deinitializes the encoder.
 * \attention \n
 * When ::HI_GO_Deinit is called, this API is also called.
 *
 * \param  N/A
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_NOTINIT
 * \see \n
 * ::HI_GO_Deinit \n
 * ::HI_GO_InitDecoder
 */
HI_S32 HI_GO_DeinitEncoder(HI_VOID);

/*
 * \brief Encodes a surface in a specified format, and saves the encoded surface in a file.
 *
 * \attention \n
 * The .bmp encoding format is supported.
 * The hardware platform determines whether the .jpeg encoding format is supported.
 *
 * (Ƿֱ֧JPEG,ȡӲƽ̨) CNend
 * \param[in] hSurface Surface to be encoded.
 * \param[in] pFile  Name of the encoded file. This parameter can be empty. If this parameter is not set, the 2nd file
 *                   is named [year]-[month]-[date]-[hour]-[minute]-[second].
 *
 * \param[in] pAttr  Encoding attributes. This parameter cannot be empty.
 * \retval ::HI_SUCCESS Success.
 * \retval ::HI_FAILURE
 * \retval ::HIGO_ERR_NULLPTR
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_UNSUPPORTED
 * \retval ::HIGO_ERR_NOMEM
 * \retval ::HIGO_ERR_INVFILE
 * \retval ::HIGO_ERR_INVSRCTYPE
 * \retval ::HIGO_ERR_INVIMAGETYPE
 * \retval ::HIGO_ERR_INVPARAM
 * \retval ::HIGO_ERR_INVMIRRORTYPE
 * \retval ::HIGO_ERR_INVROTATETYPE
 * \retval ::HIGO_ERR_INVCKEYTYPE
 * \retval ::HIGO_ERR_INVROPTYPE
 * \retval ::HIGO_ERR_NOCOLORKEY
 * \retval ::HIGO_ERR_INVPIXELFMT
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_INVCOMPTYPE
 * \retval ::HIGO_ERR_LOCKED
 * \retval ::HIGO_ERR_DEPEND_TDE
 * \retval ::HIGO_ERR_EMPTYRECT
 * \retval ::HIGO_ERR_OUTOFBOUNDS
 * \retval ::HIGO_ERR_INTERNAL
 * \retval ::HIGO_ERR_DEPEND_JPGE
 * \see \n
 */
HI_S32 HI_GO_EncodeToFile(HI_HANDLE hSurface, const HI_CHAR* pFile, const HIGO_ENC_ATTR_S* pAttr);

/*
 * \brief Encodes a surface in a specified picture format and saves it in a memory.
 *  Ensure that the memory is sufficient.
 *
 * \attention \n
 * The data can be encoded as .bmp data, and saved in a specified memory.
 * The hardware platform determines whether the .jpeg encoding format is supported.
 *
 * (Ƿֱ֧JPEG,ȡӲƽ̨) CNend
 * \param[in] hSurface  Surface to be encoded.
 * \param[in] pMem      Start address of the memory for storing the encoded pictures.
 *                      ʼַ CNend
 * \param[in] MemLen    Size of a specified memory.
 * \param[out] pOutLen  Actual size of the used memory, indicating the length of encoded data. Ensure that the value of
 *                      pOutLen is smaller than or equal to the value of MemLen.
 *
 * \param[in] pAttr     Encoding attributes. This parameter cannot be empty.
 * \retval ::HI_SUCCESS
 * \retval ::HI_FAILURE
 * \retval ::HIGO_ERR_NULLPTR
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_UNSUPPORTED
 * \retval ::HIGO_ERR_NOMEM
 * \retval ::HIGO_ERR_INVFILE
 * \retval ::HIGO_ERR_INVSRCTYPE
 * \retval ::HIGO_ERR_INVFILE
 * \retval ::HIGO_ERR_INVIMAGETY
 * \retval ::HIGO_ERR_INVIMAGETYPE
 * \retval ::HIGO_ERR_INVPARAM
 * \retval ::HIGO_ERR_INVMIRRORTYPE
 * \retval ::HIGO_ERR_INVROTATETYPE
 * \retval ::HIGO_ERR_INVCKEYTYPE
 * \retval ::HIGO_ERR_INVROPTYPE
 * \retval ::HIGO_ERR_NOCOLORKEY
 * \retval ::HIGO_ERR_INVPIXELFMT
 * \retval ::HIGO_ERR_INTERNAL
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_INVCOMPTYPE
 * \retval ::HIGO_ERR_LOCKED
 * \retval ::HIGO_ERR_DEPEND_TDE
 * \retval ::HIGO_ERR_EMPTYRECT
 * \retval ::HIGO_ERR_OUTOFBOUNDS
 * \retval ::HIGO_ERR_DEPEND_JPGE
 * \see \n
 */
HI_S32 HI_GO_EncodeToMem(HI_HANDLE hSurface, HI_U8* pMem, HI_U32 MemLen, HI_U32* pOutLen, const HIGO_ENC_ATTR_S* pAttr);

#ifdef __cplusplus
#if __cplusplus
}
#endif  /* __cplusplus */
#endif  /* __cplusplus */

#endif /* HI_GO_ENCODER_H */
