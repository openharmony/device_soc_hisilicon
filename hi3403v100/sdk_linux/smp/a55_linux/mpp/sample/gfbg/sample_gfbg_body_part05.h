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

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_GFBG_SAMPLE_GFBG_BODY_PART05_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_GFBG_SAMPLE_GFBG_BODY_PART05_H_
}
static td_s32 sample_gfbg_one_buf_mode(vo_device_info *vo_dev_info)
{
    td_s32 ret;
    pthread_gfbg_sample_info info0;
    ot_vo_dev vo_dev = vo_dev_info->vo_dev;
    g_vo_intf_type = vo_dev_info->vo_intf_type;
    ot_vb_cfg vb_conf;

    /* step  1: init variable */
    if (memset_s(&vb_conf, sizeof(ot_vb_cfg), 0, sizeof(ot_vb_cfg)) != EOK) {
        sample_print("%s:%d:memset_s failed\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }
    /* step 2: mpp system init. */
    ret = sample_comm_sys_init(&vb_conf);
    if (ret != TD_SUCCESS) {
        sample_print("system init failed with %d!\n", ret);
        return ret;
    }
    /*
     * step 3: start VO device.
     * NOTE: step 3 is optional when VO is running on other system.
     */
    ret = sample_gfbg_start_vo(vo_dev_info);
    if (ret != TD_SUCCESS) {
        sample_print("VO device %d start failed\n", vo_dev_info->vo_dev);
        goto sample_gfbg_one_buf_mode_0;
    }
    /* step 4:  start gfbg. */
    info0.layer =  vo_dev;
    info0.fd = -1;
    info0.ctrlkey = 0;   /* one buffer */
    info0.compress = TD_FALSE;
    info0.color_format = OT_FB_FORMAT_ABGR1555;
    if (pthread_create(&g_gfbg_thread, 0, sample_gfbg_refresh, (td_void*)(&info0)) != 0) {
        sample_print("start gfbg thread failed!\n");
        goto sample_gfbg_one_buf_mode_1;
    }
    sample_gfbg_to_exit();
sample_gfbg_one_buf_mode_1:
    sample_gfbg_stop_vo(vo_dev_info);
sample_gfbg_one_buf_mode_0:
    sample_comm_sys_exit();
    return ret;
}
static td_s32 sample_gfbg_none_buf_mode(vo_device_info *vo_dev_info)
{
    td_s32 ret;
    pthread_gfbg_sample_info info0;
    ot_vo_dev vo_dev = vo_dev_info->vo_dev;
    g_vo_intf_type = vo_dev_info->vo_intf_type;
    ot_vb_cfg vb_conf;

    /* step  1: init variable */
    if (memset_s(&vb_conf, sizeof(ot_vb_cfg), 0, sizeof(ot_vb_cfg)) != EOK) {
        sample_print("%s:%d:memset_s failed\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }
    /* step 2: mpp system init. */
    ret = sample_comm_sys_init(&vb_conf);
    if (ret != TD_SUCCESS) {
        sample_print("system init failed with %d!\n", ret);
        return ret;
    }
    /*
     * step 3: start VO device.
     * NOTE: step 3 is optional when VO is running on other system.
     */
    ret = sample_gfbg_start_vo(vo_dev_info);
    if (ret != TD_SUCCESS) {
        sample_print("VO device %d start failed\n", vo_dev_info->vo_dev);
        goto sample_gfbg_none_buf_mode_0;
    }
    /* step 4:  start gfbg. */
    info0.layer = vo_dev;
    info0.fd = -1;
    info0.ctrlkey = 3; /* 3: none buffer */
    info0.compress = TD_FALSE;
    info0.color_format = OT_FB_FORMAT_ABGR1555;
    if (pthread_create(&g_gfbg_thread, 0, sample_gfbg_refresh, (td_void*)(&info0)) != 0) {
        sample_print("start gfbg thread failed!\n");
        goto sample_gfbg_none_buf_mode_1;
    }
    sample_gfbg_to_exit();
sample_gfbg_none_buf_mode_1:
    sample_gfbg_stop_vo(vo_dev_info);
sample_gfbg_none_buf_mode_0:
    sample_comm_sys_exit();
    return ret;
}
static td_s32 sample_gfbg_dcmp(vo_device_info *vo_dev_info)
{
    td_s32 ret;
    pthread_gfbg_sample_info info0;
    ot_vo_dev vo_dev = vo_dev_info->vo_dev;
    g_vo_intf_type = vo_dev_info->vo_intf_type;
    ot_vb_cfg vb_conf;

    /* step  1: init variable */
    if (memset_s(&vb_conf, sizeof(ot_vb_cfg), 0, sizeof(ot_vb_cfg)) != EOK) {
        sample_print("%s:%d:memset_s failed\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }
    /* step 2: mpp system init. */
    ret = sample_comm_sys_init(&vb_conf);
    if (ret != TD_SUCCESS) {
        sample_print("system init failed with %d!\n", ret);
        return TD_FAILURE;
    }
    /*
     * step 3: start VO device.
     * NOTE: step 3 is optional when VO is running on other system.
     */
    ret = sample_gfbg_start_vo(vo_dev_info);
    if (ret != TD_SUCCESS) {
        sample_print("VO device %d start failed\n", vo_dev_info->vo_dev);
        goto sample_gfbg_double_buf_mode_0;
    }
    /* step 4:  start gfbg. */
    info0.layer =  vo_dev;
    info0.fd = -1;
    info0.ctrlkey =  1;
    info0.compress = TD_TRUE;
    info0.color_format = OT_FB_FORMAT_ABGR1555;
    if (pthread_create(&g_gfbg_thread, 0, sample_gfbg_refresh, (td_void*)(&info0)) != 0) {
        sample_print("start gfbg thread failed!\n");
        goto sample_gfbg_double_buf_mode_1;
    }
    sample_gfbg_to_exit();

sample_gfbg_double_buf_mode_1:
    sample_gfbg_stop_vo(vo_dev_info);
sample_gfbg_double_buf_mode_0:
    sample_comm_sys_exit();

    return ret;
}
static td_s32 sample_gfbg_dcmp_argb8888(vo_device_info *vo_dev_info)
{
    td_s32 ret;
    pthread_gfbg_sample_info info0;
    ot_vo_dev vo_dev = vo_dev_info->vo_dev;
    g_vo_intf_type = vo_dev_info->vo_intf_type;
    ot_vb_cfg vb_conf;

    /* step  1: init variable */
    if (memset_s(&vb_conf, sizeof(ot_vb_cfg), 0, sizeof(ot_vb_cfg)) != EOK) {
        sample_print("%s:%d:memset_s failed\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }
    /* step 2: mpp system init. */
    ret = sample_comm_sys_init(&vb_conf);
    if (ret != TD_SUCCESS) {
        sample_print("system init failed with %d!\n", ret);
        return TD_FAILURE;
    }
    /*
     * step 3: start VO device.
     * NOTE: step 3 is optional when VO is running on other system.
     */
    ret = sample_gfbg_start_vo(vo_dev_info);
    if (ret != TD_SUCCESS) {
        sample_print("VO device %d start failed\n", vo_dev_info->vo_dev);
        goto sample_gfbg_double_buf_mode_0;
    }
    /* step 4:  start gfbg. */
    info0.layer = vo_dev;
    info0.fd = -1;
    info0.ctrlkey = 1;
    info0.compress = TD_TRUE;
    info0.color_format = OT_FB_FORMAT_ARGB8888;
    if (pthread_create(&g_gfbg_thread, 0, sample_gfbg_refresh, (td_void*)(&info0)) != 0) {
        sample_print("start gfbg thread failed!\n");
        goto sample_gfbg_double_buf_mode_1;
    }
    sample_gfbg_to_exit();

sample_gfbg_double_buf_mode_1:
    sample_gfbg_stop_vo(vo_dev_info);
sample_gfbg_double_buf_mode_0:
    sample_comm_sys_exit();

    return ret;
}
static td_s32 sample_gfbg_clut_mode(vo_device_info *vo_dev_info)
{
    td_s32  ret;
    pthread_gfbg_sample_info info0;
    g_vo_intf_type  = vo_dev_info->vo_intf_type;
    g_vo_dev = vo_dev_info->vo_dev;
    ot_vb_cfg  vb_conf;

    /* step  1: init variable */
    if (memset_s(&vb_conf, sizeof(ot_vb_cfg), 0, sizeof(ot_vb_cfg)) != EOK) {
        sample_print("%s:%d:memset_s failed\n", __FUNCTION__, __LINE__);
        return TD_FAILURE;
    }
    /* step 2: mpp system init. */
    ret = sample_comm_sys_init(&vb_conf);
    if (ret != TD_SUCCESS) {
        sample_print("system init failed with %d!\n", ret);
        goto sample_gfbg_clut_mode_0;
    }
    /* step 3: start VO device.
     NOTE: step 3 is optional when VO is running on other system. */
    ret = sample_gfbg_start_vo(vo_dev_info);
    if (ret != TD_SUCCESS) {
        sample_print("VO device %d start failed\n", vo_dev_info->vo_dev);
        goto sample_gfbg_clut_mode_0;
    }
    /* step 4:  start gfbg. */
    info0.layer =  GRAPHICS_LAYER_G3;
    info0.fd  = -1;
    info0.ctrlkey = 2; /* 2 0buffer pan display */
    info0.compress = TD_FALSE;
    info0.color_format = OT_FB_FORMAT_4BPP;
    if (pthread_create(&g_gfbg_thread, 0, sample_gfbg_g3_clut, (void *)(&info0)) != 0) {
        sample_print("start gfbg thread failed!\n");
        goto sample_gfbg_clut_mode_1;
    }
    sample_gfbg_to_exit();

sample_gfbg_clut_mode_1:
    sample_gfbg_stop_vo(vo_dev_info);
sample_gfbg_clut_mode_0:
    sample_comm_sys_exit();

    return ret;
}
#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_GFBG_SAMPLE_GFBG_BODY_PART05_H_ */
