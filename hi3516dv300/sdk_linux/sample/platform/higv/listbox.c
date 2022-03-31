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
#include "higv_cextfile.h"
#include "sample_utils.h"
#include "hi_gv_parser.h"
#include "securec.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /*  __cplusplus  */

#define HIGV_CONTROL_PAGENUM  10
#define  TIMER_SPEED          (3000)

static HI_U32 g_timerId = 3;

static HI_U32 g_itemSeq = 1;

HI_S32 LISTBOX_WIN_Onhide(HIGV_HANDLE widget, HI_PARAM wParam, HI_PARAM lParam)
{
    HIGV_UNUSED(wParam);
    HIGV_UNUSED(lParam);

    HI_U32 ret = (HI_U32)HI_GV_Timer_Stop(widget, g_timerId);
    ret |= (HI_U32)HI_GV_Timer_Destroy(widget, g_timerId);

    HIGV_CHECK("HI_GV_Timer_Destory", ret);

    g_itemSeq = 0;

    return HIGV_PROC_GOON;
}

HI_S32 LISTBOX_WIN_Onrefresh(HIGV_HANDLE widget, HI_PARAM wParam, HI_PARAM lParam)
{
    HIGV_UNUSED(wParam);
    HIGV_UNUSED(lParam);

    HI_S32 ret = HI_GV_Timer_Create(widget, g_timerId, TIMER_SPEED);
    HIGV_CHECK("HI_GV_Timer_Create", ret);

    ret = HI_GV_Timer_Start(widget, g_timerId);
    HIGV_CHECK("HI_GV_Timer_Start", ret);

    return HIGV_PROC_GOON;
}

HI_S32 LISTBOX_WIN_Onshow(HIGV_HANDLE widget, HI_PARAM wParam, HI_PARAM lParam)
{
    HI_S32 ret;
    HI_U32 index;
    HIGV_HANDLE db = 0;
    HIGV_DBROW_S dbRow;
    HIGV_UNUSED(widget);
    HIGV_UNUSED(wParam);
    HIGV_UNUSED(lParam);

    HI_U32 strSet[HIGV_CONTROL_PAGENUM] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    /* Get DDB (default data base) handle */
    ret = HI_GV_ADM_GetDDBHandle(ADM_SET_STRINGID, &db);
    if (ret != HI_SUCCESS) {
        printf("Failed to add data, function:<%s>, retval %#x.\n", __FUNCTION__, ret);
        return HI_FAILURE;
    }

    /* Disable DDB change */
    HI_GV_DDB_EnableDataChange(db, HI_FALSE);

    /* Clear DDB */
    HI_GV_DDB_Clear(db);

    /* Add data */
    for (index = 0; index < HIGV_CONTROL_PAGENUM; index++) {
        ret = memset_s(&dbRow, sizeof(dbRow), 0x00, sizeof(dbRow));
        if (ret != EOK) {
            return ret;
        }
        dbRow.pData = &strSet[index];
        dbRow.Size  = sizeof(HI_U32);
        HI_GV_DDB_Append(db, &dbRow);
    }
    /* Enable DDB change */
    HI_GV_DDB_EnableDataChange(db, HI_TRUE);

    /* Sync the ADM */
    HI_GV_ADM_Sync(ADM_SET_STRINGID);

    return HIGV_PROC_GOON;
}

HI_S32 LISTBOX_WIN_Ontimer(HIGV_HANDLE widget, HI_PARAM wParam, HI_PARAM lParam)
{
    HIGV_UNUSED(widget);
    HIGV_UNUSED(wParam);
    HIGV_UNUSED(lParam);

    if (g_itemSeq < 3) { /* 3 is g_itemSeq MAX */
        HI_S32 ret = HI_GV_List_SetSelItem(LISTBOX_WIN_LISTBOX1, g_itemSeq);
        HIGV_CHECK("HI_GV_List_SetSelItem", ret);

        ret = HI_GV_Widget_Paint(LISTBOX_WIN_LISTBOX1, HI_NULL);
        HIGV_CHECK("HI_GV_Widget_Paint", ret);

        g_itemSeq++;

        ret = HI_GV_Timer_Reset(widget, g_timerId);
        HIGV_CHECK("HI_GV_Timer_Reset", ret);
    } else {
        HI_U32 ret = (HI_U32)HI_GV_PARSER_LoadViewById(SCROLLVIEW_WIN);
        ret |= (HI_U32)HI_GV_Widget_Show(SCROLLVIEW_WIN);
        ret |= (HI_U32)HI_GV_Widget_Active(SCROLLVIEW_WIN);
        ret |= (HI_U32)HI_GV_Widget_Hide(LISTBOX_WIN);

        HIGV_CHECK("HI_GV_Widget_Show", ret);
    }

    return HIGV_PROC_GOON;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /*  __cplusplus  */

