/*
 * Copyright (c) 2025 HiSilicon (Shanghai) Technologies Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see
 * <https://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>

#include "ot_type.h"
#include "ot_ir.h"
#include "ot_ir_codedef.h"
#include "securec.h"

typedef struct {
    char *name;
    unsigned int irkey_value;
} irkey_adapt;

#define REMOTE_POWER      0xef10f708
#define REMOTE_MUTE       0xf20df708
#define REMOTE_INPUT_MOD  0xff00f708
#define REMOTE_TRACK      0xfe01f708
#define REMOTE_F1         0xb24df708
#define REMOTE_F2         0xb14ef708
#define REMOTE_F3         0xae51f708
#define REMOTE_F4         0xad52f708
#define MAX_CODETYPE      3
#define MAX_FREQ          128
#define DEFAULT_READ_COUNT 1

/* irkey_name irkey_value */
static const irkey_adapt g_irkey_adapt_array[] = {
    { "REMOTE_POWER       ", REMOTE_POWER }, /* power */
    { "REMOTE_MUTE        ", REMOTE_MUTE }, /* mute */
    { "REMOTE_INPUT_MOD   ", REMOTE_INPUT_MOD }, /* in out method */
    { "REMOTE_TRACK       ", REMOTE_TRACK }, /* track */
    { "REMOTE_KEY_ONE     ", 0xe01ff708 }, /* 1 */
    { "REMOTE_KEY_TWO     ", 0xfc03f708 }, /* 2 */
    { "REMOTE_KEY_THREE   ", 0xfb04f708 }, /* 3 */
    { "REMOTE_KEY_FOUR    ", 0xfa05f708 }, /* 4 */
    { "REMOTE_KEY_FIVE    ", 0xe31cf708 }, /* 5 */
    { "REMOTE_KEY_SIX     ", 0xf906f708 }, /* 6 */
    { "REMOTE_KEY_SEVEN   ", 0xf807f708 }, /* 7 */
    { "REMOTE_KEY_EIGHT   ", 0xf708f708 }, /* 8 */
    { "REMOTE_KEY_NINE    ", 0xe21df708 }, /* 9 */
    { "REMOTE_SWITH_1_2   ", 0xf609f708 }, /* -/-- */
    { "REMOTE_KEY_ZERO    ", 0xf50af708 }, /* 0 */
    { "REMOTE_BACKSPACE   ", 0xf40bf708 }, /* <- */
    { "REMOTE_INTERACTION ", 0xeb14f708 }, /* interaction */
    { "REMOTE_SEARCH      ", 0xea15f708 }, /* search */
    { "REMOTE_UP_CURSOR   ", 0xe916f708 }, /* up */
    { "REMOTE_LEFT_CURSOR ", 0xe817f708 }, /* left */
    { "REMOTE_RIGHT_CURSOR", 0xf30cf708 }, /* right */
    { "REMOTE_DOWN_CURSOR ", 0xed12f708 }, /* down */
    { "REMOTE_ENTER       ", 0xe718f708 }, /* enter */
    { "REMOTE_MENU        ", 0xf10ef708 }, /* menu */
    { "REMOTE_PAGEUP      ", 0xec13f708 }, /* page up */
    { "REMOTE_PAGEDOWN    ", 0xee11f708 }, /* page down */
    { "REMOTE_GO_BACK     ", 0xe619f708 }, /* back */
    { "REMOTE_BTV_PLAY    ", 0xe51af708 }, /* broadcast television */
    { "REMOTE_VOD_PLAY    ", 0xe41bf708 }, /* video on demand */
    { "REMOTE_REP_PLAY    ", 0xbb44f708 }, /* repeat play */
    { "REMOTE_RECALL      ", 0xbf40f708 }, /* recall */
    { "REMOTE_PLAYANDPAUSE", 0xa55af708 }, /* play/pause */
    { "REMOTE_FAST_REWIND ", 0xbd42f708 }, /* fast rewind */
    { "REMOTE_FAST_FORWARD", 0xba45f708 }, /* fast forward */
    { "REMOTE_STOP        ", 0xbc43f708 }, /* stop */
    { "REMOTE_SEEK        ", 0xb748f708 }, /* seek */
    { "REMOTE_FAVORITE    ", 0xb54af708 }, /* favorite */
    { "REMOTE_SETTING     ", 0xb649f708 }, /* setting */
    { "REMOTE_INFO        ", 0xb44bf708 }, /* information */
    { "REMOTE_CHANNEL_PLUS", 0xb34cf708 }, /* channel+ */
    { "REMOTE_CHANNEL_MINU", 0xaf50f708 }, /* channel- */
    { "REMOTE_VOL_PLUS    ", 0xb04ff708 }, /* volume+ */
    { "REMOTE_VOL_MINUS   ", 0xac53f708 }, /* volume- */
    { "REMOTE_F1          ", REMOTE_F1 }, /* F1 green key */
    { "REMOTE_F2          ", REMOTE_F2 }, /* F2 red key */
    { "REMOTE_F3          ", REMOTE_F3 }, /* F3 yellow key */
    { "REMOTE_F4          ", REMOTE_F4 }, /* F4 blue key */
    { "MYTEST             ", 0x31ceff00 } /* F4 blue key */
};

