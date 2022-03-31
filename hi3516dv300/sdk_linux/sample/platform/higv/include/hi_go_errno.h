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

#ifndef HI_GO_ERRNO_H
#define HI_GO_ERRNO_H

/* add include here */
#include "hi_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*
 * Error IDs of HiGo projects
 *
 */
#define HIGO_ERR_APPID (0x80000000L + 0x30000000L)

typedef enum {
    HIGO_LOG_LEVEL_DEBUG = 0,  /* debug-level                                  */
    HIGO_LOG_LEVEL_INFO,       /* informational                                */
    HIGO_LOG_LEVEL_NOTICE,     /* normal but significant condition             */
    HIGO_LOG_LEVEL_WARNING,    /* warning conditions                           */
    HIGO_LOG_LEVEL_ERROR,      /* error conditions                             */
    HIGO_LOG_LEVEL_CRIT,       /* critical conditions                          */
    HIGO_LOG_LEVEL_ALERT,      /* action must be taken immediately             */
    HIGO_LOG_LEVEL_FATAL,      /* just for compatibility with previous version */
    HIGO_LOG_LEVEL_BUTT
} higo_log_errlevel;

/*
 * Macros for defining the error codes of the HiGo
 *
 */
#define higo_def_err(module, errid)                                                              \
    ((hi_s32)((HIGO_ERR_APPID) | (((hi_u32)(module)) << 16) | (((hi_u32)HIGO_LOG_LEVEL_ERROR) << 13) | \
    ((HI_U32)(errid))))

/*
 * HiGo Module encoding
 *
 */
typedef enum {
    HIGO_MOD_COMM = 0,
    HIGO_MOD_SURFACE,
    HIGO_MOD_MEMSURFACE,
    HIGO_MOD_LAYER,
    HIGO_MOD_BLITER,
    HIGO_MOD_DEC,
    HIGO_MOD_TEXTOUT,
    HIGO_MOD_WINC,
    HIGO_MOD_CURSOR,
    HIGO_MOD_TEXT_LAYOUT,
    HIGO_MOD_BUTT
} higo_mod;

/*
 * Common error codes of the HiGo
 *
 */
typedef enum {
    ERR_COMM_NOTINIT = 0,
    ERR_COMM_INITFAILED,
    ERR_COMM_DEINITFAILED,
    ERR_COMM_NULLPTR,
    ERR_COMM_INVHANDLE,
    ERR_COMM_NOMEM,
    ERR_COMM_INTERNAL,
    ERR_COMM_INVSRCTYPE,
    ERR_COMM_INVFILE,
    ERR_COMM_INVPARAM,
    ERR_COMM_INUSE,
    ERR_COMM_UNSUPPORTED,
    ERR_COMM_DEPENDTDE,
    ERR_COMM_DEPENDFB,
    ERR_COMM_DEPENDMMZ,
    ERR_COMM_DEPENDJPEG,
    ERR_COMM_DEPENDPNG,
    ERR_COMM_DEPENDBMP,
    ERR_COMM_DEPENDGIF,
    ERR_COMM_DEPENDCURSOR,
    ERR_COMM_DEPENDJPGENC,
    ERR_COMM_BUTT
} higo_err;

/*
 * The dependent module is not initialized (0xB0008000).
 *
 */
#define HIGO_ERR_NOTINIT higo_def_err(HIGO_MOD_COMM, ERR_COMM_NOTINIT)

/*
 * The module fails to be initialized (0xB0008001).
 *
 */
#define HIGO_ERR_DEINITFAILED higo_def_err(HIGO_MOD_COMM, ERR_COMM_INITFAILED)

/*
 * The module fails to be deinitialized (0xB0008002).
 *
 */
#define HIGO_ERR_INITFAILED higo_def_err(HIGO_MOD_COMM, ERR_COMM_DEINITFAILED)

/*
 * The input pointer is null (0xB0008003).
 *
 */
#define HIGO_ERR_NULLPTR higo_def_err(HIGO_MOD_COMM, ERR_COMM_NULLPTR)

/*
 * The input handle is invalid (0xB0008004).
 *
 */
#define HIGO_ERR_INVHANDLE higo_def_err(HIGO_MOD_COMM, ERR_COMM_INVHANDLE)

/*
 * The memory is insufficient (0xB0008005).
 *
 */
#define HIGO_ERR_NOMEM higo_def_err(HIGO_MOD_COMM, ERR_COMM_NOMEM)

