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
#ifndef __COMMON_H_
#define __COMMON_H_

void call_reset(void);
#define ALWAYS_INLINE inline __attribute__((always_inline))

#define STEP_AUTH                   0x5A3C

#define random_delay()

#define RSA_4096_LEN      0x200

/*-----------------------------------------------------------------
 * serial interface
------------------------------------------------------------------*/
int serial_init();
int serial_deinit();
void serial_putc(const char c);
void serial_putchar(const char c);
void serial_puts(const char *s);
void serial_flush();
int serial_getc(void);
int serial_tstc(void);
void uart_init();
void uart_deinit();
void uart_reset();
void uart_port_init();
void serial_puts(const char *s);
void log_serial_puts(const char *s);
void serial_put_hex(unsigned int hex);
/*-----------------------------------------------------------------
 * mmc interface
------------------------------------------------------------------*/
int mmc_init();
void mmc_deinit();
size_t mmc_read(void *ptr, size_t src, size_t size, size_t read_type);
void mmc_muxctrl_config(void);

/*-----------------------------------------------------------------
 * timer interface
------------------------------------------------------------------*/
void timer_init();
void timer_deinit();
void timer_start();
unsigned long timer_get_val();
unsigned long timer_get_divider();
void mdelay(unsigned long msec);
void err_print(uint8_t err_type, uint8_t err_idx);
int copy_from_usb(const void *dest, size_t count);

#endif /*__COMMON_H_*/