static int g_irkey_adapt_count = sizeof(g_irkey_adapt_array) / sizeof(irkey_adapt);
static int g_powerkey_down = 0;
static int g_mutekey_down = 0;
static int g_f1key_down = 0;
static int g_f2key_down = 0;
static int g_f3key_down = 0;
static int g_f4key_down = 0;

static void ot_ir_report_irkey(irkey_info_s rcv_irkey_info)
{
    int i;
    if (rcv_irkey_info.irkey_datah != 0) {
        return;
    }
    for (i = 0; i < g_irkey_adapt_count; i++) {
        if (rcv_irkey_info.irkey_datal == g_irkey_adapt_array[i].irkey_value) {
            printf("keyvalue=H/L 0x%lx/0x%lx\n", rcv_irkey_info.irkey_datah, rcv_irkey_info.irkey_datal);
            break;
        }
    }
    if (i >= g_irkey_adapt_count) {
        printf("Error. get a invalid code. irkey_datah=0x%.8x,irkey_datal=0x%.8x.\n",
               (int)rcv_irkey_info.irkey_datah, (int)rcv_irkey_info.irkey_datal);
        return;
    }
    printf("RECEIVE ---> %s\t", g_irkey_adapt_array[i].name);
    if (rcv_irkey_info.irkey_state_code == 1) {
        printf("KEYUP...\n");
    }
    if (rcv_irkey_info.irkey_state_code == 0) {
        if (rcv_irkey_info.irkey_datal == REMOTE_POWER) { /* POWER */
            g_powerkey_down = 1;
        } else if (rcv_irkey_info.irkey_datal == REMOTE_MUTE) { /* MUTE */
            g_mutekey_down = 1;
        } else if (rcv_irkey_info.irkey_datal == REMOTE_F1) { /* F1 */
            g_f1key_down = 1;
        } else if (rcv_irkey_info.irkey_datal == REMOTE_F2) { /* F2 */
            g_f2key_down = 1;
        } else if (rcv_irkey_info.irkey_datal == REMOTE_F3) { /* F3 */
            g_f3key_down = 1;
        } else if (rcv_irkey_info.irkey_datal == REMOTE_F4) { /* F4 */
            g_f4key_down = 1;
        }
    }
}

static void normal_report_irkey(irkey_info_s rcv_irkey_info)
{
    printf("RECEIVE ---> irkey_datah=0x%.8x, irkey_datal=0x%.8x\t", (int)(rcv_irkey_info.irkey_datah),
        (int)(rcv_irkey_info.irkey_datal));
    if (rcv_irkey_info.irkey_state_code == 1) {
        printf("KEYUP...");
    }
    printf("\n");
}

