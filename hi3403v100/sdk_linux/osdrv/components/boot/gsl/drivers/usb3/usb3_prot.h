/*
 * Copyright (c) 2025 HiSilicon (Shanghai) Technologies Co., Ltd.
 *
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
 */

#ifndef __USB3_PROT_H__
#define __USB3_PROT_H__

/*
 * macro definition
*/

/* structure of the bootstrap protocol frame */
/* FILE FRAME: TYPE(1)+SEQ(1)+CSEQ(1)+FILE(1)+LENGTH(4)+ADDRESS(4)+CRC(2) */
/* DATA FRAME: TYPE(1)+SEQ(1)+CSEQ(1)+DATA(0~1024)+CRC(2) */
/* EOT  FRAME: TYPE(1)+SEQ(1)+CSEQ(1)+CRC(2) */
#define	FRAME_HEAD			0
#define	FRAME_SEQ			1
#define	FRAME_CSEQ			2
#define	FRAME_TYPE			3
#define	FRAME_DATA_START		3
#define	FRAME_LENGTH			4
#define	FRAME_ADDRESS			8

/* type of bootstrap frame */
#define	FRAME_FILE			0xFE
#define	FRAME_DATA			0xDA
#define	FRAME_EOT			0xED
#define FRAME_INQUIRE			0xCD    /* query-frame header byte */

/* response of bootstrap frame */
#define	USB3_RESPONSE_ACK		0xAA
#define	USB3_RESPONSE_NAK		0x55

/* length of different type bootstrap frame */
#define FRAME_FILE_LEN			14
#define FRAME_EOT_LEN			5

#define FRAME_HEAD_LEN			5
#define FRAME_DATA_LEN			1024

/* type of bootstrap download */
#define	FILE_RAMINIT			1
#define	FILE_USB			2

/* return value of bootstrap protocol processing */
#define	USB3_XFR_PROT_OK		0
#define	USB3_XFR_PROT_ERR		1
#define	USB3_XFR_PROT_SKIP		2
#define	USB3_XFR_PROT_COMPLETE		3
#define USB3_XFR_PROT_INPROGRESS	4
#define USB3_XFR_PROT_WAIT		0x05    /* receiving status: no data received, continue waiting*/
#define USB3_XFR_PROT_INQUIRE		0x06    /* querying error codes */
#define USB3_XFR_PROT_ADDR_ERROR	0x07

void usb3_handle_protocol(void *dev);

#endif /* __USB3_PROT_H__ */
