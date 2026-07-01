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

#ifndef HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_HOST_UVC_HOST_UVC_BODY_PART04_H_
#define HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_HOST_UVC_HOST_UVC_BODY_PART04_H_

            continue;
        }
        if (ret < 0) {
            return ret;
        }
        id = query_ext_ctrl.id;

        ret = callback(dev, &query_ext_ctrl);
        if (ret < 0) {
            return ret;
        }
        if (ret > 0) {
            nctrls++;
        }
    }
    return nctrls;
}
static td_void sample_uvc_video_query_menu(device_info *dev, const struct v4l2_query_ext_ctrl *query_ext_ctrl,
    td_u32 value)
{
    struct v4l2_querymenu menu;
    td_s32 ret;

    for (menu.index = query_ext_ctrl->minimum; menu.index <= (td_u32)query_ext_ctrl->maximum; menu.index++) {
        menu.id = query_ext_ctrl->id;
        ret = ioctl(dev->fd, VIDIOC_QUERYMENU, &menu);
        if (ret < 0) {
            continue;
        }
        if (query_ext_ctrl->type == V4L2_CTRL_TYPE_MENU) {
            sample_print("  %u: %.32s%s\n", menu.index, menu.name, menu.index == value ? " (*)" : "");
        } else {
            sample_print("  %u: %lld%s\n", menu.index, menu.value, menu.index == value ? " (*)" : "");
        }
    };
}
static td_void sample_uvc_video_print_control_array(const struct v4l2_query_ext_ctrl *query_ext_ctrl,
    const struct v4l2_ext_control *ext_control)
{
    td_u32 i;

    printf("{");

    for (i = 0; i < query_ext_ctrl->elems; ++i) {
        switch (query_ext_ctrl->type) {
            case V4L2_CTRL_TYPE_U8:
                printf("%u", ext_control->p_u8[i]);
                break;
            case V4L2_CTRL_TYPE_U16:
                printf("%u", ext_control->p_u16[i]);
                break;
            case V4L2_CTRL_TYPE_U32:
                printf("%u", ext_control->p_u32[i]);
                break;
            default:
                printf("invalid");
                break;
        }
        if (i != query_ext_ctrl->elems - 1) {
            printf(", ");
        }
    }
    printf("}");
}
static td_void sample_uvc_video_print_control_value(const struct v4l2_query_ext_ctrl *query_ext_ctrl,
    struct v4l2_ext_control *ext_control)
{
    if (query_ext_ctrl->nr_of_dims == 0) {
        switch (query_ext_ctrl->type) {
            case V4L2_CTRL_TYPE_INTEGER:
            case V4L2_CTRL_TYPE_BOOLEAN:
            case V4L2_CTRL_TYPE_MENU:
            case V4L2_CTRL_TYPE_INTEGER_MENU:
                sample_print("%d", ext_control->value);
                break;
            case V4L2_CTRL_TYPE_BITMASK:
                sample_print("0x%08x", ext_control->value);
                break;
            case V4L2_CTRL_TYPE_INTEGER64:
                sample_print("%lld", ext_control->value64);
                break;
            case V4L2_CTRL_TYPE_STRING:
                sample_print("%s", ext_control->string);
                break;
            default:
                sample_print("invalid");
                break;
        }
        return;
    }
    switch (query_ext_ctrl->type) {
        case V4L2_CTRL_TYPE_U8:
        case V4L2_CTRL_TYPE_U16:
        case V4L2_CTRL_TYPE_U32:
            sample_uvc_video_print_control_array(query_ext_ctrl, ext_control);
            break;
        default:
            sample_print("unsupported type %u", query_ext_ctrl->type);
            break;
    }
}
static td_s32 sample_uvc_video_get_control(device_info *dev, const struct v4l2_query_ext_ctrl *query_ext_ctrl,
    td_bool full)
{
    struct v4l2_ext_control ctrl;
    td_s32 ret;

    if (query_ext_ctrl->flags & V4L2_CTRL_FLAG_DISABLED) {
        return 0;
    }
    if (query_ext_ctrl->type == V4L2_CTRL_TYPE_CTRL_CLASS) {
        printf("--- %s (class 0x%08x) ---\n", query_ext_ctrl->name, query_ext_ctrl->id);
        return 0;
    }
    if (full == TD_TRUE) {
        printf("control 0x%08x `%s' min %lld max %lld step %llu default %lld ",
            query_ext_ctrl->id, query_ext_ctrl->name, query_ext_ctrl->minimum, query_ext_ctrl->maximum,
            query_ext_ctrl->step, query_ext_ctrl->default_value);
        if (query_ext_ctrl->nr_of_dims != 0) {
            for (td_u32 i = 0; i < query_ext_ctrl->nr_of_dims; ++i) {
                printf("[%u]", query_ext_ctrl->dims[i]);
            }
            printf(" ");
        }
    } else {
        printf("control 0x%08x ", query_ext_ctrl->id);
    }
    if (query_ext_ctrl->type == V4L2_CTRL_TYPE_BUTTON) {
        printf("\n");
        return 1;
    }
    printf("current ");

    ret = sample_uvc_get_control(dev, query_ext_ctrl, &ctrl, V4L2_CTRL_WHICH_CUR_VAL);
    if (ret < 0) {
        printf("n/a\n");
        printf("unable to get control 0x%8.8x\n", query_ext_ctrl->id);
    } else {
        sample_uvc_video_print_control_value(query_ext_ctrl, &ctrl);
        printf("\n");
    }
    if (query_ext_ctrl->flags & V4L2_CTRL_FLAG_HAS_PAYLOAD) {
        if (ctrl.ptr != TD_NULL) {
            free(ctrl.ptr);
            ctrl.ptr = TD_NULL;
        }
    }
    if (!full) {
        return 1;
    }
    if (query_ext_ctrl->type == V4L2_CTRL_TYPE_MENU || query_ext_ctrl->type == V4L2_CTRL_TYPE_INTEGER_MENU) {
        sample_uvc_video_query_menu(dev, query_ext_ctrl, ctrl.value);
    }
    return 1;
}
static td_s32 sample_uvc_video_get_ctrl(device_info *dev, const struct v4l2_query_ext_ctrl *query)
{
    return sample_uvc_video_get_control(dev, query, TD_TRUE);
}
static td_s32 sample_uvc_read_ctrl_file(struct v4l2_ext_control *ext_control, const td_char *val)
{
    ssize_t size;
    td_s32 fd;
    td_char regular_path[PATH_MAX] = {0};

    val++;
    if (strlen(val) >= PATH_MAX || realpath(val, regular_path) == NULL) {
        sample_print("unable to get regular path\n");
        return -EINVAL;
    }
    fd = open(regular_path, O_RDONLY);
    if (fd < 0) {
        sample_print("unable to open control file `%s'\n", val);
        return -EINVAL;
    }
    size = read(fd, ext_control->ptr, ext_control->size);
    if (size != (ssize_t)ext_control->size) {
        sample_print("error reading control file `%s'\n", val);
        close(fd);
        return -EINVAL;
    }
    close(fd);
    return TD_SUCCESS;
}
static td_s32 sample_uvc_video_parse_control_array(const struct v4l2_query_ext_ctrl *query_ext_ctrl,
    struct v4l2_ext_control *ext_control, const td_char *value)
{
    td_u32 i;
    td_char *end_ptr;

