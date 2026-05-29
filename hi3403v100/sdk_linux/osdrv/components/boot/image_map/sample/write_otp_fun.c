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
#include <../product/security_subsys/otp/include/ot_common_otp.h>
#include <../product/security_subsys/otp/include/ot_mpi_otp.h>
#include <common.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <securec.h>

/* Constants for reported magic-number values. */
#define OTP_SAMPLE_ARRAY_INDEX_32              32
#define OTP_SAMPLE_ARRAY_INDEX_8               8
#define OTP_SAMPLE_SHIFT_2                     2

#define array_size(x)	   (sizeof(x) / sizeof((x)[0]))
#define OTP_LOCK TD_TRUE
#define INVALID_LOCK TD_FALSE  /* The lock enable bit does not lock the bit field of this attribute. */
#define OTP_UNLOCK TD_FALSE
#define USER_LEN 1024
#define DEBUG 1

typedef enum {
    NONE = 0,
    KEY = 1,
    FLAG = 2,
    VALUE = 3,
} otp_field_type;

typedef struct {
    td_char *field_name;
    otp_field_type type;
    td_u32 n_bits;
    td_bool lock_status;
    td_char *val_str;
} opt_burn_field;

typedef struct {
    const char *field_name;
    unsigned int offset;
    unsigned int length;
    td_bool lock_status;
    unsigned char data[USER_LEN];
} user_data_s;

#define OTP_INVAL_DATA 0XFFFF
#define BIT_8 8
#define BIT_4 4
#define CHAR_2 2

/*
 * Notice:
 *     1. Lock all OTPs before the commercial use of mass production.
 *     2. Lock status is invalid for items with "INVALID_LOCK".
 */

opt_burn_field g_otp_startup_burn_fields[] = {};

opt_burn_field g_otp_function_burn_fields[] = {};

user_data_s g_wr_data[] = {};

void print_str(const char *name, unsigned char *msg, int msg_len)
{
    unsigned int  i;

    if (msg_len == 0) {
    return;
    }

    if (name != NULL) {
        printf("%s: \n", name);
    }

    for (i = 0; i < msg_len; i++) {
        if (i != 0 && (i & 0x0f) == 0) {
            printf("\n");
        }
        printf("%02X ", msg[i]);
    }
    printf("\n");
}

unsigned char hextochar(unsigned char bchar)
{
    /* ascii : '0' :0x30   '9' :0x30 */
    if (bchar >= 0x30 && bchar <= 0x39) {
        bchar -= 0x30;   /* ascii : '0' :0x30 */
    } else if (bchar >= 0x41 && bchar <= 0x46) {
        /* ascii : 'A' :0x41   'F' :0x46 */
        bchar -= 0x37;  /* 0x37 : Converted to a hexadecimal number */
    } else if (bchar >= 0x61 && bchar <= 0x66) {
        /* ascii : 'a' :0x61   'f' :0x66 */
        bchar -= 0x57;   /* 0x57 : Converted to a hexadecimal number */
    } else {
        bchar = 0xff;   /* 0xff : Invalid val */
    }
    return bchar;
}

unsigned int check_arg(char *from_val)
{
    if (from_val[1] != 'x' && from_val[1] != 'X') {
        printf("error: Invalid key! Please check.\n");
        return OTP_INVAL_DATA;
    }

    return 0;
}
unsigned int padding_key_buf(td_u8 *to_buf, unsigned int bit_width, char *from_val)
{
    int i = 0;
    int j = 0;
    int count = 0;

    count = check_arg(from_val);
    if (count == OTP_INVAL_DATA)
        return TD_FAILURE;

    from_val += CHAR_2;

    if (bit_width / BIT_8 > strlen(from_val) / CHAR_2) {   /* 8: Byte conversion   2: */
        for (i = 0; i < (bit_width / BIT_4 - strlen(from_val)) / CHAR_2; i++) {
            to_buf[i] = 0x00;
        }
        if ((bit_width / BIT_4 - strlen(from_val)) % CHAR_2 != 0) {
            to_buf[i] = from_val[0] - '0';
            from_val++;
            i++;
        }
    }

    for (j = 0; j < strlen(from_val) / CHAR_2; j++) {
        to_buf[i] = (hextochar(from_val[j * CHAR_2 ]) << BIT_4) | (0x0f & (hextochar(from_val[j * CHAR_2 + 1])));
        i++;
    }

    return TD_SUCCESS;
}