static void ir_config_fun(int fd, ot_ir_dev_param dev_param)
{
    int tmp[2]; /* 2 params */

    ioctl(fd, IR_IOC_SET_CODELEN, dev_param.code_len);

    ioctl(fd, IR_IOC_SET_FORMAT, dev_param.codetype);

    ioctl(fd, IR_IOC_SET_FREQ, dev_param.frequence);

    tmp[0] = dev_param.leads_min;
    tmp[1] = dev_param.leads_max;
    ioctl(fd, IR_IOC_SET_LEADS, tmp);

    tmp[0] = dev_param.leade_min;
    tmp[1] = dev_param.leade_max;
    ioctl(fd, IR_IOC_SET_LEADE, tmp);

    tmp[0] = dev_param.sleade_min;
    tmp[1] = dev_param.sleade_max;
    ioctl(fd, IR_IOC_SET_SLEADE, tmp);

    tmp[0] = dev_param.cnt0_b_min;
    tmp[1] = dev_param.cnt0_b_max;
    ioctl(fd, IR_IOC_SET_CNT0_B, tmp);

    tmp[0] = dev_param.cnt1_b_min;
    tmp[1] = dev_param.cnt1_b_max;
    ioctl(fd, IR_IOC_SET_CNT1_B, tmp);
}

/*
 * Default configuration.
 * Press down telecontroller randomly.
 * Check whether the report of the key is correct.
 * Check whether the delay time of repeated key is correct.
 * Check whether the function is correct after set the delay time of repeated key again.
 */
static void ot_ir_func_test_001(void)
{
    int res;
    int i;
    int count;
    int fd = -1;
    int delay;
    irkey_info_s rcv_irkey_info[4]; /* rcv 4 times */

    g_powerkey_down = 0;
    g_mutekey_down = 0;
    g_f1key_down = 0;
    g_f2key_down = 0;
    g_f3key_down = 0;
    g_f4key_down = 0;

    printf("ot_ir_func_test_001 start...\n");
    printf("REMOTE codetype ...NEC with simple repeat code - uPD6121G\n");
    fd = open("/dev/"OT_IR_DEVICE_NAME, O_RDWR);
    if (fd == -1) {
        printf("ERROR:can not open %s device. read return %d\n", OT_IR_DEVICE_NAME, fd);
        return;
    }

    ioctl(fd, IR_IOC_SET_ENABLE_KEYUP, 1);

    printf("REMOTE_POWER key to finish the test...\n");
    printf("REMOTE_MUTE  key to set repkey delay time...\n");
    while (!g_powerkey_down) {
        res = read(fd, rcv_irkey_info, sizeof(rcv_irkey_info));
        count = res / sizeof(irkey_info_s);
        if ((res > 0) && (res <= sizeof(rcv_irkey_info))) {
            for (i = 0; i < count; i++) {
                ot_ir_report_irkey(rcv_irkey_info[i]);
            }
        } else {
            printf("ot_ir_func_test_001 Error. read irkey device error. result=%d.\n", res);
        }

        if (g_powerkey_down) {
            printf("ot_ir_func_test_001 pass.\n\n");
            continue;
        }
        if (g_mutekey_down) {
            g_mutekey_down = 0;
            printf("REMOTE_MUTE  key to set repkey delay time...\n");
            printf("ot_ir_func_test_001: input repkey delay = 500ms");
            delay = 500; /* 500ms */
            ioctl(fd, IR_IOC_SET_REPKEY_TIMEOUTVAL, delay);
        }
    }

    close(fd);
}

/*
 * The right way to occupy the device exclusively: first time open the OT_IR device successfully ,
 * second time open the OT_IR device failure.
 */
static void ot_ir_func_test_002(void)
{
    int fd = -1;
    int fd2 = -1;
    printf("ot_ir_func_test_002 start...\n");
    printf("REMOTE codetype ...NEC with simple repeat code - uPD6121G\n");

    fd = open("/dev/"OT_IR_DEVICE_NAME, O_RDWR);
    if (fd == -1) {
        printf("ot_ir_func_test_002 ERROR:can not open %s device. read return %d\n", OT_IR_DEVICE_NAME, fd);
        return;
    }

    fd2 = open("/dev/"OT_IR_DEVICE_NAME, O_RDWR);
    if (fd2 == -1) {
        printf("ot_ir_func_test_002 ERROR:can not open %s device. read return %d\n", OT_IR_DEVICE_NAME, fd2);
        close(fd);
        return;
    }
    close(fd2);
    close(fd);

    fd = open("/dev/"OT_IR_DEVICE_NAME, O_RDWR);
    if (fd == -1) {
        printf("ot_ir_func_test_002 ERROR:can not open %s device. read return %d\n", OT_IR_DEVICE_NAME, fd);
        return;
    }
    close(fd);
    printf("ot_ir_func_test_002 pass.\n\n");
}

