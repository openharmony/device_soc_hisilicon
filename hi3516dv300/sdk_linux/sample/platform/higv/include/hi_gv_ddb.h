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

#ifndef HI_GV_DDB_H
#define HI_GV_DDB_H

#include "hi_type.h"
#include "hi_gv_adm.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Insert position */
typedef enum {
    DDB_INSERT_PRE = 0,
    /*  Insert before,   */
    DDB_INSERT_NEXT,
    /*  Insert after,   */
    DDB_INSERT_BUTT
} HIGV_DDBINSERT_E;

/*
 * brief Create a DDB.
 * param[in] fieldCount Field count.
 * param[in] fieldAttr Field attribute.
 * param[out] ddbHandle    DDB handle.
 * retval ::HI_SUCCESS
 * retval ::HI_ERR_COMM_INVAL
 */
HI_S32 HI_GV_DDB_Create(HI_U32 fieldCount, const HIGV_FIELDATTR_S *fieldAttr, HIGV_HANDLE *ddbHandle);

/*
 * brief Destroy a DDB.
 * param[in] ddbHandle DDB handle.
 * retval ::HI_SUCCESS
 * retval ::HI_ERR_COMM_LOST
 */
HI_S32 HI_GV_DDB_Destroy(HIGV_HANDLE ddbHandle);

/*
 * brief Insert a recoder to the databuffer.
 * param[in] ddbHandle DDB handle.
 * param[in] row row position.
 * param[in] data The pointer of recoder.
 * param[in] insert The position of insert.
 * retval ::HI_SUCCESS
 * retval ::HI_ERR_COMM_LOST
 * retval ::HI_ERR_DDB_NULLPTR
 * retval ::HI_ERR_DDB_INVAIDPARA
 */
HI_S32 HI_GV_DDB_Insert(HIGV_HANDLE ddbHandle, HI_U32 row, const HIGV_DBROW_S *data, HIGV_DDBINSERT_E insert);

/*
 * brief Modify a recoder of data buffer.
 * param[in] ddbHandle DDB handle.
 * param[in] row row index.
 * param[in] pdata Recoder data.
 * retval ::HI_SUCCESS
 * retval ::HI_ERR_COMM_LOST
 * retval ::HI_ERR_DDB_NULLPTR
 * retval ::HI_ERR_DDB_OUTOFRANGE
 */
HI_S32 HI_GV_DDB_Modify(HIGV_HANDLE ddbHandle, HI_U32 row, const HIGV_DBROW_S *data);

/*
 * brief Add a recoder to the tail of data buffer.
 * param[in] DDB handle.
 * param[in] data Recoder data.
 * retval ::HI_SUCCESS
 * retval ::HI_ERR_COMM_LOST
 * retval ::HI_ERR_DDB_NULLPTR
 * retval ::HI_ERR_DDB_BUFFSMALL
 */
HI_S32 HI_GV_DDB_Append(HIGV_HANDLE ddbHandle, const HIGV_DBROW_S *data);

/*
 * brief Clear the recoder in the data buffer.
 * param[in] ddbHandle DDB handle.
 * retval ::HI_SUCCESS
 * retval ::HI_ERR_COMM_LOST
 */
HI_S32 HI_GV_DDB_Clear(HIGV_HANDLE ddbHandle);

/*
 * brief Delete a recoder by key.
 * param[in] ddbHandle DDB handle.
 * param[in] row row index.
 * retval ::HI_SUCCESS
 * retval ::HI_ERR_COMM_LOST
 * retval ::HI_ERR_DDB_OUTOFRANGE
 */
HI_S32 HI_GV_DDB_Del(HIGV_HANDLE ddbHandle, HI_U32 row);

/*
 * brief Get the recoder by row number.
 * param[in] ddbHandle DDB handle.
 * param[in] row The row of recoder.
 * param[out] data Recoder data.
 * retval ::HI_SUCCESS
 * retval ::HI_ERR_COMM_LOST
 * retval ::HI_ERR_DDB_NULLPTR
 * retval ::HI_ERR_DDB_OUTOFRANGE
 */