/*
 * An internal error occurs (0xB0008006).
 *
 */
#define HIGO_ERR_INTERNAL higo_def_err(HIGO_MOD_COMM, ERR_COMM_INTERNAL)

/*
 * The I/O source is invalid (0xB0008007).
 *
 */
#define HIGO_ERR_INVSRCTYPE higo_def_err(HIGO_MOD_COMM, ERR_COMM_INVSRCTYPE)

/*
 * The file operation fails because the file is invalid (0xB0008008).
 *
 */
#define HIGO_ERR_INVFILE higo_def_err(HIGO_MOD_COMM, ERR_COMM_INVFILE)

/*
 * The parameter is invalid (0xB0008009).
 *
 */
#define HIGO_ERR_INVPARAM higo_def_err(HIGO_MOD_COMM, ERR_COMM_INVPARAM)

/*
 * The handle is being used (0xB000800A).
 *
 */
#define HIGO_ERR_INUSE higo_def_err(HIGO_MOD_COMM, ERR_COMM_INUSE)

/*
 * The operation is invalid (0xB000800B).
 *
 */
#define HIGO_ERR_UNSUPPORTED higo_def_err(HIGO_MOD_COMM, ERR_COMM_UNSUPPORTED)

/*
 * An error occurs when the APIs related to the TDE are called (0xB000800C).
 *
 */
#define HIGO_ERR_DEPEND_TDE higo_def_err(HIGO_MOD_COMM, ERR_COMM_DEPENDTDE)

/*
 * An error occurs when the APIs related to the FB are called (0xB000800D).
 *
 */
#define HIGO_ERR_DEPEND_FB higo_def_err(HIGO_MOD_COMM, ERR_COMM_DEPENDFB)

/*
 * An error occurs when the APIs related to the MMZ are called (0xB000800E).
 *
 */
#define HIGO_ERR_DEPEND_MMZ higo_def_err(HIGO_MOD_COMM, ERR_COMM_DEPENDMMZ)

/*
 * An error occurs when the APIs related to .jpeg decoding are called (0xB000800F).
 *
 */
#define HIGO_ERR_DEPEND_JPEG higo_def_err(HIGO_MOD_COMM, ERR_COMM_DEPENDJPEG)

/*
 * An error occurs when the APIs related to .png decoding are called (0xB0008010).
 *
 */
#define HIGO_ERR_DEPEND_PNG higo_def_err(HIGO_MOD_COMM, ERR_COMM_DEPENDPNG)

/*
 * An error occurs when the APIs related to .bmp decoding are called (0xB0008011).
 *
 */
#define HIGO_ERR_DEPEND_BMP higo_def_err(HIGO_MOD_COMM, ERR_COMM_DEPENDBMP)

/*
 * An error occurs when the APIs related to .gif decoding are called (0xB0008012).
 *
 */
#define HIGO_ERR_DEPEND_GIF higo_def_err(HIGO_MOD_COMM, ERR_COMM_DEPENDGIF)

/*
 * An error occurs when the APIs related to the cursor are called (0xB0008013).
 *
 */
#define HIGO_ERR_DEPEND_CURSOR higo_def_err(HIGO_MOD_COMM, ERR_COMM_DEPENDCURSOR)

/*
 * An error occurs when the APIs related to .jpeg encoding are called (0xB0008014).
 *
 */
#define HIGO_ERR_DEPEND_JPGE higo_def_err(HIGO_MOD_COMM, ERR_COMM_DEPENDJPGENC)

/*
 * Error codes of the HiGo surface module
 *
 */
typedef enum {
    ERR_SURFACE_INVSURFACESIZE = 0,
    ERR_SURFACE_INVSURFACEPF,
    ERR_SURFACE_NOTLOCKED,
    ERR_SURFACE_LOCKED,
    ERR_SURFACE_NOCOLORKEY,
    ERR_SURFACE_BUTT
} higo_surface_err;

#define surface_def_err(err) higo_def_err(HIGO_MOD_SURFACE, err)

/*
 * The surface size is incorrect (0xB0018000).
 *
 */
#define HIGO_ERR_INVSURFACESIZE surface_def_err(ERR_SURFACE_INVSURFACESIZE)

/*
 * The pixel format of the surface is incorrect (0xB0018001).
 *
 */
#define HIGO_ERR_INVSURFACEPF surface_def_err(ERR_SURFACE_INVSURFACEPF)

/*
 * The surface cannot be unlocked because it is not locked (0xB0018002).
 *
 */