/* After a number of times to close the device,the device can be  successfully opened. */
static void ot_ir_func_test_003(void)
{
    int fd = 0;

    printf("ot_ir_func_test_003 start...\n");
    printf("REMOTE codetype ...NEC with simple repeat code - uPD6121G\n");

    fd = open("/dev/"OT_IR_DEVICE_NAME, O_RDWR);
    if (fd == -1) {
        printf("ot_ir_func_test_003 ERROR:can not open %s device. read return %d\n", OT_IR_DEVICE_NAME, fd);
        return;
    }
    close(fd);

    fd = open("/dev/"OT_IR_DEVICE_NAME, O_RDWR);
    if (fd == -1) {
        printf("ot_ir_func_test_003 ERROR:can not open %s device. read return %d\n", OT_IR_DEVICE_NAME, fd);
        return;
    }
    close(fd);

    printf("ot_ir_func_test_003 pass.\n\n");
}

/* Test all ioctl command function */
static void ot_ir_func_test_004(void)
{
    int fd = -1;
    int i;
    ot_ir_dev_param tmp;

    printf("ot_ir_func_test_004 start...\n");
    printf("REMOTE codetype ...NEC with simple repeat code - uPD6121G\n");

    fd = open("/dev/"OT_IR_DEVICE_NAME, O_RDWR);
    if (fd == -1) {
        printf("ot_ir_func_test_004 ERROR:can not open %s device. read return %d\n", OT_IR_DEVICE_NAME, fd);
        return;
    }
    for (i = 0; i < 16; i++) { /* all of 16 types */
        ir_config_fun(fd, g_static_dev_param[i]);
        ioctl(fd, IR_IOC_GET_CONFIG, &tmp);
        if (memcmp(&tmp, &(g_static_dev_param[i]), sizeof(ot_ir_dev_param)) != 0) {
            printf("ot_ir_func_test_004 ERROR. need check ioctl fun.\n");
            close(fd);
            return;
        }
    }
    printf("ot_ir_func_test_004 pass.\n\n");
    close(fd);
}

/*
 * Press down telecontroller randomly.
 * Check whether the report of the key is correct.
 * Check whether the delay time of repeated key is correct.
 * Check whether the key release message is correct.
 * Check whether the device's buffer is correct.
 */
static void ot_ir_func_test_005_read(int fd)
{
    irkey_info_s rcv_irkey_info;
    int i = 0;
    int res;
    int read_count;
    while (!g_powerkey_down) {
        res = read(fd, &rcv_irkey_info, sizeof(rcv_irkey_info));
        if ((res > 0) && (res <= sizeof(rcv_irkey_info))) {
            ot_ir_report_irkey(rcv_irkey_info);
        } else {
            printf("ot_ir_func_test_005 Error. read irkey device error. result=%d.\n", res);
        }
        if (g_powerkey_down) {
            printf("ot_ir_func_test_005 pass.\n\n");
            continue;
        }
        if (g_f1key_down || g_f2key_down) {
            g_f1key_down = 0;
            g_f2key_down = 0;
            printf("REMOTE_F1/F2  key to set support release key msg(0-not support, not 0-support)...\ninput = ");
            read_count = scanf_s("%d", &i);
            if (read_count == DEFAULT_READ_COUNT) {
                ioctl(fd, IR_IOC_SET_ENABLE_KEYUP, i);
            }
        }
        if (g_f3key_down) {
            g_f3key_down = 0;
            printf("REMOTE_F3    key to set repkey delay time...\ninput = ");
            read_count = scanf_s("%d", &i);
            if (read_count == DEFAULT_READ_COUNT) {
                ioctl(fd, IR_IOC_SET_REPKEY_TIMEOUTVAL, i);
            }
        }
        if (g_f4key_down) {
            g_f4key_down = 0;
            printf("REMOTE_F4    key to set device buffer length...\ninput = ");
            read_count = scanf_s("%d", &i);
            if (read_count == DEFAULT_READ_COUNT) {
                ioctl(fd, IR_IOC_SET_BUF, i);
            }
        }
    }
}

