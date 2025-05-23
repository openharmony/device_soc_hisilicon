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
 * Description: declare RPL APIs
 * Author: none
 * Create: 2018
 */
#ifndef __LWIP_RPL_H__
#define __LWIP_RPL_H__

#include "lwip/opt.h"
#include "lwip/netif.h"

#if LWIP_RIPPLE

#ifdef __cplusplus
extern "C" {
#endif

u8_t lwip_ipv6_routetbl_is_nbr_nexthop(s8_t nbr_idx);
err_t lwip_stop_rpl(void *nif);
uint8_t lwip_rpl_is_br(void);
uint8_t lwip_rpl_is_router(void);
uint8_t lwip_rpl_is_rpl_netif(const struct netif *nif);
struct netif *lwip_rpl_route_netif_lookup(const ip6_addr_t *src, const ip6_addr_t *dest);
err_t lwip_rpl_get_ra_prefix(const struct netif *nif, ip6_addr_t *prefix);
const ip6_addr_t *lwip_rpl_route_nexthop_lookup(const struct netif *iface, const ip6_addr_t *ip6addr);
u8_t lwip_rpl_route_is_ipv4sta(void *rte);
u8_t lwip_rpl_route_is_sync(void *route_entry);
void *lwip_rpl_route_entry_lookup(const ip6_addr_t *ip6addr);
int lwip_rpl_route_entry_nhop_get(const ip6_addr_t *dest, ip6_addr_t *nhop);
err_t lwip_rpl_route_nhop(void *route_entry, ip6_addr_t *nhop);
struct pbuf *lwip_add_rpi_hdr(struct pbuf *p, u8_t nexth, u16_t hbh_len, u8_t flags);
int lwip_verify_rplext_header(const struct pbuf *p, u16_t ext_opt_offset);
err_t ip6_update_rpi(struct pbuf *p, const struct netif *inp);
uint8_t lwip_rpl_dao_proxy(const ip6_addr_t *ip6addr, const u8_t *mac, u8_t maclen);
void lwip_rpl_dao_output_proxy_ip4sta(uint32_t *ip4, uint8_t lifetime, uint8_t flags);
err_t lwip_rpl_behind_mbr_node(const ip6_addr_t *target);
err_t lwip_rpl_nonmesh_node(const ip6_addr_t *target);
err_t lwip_rpl_behind_mbr_solicited_node(const ip6_addr_t *target);
err_t lwip_rpl_nonmesh_solicited_node(const ip6_addr_t *target);
void lwip_rpl_set_ip6_iid(const linklayer_addr_t *lladdr, uint8_t mnid, ip6_addr_t *ip6addr);
void lwip_rpl_set_addr_iid(ip6_addr_t *ip6addr, const linklayer_addr_t *lladdr);
err_t lwip_rpl_get_lladdr(const ip6_addr_t *ip6addr, linklayer_addr_t *lladdr);
void lwip_rpl_init(void);
void lwip_rpl_deinit(void);
int rpl_context_init(void);
int rpl_context_deinit(void);
int rpl_context_netif_free(const struct netif *nif);
err_t lwip_rpl_get_default_prefix(ip6_addr_t *prefix, uint8_t *len);
void lwip_rpl_trigger_global_repair(void);
void lwip_rpl_trigger_global_dao(void);
void lwip_rpl_trigger_msta(void);
uint8_t is_connect_to_ap(uint8_t *is_mesh_ap);
uint8_t is_connected_ap(const uint8_t *mac, uint8_t maclen);
uint8_t lwip_rpl_dag_parent_count(void);
uint8_t lwip_is_rpl_running(void);
void lwip_notify_rpl_get_ipv4_addr(uint8_t mnid, const ip6_addr_t *ip6addr);
err_t rpl_dodagid_get(uint8_t *dodagid, uint8_t len);
void *rpl_platform_get_dev(void);

#ifdef __cplusplus
}
#endif

#endif

#endif /* __LWIP_RPL_H__ */

