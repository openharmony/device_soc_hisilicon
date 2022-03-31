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

#ifndef HI_GO_DECODE_H
#define HI_GO_DECODE_H

#include "hi_type.h"
#include "hi_go_comm.h"
#include "hi_go_surface.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/* Decoding type of the decoding instance, */
typedef enum {
    HIGO_DEC_IMGTYPE_JPEG = HIGO_IMGTYPE_JPEG,
    /* jpeg picture, */
    HIGO_DEC_IMGTYPE_GIF = HIGO_IMGTYPE_GIF,
    /* gif picture, */
    HIGO_DEC_IMGTYPE_BMP = HIGO_IMGTYPE_BMP,
    /* bmp picture, */
    HIGO_DEC_IMGTYPE_PNG = HIGO_IMGTYPE_PNG,
    /* png picture, */
    HIGO_DEC_IMGTYPE_RLE = HIGO_IMGTYPE_RLE,
    /* rle picture, */
    HIGO_DEC_IMGTPYE_BUTT
} HIGO_DEC_IMGTYPE_E;

/* Stream source, */
typedef enum {
    HIGO_DEC_SRCTYPE_FILE = 0,
    /* Streams are derived from files. */
    HIGO_DEC_SRCTYPE_MEM,
    /* Streams are derived from the memory. */
    HIGO_DEC_SRCTYPE_STREAM,
    /* Streams are derived from stream data. */
    HIGO_DEC_SRCTYPE_BUTT
} HIGO_DEC_SRCTYPE_E;

typedef struct {
    HIGO_MEMTYPE_E MemType;
    HI_BOOL IsPubPalette; /*
                           * Decoder uses common Palette or not,
                           *
                           */
} HIGO_DEC_SURINFO_S;

typedef HI_S32 (*PTR_Dec_WriteStreamCB_FN)(unsigned long ClientData, unsigned long Offset);

/* Detailed information about the stream source, */
typedef union {
    /*
     * The following information is required if the input source is the memory.
     *
     */
    struct {
        HI_CHAR *pAddr; /* Memory pointer address, */
        HI_U32 Length; /* Length, */
    } MemInfo;

    /* Picture name, */
    const HI_CHAR *pFileName;
    struct {
        PTR_Dec_WriteStreamCB_FN pWriteCB;
        unsigned long ClientData; /* Save client data. */
    } Stream;
} HIGO_DEC_SRCINFO_U;

/* Parameters for creating a decoding instance. */
typedef struct {
    HIGO_DEC_SRCTYPE_E SrcType; /* Type of the input stream. */
    HIGO_DEC_SRCINFO_U SrcInfo; /* Information about the input source. */
} HIGO_DEC_ATTR_S;

/* Major information after decoding, */
typedef struct {
    HI_U32 Count; /* Number of pictures, */
    HIGO_DEC_IMGTYPE_E ImgType; /* Type of the input source. */
    HI_U32 ScrWidth;
    /*
     * <Screen width. The screen refers to that of the picture. The value 0 indicates "invalid".
     */
    HI_U32 ScrHeight;
    /*
     * Screen height. The screen refers to that of the picture. The value 0 indicates "invalid".
     */
    HI_BOOL IsHaveBGColor; /* Whether there is any background color. */
    HI_COLOR BGColor; /* Background color, */
} HIGO_DEC_PRIMARYINFO_S;

/* Index picture format, */
typedef struct {
    HI_U32 OffSetX;
    /*
     * <X offset value on the screen. The screen mentioned in this document refers to that of the picture.
     */
    HI_U32 OffSetY; /* offset value on the screen. */
    HI_U32 Width;   /* Width of the original picture, */
    HI_U32 Height;  /* Height of the original picture, */
    HI_U8 Alpha;
    /*
     * <Information about channel alpha used for overlaying pictures. The alpha value ranges from 0 to 255.
     */
    HI_BOOL IsHaveKey; /*
                        * Key enable. HI_FALSE: disabled; HI_TRUE: enabled,
                        *
                        */
    HI_COLOR Key; /*
                   * Information about colorkey used for overlaying pictures,
                   *
                   */
    HIGO_PF_E Format; /* Pixel format of the picture source, */
    HI_U32    DelayTime;              /*
                                       * <Time interval between this picture and the previous one (10 ms).
                                       * The value 0 indicates that the parameter is invalid.
                                       *
                                       */
    HI_U32    DisposalMethod;         /*
                                       * Processing method of the current frame.
                                       * This application programming interface (API) is valid for only GIF pictures.
                                       * 0: customized processing method; 1: no processing method;
                                         2: use the background color.
                                       *
                                          0:ûʽ1:2:ʹñɫ
                                       */
} HIGO_DEC_IMGINFO_S;

