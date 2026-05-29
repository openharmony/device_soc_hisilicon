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

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_GFBG_SAMPLE_GFBG_BODY_PART07_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_GFBG_SAMPLE_GFBG_BODY_PART07_H_

{
    td_s32 ret = TD_FAILURE;
    td_char ch;
    ch = *(argv[1]);
    g_exit_flag = 0;
    if (ch == '0') {
        sample_print("\nindex 0 selected.\n");
        ret = sample_gfbg_standard_mode(vo_dev_info);
    } else if (ch == '1') {
        sample_print("\nindex 1 selected.\n");
        ret = sample_gfbg_double_buf_mode(vo_dev_info);
    } else if (ch == '2') {
        sample_print("\nindex 2 selected.\n");
        ret = sample_gfbg_one_buf_mode(vo_dev_info);
    } else if (ch == '3') {
        sample_print("\nindex 3 selected.\n");
        ret = sample_gfbg_none_buf_mode(vo_dev_info);
    } else if (ch == '4') {
        sample_print("\nindex 4 selected.\n");
        ret = sample_gfbg_dcmp(vo_dev_info);
    } else if (ch == '5') {
        sample_print("\nindex 5 selected.\n");
        ret = sample_gfbg_dcmp_argb8888(vo_dev_info);
    } else if (ch == '6') {
        sample_print("\nindex 6 selected.\n");
        ret = sample_gfbg_clut_mode(vo_dev_info);
#ifdef CONFIG_SUPPORT_SAMPLE_ROTATION
    } else if (ch == '7') {
        sample_print("\nindex 7 selected.\n");
        ret = sample_gfbg_rotation(vo_dev_info);
#endif
    } else {
        printf("index invalid! please try again.\n");
        sample_gfbg_usage1(argv[0]);
        return TD_FAILURE;
    }
    if (ret == TD_SUCCESS) {
        sample_print("program exit normally!\n");
    } else {
        sample_print("program exit abnormally!\n");
    }
    return ret;
}

#ifdef __LITEOS__
#define SAMPLE_GFBG_NAME "sample"
void sample_vo_sel_usage(td_void)
{
    printf("usage : %s <index> <device> <intf>\n", SAMPLE_GFBG_NAME);
    sample_gfbg_usage2();
    printf("\n/****************device******************/\n");
    printf("\t 0) VO device 0#, default.\n");
    printf("\t 1) VO device 1#.\n");
    printf("\n/****************intf******************/\n");
    printf("\t 0) VO HDMI output, default.\n");
    printf("\t 1) VO BT1120 output.\n");
    return;
}
#endif

#ifdef __LITEOS__
int app_main(int argc, char *argv[])
{
    td_s32 ret;
    vo_device_info vo_dev_info;
    vo_dev_info.vo_dev = SAMPLE_VO_DEV_DHD0;
    vo_dev_info.vo_intf_type = OT_VO_INTF_HDMI; /* default:HDMI or BT1120 */
    td_char sel_dev; /* '0': VO_INTF_CVBS, else: BT1120 */
    /* 4 2 for alg */
    if ((argc < 4) || (strlen(argv[1]) != 1) || (strlen(argv[2]) != 1)) {
        sample_vo_sel_usage();
        return TD_FAILURE;
    }
    if ((argc > 2) && *argv[2] == '1') { /* 2 for alg */
        vo_dev_info.vo_dev = SAMPLE_VO_DEV_DHD1;
    }
    /* '0': OT_VO_INTF_HDMI, else: OT_VO_INTF_BT1120 */
    if ((argc > 2) && *argv[3] == '1') { /* 2 3 for alg */
        vo_dev_info.vo_intf_type = OT_VO_INTF_BT1120;
    }

    ret = sample_choose_the_case(argv, &vo_dev_info);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    return ret;
}
#else
int main(int argc, char *argv[])
{
    td_s32 ret;
    vo_device_info vo_dev_info;
    vo_dev_info.vo_dev = SAMPLE_VO_DEV_DHD0;
    vo_dev_info.vo_intf_type = OT_VO_INTF_HDMI; /* default:HDMI or BT1120 */
    /* 4 2 3 for alg */
    if ((argc != 4) || (strlen(argv[1]) != 1) || (strlen(argv[2]) != 1) || (strlen(argv[3]) != 1)) {
        printf("index invalid! please try again.\n");
        sample_gfbg_usage1(argv[0]);
        return TD_FAILURE;
    }
    if (*argv[2] != '0' && *argv[2] != '1') { /* 2 for alg */
        printf("index invalid! please try again.\n");
        sample_gfbg_usage1(argv[0]);
        return TD_FAILURE;
    }
    if (*argv[3] != '0' && *argv[3] != '1') { /* 3 for alg */
        printf("index invalid! please try again.\n");
        sample_gfbg_usage1(argv[0]);
        return TD_FAILURE;
    }

#ifdef CONFIG_USER_SPACE
    SDK_init();
#endif

    sample_sys_signal(&sample_gfbg_handle_sig);
    /* '0': DHD0, else: DHD1 */
    if ((argc > 2) && *argv[2] == '1') { /* 2 for alg */
        vo_dev_info.vo_dev = SAMPLE_VO_DEV_DHD1;
    }
    /* '0': OT_VO_INTF_HDMI, else: OT_VO_INTF_BT1120 */
    if ((argc > 2) && *argv[3] == '1') { /* 2 3 for alg */
        vo_dev_info.vo_intf_type = OT_VO_INTF_BT1120;
    }

    ret = sample_choose_the_case(argv, &vo_dev_info);
    if (ret != TD_SUCCESS) {
        return ret;
    }
#ifdef CONFIG_USER_SPACE
    SDK_exit();
#endif
    return ret;
}
#endif


#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_GFBG_SAMPLE_GFBG_BODY_PART07_H_ */
