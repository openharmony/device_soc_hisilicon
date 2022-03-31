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

#ifndef HI_PARSE_H
#define HI_PARSE_H

#include "hi_type.h"
#include "hi_gv.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LOADALLVIEW (-1)

/*
\brief Parser init.
\attention \n
N/A.
\param N/A
\retval ::HI_SUCCESS
\retval ::HI_ERR_PARSER_INITED  Parser is not inited.
\see \n
::HI_GV_PARSER_Deinit
*/
HI_S32 HI_GV_PARSER_Init(HI_VOID);

/*
\brief Uninit parser.
\attention \n
N/A.
\param N/A
\retval N/A
\see \n
::HI_GV_PARSER_Init
*/
HI_VOID HI_GV_PARSER_Deinit(HI_VOID);

/*
\brief  Load HIGVBin file and parse the basic information of higvbin.

\attention \n
N/A.
\param[in] fileName  File name.
\retval ::HI_SUCCESS
\retval ::HI_ERR_PARSER_NOINIT it is not inited.
\retval ::HI_ERR_PARSER_FILELOADED File error.
\retval ::HI_ERR_COMM_FILEOP File opreration is error.
\retval ::HI_ERR_COMM_NORES No memory.
\retval ::HI_ERR_PARSER_VERIFY CRC ERROR.
\see \n
N/A.
*/
HI_S32 HI_GV_PARSER_LoadFile(const HI_CHAR *fileName);

/*
\brief Unload higvbin file, release resource.
\attention \n
N/A.
\param N/A.
\retval ::HI_SUCCESS
\retval ::HI_ERR_PARSER_NOINIT
\retval ::HI_ERR_PARSER_FILENOTLOAD
\see \n
N/A.
*/
HI_S32 HI_GV_PARSER_UnLoadFile(HI_VOID);

/*
\brief Release the resource when load higvbin file, This api will be called at the end of load.

\attention \n
N/A.
\param N/A.
\retval ::HI_SUCCESS
\retval ::HI_ERR_PARSER_NOINIT
\retval ::HI_ERR_PARSER_FILENOTLOAD
\see \n
N/A.
*/
HI_S32 HI_GV_PARSER_ReleaseLoadRes(HI_VOID);

/*
\brief Get view number.
\attention \n
N/A.
\param[out] viewNum View number.
\retval ::HI_SUCCESS
\retval ::HI_ERR_PARSER_NOINIT
\retval ::HI_ERR_COMM_EMPTY The pointer is NULL.
\see \n
N/A.
*/
HI_S32 HI_GV_PARSER_GetViewNum(HI_U32 *viewNum);

/*
\brief Load the view by name of view.
\attention \n
N/A.
\param[in] viewName  View name.
\retval ::HI_SUCCESS
\retval ::HI_ERR_PARSER_NOINIT
\retval ::HI_ERR_PARSER_UNSUPPORT
\see \n
N/A.
*/
HI_S32 HI_GV_PARSER_LoadViewByName(const HI_CHAR *viewName);

/*
\brief Load the view by ID of view.
\attention \n
if(ViewId = LOADALLVIEW),load all view.
\param[in] viewID    View ID.
\retval ::HI_SUCCESS
\retval ::HI_ERR_PARSER_NOINIT
\retval ::HI_ERR_COMM_LOST   The view is not exit.
\retval ::HI_ERR_PARSER_DATALEN
\retval ::HI_ERR_PARSER_DATAERR
\see \n
HI_GV_PARSER_LoadViewById
*/
HI_S32 HI_GV_PARSER_LoadViewById(HI_U32 viewID);

/*
\brief Unload the view by ID of view.
\attention \n
\param[in] viewID    View ID.
\retval ::HI_SUCCESS
\retval ::HI_ERR_PARSER_NOINIT
\retval ::HI_ERR_COMM_LOST
\see \n
HI_GV_PARSER_LoadViewById
*/
HI_S32 HI_GV_PARSER_UnloadViewByID(HI_HANDLE viewID);

/*
\brief Load the view by index.
\attention \n
N/A.
\param[in] viewIndex   View index.
\retval ::HI_SUCCESS
\retval ::HI_ERR_PARSER_NOINIT
\retval ::HI_ERR_COMM_LOST
\retval ::HI_ERR_PARSER_DATALEN
\retval ::HI_ERR_PARSER_DATAERR
\see \n
N/A.
*/
HI_S32 HI_GV_PARSER_LoadViewByIndex(HI_U32 viewIndex);

/*
\brief Delete the view exclude view.
\attention \n
N/A.
\param[in] excludeViewID  Exclude view that is not deleted.
\param[in] excludeViewNum  The number of view that not need deleted.
\retval ::HI_SUCCESS
\retval ::HI_ERR_PARSER_NOINIT
\retval ::HI_ERR_COMM_INVAL
\retval ::HI_FAILURE
\see \n
N/A.
*/
HI_S32 HI_GV_PARSER_UnLoadViewExclude(const HI_HANDLE *excludeViewID, HI_U32 excludeViewNum);

/*
\brief Get windows handle by view ID.
\attention \n
N/A.
\param[in] viewID    View ID.
\param[out] winsArray  The window handle array .
\param[out] winsNum  The number of view that not need deleted.
\retval ::HI_SUCCESS
\retval ::HI_ERR_COMM_NOTINIT
\see \n
N/A.
*/
HI_S32 HI_GV_PARSER_ViewGetWinsHandle(HI_U32 viewID, HI_HANDLE **winsArray, HI_U32 *winsNum);

/*
\brief Get the version number.
\attention \n
N/A.
\param[out] mainVersion  Main version.
\param[out] subVersion   Sub version.
\retval ::HI_SUCCESS
\retval ::HI_ERR_PARSER_NOINIT
\see \n
N/A.
*/
HI_S32 HI_GV_PARSER_GetVersion(HI_U32 *mainVersion, HI_U32 *subVersion);

/*
\brief Set the evnent function at the STC mode.
\attention \n
N/A.
\param[in] eventProc Event function.
\param[in] number The number of event function.
\retval HI_SUCCESS
\retval HI_ERR_PARSER_NOINIT
\retval HI_ERR_COMM_INVAL
\see \n
N/A.
*/
HI_S32 HI_GV_PARSER_SetWidgetEventFunc(const HIGV_MSG_PROC *eventProc, HI_U32 number);

#ifdef __cplusplus
}
#endif
#endif /* HI_PARSE_H */
