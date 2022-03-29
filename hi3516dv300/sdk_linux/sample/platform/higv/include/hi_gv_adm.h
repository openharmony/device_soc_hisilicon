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

#ifndef HI_GV_ADM_H
#define HI_GV_ADM_H

#include "hi_type.h"
#include "hi_gv_widget.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ADM and DDB support  MAX filed number */
#define HIGV_DB_MAX_FIELDNUM      64
#define HIGV_DB_MAX_FIELD_STR_LEN 4096

/* The type of data are supported.; */
typedef enum {
    /* char */
    HIGV_DT_S8 = 0,
    /* unsigned char */
    HIGV_DT_U8,
    /* short */
    HIGV_DT_S16,
    /* unsigned short */
    HIGV_DT_U16,
    /* int */
    HIGV_DT_S32,
    /* unsigned int */
    HIGV_DT_U32,
    /* long long */
    HIGV_DT_S64,
    /* unsigned long long */
    HIGV_DT_U64,
    /* float */
    HIGV_DT_F32,
    /* double */
    HIGV_DT_D64,
    /* char * */
    HIGV_DT_STRING,
    /* image handle */
    HIGV_DT_HIMAGE,
    /* multi-langugae  string ID */
    HIGV_DT_STRID,
    HIGV_DT_BUTT
} HIGV_DT_E;

/* The type of data change. */
typedef enum {
    /* insert data ; */
    HIGV_DC_INSERT = 0,
    /* modify data ; */
    HIGV_DC_MODIFY,
    /*  delete data ; */
    HIGV_DC_DELETE,
    /* updata data,sealed operations ; */
    HIGV_DC_UPDATE,
    /* clear all the data ; */
    HIGV_DC_CLEAR,
    /* destroy the data source ; */
    HIGV_DC_DESTROY,
    HIGV_DC_BUTT
} HIGV_DC_E;

/* The data change info. */
typedef struct {
    /* the type of data change,  */
    HIGV_DC_E Action;
    /* the start row of data change , */
    HI_U16 StartRow;
    /* rows aggregate have data change , */
    HI_U16 Rows;
} HIGV_DCINFO_S;

/* Cell attribute.CNconmment:ֶ */
typedef struct hiHIGV_CELLATTR_S {
    /*  data type */
    HIGV_DT_E eDataType;
    /* max field str len, value must less than ::HIGV_DB_MAX_FIELD_STR_LEN */
    HI_U32 MaxSize;
} HIGV_FIELDATTR_S;

/* Row deferable. */
typedef struct hiHIGV_DBROW_S {
    /* data size */
    HI_U32 Size;
    /* data pointer */
    HI_VOID *pData;
} HIGV_DBROW_S;

typedef HI_S32 (*GetCountFunc)(HIGV_HANDLE dbSource, HI_U32 *rowCnt);
typedef HI_S32 (*GetRowValueFunc)(HIGV_HANDLE dbSource, HI_U32 row, HI_U32 num, const HI_VOID *data, HI_U32 *rowNum);
typedef HI_S32 (*RegisterDataChangeFunc)(HIGV_HANDLE dbSource, HIGV_HANDLE adm);
typedef HI_S32 (*UnregisterDataChangeFunc)(HIGV_HANDLE dbSource, HIGV_HANDLE adm);

typedef struct hiADM_OPT_S {
    /* data source ; */
    HI_U32 DBSource;
    /* data source field count ; */
    HI_U32 FieldCount;
    /* field attribute ; */
    HIGV_FIELDATTR_S *pFieldAttr;
    /* rows at buffer ; */
    HI_U32 BufferRows;
    /* get rows count function. ; */
    GetCountFunc GetCount;
    GetRowValueFunc GetRowValue;
    /* get rows value from  appoint rows function. ; */
    RegisterDataChangeFunc RegisterDataChange;
    /* Register the function when data change ; */
    UnregisterDataChangeFunc UnregisterDataChange;
    /* Unregister the function when data change. ; */
} HIGV_ADMOPT_S;

