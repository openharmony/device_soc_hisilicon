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

#include "ddr_interface.h"
#include "ddr_training_impl.h"

#define __DDR_TRAINING_CONSOLE__
#ifdef DDR_TRAINING_CONSOLE_CONFIG

#define DDR_UART_BASE_REG               0x12090000
#define UART_PL01X_FR                   0x18 /* Flag register (Read only). */
#define UART_PL01X_FR_RXFE              0x10
#define UART_PL01X_DR                   0x00 /* Data read or written from the interface. */
#define UART_PL01X_ECR                  0x04 /* Error clear register (Write). */

#define ALIGN_COUNT  4

#define isprint(c)  ((c) >= ' ' && (c) <= '~')
#define isspace(c)  ((c) == ' ' || ((c) >= '\t' && (c) <= '\r'))
#define isdigit(c)  ((c) >= '0' && (c) <= '9')
#define isxdigit(c) (isdigit(c) || \
    ((c) >= 'A' && (c) <= 'F') || \
    ((c) >= 'a' && (c) <= 'f'))

/* DDR console get char */
static int ddr_console_getc(void)
{
    unsigned int data;

    /* Wait until there is data in the FIFO */
    while (reg_read(DDR_UART_BASE_REG + UART_PL01X_FR) & UART_PL01X_FR_RXFE) {
    }

    data = reg_read(DDR_UART_BASE_REG + UART_PL01X_DR);
    /* Check for an error flag */
    if (data & 0xFFFFFF00) {
        /* Clear the error */
        reg_write(0xFFFFFFFF, DDR_UART_BASE_REG + UART_PL01X_ECR);
        return -1;
    }

    return (int)data;
}

/* DDR read line */
static char *ddr_readline(char *str, int len)
{
    unsigned int c;
    char *p = str;
    while (len > 0) {
        c = ddr_console_getc();
        switch (c) {
        case '\r':
        case '\n':
            *p = '\0';
            DDR_PUTC('\r');
            DDR_PUTC('\n');
            return str;
        case 0x08:
        case 0x7F:
            if (p > str) {
                p--;
                len++;
                DDR_PUTC('\b');
                DDR_PUTC(' ');
                DDR_PUTC('\b');
            }
            break;
        default:
            if (isprint(c)) {
                (*p++) = (char)c;
                len--;
                DDR_PUTC(c);
            }
            break;
        }
    }
    (*--p) = '\0';
    return str;
}

/* HEX to INT */
static int hex2int(char **ss, unsigned int *n)
{
    unsigned char *s = (unsigned char *)(*ss);

    while (isspace(*s))
        s++;

    if (!(*s))
        return -1;

    if (s[0] == '0' && (s[1] == 'x' || s[1] == 'X'))
        s += 2; /* Skip the first 2 characters: 0x */

    for ((*n) = 0; isxdigit(*s); s++) {
        (*n) = ((*n) << 4); /* shift left 4 */
        if ((*s) >= '0' && (*s) <= '9')
            (*n) |= ((*s) - '0');
        else if ((*s) >= 'a' && (*s) <= 'f')
            (*n) |= ((*s) + 10 - 'a'); /* transfer a-f to 10-15 */
        else if ((*s) >= 'A' && (*s) <= 'F')
            (*n) |= ((*s) + 10 - 'A'); /* transfer A-F to 10-15 */
    }

    if (isspace(*s) || !(*s)) {
        while (isspace(*s))
            s++;
        (*ss) = (char *)s;
        return 0;
    }

    return -1;
}
/*
 * DDR do memory write.
 * mw address value [count]
 */
static int ddr_do_memory_write(char *cmd)
{
    unsigned int address;
    unsigned int value;
    unsigned int count = ALIGN_COUNT;

    if (hex2int(&cmd, &address))
        return -1;

    if (hex2int(&cmd, &value))
        return -1;

    if ((*cmd) && hex2int(&cmd, &count))
        return -1;

    if ((address & 0x03) || (count & 0x03)) {
        ddr_info("parameter should align with 4 bytes.\n");
        return -1;
    }
    for (; count > 0; count -= ALIGN_COUNT, address += ALIGN_COUNT) /* align with 4 bytes */
        reg_write(value, address);

    return 0;
}
/*
 * DDR do memory display.
 * md address [count]
 */
static int ddr_do_memory_display(char *cmd)
{
    unsigned int ix;
    unsigned int loop;
    unsigned int address;
    unsigned int count = 64;

    if (hex2int(&cmd, &address))
        return -1;

    if ((*cmd) && hex2int(&cmd, &count))
        return -1;

    if (count < ALIGN_COUNT)
        count = ALIGN_COUNT;

    address &= ~0x03;
    loop = (count & ~0x03);

    while (loop > 0) {
        DDR_PUTC('0');
        DDR_PUTC('x');
        DDR_PUT_HEX(address);
        DDR_PUTC(':');

        for (ix = 0;
            ix < ALIGN_COUNT && loop > 0;
            ix++, loop -= ALIGN_COUNT, address += ALIGN_COUNT) {
            DDR_PUTC(' ');
            DDR_PUT_HEX(reg_read(address));
        }
        DDR_PUTC('\r');
        DDR_PUTC('\n');
    }

    return 0;
}

static int ddr_do_sw_training(char *cmd)
{
    int result;
    struct ddr_cfg_st ddr_cfg;
    struct ddr_cfg_st *cfg = &ddr_cfg;
#ifdef DDR_TRAINING_CMD
    struct ddr_training_result_st ddrt_result_sram;
#endif

    ddr_training_cfg_init(cfg);
#ifdef DDR_TRAINING_CMD
    cfg->res_st = (void *)&ddrt_result_sram;
#endif
    result = ddr_training_all(cfg);

    result += ddr_dcc_training_func(cfg);

    return 0;
}

static int ddr_do_hw_training(char *cmd)
{
    int result;

    result = ddr_hw_training_func();

    return result;
}

/* Do DDR training console if sw training or hw training fail */
static int ddr_training_console(void)
{
    char str[256]; /* 256: Command length range */
    char *p = NULL;

    while (1) {
        DDR_PUTC('d');
        DDR_PUTC('d');
        DDR_PUTC('r');
        DDR_PUTC('#');

        p = ddr_readline(str, sizeof(str));
        while (isspace(*p))
            p++;
        if (p[0] == 'q')
            break;
        if (p[0] == 'm' && p[1] == 'w') {
            ddr_do_memory_write(p + 2); /* p[2]:Third character */
        } else if (p[0] == 'm' && p[1] == 'd') {
            ddr_do_memory_display(p + 2); /* p[2]:Third character */
        } else if (p[0] == 's' && p[1] == 'w') {
            ddr_do_sw_training(p + 2); /* p[2]:Third character */
        } else if (p[0] == 'h' && p[1] == 'w') {
            ddr_do_hw_training(p + 2); /* p[2]:Third character */
        }
    }

    return 0;
}
#else
static int ddr_training_console(void)
{
    ddr_warning("Not support DDR training console");
    return 0;
}
#endif /* DDR_TRAINING_CONSOLE_CONFIG */

int ddr_training_console_if(void)
{
    return DDR_TRAINING_CONSOLE();
}