static void ot_ir_func_test_005(void)
{
    /*
     * Press down REMOTE_POWER key to finish the test:
     * Press down REMOTE_F1 key to set support release key msg(0--no,1--yes);
     * Press down REMOTE_F2 key to set support repeart key msg(0--no,1--yes);
     * Press down REMOTE_F3 key to set set repeart key delay time.
     * Press down REMOTE_F4 key to set device buffer length.
     */
    int fd = 0;

    printf("ot_ir_func_test_005 start...\n");
    printf("REMOTE codetype ...NEC with simple repeat code - uPD6121G\n");

    g_powerkey_down = 0;
    g_mutekey_down = 0;
    g_f1key_down = 0;
    g_f2key_down = 0;
    g_f3key_down = 0;
    g_f4key_down = 0;

    fd = open("/dev/"OT_IR_DEVICE_NAME, O_RDWR);
    if (fd == -1) {
        printf("ot_ir_func_test_005 ERROR:can not open %s device. read return %d\n", OT_IR_DEVICE_NAME, fd);
        return;
    }
    ioctl(fd, IR_IOC_ENDBG, 1);

    printf("REMOTE_POWER key to finish the test...\n");
    printf("REMOTE_F1    key to set support release key msg(0-not support, not 0-support)...\n");
    printf("REMOTE_F2    key to set support repeart key msg(0-not support, not 0-support)...\n");
    printf("REMOTE_F3    key to set repkey delay time...\n");
    printf("REMOTE_F4    key to set device buffer length...\n");

    ot_ir_func_test_005_read(fd);
    close(fd);
}

/*
 * Press down TC9012 telecontroller randomly.
 * Check whether the report of the key is correct.
 * Check whether the delay time of repeated key is correct.
 * Check whether the function is correct after set the delay time of repeated key again.
 */
static void ot_ir_func_test_006(void)
{
    int fd = 0;
    int i;
    int res;
    irkey_info_s rcv_irkey_info;

    printf("ot_ir_func_test_006 start...\n");
    printf("REMOTE codetype ...TC9012 - TC9012F/9243\n");

    fd = open("/dev/"OT_IR_DEVICE_NAME, O_RDWR);
    if (fd == -1) {
        printf("ot_ir_func_test_006 ERROR:can not open %s device. read return %d\n", OT_IR_DEVICE_NAME, fd);
        return;
    }

    ioctl(fd, IR_IOC_ENDBG, 1);

    ir_config_fun(fd, g_static_dev_param[4]); /* [4]->TC9012F */

    printf("random press 10 keys...\n");
    for (i = 0; i < 10; i++) { /* radom 10 keys */
        res = read(fd, &rcv_irkey_info, sizeof(rcv_irkey_info));
        if ((res > 0) && (res <= sizeof(rcv_irkey_info))) {
            normal_report_irkey(rcv_irkey_info);
        } else {
            printf("ot_ir_func_test_006 Error. read irkey device error. res=%d.\n", res);
        }
    }

    printf("ot_ir_func_test_006 pass.\n\n");
    close(fd);
}

/*
 * Press down NEC's telecontroller with full repeat code randomly.
 * Check whether the report of the key is correct.
 * Check whether the delay time of repeated key is correct.
 * Check whether the function is correct after set the delay time of repeated key again.
 */