HI_S32 HI_GV_DDB_GetRow(HIGV_HANDLE ddbHandle, HI_U32 row, HIGV_DBROW_S *data);

/*
 * brief Get n rows form the DDB.
 * param[in] ddbHandle DDB handle.ݻ
 * param[in] row  row index.
 * param[in] num  row number.
 * param[out] data    Data cache.
 * param[out] rowNum  row number.
 * retval ::HI_SUCCESS
 * retval ::HI_ERR_COMM_LOST
 * retval ::HI_ERR_DDB_NULLPTR
 * retval ::HI_ERR_DDB_OUTOFRANGE
 * retval ::HI_ERR_DDB_INVAIDPARA
 */
HI_S32 HI_GV_DDB_GetRows(HIGV_HANDLE ddbHandle, HI_U32 row, HI_U32 num, const HI_VOID *data, HI_U32 *rowNum);

/*
 * brief Get the column index of data.
 * param[in]  ddbHandle DDB handle.
 * param[in]  row row index.
 * param[in]  col Column index.
 * param[out] fieldData
 * param[in]  len Buffer size.
 * retval ::HI_SUCCESS
 * retval ::HI_ERR_COMM_LOST
 * retval ::HI_ERR_DDB_NULLPTR
 * retval ::HI_ERR_DDB_OUTOFRANGE
 * retval ::HI_ERR_DDB_BUFFSMALL
 */
HI_S32 HI_GV_DDB_GetCellData(HIGV_HANDLE ddbHandle, HI_U32 row, HI_U32 col, HI_VOID *fieldData, HI_U32 len);

/*
 * brief Set the column index of data.
 * param[in]  ddbHandle DDB handle.
 * param[in]  row row index.
 * param[in]  col Column index.
 * param[in]  fieldData
 * param[in]  len Buffer size.
 * retval ::HI_SUCCESS
 * retval ::HI_ERR_COMM_LOST
 * retval ::HI_ERR_DDB_NULLPTR
 * retval ::HI_ERR_DDB_OUTOFRANGE
 * retval ::HI_ERR_DDB_OUTOFRANGE
 */
HI_S32 HI_GV_DDB_SetCellData(HIGV_HANDLE ddbHandle, HI_U32 row, HI_U32 col, const HI_VOID *fieldData, HI_U32 len);

/*
 * brief Get the total row number.
 * param[in] ddbHandle DDB handle.
 * param[out] rowCount row count.
 * retval ::HI_SUCCESS
 * retval ::HI_ERR_COMM_LOST
 * retval ::HI_ERR_DDB_NULLPTR
 */
HI_S32 HI_GV_DDB_GetRowCount(HIGV_HANDLE ddbHandle, HI_U32 *rowCount);

/*
 * brief Register the function when the data change.
 * param[in] ddbHandle DDB handle.
 * param[in] admHandle The object which notify.
 * retval ::HI_SUCCESS
 * retval ::HI_ERR_COMM_LOST
 */
HI_S32 HI_GV_DDB_RegisterDataChange(HIGV_HANDLE ddbHandle, HIGV_HANDLE admHandle);

/*
 * brief Unregister the function when data change.
 * param[in] ddbHandle DDB handle.
 * param[in] admHandle The object which notify.
 * retval ::HI_SUCCESS
 * retval ::HI_ERR_COMM_LOST
 */
HI_S32 HI_GV_DDB_UnRegisterDataChange(HIGV_HANDLE ddbHandle, HIGV_HANDLE admHandle);

/*
 * brief Register callback when data change.
 * param[in] ddbHandle  DDB handle.
 * param[in] enable Enable flag.
 * retval ::HI_SUCCESS
 * retval ::HI_ERR_COMM_LOST
 */
HI_S32 HI_GV_DDB_EnableDataChange(HIGV_HANDLE ddbHandle, HI_BOOL enable);

#ifdef __cplusplus
}
#endif
#endif /* HI_GV_DDB_H */