/* Picture attributes after decoding, */
typedef struct {
    HI_U32 Width; /* Expected width after decoding, */
    HI_U32 Height; /* Expected height after decoding, */
    HIGO_PF_E Format; /* Expected pixel format after decoding, */
} HIGO_DEC_IMGATTR_S;

/* Extended data format type, */
typedef enum {
    HIGO_DEC_EXTEND_EXIF = 0,
    /* Extended-data in EXIF format, */
    HIGO_DEC_EXTEND_EXIF_BUTT
} HIGO_DEC_EXTENDTYPE_E;

/* Basic picture information, */
typedef struct {
    HI_U32 Width; /* Picture width, */
    HI_U32 Height; /* Picture height, */
    HIGO_PF_E PixelFormat; /* Pixel format, */
    HI_U32 VirAddr[3]; /*
                        * Virtual address. If the address is in RGB format, only the first VirAddr[0] is the start
                        * address, and VirAddr[1] and VirAddr[2] are invalid. If the address is in YC format,
                        * VirAddr[0] indicates the start address of the Y component, VirAddr[1] indicates the
                        * start address of the C component, and VirAddr[2] is invalid.
                        *
                        * VirAddr[1]VirAddr[2]ЧYCʽ
                        * VirAddr[0]ʾYʼַVirAddr[1]ʾCʼַ
                        * VirAddr[2]Ч
                        */
    HI_U32 PhyAddr[3]; /*
                        * Physical address. The usage principles are the same as previous ones.
                        *
                        */
    HI_U32 Pitch[3]; /*
                      * Pitch. The usage principles are the same as previous ones.
                      *
                      */
    HI_U32 Palate[256]; /* Palette, */
} HIGO_DEC_IMGDATA_S;

typedef struct {
    HI_VOID *StreamBuf; /* Virtual start address of streams, */
    HI_U32 StreamLen; /* Actual read stream length, */
    HI_U32 ExpectStreamLen; /* Expect read stream length, */
} HIGO_DEC_WRITESTREAM_S;

/*
 * \brief Initializes the decoder.
 * \attention \n
 * ::HI_GO_Init already calls the interface
 * \param  N/A
 * \retval ::HI_SUCCESS
 * \retval ::HI_FAILURE
 * \retval ::HIGO_ERR_DEPEND_TDE
 * \see \n
 * ::HI_GO_Init \n
 * ::HI_GO_DeinitDecoder
 */
HI_S32 HI_GO_InitDecoder(HI_VOID);

/*
 * \brief Deinitializes the decoder.
 * \attention \n
 * When ::HI_GO_Deinit is called, this API is also called.
 * \param  N/A.
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_NOTINIT
 * \see \n
 * ::HI_GO_Deinit \n
 * ::HI_GO_InitDecoder
 */
HI_S32 HI_GO_DeinitDecoder(HI_VOID);

/*
 * \brief Creates a decoding handle.
 * \attention \n
 * Currently, the formats of .bmp, .png, .jpeg, and .gif are supported.
 * The picture input source can be the memory, files
 * , and streams. The picture input source of streams is only applicable to the .jpeg format.
 *
 * \param[in] pSrcDesc Information about the input source. This parameter cannot be empty.
 * The type of the input stream must match the information about the input source. For example,
 * corresponding file names must be entered for the file stream, and the memory address and memory size
 * must be entered
 * for the memory stream.
 * ҪԴϸϢһһƥ䣬FILEҪӦļ\n
 * MemoryҪmemoryĵַԼ CNend
 * \param[out] pDecoder Pointer of the decoding instance handle. This parameter cannot be empty.
 * ʵָ룬Ϊ CNend
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_NULLPTR
 * \retval ::HIGO_ERR_NOTINIT
 * \retval ::HIGO_ERR_NOMEM
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_INVFILE
 * \retval ::HIGO_ERR_INVSRCTYPE
 * \retval ::HIGO_ERR_INVIMAGETYPE
 * \retval ::HIGO_ERR_INVIMGDATA
 * \see \n
 * ::HI_GO_DestroyDecoder
 */