/*
 * brief
 * param[in] dataSource
 * param[out] admHandle  ADM handle.
 * retval ::HI_SUCCESS
 * retval ::HI_ERR_ADM_NULLPTR
 * retval ::HI_ERR_ADM_ZEROFIELD
 * retval ::HI_ERR_ADM_OUTOFRANGE
 * retval ::HI_ENORES
 */
HI_S32 HI_GV_ADM_Create(const HIGV_ADMOPT_S *dataSource, HIGV_HANDLE *admHandle);

/*
 * brief Create ADM by handle.
 * param[in] dataSource operation of datasouce.
 * param[in] admHandle   ADM handle.
 * retval ::HI_SUCCESS
 * retval ::HI_ERR_ADM_NULLPTR
 * retval ::HI_ERR_ADM_ZEROFIELD
 * retval ::HI_ERR_ADM_OUTOFRANGE
 * retval ::HI_ENORES
 */
HI_S32 HI_GV_ADM_CreateByHandle(const HIGV_ADMOPT_S *dataSource, HIGV_HANDLE admHandle);

/*
 * brief Create DDB by default.
 * param[in] dbHandle DB handle.
 * param[in] fieldCount Field count.
 * param[in] fieldAttr  Field attribute.
 * param[out] admHandle ADM handle.
 * retval ::HI_SUCCESS
 * retval ::HI_ERR_ADM_NULLPTR
 * retval ::HI_ERR_ADM_ZEROFIELD
 * retval ::HI_ERR_ADM_OUTOFRANGE
 * retval ::HI_ENORES
 */
HI_S32 HI_GV_ADM_CreateDefault(HIGV_HANDLE dbHandle, HI_U32 fieldCount, HIGV_FIELDATTR_S *fieldAttr,
    HIGV_HANDLE *admHandle);

/*
 * brief Create default DDB by handle.
 * param[in] dbHandle  DB handle.
 * param[in] fieldCount Field count.
 * param[in] fieldAttr  Field attribute.
 * param[in] admHandle ADM handle.
 * retval ::HI_SUCCESS
 * retval ::HI_ERR_ADM_NULLPTR
 * retval ::HI_ERR_ADM_OUTOFRANGE
 * retval ::HI_ENORES
 */
HI_S32 HI_GV_ADM_CreateDefaultByHandle(HIGV_HANDLE dbHandle, HI_U32 fieldCount,
    const HIGV_FIELDATTR_S *fieldAttr, HIGV_HANDLE admHandle);

/*
 * brief Destroy ADM.
 * param[in] admHandle ADM handle.
 * retval ::HI_SUCCESS
 * retval ::HI_ERR_COMM_LOST
 */
HI_S32 HI_GV_ADM_Destroy(HIGV_HANDLE admHandle);

/*
 * brief Bind adm to widget.
 * param[in] widgetHandle Widget handle.
 * param[in] admHandle ADM handle.
 * retval ::HI_SUCCESS
 */
HI_S32 HI_GV_ADM_Bind(HIGV_HANDLE widgetHandle, HIGV_HANDLE admHandle);

/*
 * brief Unbinde adm to widget.
 * param[in] widgetHandle Widget handle.
 * param[in] admHandle ADM handle.
 * retval ::HI_SUCCESS
 * retval ::HI_ERR_COMM_LOST
 */
HI_S32 HI_GV_ADM_UnBind(HIGV_HANDLE widgetHandle, HIGV_HANDLE admHandle);

/*
 * brief Get the data type of field.
 * param[in] admHandle ADM handle.
 * param[in] col Column index.
 * param[out] fieldAttr Field attribute.
 * retval ::HI_SUCCESS
 * retval ::HI_ERR_COMM_LOST
 * retval ::HI_ERR_ADM_NULLPTR
 * retval ::HI_ERR_ADM_OUTOFRANGE
 */
