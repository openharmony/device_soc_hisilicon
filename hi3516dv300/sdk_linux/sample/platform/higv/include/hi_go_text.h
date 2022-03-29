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

#ifndef __HI_GO_TEXT_H__
#define __HI_GO_TEXT_H__

#include "hi_go_comm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

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
    HIGO_LAYOUT_MOVECURSOR_RIGHT,  /* move cursor to right positon */
    HIGO_LAYOUT_MOVECURSOR_SOT,    /* move cursor to start position of text */
    HIGO_LAYOUT_MOVECURSOR_EOT,    /* move curosr to end position of text */
    HIGO_LAYOUT_MOVECURSOR_SOL,    /* move cursor to start of current line */
    HIGO_LAYOUT_MOVECURSOR_EOL,    /* move cursor to end of current line */
    HIGO_LAYOUT_MOVECURSOR_SOP,    /* move cursor to start of current paragraph */
    HIGO_LAYOUT_MOVECURSOR_EOP,    /* move cursor to end of current paragraph */
    HIGO_LAYOUT_MOVECURSOR_BUTT,
} HIGO_TEXT_CURSOR_DIR_E;

/* *
\brief Creates a text output object.
\attention \n
The values of pSbFontFile and pMbFontFile cannot be empty at the same time. If the received file name is in vector
font, the font height is set based on the 22-dot array.

22󴴽 CNend
\param[in] pSbFontFile Font file of the single-byte character set. If the value is empty, it indicates that the single
-byte character set library is not used.
\param[in] pMbFontFile  Font file of the multi-byte character set. If the value is empty, it indicates that the multi-
byte character set library is not used. Ensure that the font files of the single-byte and multi-byte character sets
cannot be empty concurrently.
ַֽļΪձʾʹöַֽֿ⣬ǲ뵥ַֽļͬʱΪա CNend \param[out]  pTextOut
Handle of a text output object.
\retval ::HI_SUCCESS
\retval ::HIGO_ERR_NULLPTR
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_NOMEM
\retval ::HIGO_ERR_INVFILE
\retval ::HIGO_ERR_INVPARAM
\retval ::HIGO_ERR_NOMEM
\see \n
::HI_GO_DestroyText
*/
HI_S32 HI_GO_CreateText(const HI_CHAR *pSbFontFile, const HI_CHAR *pMbFontFile, HI_HANDLE *pTextOut);

/* *
\brief Destroys a text output object.
\attention \n
N/A.
\param[in] TextOut Handle of a text output object.
\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE
\see \n
::HI_GO_CreateText
*/
HI_S32 HI_GO_DestroyText(HI_HANDLE TextOut);

/* *
\brief set the input charset.
\attention \n
N/A.
\param[in] TextOut Handle of a text output object.
\param[out] CharSet  charset of input text.
\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_UNSUPPORT_CHARSET
\see \n
::HI_GO_CreateText
*/
HI_S32 HI_GO_SetInputCharSet(HI_HANDLE TextOut, HIGO_CHARSET_E CharSet);

/* *
\brief Obtains the attributes of a text output object.
\attention \n
N/A.
\param[in] TextOut Handle of a text output object.
\param[out] pTextOutAttr  Attributes of a text output object.
\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVPARAM
\retval ::HIGO_ERR_NULLPTR
\retval ::HIGO_ERR_INVHANDLE
\see \n
N/A.
*/
HI_S32 HI_GO_GetTextAttr(HI_HANDLE TextOut, HIGO_TEXTOUTATTR_S *pTextOutAttr);

/* *
\brief Obtains the width and height of text contents.
\attention \n
N/A.
\param[in] TextOut Handle of a text output object.
\param[in] pText Text contents.
\param[out] pWidth Width of text contents. It cannot be empty.
\param[out] pHeight Height of text contents. It cannot be empty.
\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVPARAM
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_UNSUPPORT_CHARSET
\see \n
N/A.
*/
HI_S32 HI_GO_GetTextExtent(HI_HANDLE TextOut, const HI_CHAR *pText, HI_S32 *pWidth, HI_S32 *pHeight);