#define HIGO_ERR_NOTLOCKED surface_def_err(ERR_SURFACE_NOTLOCKED)

/*
 * The surface cannot be written because it is locked (0xB0018003).
 *
 */
#define HIGO_ERR_LOCKED surface_def_err(ERR_SURFACE_LOCKED)

/*
 * The surface does not contain the colorkey value (0xB0018004).
 *
 */
#define HIGO_ERR_NOCOLORKEY surface_def_err(ERR_SURFACE_NOCOLORKEY)

/*
 * Error codes of the HiGo Gdev module
 *
 */
typedef enum {
    ERR_LAYER_INVSIZE = 0,
    ERR_LAYER_INVLAYERID,
    ERR_LAYER_INVPIXELFMT,
    ERR_LAYER_FLUSHTYPE,
    ERR_LAYER_FREEMEM,
    ERR_LAYER_CLOSELAYER,
    ERR_LAYER_CANNOTCHANGE,
    ERR_LAYER_INVORDERFLAG,
    ERR_LAYER_SETALPHA,
    ERR_LAYER_ALREADYSHOW,
    ERR_LAYER_ALREADYHIDE,
    ERR_LAYER_INVLAYERPOS,
    ERR_LAYER_INVSURFACE,
    ERR_LAYER_INVLAYERSIZE,
    ERR_LAYER_INVFLUSHTYPE,
    ERR_LAYER_INVANILEVEL,
    ERR_LAYER_NOTOPEN,
    ERR_LAYER_FB_OPENFAILURE,
    ERR_LAYER_FB_GETVSCREENINFOFAILURE,
    ERR_LAYER_FB_PUTVSCREENINFOFAILURE,
    ERR_LAYER_FB_GETLAYERINFO,
    ERR_LAYER_FB_PUTLAYERINFO,
    ERR_LAYER_FB_GETLAYERALPHA,
    ERR_LAYER_FB_PUTLAYERALPHA,
    ERR_LAYER_FB_WRONG_LAYERID,
    ERR_LAYER_FB_REFRESHFAILURE,
    ERR_LAYER_FB_GETCANVASBUFFER,
    ERR_LAYER_FB_PUTCANVASBUFFER,
    ERR_LAYER_FB_GETZORDER,
    ERR_LAYER_FB_PUTZORDER,
    ERR_LAYER_BUTT
} higo_layer_err;

#define layer_def_err(err) higo_def_err(HIGO_MOD_LAYER, err)

/*
 * The layer size is invalid (0xB0038000).
 *
 */
#define HIGO_ERR_INVSIZE layer_def_err(ERR_LAYER_INVSIZE)

/*
 * The hardware layer ID is invalid (0xB0038001).
 *
 */
#define HIGO_ERR_INVLAYERID layer_def_err(ERR_LAYER_INVLAYERID)

/*
 * The pixel format is invalid (0xB0038002).
 *
 */
#define HIGO_ERR_INVPIXELFMT layer_def_err(ERR_LAYER_INVPIXELFMT)

/*
 * The layer refresh mode is incorrect (0xB0038003).
 *
 */
#define HIGO_ERR_INVFLUSHTYPE layer_def_err(ERR_LAYER_FLUSHTYPE)

/*
 * The display buffer fails to be released (0xB0038004).
 *
 */
#define HIGO_ERR_FREEMEM layer_def_err(ERR_LAYER_FREEMEM)

/*
 * The layer device fails to be stopped (0xB0038005).
 *
 */
#define HIGO_ERR_CLOSELAYERFAILED layer_def_err(ERR_LAYER_CLOSELAYER)

/*
 * The z-order of the graphics layer cannot be changed (0xB0038006).
 *
 */
#define HIGO_ERR_CANNOTCHANGE layer_def_err(ERR_LAYER_CANNOTCHANGE)

/*
 * The z-order change flag is invalid (0xB0038007).
 *
 */
#define HIGO_ERR_INVORDERFLAG layer_def_err(ERR_LAYER_INVORDERFLAG)

/*
 * The surface alpha value fails to be set (0xB0038008).
 *
 */
#define HIGO_ERR_SETALPHAFAILED layer_def_err(ERR_LAYER_SETALPHA)

/*
 * The graphics layer has been displayed (0xB0038009).
 *
 */
#define HIGO_ERR_ALREADYSHOW layer_def_err(ERR_LAYER_ALREADYSHOW)