static void ot_ir_func_test_007(void)
{
    /* NEC with full repeat code */
    int fd = 0;
    int i;
    int res;
    irkey_info_s rcv_irkey_info;

    printf("ot_ir_func_test_007 start...\n");
    printf("REMOTE codetype ...NEC with full repeat code\n");

    fd = open("/dev/"OT_IR_DEVICE_NAME, O_RDWR);
    if (fd == -1) {
        printf("ot_ir_func_test_007 ERROR:can not open %s device. read return %d\n", OT_IR_DEVICE_NAME, fd);
        return;
    }
    ir_config_fun(fd, g_static_dev_param[5]); /* [5]->NEC with full repeat code */
    printf("random press 10 keys...\n");
    for (i = 0; i < 10; i++) { /* radom 10 keys */
        res = read(fd, &rcv_irkey_info, sizeof(rcv_irkey_info));
        if ((res > 0) && (res <= sizeof(rcv_irkey_info))) {
            normal_report_irkey(rcv_irkey_info);
        } else {
            printf("ot_ir_func_test_007 Error. read irkey device error. res=%d.\n", res);
        }
    }

    printf("ot_ir_func_test_007 pass.\n\n");
    close(fd);
}

/*
 * Press down SONY's telecontroller randomly.
 * Check whether the report of the key is correct.
 * Check whether the delay time of repeated key is correct.
 * Check whether the function is correct after set the delay time of repeated key again.
 */
static void ot_ir_func_test_008(void)
{
    /* SONY */
    int fd = 0;
    int i;
    int res;
    irkey_info_s rcv_irkey_info;

    printf("ot_ir_func_test_008 start...\n");
    printf("REMOTE codetype ...SONY\n");

    fd = open("/dev/"OT_IR_DEVICE_NAME, O_RDWR);
    if (fd == -1) {
        printf("ot_ir_func_test_008 ERROR:can not open %s device. read return %d\n", OT_IR_DEVICE_NAME, fd);
        return;
    }
    ir_config_fun(fd, g_static_dev_param[12]); /* [12]->NEC with full repeat code */
    printf("random press 10 keys...\n");
    for (i = 0; i < 10; i++) { /* radom 10 keys */
        res = read(fd, &rcv_irkey_info, sizeof(rcv_irkey_info));
        if ((res > 0) && (res <= sizeof(rcv_irkey_info))) {
            normal_report_irkey(rcv_irkey_info);
        } else {
            printf("ot_ir_func_test_008 Error. read irkey device error. res=%d.\n", res);
        }
    }

    printf("ot_ir_func_test_008 pass.\n\n");
    close(fd);
}

/* -1: failure, 0: success */
static int ot_ir_strong_test_get_one_input(const char *prompt, unsigned short int *dev_param)
{
    int tmp;

    printf("%s", prompt);
    if (scanf_s("%d", &tmp) != DEFAULT_READ_COUNT) {
        return -1;
    }

    *dev_param = (unsigned short int)tmp;
    return 0;
}

#define ot_ir_strong_test_get_one_input_return(prompt, dev_param) \
    do { \
        if (ot_ir_strong_test_get_one_input(prompt, dev_param) != 0) { \
            return; \
        } \
    } while (0)

static void ot_ir_strong_test_get_input(ot_ir_dev_param *tmp)
{
    ot_ir_strong_test_get_one_input_return("input codetype = ", &tmp->codetype);
    ot_ir_strong_test_get_one_input_return("input code_len = ", &tmp->code_len);
    ot_ir_strong_test_get_one_input_return("input leads_min = ", &tmp->leads_min);
    ot_ir_strong_test_get_one_input_return("input leads_max = ", &tmp->leads_max);
    ot_ir_strong_test_get_one_input_return("input leade_min = ", &tmp->leade_min);
    ot_ir_strong_test_get_one_input_return("input leade_max = ", &tmp->leade_max);
    ot_ir_strong_test_get_one_input_return("input cnt0_b_min = ", &tmp->cnt0_b_min);
    ot_ir_strong_test_get_one_input_return("input cnt0_b_max = ", &tmp->cnt0_b_max);
    ot_ir_strong_test_get_one_input_return("input cnt1_b_min = ", &tmp->cnt1_b_min);
    ot_ir_strong_test_get_one_input_return("input cnt1_b_max = ", &tmp->cnt1_b_max);
    ot_ir_strong_test_get_one_input_return("input sleade_min = ", &tmp->sleade_min);
    ot_ir_strong_test_get_one_input_return("input sleade_max = ", &tmp->sleade_max);
}

