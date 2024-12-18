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
 * Description: implementation of ethtool APIs
 * Author: none
 * Create: 2020
 */
#include "lwip/opt.h"

#if LWIP_NETIF_ETHTOOL && LWIP_SOCKET /* don't build if not configured for use in lwipopts.h */
#include "lwip/ethtool.h"
static s32_t ethtool_get_link(struct netif *netif, struct ethtool_value *edata)
{
  struct ethtool_ops *ops = (struct ethtool_ops *)(netif->ethtool_ops);

  LWIP_ASSERT("netif != NULL", netif != NULL);
  LWIP_ASSERT("edata != NULL", edata != NULL);

  edata->cmd = ETHTOOL_GLINK;

  if ((ops == NULL) || (ops->get_link == NULL)) {
    edata->data = netif_is_up(netif) && netif_is_link_up(netif);
    return 0;
  }

  edata->data = netif_is_up(netif) && ops->get_link(netif);
  return 0;
}

static s32_t ethtool_get_settings(struct netif *netif, struct ethtool_cmd *edata)
{
  struct ethtool_ops *ops = (struct ethtool_ops *)(netif->ethtool_ops);
  LWIP_ASSERT("netif != NULL", netif != NULL);
  LWIP_ASSERT("edata != NULL", edata != NULL);

  edata->cmd = ETHTOOL_GSET;

  if ((ops == NULL) || (ops->get_settings == NULL)) {
    return EOPNOTSUPP;
  }

  return ops->get_settings(netif, edata);
}

static s32_t ethtool_set_settings(struct netif *netif, struct ethtool_cmd *edata)
{
  struct ethtool_ops *ops = (struct ethtool_ops *)(netif->ethtool_ops);

  LWIP_ASSERT("netif != NULL", netif != NULL);
  LWIP_ASSERT("edata != NULL", edata != NULL);

  edata->cmd = ETHTOOL_SSET;

  if ((ops == NULL) || (ops->set_settings == NULL)) {
    return EOPNOTSUPP;
  }

  return ops->set_settings(netif, edata);
}

s32_t dev_ethtool(struct netif *netif, struct ifreq *ifr)
{
  u32_t ethcmd;
  s32_t retval;
  struct ethtool_ops *ops = (struct ethtool_ops *)(netif->ethtool_ops);

  LWIP_ASSERT("netif != NULL", netif != NULL);
  LWIP_ASSERT("ifr != NULL", ifr != NULL);

  if (ops && ops->begin) {
    if (ops->begin(netif) < 0) {
      return EINVAL;
    }
  }
  if (ifr->ifr_data == NULL) {
    return EINVAL;
  }

  ethcmd = *(u32_t *)ifr->ifr_data;

  switch (ethcmd) {
    case ETHTOOL_GLINK:
      retval = ethtool_get_link(netif, (struct ethtool_value *)ifr->ifr_data);
      break;
    case ETHTOOL_GSET:
      retval = ethtool_get_settings(netif, (struct ethtool_cmd *)ifr->ifr_data);
      break;
    case ETHTOOL_SSET:
      retval = ethtool_set_settings(netif, (struct ethtool_cmd *)ifr->ifr_data);
      break;
    default:
      retval = EOPNOTSUPP;
      break;
  }

  if (ops && ops->complete) {
    ops->complete(netif);
  }
  return retval;
}
#endif /* LWIP_NETIF_ETHTOOL && LWIP_SOCKET */