/*
 * The graphics layer has been hidden (0xB003800A).
 *
 */
#define HIGO_ERR_ALREADYHIDE layer_def_err(ERR_LAYER_ALREADYHIDE)

/*
 * The start position of the graphics layer is invalid (0xB003800B).
 *
 */
#define HIGO_ERR_INVLAYERPOS layer_def_err(ERR_LAYER_INVLAYERPOS)

/*
 * Alignment fails because the surface is invalid (0xB003800C).
 *
 */
#define HIGO_ERR_INVLAYERSURFACE layer_def_err(ERR_LAYER_INVSURFACE)

/*
 * The anti-flicker level of the graphics layer is invalid (0xB003800F).
 *
 */
#define HIGO_ERR_INVANILEVEL layer_def_err(ERR_LAYER_INVANILEVEL)

/*
 * The graphics layer is not started (0xB0038010).
 *
 */
#define HIGO_ERR_NOTOPEN layer_def_err(ERR_LAYER_NOTOPEN)

/*
 * frame buffer: open fb device failure (0xB0038011).
 *
 */
#define HIGO_ERR_FB_OPEN_FAILURE                    layer_def_err(ERR_LAYER_FB_OPENFAILURE)

/*
 * frame buffer: get vscreeninfo failure (0xB0038012).
 *
 */
#define HIGO_ERR_FB_GET_VSCREENINFO_FAILURE         layer_def_err(ERR_LAYER_FB_GETVSCREENINFOFAILURE)

/*
 * frame buffer: put vscreeninfo failure (0xB0038013).
 *
 */
#define HIGO_ERR_FB_PUT_VSCREENINFO_FAILURE         layer_def_err(ERR_LAYER_FB_PUTVSCREENINFOFAILURE)

/*
 * frame buffer: get layer info failure (0xB0038014).
 *
 */
#define HIGO_ERR_FB_GET_LAYERINFO_FAILURE           layer_def_err(ERR_LAYER_FB_GETLAYERINFO)

/*
 * frame buffer: put layer info failure (0xB0038015).
 *
 */
#define HIGO_ERR_FB_PUT_LAYERINFO_FAILURE           layer_def_err(ERR_LAYER_FB_PUTLAYERINFO)

/*
 * frame buffer: get layer alpha failure (0xB0038016).
 *
 */
#define HIGO_ERR_FB_GET_LAYERALPHA_FAILURE          layer_def_err(ERR_LAYER_FB_GETLAYERALPHA)

/*
 * frame buffer: put layer alpha failure (0xB0038017).
 *
 */
#define HIGO_ERR_FB_PUT_LAYERALPHA_FAILURE          layer_def_err(ERR_LAYER_FB_PUTLAYERALPHA)

/*
 * frame buffer: wrong layre id (0xB0038018).
 *
 */
#define HIGO_ERR_FB_WRONG_LAYER_ID                  layer_def_err(ERR_LAYER_FB_WRONG_LAYERID)

/*
 * frame buffer: refresh failure (0xB0038019).
 *
 */
#define HIGO_ERR_FB_REFRESH_FAILURE                 layer_def_err(ERR_LAYER_FB_REFRESHFAILURE)

/*
 * frame buffer: get canvas buffer failure (0xB003801A).
 *
 */
#define HIGO_ERR_FB_GET_CANVAS_BUFFER_FAILURE       layer_def_err(ERR_LAYER_FB_GETCANVASBUFFER)

/*
 * frame buffer: put canvas buffer failure (0xB003801B).
 *
 */
#define HIGO_ERR_FB_PUT_CANVAS_BUFFER_FAILURE       layer_def_err(ERR_LAYER_FB_PUTCANVASBUFFER)

/*
 * frame buffer: get zorder failure (0xB003801C).
 *
 */
#define HIGO_ERR_FB_GET_ZORDER_FAILURE              layer_def_err(ERR_LAYER_FB_GETZORDER)

/*
 * frame buffer: put zorder failure (0xB003801D).
 *
 */
#define HIGO_ERR_FB_PUT_ZORDER_FAILURE              layer_def_err(ERR_LAYER_FB_PUTZORDER)

/*
 * Error codes of the HiGo Bliter module
 *
 */