/* According to test program,randomly enter 13 numbers  to test input function. */
static void ot_ir_strong_test_001(void)
{
    int fd = 0;
    int i = 0;
    ot_ir_dev_param tmp = g_static_dev_param[0];
    int read_count;

    printf("ot_ir_strong_test_001 start...\n");

    fd = open("/dev/"OT_IR_DEVICE_NAME, O_RDWR);
    if (fd == -1) {
        printf("ot_ir_strong_test_001 ERROR:can not open %s device. read return %d\n", OT_IR_DEVICE_NAME, fd);
        return;
    }

    /* input and config */
    printf("input frequence = ");
    read_count = scanf_s("%d", &i);
    if (read_count == DEFAULT_READ_COUNT) {
        tmp.frequence = (unsigned short int)i;
        ot_ir_strong_test_get_input(&tmp);
    }
    ir_config_fun(fd, tmp);

    printf("\n");

    /* then echo the config */
    ioctl(fd, IR_IOC_GET_CONFIG, &tmp);
    printf("output frequence = %d\n", tmp.frequence);
    printf("output codetype = %d\n", tmp.codetype);
    printf("output code_len = %d\n", tmp.code_len);
    printf("output leads_min = %d\n", tmp.leads_min);
    printf("output leads_max = %d\n", tmp.leads_max);
    printf("output leade_min = %d\n", tmp.leade_min);
    printf("output leade_max = %d\n", tmp.leade_max);
    printf("output cnt0_b_min = %d\n", tmp.cnt0_b_min);
    printf("output cnt0_b_max = %d\n", tmp.cnt0_b_max);
    printf("output cnt1_b_min = %d\n", tmp.cnt1_b_min);
    printf("output cnt1_b_max = %d\n", tmp.cnt1_b_max);
    printf("output sleade_min = %d\n", tmp.sleade_min);
    printf("output sleade_max = %d\n", tmp.sleade_max);

    printf("ot_ir_strong_test_001 pass.\n\n");
    close(fd);
}

/* Open device with ioctl function to check the range of legal paramers. */
static void set_ot_ir_dev_param(int fd, ot_ir_dev_param *out, int codetype, int frequence)
{
    ioctl(fd, IR_IOC_SET_FORMAT, codetype);
    ioctl(fd, IR_IOC_SET_FREQ, frequence);
    ioctl(fd, IR_IOC_GET_CONFIG, out);
}

static void ot_ir_strong_test_002(void)
{
    int fd = 0;
    ot_ir_dev_param in;
    ot_ir_dev_param out;

    printf("ot_ir_strong_test_002 start...\n");

    fd = open("/dev/"OT_IR_DEVICE_NAME, O_RDWR);
    if (fd == -1) {
        printf("ot_ir_strong_test_002 ERROR:can not open %s device. read return %d\n", OT_IR_DEVICE_NAME, fd);
        return;
    }

    /* Check the legal minimum of paramers */
    in = g_static_dev_param[0];
    in.codetype = 0;
    in.frequence = 1;
    set_ot_ir_dev_param(fd, &out, in.codetype, in.frequence);
    if (memcmp(&in, &out, sizeof(ot_ir_dev_param)) != 0) {
        printf("ot_ir_strong_test_002 ERROR: critical input check fail.\n");
        close(fd);
        return;
    }

    /* Check the legal maximum of paramers */
    in = g_static_dev_param[0];
    in.codetype = MAX_CODETYPE;
    in.frequence = MAX_FREQ;
    set_ot_ir_dev_param(fd, &out, in.codetype, in.frequence);
    if (memcmp(&in, &out, sizeof(ot_ir_dev_param)) != 0) {
        printf("ot_ir_strong_test_002 ERROR: critical input check fail.\n");
        close(fd);
        return;
    }

    /* Check the illegal minimum of paramers */
    in = g_static_dev_param[0];
    in.codetype = -1;
    in.frequence = 0;
    set_ot_ir_dev_param(fd, &out, in.codetype, in.frequence);
    if (memcmp(&in, &out, sizeof(ot_ir_dev_param)) == 0) {
        printf("ot_ir_strong_test_002 ERROR: critical input check fail.\n");
        close(fd);
        return;
    }

    /* Check the illegal maximum of paramers */
    in = g_static_dev_param[0];
    in.codetype = MAX_CODETYPE + 1;
    in.frequence = MAX_FREQ + 1;
    set_ot_ir_dev_param(fd, &out, in.codetype, in.frequence);
    if (memcmp(&in, &out, sizeof(ot_ir_dev_param)) == 0) {
        printf("ot_ir_strong_test_002 ERROR: critical input check fail.\n");
        close(fd);
        return;
    }

    printf("ot_ir_strong_test_002 pass.\n\n");
    close(fd);
}

