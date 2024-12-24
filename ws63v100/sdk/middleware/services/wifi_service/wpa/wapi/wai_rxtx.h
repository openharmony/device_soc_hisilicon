/*
 *   Copyright (c) 2020 HiSilicon (Shanghai) Technologies CO., LIMITED.
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
 * Description: WAPI Implementation
 */

#ifndef WAI_RXTX_H
#define WAI_RXTX_H

typedef struct _wai_hdr_stru {
    unsigned char version[2];
    unsigned char type;
    unsigned char subtype;
    unsigned char reserve[2];
    unsigned char length[2];
    unsigned char frameseq[2];
    unsigned char fragseq;
    unsigned char flag;
    /* Followed by Data */
} STRUCT_PACKED wai_hdr_stru;

void wai_rx_packet(void *ctx, const unsigned char *src_mac,
    const unsigned char *buf, unsigned int datalen);
int wai_tx_packet(struct wpa_supplicant *wpa,
    const unsigned char *buf, unsigned int datalen);

#endif /* end of wai_rxtx.h */