/* *
\brief Obtains the width and height of vertical text contents.
\attention \n
N/A.
\param[in] TextOut Handle of a text output object.
\param[in] pText Text contents.
\param[out] pTextWidth Actual width of text contents. It cannot be empty.
\param[out] pHeight Height of text contents. It cannot be empty.
\param[out] pLayoutAreaWidth Minimal width of layout area. It cannot be empty.
CNend
\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVPARAM
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_UNSUPPORT_CHARSET
\see \n
N/A.
*/
HI_S32 HI_GO_GetVerticalTextExtent(HI_HANDLE hFont, const HI_CHAR *pText, HI_S32 *pTextWidth, HI_S32 *pHeight,
    HI_S32 *pLayoutAreaWidth);

/* *
\brief Sets to display the background color of a text output object.
\attention \n
N/A.
\param[in] TextOut Handle of a text output object.
\param[in] bTransparent  Whether to display the background color of a text output object. If the value is set to
HI_TRUE, the background color is not displayed. The default value is
HI_TRUE.
\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE
\see \n
N/A
*/
HI_S32 HI_GO_SetTextBGTransparent(HI_HANDLE TextOut, HI_BOOL bTransparent);

/* *
\brief Sets the background color of a text output object.
\attention \n
If the target surface is a palette, the color is the palette
index.
object.
object.
\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE
\see \n
N/A.
*/
HI_S32 HI_GO_SetTextBGColor(HI_HANDLE TextOut, HI_COLOR Color);

/* *
\brief Sets size of a text output object.
\attention \n
\param[in] font_handle Handle of a text output object.
\param[in] size  size a text output object.
\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE
\see \n
N/A.
*/
hi_s32 HI_GO_SetTextSize(HI_HANDLE font_handle, hi_u32 size);

/* *
\brief get size of a text output object.
\attention \n
\param[in] font_handle Handle of a text output object.
\param[in] size  size a text output object.
\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE
\see \n
N/A.
*/
hi_s32 HI_GO_GetTextSize(HI_HANDLE font_handle, hi_u32 *size);

/* *
\brief Sets the font color of a text output object.
\attention \n
If the target surface is a palette, the color is the palette index.
Colorǵɫ CNend \param[in] TextOut Handle of a text output object.
\param[in] Color  Font color of a text output object.
\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE
\see \n
N/A.
*/
HI_S32 HI_GO_SetTextColor(HI_HANDLE TextOut, HI_COLOR Color);

/* *
\brief Outputs text contents to a specified surface.
\attention
N/A.
\param[in] TextOut Handle of a text output object.
\param[in] Surface Surface handle.
\param[in] pText  Text contents.
\param[in] pRect Text output region. If the value is empty, it indicates that text contents are output from the
surface origin.
\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVPARAM
\retval ::HIGO_ERR_INVHANDLE
\see \n
N/A.
*/
HI_S32 HI_GO_TextOut(HI_HANDLE TextOut, HI_HANDLE Surface, const HI_CHAR *pText, const HI_RECT *pRect);

/* *
\brief Outputs texts. This API is an extended API. You can call this API to output contents based on customized
styles and formatting.
\param[in] hTextOut Text output handle.
\param[in] hSurface Surface handle.
\param[in] pText    Text contents ending with /0.
\param[in] pRect    Text output region.
\param[in] Style    Styles and formatting including LAYOUT_LEFT, LAYOUT_RIGHT, and
LAYOUT_HCENTER.
\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVPARAM
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_NULLPTR
\see \n
N/A.
*/
HI_S32 HI_GO_TextOutEx(HI_HANDLE hTextOut, HI_HANDLE hSurface, const HI_CHAR *pText, const HI_RECT *pRect,
    HIGO_LAYOUT_E Style);