/*
 * Test for boundary value of repeart key delay time and device buffer length.
 * Open the device and sleep 1 second,then input at most 20 numbers at one time.
 * Should be checked by human being.
 * See print messages for other settings
 */
static void ot_ir_strong_test_003(void)
{
    /*
     * Press down REMOTE_POWER key to finish the test:
     * Press down REMOTE_F1 key to set support release key msg(0--no,1--yes);
     * Press down REMOTE_F2 key to set support repeart key msg(0--no,1--yes);
     * Press down REMOTE_F3 key to set set repeart key delay time.
     * Press down REMOTE_F4 key to set device buffer length.
     */
    int fd = -1;
    int i;
    int read_count;
    irkey_info_s rcv_irkey_info[20]; /* rcv 20 times */

    printf("ot_ir_strong_test_003 start...\n");
    printf("REMOTE codetype ...NEC with simple repeat code - uPD6121G\n");

    g_powerkey_down = 0;
    g_mutekey_down = 0;
    g_f1key_down = 0;
    g_f2key_down = 0;
    g_f3key_down = 0;
    g_f4key_down = 0;

    fd = open("/dev/"OT_IR_DEVICE_NAME, O_RDWR);
    if (fd == -1) {
        printf("ot_ir_strong_test_003 ERROR:can not open %s device. read return %d\n", OT_IR_DEVICE_NAME, fd);
        return;
    }
    ioctl(fd, IR_IOC_ENDBG, 1); /* open debug */

    printf("REMOTE_POWER key to finish the test...\nREMOTE_F4    key to set device buffer length...\n");
    printf("Each loop will sleep 1s...\n");

    while (!g_powerkey_down) {
        read_count = read(fd, rcv_irkey_info, sizeof(rcv_irkey_info));
        if ((read_count > 0) && (read_count <= sizeof(rcv_irkey_info))) {
            read_count = read_count / sizeof(irkey_info_s);
            printf("read %d unit key...\n", read_count);
            for (i = 0; i < read_count; i++) {
                ot_ir_report_irkey(rcv_irkey_info[i]);
            }
        } else {
            printf("ot_ir_strong_test_003 Error. read irkey device error. result=%d.\n", read_count);
        }

        if (g_powerkey_down) {
            printf("ot_ir_strong_test_003 pass.\n\n");
            continue;
        }
        if (g_f4key_down) {
            g_f4key_down = 0;
            printf("REMOTE_F4    key to set device buffer length...\ninput = ");
            read_count = scanf_s("%d", &i);
            if (read_count == DEFAULT_READ_COUNT) {
                ioctl(fd, IR_IOC_SET_BUF, i);
            }
        }

        printf("sleep 1s...\n");
        sleep(1);
    }

    close(fd);
}

#ifdef __LITEOS__
int app_main(int argc, char *argv[])
#else
int main(int argc, char *argv[])
#endif
{
    ot_ir_func_test_001();
    ot_ir_func_test_002();
    ot_ir_func_test_003();
    ot_ir_func_test_004();
    ot_ir_func_test_005();
    ot_ir_func_test_006();
    ot_ir_func_test_007();
    ot_ir_func_test_008();
    ot_ir_strong_test_001();
    ot_ir_strong_test_002();
    ot_ir_strong_test_003();

    return 0;
}
