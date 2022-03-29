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

#ifndef HI_GV_LAN_H
#define HI_GV_LAN_H

#ifdef __cplusplus
extern "C" {
#endif

/* language country abbreviation */
/* *he format of language is <language>_<territory>. The <language> should satisfy iso639-2 and <territory> should
 * satisfy iso3166-1
*/
#define LAN_CN    "cn"
#define LAN_ZH    "zh"
#define LAN_ZH_CN "zh_CN"
#define LAN_ZH_TW "zh_TW"
#define LAN_ZH_HK "zh_HK"
#define LAN_AR    "ar"
#define LAN_CS    "cs"
#define LAN_DA    "da"
#define LAN_DE    "de"
#define LAN_DE_AT "de_AT"
#define LAN_DE_CH "de_CH"
#define LAN_EL    "el"
#define LAN_EN    "en"
#define LAN_EN_US "en_US"
#define LAN_EN_CA "en_CA"
#define LAN_EN_GB "en_GB"
#define LAN_ES    "es"
#define LAN_FA    "fa"
#define LAN_FI    "fi"
#define LAN_FR    "fr"
#define LAN_FR_CA "fr_CA"
#define LAN_FR_CH "fr_CH"
#define LAN_IT    "it"
#define LAN_IT_CH "it_CH"
#define LAN_HE    "he"
#define LAN_JA    "ja"
#define LAN_KO    "ko"
#define LAN_KO_KR "ko_KR"
#define LAN_NL    "nl"
#define LAN_NL_BE "nl_BE"
#define LAN_PT    "pt"
#define LAN_PT_BR "pt_BR"
#define LAN_RU    "ru"
#define LAN_SV    "sv"
#define LAN_TH    "th"
#define LAN_TR    "tr"
#define LAN_PO    "po"
#define LAN_VN    "vn"
#define LAN_HU    "hu"
#define LAN_UKR   "ukr"
#define LAN_RO    "ro"
#define LAN_HR    "hr"
#define LAN_SR    "sr"
#define LAN_BS    "bs"
#define LAN_MK    "mk"
#define LAN_BG    "bg"
#define LAN_ET    "et"
#define LAN_LV    "lv"
#define LAN_LT    "lt"
#define LAN_IND   "ind"

/* Direction of language   **/
typedef enum {
    HI_LAN_DIR_LTR = 0, /* Left to right */
    HI_LAN_DIR_RTL = 1,  /* Right to left */
    HI_LAN_DIR_BUTT
} HI_LAN_DIR;

/*
 * brief Register the multi-language.
 * attention:This api should call after the BIN loaded.
 *
 * param[in] resFile
 * param[in] fontHandle
 * param[in] langId Language ID.
 * retval ::HI_SUCCESS Success.
 * retval ::HI_ERR_COMM_INVAL Invalid parameter.
 * retval ::HI_ERR_COMM_EXIST This language has registered.
 * retval ::HI_ERR_LANG_INVALID_LOCALE  Locale is not setting.
 * retval ::HI_ERR_COMM_FULL  the max language is 16 or The total string count is over
 * 128*1024.
 *
 * see: HI_GV_Lan_UnRegister
*/
HI_S32 HI_GV_Lan_Register(const HI_CHAR *resFile, HIGV_HANDLE fontHandle, const HI_CHAR *langId);

/*
 * brief Unregister the language.
 * param[in] langId Language ID.
 * retval ::HI_SUCCESS Success.
 * retval ::HI_ERR_COMM_INVAL Invalid parameter.
 * etval ::HI_ERR_COMM_PAERM Unsupported operation.
 * see :HI_GV_Lan_Register
*/
HI_S32 HI_GV_Lan_UnRegister(const HI_CHAR *langId);

/*
 * brief Register the multi-language.
 * param[in] langId Language ID.
 * retval ::HI_SUCCESS Success.
 * retval ::HI_ERR_COMM_INVAL Invalid parameter.
 * retval ::HI_ERR_COMM_PAERM Unsupported operation.
*/
HI_S32 HI_GV_Lan_SetLocale(const HI_CHAR *locale);

/*
 * brief Register font info of the language.
 * param[in]  langId  Language ID.
 * param[in]  fontHandle Font handle.
 * retval ::HI_SUCCESS Success.
 * retval ::HI_ERR_LOSTInvalid handle.
 * retval ::HI_ERR_COMM_INVAL Invalid parameter.
*/
HI_S32 HI_GV_Lan_FontRegister(const HI_CHAR *langId, HIGV_HANDLE fontHandle);

/*
 * brief Change the language.
 * param[in] langId Language ID.
 * retval ::HI_SUCCESS Success.
 * retval ::HI_ERR_COMM_EMPTY Invalid parameter.
 * see:::HI_GV_Lan_Register
*/
HI_S32 HI_GV_Lan_Change(const HI_CHAR *langId);

/*
 * brief Find current language id.
 * param[in] langId Language ID.
 * retval ::HI_SUCCESS Success.
 * retval ::HI_ERR_COMM_EMPTY The language is not registered.
 * retval ::HI_ERR_COMM_INVAL Invalid parameter.
 * see : ::HI_GV_Lan_Register
*/
HI_S32 HI_GV_Lan_GetCurLangID(HI_CHAR **langId);

/*
 * brief Get the text by string id.
 * param[in]  strID  String ID.
 * param[in]  langId  Language ID.
 * param[out] str The pointer of string.
 * retval ::HI_SUCCESS Success.
 * retval ::HI_ERR_COMM_LOST Invalid handle.
 * retval ::HI_ERR_COMM_INVAL Invalid parameter.
*/
HI_S32 HI_GV_Lan_GetLangString(const HI_CHAR *langId, const HI_U32 strId, HI_CHAR **str);

/*
 * brief Get the dirction of the language.
 * param[in] direction Direction.
 * param[in] langId   Language ID.
 * retval ::HI_SUCCESS Success.
 * see : HI_GV_Lan_Register
*/
HI_S32 HI_GV_Lan_GetLangIDDirection(const HI_CHAR *langId, HI_LAN_DIR *direction);

#ifdef __cplusplus
}
#endif
#endif /* HI_GV_LAN_H */