/* *
\brief Calculates the number of characters in a specified text output region.
CNend \attention \n N/A.
\param[in] pText    Text contents ending with /0.
\param[in] pRect    Text output region. The value cannot be empty.
\param[in] pLen      Pointer to the string length.
\retval ::HIGO_ERR_INVPARAM
\retval ::HIGO_ERR_INVHANDLE
\retval ::HI_SUCCESS
\see \n
N/A.
*/
HI_S32 HI_GO_GetTextExtentEx(HI_HANDLE TextOut, const HI_CHAR *pText, const HI_RECT *pRect, HI_U32 *pLen);

/* *
\brief Creates a font.
\attention \n
N/A.
\param[in] phText Text output handle.
\param[in] pInfo  Text information.
\retval ::HI_SUCCESS A font is created successfully.
\retval ::HI_FAILURE A font fails to be created.
\retval ::HIGO_ERR_INVPARAM
\retval ::HIGO_ERR_NOMEM
\retval ::HIGO_ERR_INVHANDLE
\see \n
N/A.
*/
HI_S32 HI_GO_CreateTextEx(const HIGO_TEXT_INFO_S *pInfo, HI_HANDLE *phText);

/* *
\brief Sets the style of a font, such as bold, italic, or normal.
\attention \n
N/A.
\param[in] hTextOut Text output handle.
\param[in] eStyle   Font style.
\retval ::HI_SUCCESS Success.
\retval ::HIGO_ERR_INVHANDLE
\see \n
N/A.
*/
HI_S32 HI_GO_SetTextStyle(HI_HANDLE hTextOut, HIGO_TEXT_STYLE_E eStyle);

/* *
\brief Sets the the text base direction
lines.
\attention \n
N/A.
\param[in] hTextOut Text output handle.
\param[in] BaseDir the base direction of the text.
\retval ::HI_SUCCESS Success.
\retval ::HIGO_ERR_INVHANDLE Failure.
\see \n
N/A.
*/
HI_S32 HI_GO_SetBaseDir(HI_HANDLE TextOut, HIGO_TEXT_DIR_E eBaseDir);

/* *
\brief Sets the horizontal spacing between displayed characters, that is, the horizontal spacing between character
lines.
\attention \n
N/A.
\param[in] hTextOut Text output handle.
\param[in] u32Distance Horizontal spacing.
\retval ::HI_SUCCESS Success.
\retval ::HIGO_ERR_INVHANDLE Failure.
\see \n
N/A.
*/
HI_S32 HI_GO_SetCharExtra(HI_HANDLE hTextOut, HI_U32 u32Distance);

/* *
\brief Sets the vertical and horizontal spacing.
\attention \n
This API is available only when the font style is changed.
\param[in] hTextOut Text output handle.
\param[in] u32Distance Vertical spacing.
\retval ::HI_SUCCESS Success.
\retval ::HIGO_ERR_INVHANDLE Failure.
\see \n
N/A.
*/
HI_S32 HI_GO_SetLineExtra(HI_HANDLE hText, HI_U32 u32Distance);

/* *
\brief Initializes the character module.
\attention \n
N/A.
\param N/A.
\retval ::HI_SUCCESS Success.
\retval ::HI_FAILURE Failure.
\retval ::HIGO_ERR_DEPEND_TDE
\see \n
N/A.
*/
HI_S32 HI_GO_InitText(void);

/* *
\brief Deinitializes the character module.
\attention \n
N/A.
\param N/A.
\retval ::HI_SUCCESS Success.
\retval ::HIGO_ERR_NOTINIT Failure.
\see \n
N/A.
*/
HI_S32 HI_GO_DeinitText(void);

/* *
\brief Move coursor to a point
\attention \n
N/A.
\param[in] hlayout handle of layout context CNcommnet Űľ CNend
\param[in] ppoint  target point CNcommnet Ŀ CNend
\retval ::HI_SUCCESS Success.
\retval ::HIGO_ERR_NOTINIT Failure.
\see \n
N/A.
*/
HI_S32 HI_GO_MoveCursor2Point(HI_HANDLE hlayout, const HI_POINT *pPoint);

