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
#ifndef HI_GO_TEXT_H
#define HI_GO_TEXT_H

#include "hi_go_comm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define LAYOUT_LEFT HIGO_LAYOUT_LEFT                 /* Horizontally left */
#define LAYOUT_RIGHT HIGO_LAYOUT_RIGHT               /* Horizontally right */
#define LAYOUT_HCENTER HIGO_LAYOUT_HCENTER           /* Horizontally center */
#define LAYOUT_TOP HIGO_LAYOUT_TOP                   /* Vertically top */
#define LAYOUT_BOTTOM HIGO_LAYOUT_BOTTOM             /* Vertically bottom */
#define LAYOUT_VCENTER HIGO_LAYOUT_VCENTER           /* Vertically center */
#define LAYOUT_WRAP HIGO_LAYOUT_WRAP                 /* Wrap */
#define LAYOUT_WORDELLIPSIS HIGO_LAYOUT_WORDELLIPSIS /* Ellipsis format */
#define LAYOUT_FORCE_LEFT HIGO_LAYOUT_FORCE_LEFT     /* Force Horizontally left */
#define LAYOUT_FORCE_RIGHT HIGO_LAYOUT_FORCE_RIGHT   /* Force Horizontally right */

typedef enum {
    HIGO_LAYOUT_LEFT = 0x0001,
    HIGO_LAYOUT_RIGHT = 0x0002,
    HIGO_LAYOUT_HCENTER = 0x0004,
    HIGO_LAYOUT_WRAP = 0x0008,
    HIGO_LAYOUT_WORDELLIPSIS = 0x0010,
    HIGO_LAYOUT_TOP = 0x0100,
    HIGO_LAYOUT_BOTTOM = 0x0200,
    HIGO_LAYOUT_VCENTER = 0x0400,
    HIGO_LAYOUT_FORCE_LEFT = 0x8000,
    HIGO_LAYOUT_FORCE_RIGHT = 0x10000,
    HIGO_LAYOUT_VERTICAL = 0x20000,
    HIGO_LAYOUT_BUTT = 0x40000,
} HIGO_LAYOUT_E;

/* *< Definition of a character set */
typedef enum {
    HIGO_CHARSET_UNKOWN,
    HIGO_CHARSET_ASCI,
    HIGO_CHARSET_GB2312,
    HIGO_CHARSET_UTF8,
    HIGO_CHARSET_UTF16,
    HIGO_CHARSET_UCS2 = HIGO_CHARSET_UTF16,
    HIGO_CHARSET_UTF32,
    HIGO_CHARSET_BUTT,
} HIGO_CHARSET_E;

/* Definition of a text base direction */
typedef enum {
    HIGO_TEXT_DIR_NEUTRAL = 0, /* neutral deirction */
    HIGO_TEXT_DIR_LTR,         /* Left to right */
    HIGO_TEXT_DIR_RTL,         /* Right to left */
    HIGO_TEXT_DIR_SYSTEM,
    HIGO_TEXT_DIR_BUTT
} HIGO_TEXT_DIR_E;


typedef struct {
    HIGO_CHARSET_E Charset; /* Character set name */
    HI_U8 Height;           /* Font Height */
    HI_U8 MaxWidth;         /* Maximum font width */
} HIGO_FONTATTR_S;

/* Attributes of a text output object */
typedef struct {
    HI_COLOR BgColor;           /* Background color */
    HI_COLOR FgColor;           /* Foreground color */
    HIGO_FONTATTR_S SbFontAttr; /* Font attributes of the SBC */
    HIGO_FONTATTR_S MbFontAttr; /* Font attributes of the MBC */
} HIGO_TEXTOUTATTR_S;

typedef enum {
    HIGO_TEXT_STYLE_NORMAL = 0x00,         /* Normal font */
    HIGO_TEXT_STYLE_ITALIC = 0x01,         /* Italic font */
    HIGO_TEXT_STYLE_BOLD = 0x02,           /* Bold font */
    HIGO_TEXT_STYLE_HALF_HEIGHT = 0x04,    /* Half-height font */
    HIGO_TEXT_STYLE_HALF_WIDTH = 0x08,     /* Half-width font */
    HIGO_TEXT_STYLE_DOUBLE_HEIGHT = 0x10,  /* Double-height font */
    HIGO_TEXT_STYLE_DOUBLE_WIDTH = 0x20,   /* Double-width font */
    HIGO_TEXT_STYLE_BUTT = 0x40
} HIGO_TEXT_STYLE_E;

typedef struct {
    HI_HANDLE hFont;          /* Handle of font file */
    HI_U32 Style;             /* Layout Style */
    HI_S32 s32Width;          /* Width of display arear */
    HI_S32 s32Height;         /* Height of display arear */
    HI_S32 horz_dist;         /* Character horizontal spacing */
    HI_S32 vert_dist;         /* Raw vertical spacing */
    HI_BOOL single_para_flg;  /* Whether it is a single paragraph */
    HI_BOOL auto_dir;         /* Whether it is automatic direction */
    HIGO_TEXT_DIR_E base_dir; /* Base direction */
    HI_U32 max_char;          /* Max character */
    HI_COLOR select_bg;       /* Selected background color */
    HI_COLOR select_fg;       /* Selected frontground color */
} HIGO_LAYOUT_INFO_S;

