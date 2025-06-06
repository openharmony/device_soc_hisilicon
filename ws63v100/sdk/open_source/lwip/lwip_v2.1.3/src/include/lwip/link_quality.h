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
 * Description: declaration of link quality APIs
 * Author: none
 * Create: 2013-12-22
 */

#ifndef __LINK_QUALITY_H__
#define __LINK_QUALITY_H__

#include "lwip/opt.h"
#include "lwip/netif.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef LWIP_RPL_USE_MACACK_FOR_NBR
#define LWIP_RPL_USE_MACACK_FOR_NBR 1
#endif

typedef unsigned char LQI_TYPE;
typedef signed char RSSI_TYPE;

struct netif;
struct linklayer_addr;

typedef struct _lwip_nbr_linkquality {
  /* Link Metric is derived from ETX */
  u16_t link_metric;
} lwip_nbr_linkquality_t;

void lwip_update_nbr_signal_quality
(
  const struct netif *linkiface,
  const struct linklayer_addr *sendermac,
  RSSI_TYPE link_rssi,
  LQI_TYPE link_lqi
);

typedef enum mac_status {
  MAC_STATUS_TX_ACK_OK = 0,
  MAC_STATUS_MAX_RETRIES,
  MAC_STATUS_ERR_FATAL,
  /*
   * case of wifi, beacon unavailability can be used to decide peer unreachability.
   * In this case if the peer is a parent, then the parent entry should be removed.
   */
  MAC_STATUS_PEER_UNREACHABLE,
  MAC_STATUS_DROP_POLICY,
  MAC_STATUS_BUTT
} mac_status_e;

int lwip_get_rssi(void);

#ifdef __cplusplus
}
#endif

#endif /* __LINK_QUALITY_H__ */