/* *
\brief insert text to context at current cursor pos
\attention \n
N/A.
\param[in] hlayout handle of layout context
\param[in] pText UTF-8 string to insert
\retval ::HI_SUCCESS Success.
\retval ::HIGO_ERR_NOTINIT Failure.
\see \n
N/A.
*/
HI_S32 HI_GO_LayoutInsertText(HI_HANDLE hlayout, const HI_CHAR *pText, HI_U32 text_len);

/* *
\brief Get line height of context
\attention \n
N/A.
\param[in] hlayout handle of layout context
\param[in] line_no index of line
\retval ::height of line or 0 if failed.
\see \n
N/A.
*/
HI_S32 HI_GO_LayoutGetLineHeight(HI_HANDLE hlayout, HI_S32 line_no);

/* *
\brief Outputs texts. You can call this API to output contents based on customized
styles and formatting.
\param[in] hLayout  Text layout handle.
\param[in] hFont    Text output handle.
\param[in] hSurface Surface handle.
\param[in] pText    Text contents ending with /0.
\param[in] pRect    Text output region.
\param[in] eLayout  Styles and formatting including LAYOUT_LEFT, LAYOUT_RIGHT,
                    and LAYOUT_HCENTER.
\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVPARAM
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_NULLPTR
\see \n
N/A.
*/
HI_S32 HI_GO_TextLayoutTextOut(HI_HANDLE hTextLayout, HI_HANDLE hFont, HI_HANDLE hSurface, const HI_CHAR *pText,
    const HI_RECT *pRect, HIGO_LAYOUT_E eLayout);

/* *
\brief Check the layout context need to draw text cursor or not.
\param[in] hlayout  Text layout handle.
\retval ::HI_TRUE
\retval ::HI_FALSE
\see \n
N/A.
*/
HI_BOOL HI_GO_NeedDrawCursor(HI_HANDLE hlayout);

/* *
\brief Get cursor linear pos of cursor in text
\attention \n
N/A.
\param[in] hLayout Handle of a text layout object.
\retval position of cursor if succeed or -1 if failed.
\see \n
N/A.
*/
HI_S32 HI_GO_GetCursorLinearPos(HI_HANDLE hLayout);

/* *
\brief Obtains rectangle of text contents.
\attention \n
N/A.
\param[in] hFont Handle of a text output object.
\param[in] pText Text contents.
\param[in] pRect Text output region.
\param[in] eLayout    Styles and formatting including LAYOUT_LEFT, LAYOUT_RIGHT,
            and LAYOUT_HCENTER.
\param[out] pRcText Actual rectangle of text contents. It cannot be empty.
\param[out] pLen    Pointer to the string length.
\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVPARAM
\retval ::HIGO_ERR_INVHANDLE
\see \n
N/A.
*/
HI_S32 HI_GO_GetTextExtentEx2(HI_HANDLE hFont, const HI_CHAR *pText, const HI_RECT *pRect, HIGO_LAYOUT_E eLayout,
    HI_RECT *pRcText, HI_U32 *pLen);

/* *
\brief Move cursor
\attention \n
N/A.
\param[in] hlayout handle of layout context CNcommnet Űľ CNend
\param[in] dir  direction to move CNcommnet Ŀ귽 CNend
\retval ::HI_SUCCESS Success.
\retval ::HIGO_ERR_NOTINIT Failure.
\see \n
N/A.
*/
HI_S32 HI_GO_CursorMove(HI_HANDLE hlayout, HIGO_TEXT_CURSOR_DIR_E Dir);

/* *
\brief Get position of cursor on screen
\attention \n
N/A.
\param[in] hlayout handle of layout context CNcommnet Űľ CNend
\param[out] prect cursor position CNcommnet λ CNend
\retval ::HI_SUCCESS Success.
\retval ::HIGO_ERR_NOTINIT Failure.
\see \n
N/A.
*/
HI_S32 HI_GO_CursorGetScreenPos(HI_HANDLE hlayout, HI_RECT *pRect);

