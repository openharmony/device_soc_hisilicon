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

#ifndef HI_GV_ERRNO_H
#define HI_GV_ERRNO_H

#include "hi_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    /* unknown error */
    HI_ERR_COMM_UNKNOWN = 1,
    /* other error */
    HI_ERR_COMM_OTHER,
    /* inner error */
    /*
    CPUԲI/Oѧ */
    HI_ERR_COMM_INTER,
    /* version error */
    HI_ERR_COMM_VERSION,
    /* unsupported function, operation, feature */
    /*
    δİ汾Ʒ֧ */
    HI_ERR_COMM_PAERM = 5,
    /* invalid parameter */
    /*
    ʵıšͨš豸šָ롢ַȵ */
    HI_ERR_COMM_INVAL,
    /* not init */
    /* */
    HI_ERR_COMM_NOTINIT,
    /* not ready */
    /*
     ȱԴ */
    HI_ERR_COMM_NOTREADY,
    /* no resource */
    HI_ERR_COMM_NORES,
    /* resource is  exist */
    HI_ERR_COMM_EXIST = 10,
    /* resource is not exist */
    HI_ERR_COMM_LOST,
    /* no operation */
    HI_ERR_COMM_NOOP,
    /* busy */
    HI_ERR_COMM_BUSY,
    /* idle */
    HI_ERR_COMM_IDLE,
    /* full */
    HI_ERR_COMM_FULL = 15,
    /* empty */
    HI_ERR_COMM_EMPTY,
    /* underflow */
    HI_ERR_COMM_UNDERFLOW,
    /* Overflow */
    HI_ERR_COMM_OVERFLOW,
    /* Couldn't access */
    HI_ERR_COMM_ACCES,
    /* operation is not finish when get interrupt */
    HI_ERR_COMM_INTR = 20,
    /* Operation is not finish, it will continue */
    HI_ERR_COMM_CONTINUE,
    /* Operation is over, there is no any object to opreate */
    HI_ERR_COMM_OVER,
    /* Operation is not support */
    HI_ERR_COMM_UNSUPPORT,
    /* out of bound */
    HI_ERR_COMM_OUTOFBOUND,
    /* File operation error */
    HI_ERR_COMM_FILEOP,
    /* Secure function error */
    HI_ERR_COMM_SECURE,
    /* No effect error */
    HI_ERR_COMM_NOEFFECT,

    HI_ERR_COMM_END = 28,

    /* Message module */
    /* Send message failure */
    HI_ERR_MSGM_VTOPMSGSEND = 500,
    /* Create message queue failure */
    HI_ERR_MSGM_VTOPMSGCREATE,
    /* Create message server failure */
    HI_ERR_MSGM_VTOPMSGSERVER,

    /* APP */
    /* The base of task manager */
    /* Tase not stop */
    HI_ERR_TASK_TASKNOTSTOP,

    /* resource manager */
    /* no memory */
    HI_ERR_RES_NOMEN,
    /* unknown resource type */
    HI_ERR_RES_UNKNOWRES = 505,
    /* resource is using */
    HI_ERR_RES_USED,
    /* invalid resource type */
    HI_ERR_RES_INVRESTYPE,
    /* The pointer is null when save resource infoamtion */
    HI_ERR_RES_NULL,
    /* Load resource is failure */
    HI_ERR_RES_LOAD,
    /* The rource is not loaded */
    HI_ERR_RES_NOLOAD = 510,
    /* Create resource failure */
    HI_ERR_RES_CREATE,

    /* Window manager */
    /* The base of window manager */
    HI_ERR_WM_CREATE,

    /* widget framework */
    /* Widget type is no exist */
    HI_ERR_TYPE_NOEXISIT,
    /* Unsupported multi thread to call */
    HI_ERR_MULTIPLE_THREAD_CALL,
    /* binded db is not used */
    HI_ERR_WIDGET_INVALDB = 515,
    /* The widget is shown */
    HI_ERR_WIDGET_SHOW,
    /* the widget is hidden */
    HI_ERR_WIDGET_HIDE,
    /* the rect is null */
    HI_ERR_INVSIBLE,

    /* DDB */
    /* no field */
    HI_ERR_DDB_ZEROFIELD,
    /* out of range */
    HI_ERR_DDB_OUTOFRANGE = 520,
    /* null pointer */
    HI_ERR_DDB_NULLPTR,
    /* invalid parameter */
    HI_ERR_DDB_INVAIDPARA,
    /* Buffer is too small */
    HI_ERR_DDB_BUFFSMALL,

    /* ADM */
    /* no filed */
    HI_ERR_ADM_ZEROFIELD,
    /* Out of range */
    HI_ERR_ADM_OUTOFRANGE = 525,
    /* Null pointer */
    HI_ERR_ADM_NULLPTR,
    /* Invalid parameter */
    HI_ERR_ADM_INVAIDPARA,
    /* The ADM buffer is too small */
    HI_ERR_ADM_BUFFSMALL,
    /* Get data failure */
    HI_ERR_ADM_GETDATA,
    /* Unknown operation */
    HI_ERR_ADM_UNKNOWNOPT = 530,

    /* LANGUAGE */
    /* *Not set locale */
    HI_ERR_LANG_INVALID_LOCALE,

    /* 豸 */
    /* IR豸ʧ */
    HI_ERR_IM_OPENIRDEVICE,
    /* ȡIRֵʧ */
    HI_ERR_IM_GETIRVALUE,
    /* ûжӦ */
    HI_ERR_IM_NOVIRKEY,
    /* ַ */
    /* ֵַ֧ */
    HI_ERR_CHARSET_UNSUPPORT = 535,
    /* Чַת */
    HI_ERR_CHARSET_CONVERT,

    /* */
    /* Not init */
    HI_ERR_PARSER_NOINIT,
    /* it is inited */
    HI_ERR_PARSER_INITED,
    /* Check code is error */
    HI_ERR_PARSER_VERIFY = 540,
    /* The identifier is error */
    HI_ERR_PARSER_MARK,
    /* The file type is error */
    HI_ERR_PARSER_TYPE,
    /* The length is error */
    HI_ERR_PARSER_DATALEN,
    /* Unsupported  */
    HI_ERR_PARSER_UNSUPPORT,
    /* The data of higv bin is error  */
    HI_ERR_PARSER_DATAERR = 545,
    /* The view is not loaded fully  */
    HI_ERR_PARSER_VIEWNOLOAD,
    /* File is loaded  */
    HI_ERR_PARSER_FILELOADED,
    /* File is not loaded */
    HI_ERR_PARSER_FILENOTLOAD,
} VResult;

#ifdef __cplusplus
}
#endif
#endif /* HI_GV_ERRNO_H */