typedef struct {
    const HI_CHAR *pSbcFontFile; /* Single-byte character font file. */
    const HI_CHAR *pMbcFontFile; /* Multi-byte character font file. */
    HI_U32 u32Size; /* This value is ignored for the dot-matrix font. */
} HIGO_TEXT_INFO_S;

typedef enum {
    HIGO_LAYOUT_MOVECURSOR_UP,     /* move cursor to up position */
    HIGO_LAYOUT_MOVECURSOR_DOWN,   /* move cursor to down position */
    HIGO_LAYOUT_MOVECURSOR_PREV,   /* move cursor to previous position */
    HIGO_LAYOUT_MOVECURSOR_NEXT,   /* move cursor to next position */
    HIGO_LAYOUT_MOVECURSOR_LEFT,   /* move cursor to left position */
    HIGO_LAYOUT_MOVECURSOR_RIGHT,  /* move cursor to right position */
    HIGO_LAYOUT_MOVECURSOR_SOT,    /* move cursor to start position of text */
    HIGO_LAYOUT_MOVECURSOR_EOT,    /* move curosr to end position of text */
    HIGO_LAYOUT_MOVECURSOR_SOL,    /* move cursor to start of current line */
    HIGO_LAYOUT_MOVECURSOR_EOL,    /* move cursor to end of current line */
    HIGO_LAYOUT_MOVECURSOR_SOP,    /* move cursor to start of current paragraph */
    HIGO_LAYOUT_MOVECURSOR_EOP,    /* move cursor to end of current paragraph */
    HIGO_LAYOUT_MOVECURSOR_BUTT,
} HIGO_TEXT_CURSOR_DIR_E;

HI_S32 HI_GO_CreateText(const HI_CHAR *pSbFontFile, const HI_CHAR *pMbFontFile, HI_HANDLE *pTextOut);

HI_S32 HI_GO_DestroyText(HI_HANDLE TextOut);

HI_S32 HI_GO_SetInputCharSet(HI_HANDLE TextOut, HIGO_CHARSET_E CharSet);

HI_S32 HI_GO_GetTextAttr(HI_HANDLE TextOut, HIGO_TEXTOUTATTR_S *pTextOutAttr);

HI_S32 HI_GO_GetTextExtent(HI_HANDLE TextOut, const HI_CHAR *pText, HI_S32 *pWidth, HI_S32 *pHeight);

HI_S32 HI_GO_GetVerticalTextExtent(HI_HANDLE hFont, const HI_CHAR *pText, HI_S32 *pTextWidth, HI_S32 *pHeight,
    HI_S32 *pLayoutAreaWidth);

HI_S32 HI_GO_SetTextBGTransparent(HI_HANDLE TextOut, HI_BOOL bTransparent);

HI_S32 HI_GO_SetTextBGColor(HI_HANDLE TextOut, HI_COLOR Color);

hi_s32 HI_GO_SetTextSize(HI_HANDLE font_handle, hi_u32 size);

hi_s32 HI_GO_GetTextSize(HI_HANDLE font_handle, hi_u32 *size);

HI_S32 HI_GO_SetTextColor(HI_HANDLE TextOut, HI_COLOR Color);

HI_S32 HI_GO_TextOut(HI_HANDLE TextOut, HI_HANDLE Surface, const HI_CHAR *pText, const HI_RECT *pRect);

HI_S32 HI_GO_TextOutEx(HI_HANDLE hTextOut, HI_HANDLE hSurface, const HI_CHAR *pText, const HI_RECT *pRect,
    HIGO_LAYOUT_E Style);

HI_S32 HI_GO_GetTextExtentEx(HI_HANDLE TextOut, const HI_CHAR *pText, const HI_RECT *pRect, HI_U32 *pLen);

HI_S32 HI_GO_CreateTextEx(const HIGO_TEXT_INFO_S *pInfo, HI_HANDLE *phText);

HI_S32 HI_GO_SetTextStyle(HI_HANDLE hTextOut, HIGO_TEXT_STYLE_E eStyle);

HI_S32 HI_GO_SetBaseDir(HI_HANDLE TextOut, HIGO_TEXT_DIR_E eBaseDir);

HI_S32 HI_GO_SetCharExtra(HI_HANDLE hTextOut, HI_U32 u32Distance);

HI_S32 HI_GO_SetLineExtra(HI_HANDLE hText, HI_U32 u32Distance);

HI_S32 HI_GO_InitText(void);

HI_S32 HI_GO_DeinitText(void);

HI_S32 HI_GO_MoveCursor2Point(HI_HANDLE hlayout, const HI_POINT *pPoint);