HI_S32 HI_GV_ADM_GetFieldAttr(HIGV_HANDLE admHandle, HI_U32 col, HIGV_FIELDATTR_S *fieldAttr);

/*
 * brief Get total row number.
 * param[in] admHandle ADM handle.
 * param[out] count row count.
 * retval ::HI_SUCCESS
 * retval ::HI_ERR_COMM_LOST
 * retval ::HI_ERR_ADM_NULLPTR
 */
HI_S32 HI_GV_ADM_GetRowCount(HIGV_HANDLE admHandle, HI_U32 *count);

/*
 * brief Get the data by row and column index.
 * param[in] admHandle ADM handle.
 * param[in] row row index.
 * param[in] col Column index.
 * param[out] data
 * param[in] len data
 * retval ::HI_SUCCESS
 * retval ::HI_ERR_COMM_LOST
 * retval ::HI_ERR_ADM_NULLPTR
 * retval ::HI_ERR_ADM_OUTOFRANGE
 * retval ::HI_ERR_ADM_BUFFSMALL
 * retval ::HI_ERR_ADM_GETDATA
*/
HI_S32 HI_GV_ADM_GetCellData(HIGV_HANDLE admHandle, HI_U32 row, HI_U32 col, HI_VOID *data, HI_U32 len);

/*
 * brief Get the data of column.
 * param[in] admHandle ADM handle.
 * param[in] row row index.
 * param[in] col Column index.
 * param[out] dataString Data content.
 * param[in] len dataString buffer size.
 * retval ::HI_SUCCESS
 * retval ::HI_ERR_COMM_LOST
 * retval ::HI_ERR_ADM_NULLPTR
 * retval ::HI_ERR_ADM_OUTOFRANGE
 * retval ::HI_ERR_ADM_BUFFSMALL data
 * retval ::HI_ERR_ADM_GETDATA
 */
HI_S32 HI_GV_ADM_GetCellDataString(HIGV_HANDLE admHandle, HI_U32 row, HI_U32 col, HI_CHAR *dataString, HI_U32 len);

/*
 * brief Set the callback function when data change.
 * param[in] admHandle ADM handle.
 * param[in] customProc notify callback of data change.
 * param[in] procOrder callback of time choice.
 * retval ::HI_SUCCESS
 * retval ::HI_ERR_COMM_LOST
 */
HI_S32 HI_GV_ADM_SetDCCallBack(HIGV_HANDLE admHandle, HIGV_MSG_PROC customProc, HIGV_PROCORDER_E procOrder);

/*
 * brief Get the DDB handle by ADM handle.
 * param[in] ADM handle.
 * param[out] ddbHandle DDB handle.
 * retval ::HI_SUCCESS
 * retval ::HI_ERR_COMM_LOST
 * retval ::HI_ERR_ADM_NULLPTR
 */
HI_S32 HI_GV_ADM_GetDDBHandle(HIGV_HANDLE admHandle, HIGV_HANDLE *ddbHandle);

/*
 * brief Get column number.
 * param[in] admHandle ADM handle.
 * param[out] colNum colunm count.
 * retval ::HI_SUCCESS
 * retval ::HI_ERR_COMM_LOST
 * retval ::HI_ERR_ADM_NULLPTR
 */
HI_S32 HI_GV_ADM_GetColNum(HIGV_HANDLE admHandle, HI_U32 *colNum);

/*
 * brief Sync between  ADM and DB.
 * param[in] ADM handle.
 * retval ::HI_SUCCESS
 * retval ::HI_ERR_COMM_LOST
 */
HI_S32 HI_GV_ADM_Sync(HIGV_HANDLE admHandle);

/*
 * brief Clear all cache data in the ADM.
 * retval ::HI_SUCCESS
 * retval ::HI_ERR_COMM_LOST
 */
HI_S32 HI_GV_ADM_ClearAllData(HI_VOID);

#ifdef __cplusplus
}
#endif
#endif /* HI_GV_ADM_H */