typedef enum {
    ERR_BLITER_INVCOMPTYPE = 0,
    ERR_BLITER_INVCKEYTYPE,
    ERR_BLITER_INVMIRRORTYPE,
    ERR_BLITER_INVROTATETYPE,
    ERR_BLITER_INVROPTYPE,
    ERR_BLITER_INVSCALING,
    ERR_BLITER_OUTOFBOUNDS,
    ERR_BLITER_EMPTYRECT,
    ERR_BLITER_OUTOFPAL,
    ERR_BLITER_NOP,
    ERR_BLITER_BUTT
} higo_bliter_err;

/*
 * The blending mode is incorrect (0xB0048000).
 *
 */
#define HIGO_ERR_INVCOMPTYPE higo_def_err(HIGO_MOD_BLITER, ERR_BLITER_INVCOMPTYPE)

/*
 * The colorkey operation is invalid (0xB0048001).
 *
 */
#define HIGO_ERR_INVCKEYTYPE higo_def_err(HIGO_MOD_BLITER, ERR_BLITER_INVCKEYTYPE)

/*
 * The mirror operation is invalid (0xB0048002).
 *
 */
#define HIGO_ERR_INVMIRRORTYPE higo_def_err(HIGO_MOD_BLITER, ERR_BLITER_INVMIRRORTYPE)

/*
 * The rotation operation is invalid (0xB0048003).
 *
 */
#define HIGO_ERR_INVROTATETYPE higo_def_err(HIGO_MOD_BLITER, ERR_BLITER_INVROTATETYPE)

/*
 * The ROP operation is invalid (0xB0048004).
 *
 */
#define HIGO_ERR_INVROPTYPE higo_def_err(HIGO_MOD_BLITER, ERR_BLITER_INVROPTYPE)

/*
 * The scaling is abnormal (0xB0048005).
 *
 */
#define HIGO_ERR_INVSCALING higo_def_err(HIGO_MOD_BLITER, ERR_BLITER_INVSCALING)

/*
 * The rectangle exceeds the boundary (0xB0048006).
 *
 */
#define HIGO_ERR_OUTOFBOUNDS higo_def_err(HIGO_MOD_BLITER, ERR_BLITER_OUTOFBOUNDS)

/*
 * The rectangle is empty (0xB0048007).
 *
 */
#define HIGO_ERR_EMPTYRECT higo_def_err(HIGO_MOD_BLITER, ERR_BLITER_EMPTYRECT)

/*
 * The palette does not contain this color (0xB0048008).
 *
 */
#define HIGO_ERR_OUTOFPAL higo_def_err(HIGO_MOD_BLITER, ERR_BLITER_OUTOFPAL)

/*
 * Error codes of the HiGo decoder
 *
 */
typedef enum {
    ERR_DEC_INVIMAGETYPE = 0,
    ERR_DEC_INVINDEX,
    ERR_DEC_INVIMGDATA,
    ERR_DEC_BUTT
} higo_err_dec;

/*
 * The picture format is invalid (0xB0058000).
 *
 */
#define HIGO_ERR_INVIMAGETYPE higo_def_err(HIGO_MOD_DEC, ERR_DEC_INVIMAGETYPE)

/*
 * The picture index number is invalid (0xB0058001).
 *
 */
#define HIGO_ERR_INVINDEX higo_def_err(HIGO_MOD_DEC, ERR_DEC_INVINDEX)

/*
 * The picture data is invalid (0xB0058002).
 *
 */
#define HIGO_ERR_INVIMGDATA higo_def_err(HIGO_MOD_DEC, ERR_DEC_INVIMGDATA)

/*
 * Error codes of the HiGo textout module
 *
 */
typedef enum {
    ERR_TEXTOUT_INVRECT = 0,
    ERR_TEXTOUT_UNSUPPORT_CHARSET,
    ERR_TEXTOUT_ISUSING,
    ERR_TEXTOUT_NOIMPLEMENT,
    ERR_TEXTOUT_SHAPE,
    ERR_TEXTOUT_MAX_CHAR,
    ERR_TEXTOUT_CHAR_SET,
    ERR_TEXTOUT_BIDI,
    ERR_TEXTOUT_ERRCODE_MAX = 0x1F,
    ERR_TEXTOUT_INTERNAL,
    ERR_TEXTOUT_BUTT
} higo_textout_err;

/*
 * The rectangle region is invalid (0xB0068000).
 *
 */
#define HIGO_ERR_INVRECT higo_def_err(HIGO_MOD_TEXTOUT, ERR_TEXTOUT_INVRECT)

/*
 * The character set is not supported (0xB0068001).
 *
 */