/* *
\brief Create layout context
\attention \n
N/A.
\param[in] pLayoutInfo layout parameters CNcommnet Ű CNend
\param[out] hlayout handle of layout context CNcommnet Űľ CNend
\retval ::HI_SUCCESS Success.
\retval ::HIGO_ERR_NOTINIT Failure.
\see \n
N/A.
*/
HI_S32 HI_GO_CreateLayout(HIGO_LAYOUT_INFO_S *pLayoutInfo, HI_HANDLE *hlayout);

/* *
\brief get text from context
\attention \n
N/A.
\param[in] hlayout handle of layout context
\prarm[out] length length of string
\param[out] str address of UTF-8 string
\retval ::HI_SUCCESS Success.
\retval ::HIGO_ERR_NOTINIT Failure.
\see \n
N/A.
*/
HI_S32 HI_GO_LayoutGetText(HI_HANDLE hlayout, HI_S32 *length, HI_CHAR **str);

/* *
\brief clear text in context
\attention \n
N/A.
\param[in] hlayout handle of layout context
\retval ::HI_SUCCESS Success.
\retval ::HIGO_ERR_NOTINIT Failure.
\see \n
N/A.
*/
HI_S32 HI_GO_LayoutClearText(HI_HANDLE hlayout);

/* *
\brief delete some character from context at current cursor pos
\attention \n
N/A.
\param[in] hlayout handle of layout context
\param[in] sCount
\retval ::HI_SUCCESS Success.
\retval ::HIGO_ERR_NOTINIT Failure.
\see \n
N/A.
*/
HI_S32 HI_GO_LayoutDeleteText(HI_HANDLE hlayout, HI_S32 sCount);

/* *
\brief Get layout rectangle's width & height
\attention \n
N/A.
\param[in] hlayout handle of layout context
\param[out] layout rectangle, the x,y fields set to 0
\retval ::HI_SUCCESS Success.
\retval ::HIGO_ERR_NOTINIT Failure.
\see \n
N/A.
*/
HI_S32 HI_GO_LayoutGetRect(HI_HANDLE hlayout, HI_RECT *pRect);

/* *
\brief Set view port position
\attention \n
N/A.
\param[in] hlayout handle of layout context
\param[in] position of view port
\retval ::HI_SUCCESS Success.
\retval ::HIGO_ERR_NOTINIT Failure.
\see \n
N/A.
*/
HI_S32 HI_GO_SetViewPort(HI_HANDLE hlayout, const HI_POINT *pPos);

/* *
\brief Set view port size
\attention \n
N/A.
\param[in] hlayout handle of layout context
\param[in] w    width of viewport
\param[in] h    height of viewport
\retval ::HI_SUCCESS Success.
\retval ::HIGO_ERR_NOTINIT Failure.
\see \n
N/A.
*/
HI_S32 HI_GO_SetViewPortSize(HI_HANDLE hlayout, HI_S32 w, HI_S32 h);

/* *
\brief get view port position
\attention \n
N/A.
\param[in] hlayout handle of layout context
\param[in] position of view port
\retval ::HI_SUCCESS Success.
\retval ::HIGO_ERR_NOTINIT Failure.
\see \n
N/A.
*/
HI_S32 HI_GO_GetViewPort(HI_HANDLE hlayout, HI_POINT *pPos);

/* *
\brief Set view port position
\attention \n
N/A.
\param[in] hlayout handle of layout context
\param[in] size & position of view port
\retval ::HI_SUCCESS Success.
\retval ::HIGO_ERR_NOTINIT Failure.
\see \n
N/A.
*/
HI_S32 HI_GO_SetViewPortEx(HI_HANDLE hlayout, const HI_RECT *pPos);

/* *
\brief Output text
\attention \n
N/A.
\param[in] hlayout handle of layout context
\param[in] hSurface surface to draw on
\param[in] pRect display rectangle
\retval ::HI_SUCCESS Success.
\retval ::HIGO_ERR_NOTINIT Failure.
\see \n
N/A.
*/
HI_S32 HI_GO_LayoutTextOut(HI_HANDLE hlayout, HI_HANDLE hSurface, const HI_RECT *pRect);