unsigned int padding_pv_buf(td_u8 *to_buf, unsigned int bit_width, char *from_val)
{
    int i = 0;
    int j = 0;
    int count = 0;

    count = check_arg(from_val);
    if (count == OTP_INVAL_DATA)
        return TD_FAILURE;

    from_val += CHAR_2;

    if (bit_width == 1) {
        if (strlen(from_val) % CHAR_2 == 1)
            to_buf[0] = (hextochar(from_val[0]) & 0x01);
        else if (strlen(from_val) % CHAR_2 == 0)
            to_buf[0] = (hextochar(from_val[1]) & 0x01);
    } else if (bit_width == BIT_4) {
        if (strlen(from_val) % CHAR_2 == 1)
            to_buf[0] = (hextochar(from_val[0]) & 0x0f);
        else if (strlen(from_val) % CHAR_2 == 0)
            to_buf[0] = (hextochar(from_val[1]) & 0x0f);
    } else if (bit_width >= BIT_8) {
        if (bit_width / BIT_8 > strlen(from_val) / CHAR_2) {
            for (i = 0; i < (bit_width / BIT_4 - strlen(from_val)) / CHAR_2; i++)
                to_buf[(bit_width / BIT_8 - 1) - i] = 0x00;

            if ((bit_width / BIT_4 - strlen(from_val)) % CHAR_2 != 0) {
                to_buf[(bit_width / BIT_8 - 1) - i] = from_val[0] - '0';
                from_val++;
                i++;
            }
        }

        for (j = 0; j < strlen(from_val) / CHAR_2; j++) {
            to_buf[(bit_width / BIT_8 - 1) - i] =
                (hextochar(from_val[j * CHAR_2]) << BIT_4) | (0x0f & (hextochar(from_val[j * OTP_SAMPLE_SHIFT_2 + 1])));
            i++;
        }
    }

    return TD_SUCCESS;
}

unsigned int padding_user_buf(td_u8 *to_buf, unsigned int value_len, char *from_val)
{
    int i = 0;
    int j = 0;
    int count = 0;

    count = check_arg(from_val);
    if (count == OTP_INVAL_DATA)
        return TD_FAILURE;

    from_val += CHAR_2;

    if (value_len > strlen(from_val) / CHAR_2) {
        for (i = 0; i < (value_len * CHAR_2 - strlen(from_val)) / CHAR_2; i++)
            to_buf[(value_len - 1) - i] = 0x00;

        if ((value_len * CHAR_2 - strlen(from_val)) % CHAR_2 != 0) {
            to_buf[(value_len - 1) - i] = from_val[0] - '0';
            from_val++;
            i++;
        }
    }

    for (j = 0; j < strlen(from_val) / CHAR_2; j++) {
        to_buf[(value_len - 1) - i] =
            (hextochar(from_val[j * CHAR_2]) << BIT_4) | (0x0f & (hextochar(from_val[j * CHAR_2 + 1])));
        i++;
    }

    return TD_SUCCESS;
}

td_s32 sample_burn_single_pv(ot_otp_burn_pv_item *item, td_u32 item_num)
{
    td_s32 ret;
    td_u32 i;
    ot_otp_burn_pv_item *tmp_item = TD_NULL;
    td_bool sta = TD_TRUE;

    for (i = 0; i < item_num; i++) {
        if (strcmp(item[i].field_name, "tee_sec_version") != 0) {
            tmp_item = &item[i];
            printf("\nburn pv %s --->\n", tmp_item->field_name);
            udelay(5000); /* 5000us */
            ret = ot_mpi_otp_burn_product_pv(tmp_item, 1);
            if (ret != TD_SUCCESS) {
                printf("error: burn %s item failed!\n", tmp_item->field_name);
                sta = TD_FALSE;
            } else {
                printf("burn %s item success!\n", tmp_item->field_name);
            }
        }
    }

    if (sta == TD_TRUE) {
        return TD_SUCCESS;
    } else {
        return TD_FAILURE;
    }
}

td_s32 sample_write_user_data(td_void)
{
    td_s32 ret;
    td_u32 i;
    td_u32 dat_num;
    user_data_s *usr_dat = TD_NULL;
    td_bool sta = TD_TRUE;

    dat_num = array_size(g_wr_data);

    for (i = 0; i < dat_num; i++) {
        usr_dat = &g_wr_data[i];
        printf("\nwrite data offset %u, length %u ", usr_dat->offset, usr_dat->length);
        udelay(5000); /* 5000us */
        ret = ot_mpi_otp_set_user_data(usr_dat->field_name, usr_dat->offset, usr_dat->data, usr_dat->length);
        if (ret != TD_SUCCESS) {
            printf("set user data failed\n");
            sta = TD_FALSE;
        } else {
            printf("set user data success\n");
        }
        if (g_wr_data[i].lock_status == OTP_LOCK) {
            ret = ot_mpi_otp_set_user_data_lock(usr_dat->field_name, usr_dat->offset, usr_dat->length);
            if (ret != TD_SUCCESS) {
                printf("error: set user data lock status failed!\n");
                sta = TD_FALSE;
            } else {
                printf("set user data lock status success!\n");
            }
        }
    }

    if (sta == TD_TRUE) {
        return TD_SUCCESS;
    } else {
        return TD_FAILURE;
    }
}

int otp_val_str_to_value(td_char *val_str, td_u8 *to_buf, td_u32 num, otp_field_type type)
{
    int ret = -1;
    switch (type) {
        case KEY:
            ret = padding_key_buf(to_buf, num, val_str);
            break;
        case FLAG:
            ret = padding_pv_buf(to_buf, num, val_str);
            break;
        case VALUE:
            ret = padding_user_buf(to_buf, num, val_str);
            break;
        default:
            printf("set user data lock status failed\n");
            return ret;
    }

    return ret;
}