#define HIGO_ERR_UNSUPPORT_CHARSET higo_def_err(HIGO_MOD_TEXTOUT, ERR_TEXTOUT_UNSUPPORT_CHARSET)

/*
 * The character set is not supported (0xB0068002).
 *
 */
#define HIGO_ERR_ISUSING higo_def_err(HIGO_MOD_TEXTOUT, ERR_TEXTOUT_ISUSING)
/*
 * The function not implement yet (0xB0068003)
 *
 */
#define HIGO_ERR_NOIMPLEMENT  higo_def_err(HIGO_MOD_TEXTOUT, ERR_TEXTOUT_NOIMPLEMENT)
/*
 * Shape Failed (0xB0068004)
 *
 */
#define HIGO_ERR_SHAPEFAILED higo_def_err(HIGO_MOD_TEXTOUT, ERR_TEXTOUT_SHAPE)

/*
 * Number of characters greater than limit (0xB0068005)
 *
 */
#define HIGO_ERR_MAX_CHAR higo_def_err(HIGO_MOD_TEXTOUT, ERR_TEXTOUT_MAX_CHAR)

/*
 * Char set error (0xB0068006)
 *
 */
#define HIGO_ERR_CHAR_SET higo_def_err(HIGO_MOD_TEXTOUT, ERR_TEXTOUT_CHAR_SET)

/*
 * bi-directional process error(0xB)068007)
 *
 */
#define HIGO_ERROR_BIDI higo_def_err(HIGO_MOD_TEXTOUT, ERR_TEXTOUT_BIDI)

/*
 * Internal error (0xB006801F)
 *
 */
#define HIGO_ERR_TEXTINTERNAL higo_def_err(HIGO_MOD_TEXTOUT, ERR_TEXTOUT_INTERNAL)

/*
 * Error codes of the HiGo Winc module
 *
 */
typedef enum {
    ERR_WINC_ALREADYBIND = 0, /* The Winc module has been attached. */
    ERR_WINC_INVZORDERTYPE,   /* The z-order adjustment mode is invalid. */
    ERR_WINC_NOUPDATE,        /* The desktop is not refreshed. */
    ERR_WINC_INVPF,           /* The pixel format is invalid. */
    ERR_WINC_INVTREE,         /* The window tree is invalid. */
    ERR_WINC_ALREADYSETMODE,  /* The window already be set mode. */
    ERR_WINC_BUTT
} higo_err_winc;

/*
 * The desktop has been attached to a graphics layer (0xB0078000).
 *
 */
#define HIGO_ERR_ALREADYBIND higo_def_err(HIGO_MOD_WINC, ERR_WINC_ALREADYBIND)

/*
 * The z-order adjustment mode is invalid (0xB0078001).
 *
 */
#define HIGO_ERR_INVZORDERTYPE higo_def_err(HIGO_MOD_WINC, ERR_WINC_INVZORDERTYPE)

/*
 * The desktop is not refreshed (0xB0078002).
 *
 */
#define HIGO_ERR_NOUPDATE higo_def_err(HIGO_MOD_WINC, ERR_WINC_NOUPDATE)

/*
 * The desktop is not refreshed (0xB0078003).
 *
 */
#define HIGO_ERR_INVPF higo_def_err(HIGO_MOD_WINC, ERR_WINC_INVPF)

/*
 * The desktop is not refreshed (0xB0078004).
 *
 */
#define HIGO_ERR_INVTREE higo_def_err(HIGO_MOD_WINC, ERR_WINC_INVTREE)

/*
 * The window already be set mode (0xB0078005)
 *
 */
#define HIGO_ERR_ALREADYSETMODE higo_def_err(HIGO_MOD_WINC, ERR_WINC_ALREADYSETMODE)

/*
 * Error codes of the HiGo surface module
 *
 */
typedef enum {
    ERR_CURSOR_INVHOTSPOT = 0,
    ERR_CURSOR_NOCURSORINFO,
    ERR_CURSOR_BUTT
} higo_cursor_err;

/*
 * The hot spot coordinate of the cursor is invalid (0xB0088000)
 *
 */
#define HIGO_ERR_INVHOTSPOT  higo_def_err(HIGO_MOD_CURSOR, ERR_CURSOR_INVHOTSPOT)

/*
 * The cursor information is not set (0xB0088001).
 *
 */
#define HIGO_ERR_NOCURSORINF higo_def_err(HIGO_MOD_CURSOR, ERR_CURSOR_NOCURSORINFO)

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* HI_GO_ERRNO_H */