HI_S32 HI_GO_CreateDecoder(const HIGO_DEC_ATTR_S *pSrcDesc, HI_HANDLE *pDecoder);

/*
 * \brief Destroys a decoding handle.
 * \attention \n
 * N/A.
 * \param[in] Decoder Handle of the decoding instance.
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_INVHANDLE
 * \see \n
 * ::HI_GO_CreateDecoder
 */
HI_S32 HI_GO_DestroyDecoder(HI_HANDLE Decoder);

/*
 * \brief Stream API
 * \attention \n
 * N/A.
 * \param[in] Decoder Handle of the decoding instance.
 * \param[in] pStream Pointer to the stream information. This parameter cannot be empty.
 *
 * \retval ::HI_SUCCESS
 * \retval ::HI_FAILURE
 * \retval ::HIGO_ERR_NULLPTR
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_UNSUPPORTED
 * \see \n
 * ::HI_GO_CreateDecoder
 */
HI_S32 HI_GO_DecWriteStream(HI_HANDLE Decoder, HIGO_DEC_WRITESTREAM_S *pStream);

/*
 * \brief Reset decoding.
 * \attention only support jpeg decode\n
 * N/A.
 * \param[in] Decoder Handle of the decoding instance.
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_INVIMAGETYPE
 * \see sample_dec_stream.c\n
 * ::HI_GO_ResetDecoder
 */
HI_S32 HI_GO_ResetDecoder(HI_HANDLE Decoder);

/*
 * \brief Obtains the major information about pictures.
 * \attention
 * N/A.
 * \param[in] Decoder Handle of the decoding instance.
 * \param[out] pPrimaryInfo  Pointer to the major information about the decoded picture. This parameter cannot be
 * empty.
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_NULLPTR
 * \retval ::HI_NULL
 * \retval ::HIGO_ERR_INVIMGDATA
 * \see \n
 * ::HI_GO_DecImgInfo
 */
HI_S32 HI_GO_DecCommInfo(HI_HANDLE Decoder, HIGO_DEC_PRIMARYINFO_S *pPrimaryInfo);

/*
 * \brief Obtains the information about the specified picture. You can customize the picture format.
 * ȡָͼƬϢͬʱûͼʽ CNend \attention \n N/A.
 *
 * Handle of the decoding instance.
 * For .jpeg pictures,
 * the index number of the main picture is 0, and index numbers of thumbnails start from 1. The index numbers of GIF
 * pictures start from 0 and increase by 1 according to the sequence of the pictures stored in a file.
 * ͼƬšJPEGʽͼƬͼ0ͼ1ʼ\n
 * GIFʽͼƬļеĴ洢˳0ʼݼ
 * CNend \param[out] pImgInfo Picture information. This parameter cannot be empty.
 *
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_NULLPTR
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_INVINDEX
 * \retval ::HI_FAILURE
 * \retval ::HI_NULL
 * \retval ::HIGO_ERR_INVIMGDATA
 * \see \n
 * ::HI_GO_DecCommInfo
 */
HI_S32 HI_GO_DecImgInfo(HI_HANDLE Decoder, HI_U32 Index, HIGO_DEC_IMGINFO_S *pImgInfo);

/*
 * \brief Obtains the data of a picture.
 * \attention \n
 * The format in the destination picture attributes does not support the macro block
 * format.
 * \param[in] Decoder Handle of the decoding instance.
 * \param[in] Index Index number of a picture, starting from 0.
 * \param[in] pImgAttr Attribute of the destination picture. If the value is empty, it indicates that the original
 * picture attributes are retained.
 * \param[out] pSurface Handle of the surface where the decoded data is stored.
 * CNend
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_NULLPTR
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_INVINDEX
 * \retval ::HI_FAILURE
 * \retval ::HI_NULL
 * \retval ::HIGO_ERR_INVIMGDATA
 * \retval ::HIGO_ERR_NOMEM
 * \retval ::HIGO_ERR_UNSUPPORTED
 * \see \n
 * ::HI_GO_DecCommInfo
 */
HI_S32 HI_GO_DecImgData(HI_HANDLE Decoder, HI_U32 Index, const HIGO_DEC_IMGATTR_S *pImgAttr, HI_HANDLE *pSurface);

