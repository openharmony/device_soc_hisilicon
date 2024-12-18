/*
 * Copyright (c) 2020 HiSilicon (Shanghai) Technologies CO., LIMITED.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * Description: declaration of driverif APIs
 * Author: none
 * Create: 2020
 */

#ifndef __DRIVERIF_H__
#define __DRIVERIF_H__
/**
 * @file driverif.h
 */
#include "lwip/opt.h"
#include "netif/etharp.h"

#ifdef __cplusplus
extern "C" {
#endif

#if defined(LWIP_ACHBA_SUPPORT) && (LWIP_ACHBA_SUPPORT)
typedef bool (*lwip_achba_func)(struct netif *netif, struct pbuf *p);
void driverif_achba_register_hook(void *func);
void driverif_achba_unregister_hook(void);
#endif

/**
* @defgroup Driver_Interfaces Driver Interfaces
* This section provides information about the Network driver related interfaces.
*/

err_t driverif_init(struct netif *netif);

/*
Func Name:  driverif_input
*/
/**
* @ingroup   Driver_Interfaces
* @brief     This API must be called by the network driver to pass the input packet to lwIP.
* Before calling this API, the driver has to keep the packet in the pbuf structure. The driver must
* call pbuf_alloc() with the type as PBUF_RAM to create the pbuf structure. The driver
* has to pass the pbuf structure to this API to add the pbuf into the TCPIP thread.
* After this packet is processed by TCPIP thread, pbuf will be freed. The driver is not required to
* free the pbuf.
* @param[in]    netif                 Indicates the lwIP network interface.
* @param[in]    p                     Indicates the packet in the pbuf structure.
* @return None
* @note
* None
*/
void  driverif_input(struct netif *netif, struct pbuf *p);

#ifdef __cplusplus
}
#endif

#endif