HI_S32 HI_GO_LayoutInsertText(HI_HANDLE hlayout, const HI_CHAR *pText, HI_U32 text_len);

HI_S32 HI_GO_LayoutGetLineHeight(HI_HANDLE hlayout, HI_S32 line_no);

/* 
 * param[in] hLayout  Text layout handle.
 * param[in] hFont    Text output handle.
 * param[in] hSurface Surface handle.
 * param[in] pText    Text contents ending with /0.
 * param[in] pRect    Text output region.
 * param[in] eLayout  Styles and formatting including LAYOUT_LEFT, LAYOUT_RIGHT,
 *                    and LAYOUT_HCENTER.
 * retval ::HI_SUCCESS
 * retval ::HIGO_ERR_INVPARAM
 * retval ::HIGO_ERR_INVHANDLE
 * retval ::HIGO_ERR_NULLPTR
 */
HI_S32 HI_GO_TextLayoutTextOut(HI_HANDLE hTextLayout, HI_HANDLE hFont, HI_HANDLE hSurface, const HI_CHAR *pText,
    const HI_RECT *pRect, HIGO_LAYOUT_E eLayout);

HI_BOOL HI_GO_NeedDrawCursor(HI_HANDLE hlayout);

HI_S32 HI_GO_GetCursorLinearPos(HI_HANDLE hLayout);

/* 
 * param[in] hFont Handle of a text output object.
 * param[in] pText Text contents.
 * param[in] pRect Text output region.
 * param[in] eLayout Styles and formatting including LAYOUT_LEFT, LAYOUT_RIGHT, and LAYOUT_HCENTER.
 * param[out] pRcText Actual rectangle of text contents. It cannot be empty.
 * param[out] pLen    Pointer to the string length.
 * retval ::HI_SUCCESS
 * retval ::HIGO_ERR_INVPARAM
 * retval ::HIGO_ERR_INVHANDLE
 */
HI_S32 HI_GO_GetTextExtentEx2(HI_HANDLE hFont, const HI_CHAR *pText, const HI_RECT *pRect, HIGO_LAYOUT_E eLayout,
    HI_RECT *pRcText, HI_U32 *pLen);

HI_S32 HI_GO_CursorMove(HI_HANDLE hlayout, HIGO_TEXT_CURSOR_DIR_E Dir);

HI_S32 HI_GO_CursorGetScreenPos(HI_HANDLE hlayout, HI_RECT *pRect);

HI_S32 HI_GO_CreateLayout(HIGO_LAYOUT_INFO_S *pLayoutInfo, HI_HANDLE *hlayout);

HI_S32 HI_GO_LayoutGetText(HI_HANDLE hlayout, HI_S32 *length, HI_CHAR **str);

HI_S32 HI_GO_LayoutClearText(HI_HANDLE hlayout);

HI_S32 HI_GO_LayoutDeleteText(HI_HANDLE hlayout, HI_S32 sCount);

HI_S32 HI_GO_LayoutGetRect(HI_HANDLE hlayout, HI_RECT *pRect);

HI_S32 HI_GO_SetViewPort(HI_HANDLE hlayout, const HI_POINT *pPos);

HI_S32 HI_GO_SetViewPortSize(HI_HANDLE hlayout, HI_S32 w, HI_S32 h);

HI_S32 HI_GO_GetViewPort(HI_HANDLE hlayout, HI_POINT *pPos);

HI_S32 HI_GO_SetViewPortEx(HI_HANDLE hlayout, const HI_RECT *pPos);

HI_S32 HI_GO_LayoutTextOut(HI_HANDLE hlayout, HI_HANDLE hSurface, const HI_RECT *pRect);

HI_S32 HI_GO_DestroyLayout(HI_HANDLE hlayout);

HI_S32 HI_GO_SetSelecting(HI_HANDLE hlayout, const HI_POINT *pos);

HI_S32 HI_GO_SetSelected(HI_HANDLE hlayout, const HI_POINT *pos);

HI_S32 HI_GO_SelectFinish(HI_HANDLE hlayout);

HI_S32 HI_GO_GetSelectString(HI_HANDLE hlayout, HI_CHAR **str, HI_U32 *len);

HI_S32 HI_GO_SetSelectColor(HI_HANDLE hlayout, HI_COLOR fg, HI_COLOR bg);

HI_S32 HI_GO_DeleteSelectString(HI_HANDLE hlayout);

HI_S32 HI_GO_LayoutGetLineNum(HI_HANDLE hlayout);

HI_S32 HI_GO_LayoutSetFont(HI_HANDLE hlayout, HI_HANDLE hFont);

HI_S32 HI_GO_LayoutAddText(HI_HANDLE hlayout, const HI_CHAR *pText);

HI_BOOL HI_GO_CursorInSot(HI_HANDLE hLayout);

HI_BOOL HI_GO_CursorInEot(HI_HANDLE hLayout);

#ifdef __cplusplus
#if __cplusplus
}
#endif 
#endif

#endif /* HI_GO_TEXT_H */