/* *
\brief Destroy layout context
\attention \n
N/A.
\param[in] hlayout handle of layout context
\retval ::HI_SUCCESS Success.
\retval ::HIGO_ERR_NOTINIT Failure.
\see \n
N/A.
*/
HI_S32 HI_GO_DestroyLayout(HI_HANDLE hlayout);

/* *
\brief begin select
\attention \n
N/A.
\param[in] hlayout handle of layout context
\param[in] pos position of select beginning
\retval ::HI_SUCCESS Success.
\retval ::HIGO_ERR_NOTINIT Failure.
\see \n
N/A.
*/
HI_S32 HI_GO_SetSelecting(HI_HANDLE hlayout, const HI_POINT *pos);

/* *
\brief select area confirmed
\attention \n
N/A.
\param[in] hlayout handle of layout context
\param[in] pos end position of select area
\retval ::HI_SUCCESS Success.
\retval ::HIGO_ERR_NOTINIT Failure.
\see \n
N/A.
*/
HI_S32 HI_GO_SetSelected(HI_HANDLE hlayout, const HI_POINT *pos);

/* *
\brief select finished
\attention \n
N/A.
\param[in] hlayout handle of layout context
\retval ::HI_SUCCESS Success.
\retval ::HIGO_ERR_NOTINIT Failure.
\see \n
N/A.
*/
HI_S32 HI_GO_SelectFinish(HI_HANDLE hlayout);

/* *
\brief get select string from ctx
\attention \n
N/A.
\param[in] hlayout handle of layout context
\param[in] str  UTF8 string to output
\param[in] len  length of output string ַ CNend
\retval ::HI_SUCCESS Success.
\retval ::HIGO_ERR_NOTINIT Failure.
\see \n
N/A.
*/
HI_S32 HI_GO_GetSelectString(HI_HANDLE hlayout, HI_CHAR **str, HI_U32 *len);

/* *
\brief set selected area text color
\attention \n
N/A.
\param[in] hlayout handle of layout context
\param[in] fg foreground color
\param[in] bg background color
\retval ::HI_SUCCESS Success.
\see \n
N/A.
*/
HI_S32 HI_GO_SetSelectColor(HI_HANDLE hlayout, HI_COLOR fg, HI_COLOR bg);

/* *
\brief delete select string
\attention \n
N/A.
\param[in] hlayout handle of layout context
\param[in] fg foreground color
\retval ::HI_SUCCESS Success.
\see \n
N/A.
*/
HI_S32 HI_GO_DeleteSelectString(HI_HANDLE hlayout);

/* *
\brief Get line count of layout context
\attention \n
N/A.
\param[in] hlayout handle of layout context
\retval ::line count of layout context or zero if failed.
\see \n
N/A.
*/
HI_S32 HI_GO_LayoutGetLineNum(HI_HANDLE hlayout);

/* *
\brief Set font of layout context
\attention \n
N/A.
\param[in] hlayout handle of layout context
\param[in] hFont handle of font to set
\retval ::HI_SUCCESS Success.
\see \n
N/A.
*/
HI_S32 HI_GO_LayoutSetFont(HI_HANDLE hlayout, HI_HANDLE hFont);

/* *
\brief Add text to layout context.
\param[in] hlayout  Text layout handle.
\param[in] pText    Text contents ending with /0.
\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVPARAM
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_NULLPTR
\see \n
N/A.
*/
HI_S32 HI_GO_LayoutAddText(HI_HANDLE hlayout, const HI_CHAR *pText);

/* *
\brief Check the cursor of context in SOT(Start of Text) position

Handle of a text layout object.
N/A.
*/
HI_BOOL HI_GO_CursorInSot(HI_HANDLE hLayout);

/* *
\brief Check the cursor of context in EOT(End of Text) position

Handle of a text layout object.
N/A.
*/
HI_BOOL HI_GO_CursorInEot(HI_HANDLE hLayout);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* __HI_GO_TEXT_H__ */