/*
 * \brief Obtains the extended data.
 * \attention \n
 * Hi3110E does not support the API.
 * \param[in] Decoder Handle of the decoding instance.
 * \param[in] DecExtendType
 * Type of the obtained extended data. Currently, only the EXIF information in .jpeg format is available.
 *
 * \param[out] ppData Pointer to the extended data pointer.
 * \param[out] pLength ppData Length.
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_NULLPTR
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_UNSUPPORTED
 * \see \n
 * ::HI_GO_ReleaseDecExtendData
 */
HI_S32 HI_GO_DecExtendData(HI_HANDLE Decoder, HIGO_DEC_EXTENDTYPE_E DecExtendType, HI_VOID **ppData, HI_U32 *pLength);

/*
 * \brief Releases extended data.
 * \attention \n
 * Hi3110E does not support the API.
 * \param[in] Decoder Decoder handle.
 * \param[in] DecExtendType Type of the obtained extended data. Currently,
 * only the EXIF information in .jpeg format is
 * available.
 * \param[in] pData Pointer to the extended data.
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_NULLPTR
 * \retval ::HIGO_ERR_UNSUPPORTED
 * \see \n
 * ::HI_GO_DecExtendData
 */
HI_S32 HI_GO_ReleaseDecExtendData(HI_HANDLE Decoder, HIGO_DEC_EXTENDTYPE_E DecExtendType, HI_VOID *pData);

/*
 * \brief Decodes a picture to a specified surface.
 * \attention \n
 * The decoded surface is scaled automatically based on the size of the target surface. In addition,
 * the decoded surface
 * modifies the attributes of the target surface, such as colorkey, alpha value, and palette.
 *
 * ALPHAԼɫ CNend
 * \param[in] Decoder Handle of the decoding instance.
 * \param[in] Index   Index number of a decoded picture.
 * \param[out] Surface Decoded target surface.
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_INVINDEX
 * \retval ::HIGO_ERR_NULLPTR
 * \retval ::HI_FAILURE
 * \retval ::HI_NULL
 * \retval ::HIGO_ERR_INVIMGDATA
 * \retval ::HIGO_ERR_NOMEM
 * \retval ::HIGO_ERR_UNSUPPORTED
 * \see \n
 * N/A.
 */
HI_S32 HI_GO_DecImgToSurface(HI_HANDLE Decoder, HI_U32 Index, HI_HANDLE Surface);

/*
 * \brief Decodes a specified file, and generates a new surface.
 *
 * \attention \n
 * If there are multiple pictures in the file (such as .gif animation, .jpeg main picture, and .jpeg thumbnail),
 * only the .jpeg main picture and the first picture of the .gif animation are decoded.
 * In addition, the surface size is the same as the picture size.
 *
 * JPEGֻͼGIFһͼƬsurfaceĴСͼƬĴСһ CNend
 * \param[in] pszFile Picture name. The picture can be in the format of .gif, .png, .jpg, or
 * .bmp.
 * pictures.
 * \retval ::HI_SUCCESS
 * \retval ::HI_FAILURE
 * \retval ::HIGO_ERR_INVHANDLE
 * \retval ::HIGO_ERR_INVINDEX
 * \retval ::HIGO_ERR_NULLPTR
 * \retval ::HI_NULL
 * \retval ::HIGO_ERR_INVIMGDATA
 * \retval ::HIGO_ERR_NOMEM
 * \retval ::HIGO_ERR_UNSUPPORTED
 * \retval ::HIGO_ERR_INVFILE
 * \retval ::HIGO_ERR_INVSRCTYPE
 * \retval ::HIGO_ERR_INVIMAGETYPE
 * \see \n
 * N/A.
 */
HI_S32 HI_GO_DecodeFile(const HI_CHAR *pszFile, HI_HANDLE *pSurface);

/*
 * \brief Set the attribute of Decoder out Surface memrory.
 * \attention \n
 * N/A.
 * \param[in] Decoder handle.
 * \param[in] pSurfInfo.
 * \retval ::HI_SUCCESS
 * \retval ::HIGO_ERR_INVHANDLE
 * \see \n
 * N/A.
 */
HI_S32 HI_GO_SetDecSurfInfo(HI_HANDLE Decoder, const HIGO_DEC_SURINFO_S *pSurfInfo);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* HI_GO_DECODE_H */
