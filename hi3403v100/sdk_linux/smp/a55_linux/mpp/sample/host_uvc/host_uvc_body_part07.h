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

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_HOST_UVC_HOST_UVC_BODY_PART07_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_HOST_UVC_HOST_UVC_BODY_PART07_H_
    }
    if (ctrl_info->do_set_control) {
        sample_uvc_video_set_control(dev, ctrl_info->ctrl_name, ctrl_info->ctrl_value);
    }
    if (ctrl_info->do_send_extension) {
        sample_uvc_video_send_extension(dev, ctrl_info->extension_name, ctrl_info->extension_channel);
    }
    if (ctrl_info->do_list_controls) {
        sample_uvc_video_list_controls(dev);
    }
    if (ctrl_info->do_reset_controls) {
        sample_uvc_video_reset_controls(dev);
    }
}
static td_s32 sample_uvc_set_video_format(device_info *dev, uvc_ctrl_info *ctrl_info)
{
    td_s32 ret;

    if (ctrl_info->do_enum_formats) {
        printf("- Available formats:\n");
        sample_uvc_video_enum_formats(dev, V4L2_BUF_TYPE_VIDEO_CAPTURE);
        sample_uvc_video_enum_formats(dev, V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE);
        sample_uvc_video_enum_formats(dev, V4L2_BUF_TYPE_VIDEO_OUTPUT);
        sample_uvc_video_enum_formats(dev, V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE);
        sample_uvc_video_enum_formats(dev, V4L2_BUF_TYPE_VIDEO_OVERLAY);
        sample_uvc_video_enum_formats(dev, V4L2_BUF_TYPE_META_CAPTURE);
    }
    if (ctrl_info->do_enum_inputs) {
        printf("- Available inputs:\n");
        sample_uvc_video_enum_inputs(dev);
    }
    if (ctrl_info->do_set_input) {
        sample_uvc_video_set_input(dev, ctrl_info->input);
        ret = sample_uvc_video_get_input(dev);
        printf("Input %d selected\n", ret);
    }
    if (ctrl_info->do_set_format) {
        if (sample_uvc_video_set_format(dev, ctrl_info) < 0) {
            sample_uvc_video_close(dev);
            return TD_FAILURE;
        }
    }
    if (!ctrl_info->no_query || ctrl_info->do_capture) {
        sample_uvc_video_get_format(dev);
    }
    if (ctrl_info->do_set_time_per_frame) {
        if (sample_uvc_video_set_frame_rate(dev, &ctrl_info->time_per_frame) < 0) {
            sample_uvc_video_close(dev);
            return TD_FAILURE;
        }
    }
    return TD_SUCCESS;
}
static td_bool sample_uvc_is_close_video(device_info *dev, uvc_ctrl_info *ctrl_info)
{
    struct sched_param sched;
    td_s32 ret;

    while (ctrl_info->do_sleep_forever) {
        sleep(1000);    /* 1000:time */
    }
    if (!ctrl_info->do_capture) {
        sample_uvc_video_close(dev);
        return TD_TRUE;
    }
    if (sample_uvc_video_set_quality(dev, ctrl_info->quality) < 0) {
        sample_uvc_video_close(dev);
        return TD_TRUE;
    }
    if (sample_uvc_video_prepare_capture(dev, ctrl_info->nbufs, ctrl_info->userptr_offset,
        ctrl_info->pattern, ctrl_info->fill)) {
        sample_uvc_video_close(dev);
        return TD_TRUE;
    }
    if (!ctrl_info->do_queue_late && sample_uvc_video_queue_all_buffers(dev, ctrl_info->fill)) {
        sample_uvc_video_close(dev);
        return TD_TRUE;
    }
    if (ctrl_info->do_rt) {
        (td_void)memset_s(&sched, sizeof(sched), 0, sizeof(sched));
        sched.sched_priority = ctrl_info->rt_priority;
        ret = sched_setscheduler(0, SCHED_RR, &sched);
        if (ret < 0) {
            sample_print("Failed to select RR scheduler\n");
        }
    }
    return TD_FALSE;
}
static td_void sample_uvc_init(device_info *dev, uvc_ctrl_info *ctrl_info)
{
    sample_uvc_ctrl_info_init(ctrl_info);
    sample_uvc_video_init(dev);
}
#if (MEDIA_WORK == 1)
static td_void sample_uvc_setup_signal(void)
{
    struct sigaction sig_exit = {0};

    sig_exit.sa_handler = sample_uvc_exit_signal_handler;
    sigaction(SIGINT, &sig_exit, TD_NULL);
    sigaction(SIGTERM, &sig_exit, TD_NULL);
}
#endif

td_s32 main(td_s32 argc, td_char *argv[])
{
    device_info dev = {0};
    td_s32 ret;
    td_s32 type;
    uvc_ctrl_info ctrl_info = {0};

#if (MEDIA_WORK == 1)
    sample_uvc_setup_signal();
#endif
    SDK_init();
    sample_uvc_init(&dev, &ctrl_info);
    opterr = 0;

    ret = sample_uvc_get_opt(argc, argv, &dev, &ctrl_info);
    if (ret != TD_SUCCESS) {
        return TD_FAILURE;
    }
    ret = sample_uvc_prepare_to_set_ctrl(argc, argv, &dev, &ctrl_info, &type);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    sample_uvc_set_ctrl(&dev, &ctrl_info, type);

    ret = sample_uvc_set_video_format(&dev, &ctrl_info);
    if (ret != TD_SUCCESS) {
        return ret;
    }
    if (sample_uvc_is_close_video(&dev, &ctrl_info) == TD_TRUE) {
        return TD_FAILURE;
    }
#if (MEDIA_WORK == 1)
    if (sample_uvc_media_init(sample_uvc_video_get_data_type(&dev), ctrl_info.width, ctrl_info.height) != TD_SUCCESS) {
        sample_print("media start failed!\n");
        goto video_close;
    }
#endif

    ctrl_info.type_name = sample_uvc_v4l2_format_name(ctrl_info.pixelformat);
    if (sample_uvc_video_do_capture(&dev, &ctrl_info) < 0) {
        ret = TD_FAILURE;
    }
#if (MEDIA_WORK == 1)
    sample_uvc_media_stop_receive_data();
    sample_uvc_media_exit();
    SDK_exit();
    printf("media exit...\n");
#endif
video_close:
    sample_uvc_video_close(&dev);
    sample_uvc_pause_cleanup();

    return ret;
}
#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_HOST_UVC_HOST_UVC_BODY_PART07_H_ */