    for (; isspace(*value); ++value) { }

    if (*value == '<') {
        return sample_uvc_read_ctrl_file(ext_control, value);
    }
    if (*value++ != '{') {
        return -EINVAL;
    }
    for (i = 0; i < query_ext_ctrl->elems; ++i) {
        for (; isspace(*value); ++value) { }

        unsigned long val = strtoul(value, &end_ptr, 0);
        if (end_ptr == TD_NULL || val == ULONG_MAX) {
            return -EINVAL;
        }
        switch (query_ext_ctrl->type) {
            case V4L2_CTRL_TYPE_U8:
                ext_control->p_u8[i] = (td_u8)val;
                break;
            case V4L2_CTRL_TYPE_U16:
                ext_control->p_u16[i] = (td_u16)val;
                break;
            case V4L2_CTRL_TYPE_U32:
                ext_control->p_u32[i] = (td_u32)val;
                break;
            default:
                ext_control->p_u8[i] = (td_u8)val;
                break;
        }
        value = end_ptr;
        for (; isspace(*value); ++value) { }
        if (*value != ',') {
            break;
        }
        value++;
    }
    if (i < query_ext_ctrl->elems - 1) {
        return -EINVAL;
    }
    for (; isspace(*value); ++value) { }
    if (*value++ != '}') {
        return -EINVAL;
    }
    for (; isspace(*value); ++value) { }
    if (*value++ != '\0') {
        return -EINVAL;
    }
    return 0;
}
static td_s32 sample_uvc_get_non_nr_of_dims_ctrl(struct v4l2_query_ext_ctrl *query_ext_ctrl, const td_char *val,
    struct v4l2_ext_control *ext_control)
{
    td_char *end_ptr;
    errno_t ret;

