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

#ifndef HI_GV_LOG_H
#define HI_GV_LOG_H

#include "hi_gv.h"
#include "hi_gv_errno.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DOMAIN_BUTTON       "Button"
#define DOMAIN_SCROLLBOX    "ScrollBox"
#define DOMAIN_EDIT         "Edit"
#define DOMAIN_LABEL        "Label"
#define DOMAIN_PROGRESSBAR  "ProgressBar"
#define DOMAIN_SCROLLTEXT   "ScrollText"
#define DOMAIN_IMAGE        "Image"
#define DOMAIN_IMAGEEX      "ImageEx"
#define DOMAIN_SCROLLBAR    "ScrollBar"
#define DOMAIN_SCROLLGRID   "ScrollGrid"
#define DOMAIN_CLOCK        "Clock"
#define DOMAIN_LISTBOX      "ListBox"
#define DOMAIN_TRACKBAR     "TrackBar"
#define DOMAIN_SLIDEUNLOCK  "SlideUnlock"
#define DOMAIN_MSGBOX       "MessageBox"
#define DOMAIN_SCROLLVIEW   "ScrollView"
#define DOMAIN_CONTENTBOX   "ContentBox"
#define DOMAIN_SCALEVIEW    "ScaleView"
#define DOMAIN_CHARTS       "Charts"

#define DOMAIN_WIDGET       "Widget"
#define DOMAIN_MSGM         "Msgm"
#define DOMAIN_MTASK        "MultiTask"
#define DOMAIN_IM           "InputManager"
#define DOMAIN_RESM         "ResManager"
#define DOMAIN_WM           "WndManager"
#define DOMAIN_TIMER        "Timer"
#define DOMAIN_DDB          "DDatabase"
#define DOMAIN_ADM          "Datamodel"
#define DOMAIN_MLAYER       "MultiLayer"
#define DOMAIN_ANIM         "Animation"
#define DOMAIN_VSYNC        "Vsync"
#define DOMAIN_CURSOR       "Cursor"
#define DOMAIN_GESTURE      "Gesture"
#define DOMAIN_KEY          "Key"
#define DOMAIN_PERFORMANCE  "Performance"

typedef enum {
    HIGV_LOG_UNKNOWN = 0,
    HIGV_LOG_DEFAULT, /* only for SetMinPriority() */
    HIGV_LOG_VERBOSE, /* lowest priority */
    HIGV_LOG_DEBUG,   /*  Print debug, info, warring and error log info */
    HIGV_LOG_INFO,    /*  Print info, warring and error log info */
    HIGV_LOG_WARNING, /*  Print warring and error log info */
    HIGV_LOG_ERROR,   /*  Only print error log info */
    HIGV_LOG_FATAL,   /*  Only print fatal log info */
    HIGV_LOG_SILENT,  /* only for SetMinPriority() must be last */
    HIGV_LOG_BUTT
} HIGV_LOG_LEVEL_E;

/*
* brief Set the verbose level of a domain.
* attention:
* The parameter |pDomain| is a string which is module name, The higv system will use module id,
* such as |HIGV_MODID_PARSER_E|, |HIGV_MODID_CORE_E|, |HIGV_MODID_WIDGET_E|. So application
* should not use the module id which will conflict with the higv module.
* If |pDomain| is set to HI_NULL, |Level| will be applied to all known domains
* and used as the default verbose level for yet unknown domains.
* param[in] pDomain domain name.
* param[in] Level new verbose level.
* retval ::HI_SUCCESS
* retval ::HI_ERR_COMM_LOST
* retval ::HI_ERR_COMM_INVAL
*/
HI_S32 HI_GV_Log_SetLevel(const HI_CHAR *domain, HIGV_LOG_LEVEL_E level);

/*
* brief Get the verbose level of a domain.
* param[in] Get the verbose level from a domain name.
* param[out] verbose level
* retval ::HI_SUCCESS
* retval ::HI_FAILUER
*/
HIGV_LOG_LEVEL_E HI_GV_Log_GetLevel(const HI_CHAR *domain);

/*
* brief Output the verbose text info of a domain.
* param[in] Domain name.
* param[in] The verbose level.
* param[in] Get the verbose log info.
* retval ::HI_SUCCESS
* retval ::HI_FAILUER
*/
HI_S32 HI_GV_Log_Output(const HI_CHAR *domain, HIGV_LOG_LEVEL_E level, HI_S32 error, const HI_CHAR *fileName,
                        const HI_CHAR *functionName, HI_U32 line, const HI_CHAR *text, ...)
__attribute__((format(printf, 7, 8))); // 7 is format string index, 8 is first to check

#define HIGV_DEBUG(domain, format...)                                                                           \
    HI_GV_Log_Output(domain, HIGV_LOG_DEBUG, HI_SUCCESS, (const HI_CHAR *)__FILE__, (const HI_CHAR *)__FUNCTION__, \
        __LINE__, format)
#define HIGV_INFO(domain, format...)                                                                           \
    HI_GV_Log_Output(domain, HIGV_LOG_INFO, HI_SUCCESS, (const HI_CHAR *)__FILE__, (const HI_CHAR *)__FUNCTION__, \
        __LINE__, format)
#define HIGV_WARNING(domain, format...)                                                                           \
    HI_GV_Log_Output(domain, HIGV_LOG_WARNING, HI_SUCCESS, (const HI_CHAR *)__FILE__, (const HI_CHAR *)__FUNCTION__, \
        __LINE__, format)
#define HIGV_ERROR(errorNum, domain, format...)                                                                 \
    HI_GV_Log_Output(domain, HIGV_LOG_ERROR, (errorNum), (const HI_CHAR *)__FILE__, (const HI_CHAR *)__FUNCTION__, \
        __LINE__, format)
#define HIGV_FATAL(errorNum, domain, format...)                                                                 \
    HI_GV_Log_Output(domain, HIGV_LOG_FATAL, (errorNum), (const HI_CHAR *)__FILE__, (const HI_CHAR *)__FUNCTION__, \
        __LINE__, format)

#ifdef __cplusplus
}
#endif
#endif /* HI_GV_LOG_H */
