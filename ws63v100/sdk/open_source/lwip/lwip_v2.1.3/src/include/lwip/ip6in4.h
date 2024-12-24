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
 * Description: IP6 in IP4 header
 * Author: NA
 * Create: 2020
 */
#ifndef LWIP_HDR_IP6_IN_IP4_H
#define LWIP_HDR_IP6_IN_IP4_H

#if LWIP_IP6IN4

#include "lwip/opt.h"
#include "lwip/netif.h"
#include "lwip/pbuf.h"
#include "lwip/ip.h"

#define IP6IN4_TMR_INTERVAL 1000

typedef struct {
  ip6_addr_t ip6; /* mbr wlan0 ipv6 addr */
  ip4_addr_t ip4; /* mbr wlan0 ipv4 addr */
  u32_t lifetime;
  uint8_t state;
  uint8_t in_use : 1;
} ip6in4_entry_t;

int ip6in4_ip6_input(struct pbuf *p, struct ip6_hdr *iphdr, const struct netif *inp, const ip6_addr_t *tunnel_end);
void ip6in4_ip4_input(struct pbuf *p, struct netif *inp, const ip4_addr_t *src, const ip4_addr_t *dst);

err_t ip6in4_entry_ip4_get(const ip6_addr_t *ip6, ip4_addr_t *ip4);

ip6in4_entry_t *ip6in4_entry_get_next_inuse(uint32_t *state);
#if LWIP_LOWPOWER
u32_t ip6in4_tmr_tick(void);
#endif
void ip6in4_tmr(void);
err_t ip6in4_deinit_netif(const struct netif *ntf);

#endif /* LWIP_IP6IN4 */
#endif /* LWIP_HDR_IP6_IN_IP4_H */