    switch (query_ext_ctrl->type) {
        case V4L2_CTRL_TYPE_INTEGER:
        case V4L2_CTRL_TYPE_BOOLEAN:
        case V4L2_CTRL_TYPE_MENU:
        case V4L2_CTRL_TYPE_INTEGER_MENU:
        case V4L2_CTRL_TYPE_BITMASK:
            ext_control->value = (td_s32)strtol(val, &end_ptr, 0);
            if (*end_ptr != 0) {
                sample_print("control value '%s' error!\n", val);
                return TD_FAILURE;
            }
            break;
        case V4L2_CTRL_TYPE_INTEGER64:
            ext_control->value64 = strtoll(val, &end_ptr, 0);
            if (*end_ptr != 0) {
                sample_print("control value '%s' error!\n", val);
                return TD_FAILURE;
            }
            break;
        case V4L2_CTRL_TYPE_STRING:
            ext_control->size = query_ext_ctrl->elem_size;
            ext_control->ptr = malloc(ext_control->size);
            if (ext_control->ptr == TD_NULL) {
                return TD_FAILURE;
            }
            ret = strncpy_s(ext_control->string, ext_control->size, val, ext_control->size);
            if (ret != EOK) {
                sample_print("strncpy_s string fail %x\n", ret);
                return TD_FAILURE;
            }
            break;
        default:
            sample_print("control type not support!\n");
            return TD_FAILURE;
    }
    return TD_SUCCESS;
}
static td_void sample_uvc_video_set_control(device_info *dev, td_u32 id, const td_char *val)
{
    struct v4l2_query_ext_ctrl query_ext_ctrl;
    struct v4l2_ext_control ext_control = {0};
    td_s32 ret;

    ret = sample_uvc_query_control(dev, id, &query_ext_ctrl);
    if (ret < 0) {
        return;
    }
    if (query_ext_ctrl.nr_of_dims == 0) {
        ret = sample_uvc_get_non_nr_of_dims_ctrl(&query_ext_ctrl, val, &ext_control);
        if (ret != TD_SUCCESS) {
            return;
        }
    } else {
        switch (query_ext_ctrl.type) {
            case V4L2_CTRL_TYPE_U8:
            case V4L2_CTRL_TYPE_U16:
            case V4L2_CTRL_TYPE_U32:
                ext_control.size = query_ext_ctrl.elem_size * query_ext_ctrl.elems;
                ext_control.ptr = malloc(ext_control.size);
                if (ext_control.ptr == TD_NULL) {
                    return;
                }
                ret = sample_uvc_video_parse_control_array(&query_ext_ctrl, &ext_control, val);
                if (ret < 0) {
                    free(ext_control.ptr);
                    ext_control.ptr = TD_NULL;
                    sample_print("Invalid compound control value '%s'\n", val);
                    return;
                }
                break;
            default:
                sample_print("Unsupported control type %u\n", query_ext_ctrl.type);
                break;
        }
    }
    ret = sample_uvc_set_control(dev, &query_ext_ctrl, &ext_control);
    if (ret < 0) {
        sample_print("unable to set control 0x%8.8x\n", id);
    } else {
        sample_print("Control 0x%08x set to %s, is ", id, val);

        sample_uvc_video_print_control_value(&query_ext_ctrl, &ext_control);
        sample_print("\n");
    }
    if ((query_ext_ctrl.flags & V4L2_CTRL_FLAG_HAS_PAYLOAD) && ext_control.ptr) {
        free(ext_control.ptr);
        ext_control.ptr = TD_NULL;
    }
}
static td_void sample_uvc_video_list_controls(device_info *dev)
{
    td_s32 ret;

    ret = sample_uvc_video_for_each_control(dev, sample_uvc_video_get_ctrl);
    if (ret < 0) {
        return;
    }
    if (ret != 0) {
        sample_print("%d control%s found.\n", ret, ret > 1 ? "s" : "");
    } else {
        sample_print("No control found.\n");
    }
}
static td_s32 sample_uvc_video_reset_control(device_info *dev, const struct v4l2_query_ext_ctrl *query)
{
    struct v4l2_ext_control ctrl = { .value = query->default_value, };
    td_s32 ret;

    if (query->flags & V4L2_CTRL_FLAG_DISABLED) {
        return 0;
    }
    if (query->type == V4L2_CTRL_TYPE_CTRL_CLASS) {
        return 0;
    }
    if (query->flags & V4L2_CTRL_FLAG_HAS_PAYLOAD) {
        ret = sample_uvc_get_control(dev, query, &ctrl, V4L2_CTRL_WHICH_DEF_VAL);
        if (ret < 0) {
            return 0;
        }
    }
    sample_uvc_set_control(dev, query, &ctrl);

    if (query->flags & V4L2_CTRL_FLAG_HAS_PAYLOAD) {
        if (ctrl.ptr != TD_NULL) {
            free(ctrl.ptr);
            ctrl.ptr = TD_NULL;
        }
    }
    return 1;
}
static td_void sample_uvc_video_reset_controls(device_info *dev)
{
    td_s32 ret;

    ret = sample_uvc_video_for_each_control(dev, sample_uvc_video_reset_control);
    if (ret < 0) {
        return;
    }
    if (ret != 0) {
#endif /* HI3403V100_SDK_LINUX_SMP_A55_LINUX_MPP_SAMPLE_HOST_UVC_HOST_UVC_BODY_PART04_H_ */