static int fill_otp_burn_item(ot_otp_burn_pv_item *pv_item, const opt_burn_field *field)
{
    pv_item->burn = TD_TRUE; /* TD_TURE: Burn OTP, TD_FALSE: Read OTP */
    pv_item->value_len = field->n_bits;
    pv_item->lock = field->lock_status;
    if (memcpy_s(pv_item->field_name, OT_OTP_PV_NAME_MAX_LEN,
        field->field_name, strlen(field->field_name)) != EOK) {
        return -1;
    }

    if (otp_val_str_to_value(field->val_str, pv_item->value, field->n_bits, field->type) != 0) {
        return -1;
    }
#if DEBUG
    if (pv_item->value_len <= OTP_SAMPLE_ARRAY_INDEX_8) {
        print_str(pv_item->field_name, pv_item->value, 1);
    } else {
        print_str(pv_item->field_name, pv_item->value, pv_item->value_len / OTP_SAMPLE_ARRAY_INDEX_8);
    }
#endif
    return 0;
}

static int fill_otp_burn_items(ot_otp_burn_pv_item *pv_items, opt_burn_field *fields, size_t count)
{
    int i;

    for (i = 0; i < count; i++) {
        if (fill_otp_burn_item(&pv_items[i], &fields[i]) != 0) {
            return -1;
        }
    }
    return 0;
}

static int write_tee_sec_version(const opt_burn_field *field)
{
    td_s32 ret;
    unsigned char tee_sec_version_buff[16] = {0};

    if (otp_val_str_to_value(field->val_str, tee_sec_version_buff, field->n_bits, field->type) != 0) {
        return -1;
    }
#if DEBUG
    print_str(field->field_name, tee_sec_version_buff, field->n_bits / OTP_SAMPLE_ARRAY_INDEX_8);
#endif
    udelay(5000); /* 5000us */
    ret = ot_mpi_otp_set_user_data(field->field_name, 0, tee_sec_version_buff,
        field->n_bits / OTP_SAMPLE_ARRAY_INDEX_8);
    if (ret != TD_SUCCESS) {
        printf("error: set tee_sec_version user data failed!\n");
        return -1;
    }
    printf("set tee_sec_version user data success!\n");
    return 0;
}

static int write_tee_sec_version_if_exist(opt_burn_field *fields, size_t count)
{
    int i;

    for (i = 0; i < count; i++) {
        if (strcmp(fields[i].field_name, "tee_sec_version") == 0) {
            return write_tee_sec_version(&fields[i]);
        }
    }
    return 0;
}

int burn_otp_fields(opt_burn_field *fields, size_t count)
{
    ot_otp_burn_pv_item *pv_items = NULL;
    int ret = -1;

    if (count == 0 || count > (SIZE_MAX / sizeof(ot_otp_burn_pv_item))) {
        return -1;
    }

    pv_items = (ot_otp_burn_pv_item *)malloc(sizeof(ot_otp_burn_pv_item) * count);
    if (pv_items == NULL) {
        return -1;
    }

    memset_s(pv_items, count * sizeof(ot_otp_burn_pv_item), 0, count * sizeof(ot_otp_burn_pv_item));
    if (fill_otp_burn_items(pv_items, fields, count) != 0) {
        goto free;
    }

    sample_burn_single_pv(pv_items, count);
    ret = write_tee_sec_version_if_exist(fields, count);

free:
    free(pv_items);
    return ret;
}

int burn_otp_user_data(user_data_s *fields, size_t count)
{
    int i;
    char *buf = NULL;

    if (count == 0) {
        return -1;
    }

    buf = malloc(sizeof(fields[0].data));
    if (buf == NULL) {
        return -1;
    }

    memset_s(buf, sizeof(fields[0].data), 0, sizeof(fields[0].data));

    for (i = 0; i < count; i++) {
        if (memcpy_s(buf, sizeof(fields[0].data), fields[i].data, sizeof(fields[0].data)) != EOK) {
            free(buf);
            return -1;
        }

        if (otp_val_str_to_value(buf, fields[i].data, fields[i].length, VALUE) != 0) {
            free(buf);
            return -1;
        }
#if DEBUG
        print_str(fields[i].field_name, fields[i].data, OTP_SAMPLE_ARRAY_INDEX_32);
#endif
    }

    sample_write_user_data();

    free(buf);
    return 0;
}

static td_s32 do_write_otp(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    ot_mpi_otp_init();

    burn_otp_fields(g_otp_startup_burn_fields, array_size(g_otp_startup_burn_fields));
    burn_otp_fields(g_otp_function_burn_fields, array_size(g_otp_function_burn_fields));

    burn_otp_user_data(g_wr_data, array_size(g_wr_data));

    ot_mpi_otp_deinit();

    printf("\n write otp end\n");
    return 0;
}
U_BOOT_CMD (
    write_otp, CONFIG_SYS_MAXARGS, 0, do_write_otp,
    "write otp operation",
    "write otp\n"
);
