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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <fcntl.h>
#include <signal.h>
#include <limits.h>

#include <linux/fb.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

#include "securec.h"

#include "ot_common_tde.h"
#include "ss_mpi_tde.h"
#include "gfbg.h"
#include "ot_type.h"
#include "ot_common_vo.h"
#include "ss_mpi_sys.h"
#include "ss_mpi_vo.h"
#include "sample_comm.h"
#include "sdk_module_init.h"


#define TDE_PRINT printf
#define VO_DEV 0
#define VO_CHN 0
#define min(x, y) ((x) > (y) ? (y) : (x))
#define max(x, y) ((x) > (y) ? (x) : (y))

#ifdef __LITEOS__
static const td_char *g_psz_image_names[] = {
    "/nfs/res/apple.bits",
    "/nfs/res/applets.bits",
    "/nfs/res/calendar.bits",
    "/nfs/res/foot.bits",
    "/nfs/res/gmush.bits",
    "/nfs/res/gimp.bits",
    "/nfs/res/gsame.bits",
    "/nfs/res/keys.bits"
};
#define BACKGROUND_NAME  "/nfs/res/background.bits"
#else
static const td_char *g_psz_image_names[] = {
    "res/apple.bits",
    "res/applets.bits",
    "res/calendar.bits",
    "res/foot.bits",
    "res/gmush.bits",
    "res/gimp.bits",
    "res/gsame.bits",
    "res/keys.bits"
};
#define BACKGROUND_NAME  "res/background.bits"
#endif

#define N_IMAGES (td_s32)((sizeof (g_psz_image_names) / sizeof (g_psz_image_names[0])))
#define PIXFMT  OT_TDE_COLOR_FORMAT_ARGB1555
#define BPP     2
#define SCREEN_WIDTH    720
#define SCREEN_HEIGHT   576
#define CYCLE_LEN       60

static td_s32   g_frame_num;
static ot_tde_surface g_screen[2]; /* 2 screen0 and screen1 */
static ot_tde_surface g_back_ground;
static ot_tde_surface g_img_sur[N_IMAGES];
td_s32 g_fd = -1;
td_u32 g_size;
td_u8 *g_sample_screen = TD_NULL;
td_u8 *g_back_ground_vir = TD_NULL;
static int g_sample_tde_exit = 0;

int g_int_type = 0;

static td_void sample_tde_usage(td_char *s_prg_nm)
{
    printf("usage : %s <intf>\n", s_prg_nm);
    printf("intf:\n");
    printf("\t 0) vo HDMI output, default.\n");
    printf("\t 1) vo BT1120 output.\n");

    return;
}

static td_void sample_tde_handle_sig(td_s32 signo)
{
    static int sig_handled = 0;
    if (sig_handled == 0 && (signo == SIGINT || signo == SIGTERM)) {
        sig_handled = 1;
        g_sample_tde_exit = 1;
    }
}

static td_void sample_tde_to_exit_signal(td_void)
{
    if (g_sample_screen != TD_NULL) {
        (td_void)munmap(g_sample_screen, g_size);
        g_sample_screen = TD_NULL;
    }

    if (g_back_ground_vir != TD_NULL) {
        ss_mpi_sys_mmz_free(g_back_ground.phys_addr, g_back_ground_vir);
        g_back_ground_vir = TD_NULL;
    }

    if (g_fd != -1) {
        close(g_fd);
        g_fd = -1;
    }

    ss_tde_close();
    ss_mpi_vo_disable(VO_DEV);
    sample_comm_sys_exit();
    printf("\033[0;31mprogram termination abnormally!\033[0;39m\n");
    exit(-1);
}

static int sample_tde_getchar(td_void)
{
    int c;
    if (g_sample_tde_exit == 1) {
        sample_tde_to_exit_signal();
    }

    c = getchar();

    if (g_sample_tde_exit == 1) {
        sample_tde_to_exit_signal();
    }

    return c;
}

static td_void tde_create_surface(ot_tde_surface *surface, td_u32 colorfmt, td_u32 w, td_u32 h, td_u32 stride)
{
    surface->color_format = colorfmt;
    surface->width = w;
    surface->height = h;
    surface->stride = stride;
    surface->alpha0 = 0xff;
    surface->alpha1 = 0xff;
    surface->alpha_max_is_255 = TD_TRUE;
    surface->support_alpha_ex_1555 = TD_TRUE;
}

static FILE *tde_open_surface_file(const td_char *psz_file_name)
{
    FILE *fp = TD_NULL;
    td_char path[PATH_MAX] = {0x00};

    if (strlen(psz_file_name) >= PATH_MAX || realpath(psz_file_name, path) == TD_NULL) {
        return TD_NULL;
    }
    fp = fopen(path, "rb");
    if (fp == TD_NULL) {
        TDE_PRINT("error when open psz_file_name %s, line:%d\n", psz_file_name, __LINE__);
    }

    return fp;
}

static td_s32 tde_read_surface_header(FILE *fp, const td_char *psz_file_name, ot_tde_surface *surface)
{
    td_u32 colorfmt;
    td_u32 w;
    td_u32 h;
    td_u32 stride;

    if (fread(&colorfmt, 1, 4, fp) != 4) { /* 4 bytes */
        TDE_PRINT("error when read psz_file_name %s, line:%d\n", psz_file_name, __LINE__);
        return -1;
    }
    if (fread(&w, 1, 4, fp) != 4) { /* 4 bytes */
        TDE_PRINT("error when read psz_file_name %s, line:%d\n", psz_file_name, __LINE__);
        return -1;
    }
    if (fread(&h, 1, 4, fp) != 4) { /* 4 bytes */
        TDE_PRINT("error when read psz_file_name %s, line:%d\n", psz_file_name, __LINE__);
        return -1;
    }
    if (fread(&stride, 1, 4, fp) != 4) { /* 4 bytes */
        TDE_PRINT("error when read psz_file_name %s, line:%d\n", psz_file_name, __LINE__);
        return -1;
    }

    tde_create_surface(surface, colorfmt, w, h, stride);
    return 0;
}

static td_s32 tde_read_surface_data(FILE *fp, ot_tde_surface *surface, td_u8 *virt)
{
    td_u64 packagelen = (td_u64)surface->stride * (td_u64)surface->height;

    if (packagelen > 0x7FFFFFFF) {
        TDE_PRINT("stride * h not valid: %d %d, line:%d\n", surface->stride, surface->height, __LINE__);
        return -1;
    }

    if (fread(virt, 1, surface->stride * surface->height, fp) != surface->stride * surface->height) {
        return -1;
    }
    return 0;
}

static td_s32 tde_create_surface_by_file(const td_char *psz_file_name, ot_tde_surface *surface, td_u8 *virt)
{
    FILE *fp = TD_NULL;
    td_s32 ret;

    if ((psz_file_name == TD_NULL) || (surface == TD_NULL)) {
        TDE_PRINT("%s, LINE %d, NULL ptr!\n", __FUNCTION__, __LINE__);
        return -1;
    }

    fp = tde_open_surface_file(psz_file_name);
    if (fp == TD_NULL) {
        return -1;
    }

    ret = tde_read_surface_header(fp, psz_file_name, surface);
    if (ret != 0) {
        (td_void)fclose(fp);
        return -1;
    }

    ret = tde_read_surface_data(fp, surface, virt);
    if (ret != 0) {
        (td_void)fclose(fp);
        return -1;
    }

    return (fclose(fp) == 0) ? 0 : -1;
}

static td_void sample_tde_bit_blit(td_s32 handle, ot_tde_rect *src_rect, ot_tde_opt *opt, td_u32 next_on_show)
{
    td_u32 i;
    td_float ang;
    td_float r;
    td_float f;
    td_float e_x_mid;
    td_float e_y_mid;
    td_float e_radius;
    ot_tde_rect dst_rect;
    ot_tde_double_src double_src = {0};
    td_s32 ret;

    f = (float)(g_frame_num % CYCLE_LEN) / CYCLE_LEN;
    e_x_mid = g_back_ground.width / 2.16f; /* 2.16f alg data */
    e_y_mid = g_back_ground.height / 2.304f; /* 2.304f alg data */
    e_radius = min(e_x_mid, e_y_mid) / 2.0f; /* 2.0f alg data */

    for (i = 0; i < N_IMAGES; i++) {
        src_rect->pos_x = 0;
        src_rect->pos_y = 0;
        src_rect->width = g_img_sur[i].width;
        src_rect->height = g_img_sur[i].height;

        /* 3. calculate new pisition */
        ang = 2.0f * (td_float)M_PI * (td_float)i / N_IMAGES - f * 2.0f * (td_float)M_PI; /* 2.0f alg data */
        r = e_radius + (e_radius / 3.0f) * sinf(f * 2.0 * M_PI); /* 3.0f 2.0 alg data */

        dst_rect.pos_x = e_x_mid + r * cosf(ang) - g_img_sur[i].width / 2.0f; /* 2.0f alg data */
        dst_rect.pos_y = e_y_mid + r * sinf(ang) - g_img_sur[i].height / 2.0f; /* 2.0f alg data */
        dst_rect.width = g_img_sur[i].width;
        dst_rect.height = g_img_sur[i].height;

        /* 4. bitblt image to screen */
        double_src.bg_surface = &g_screen[next_on_show];
        double_src.fg_surface = &g_img_sur[i];
        double_src.dst_surface = &g_screen[next_on_show];
        double_src.bg_rect = &dst_rect;
        double_src.fg_rect = src_rect;
        double_src.dst_rect = &dst_rect;
        ret = ss_tde_bit_blit(handle, &double_src, opt);
        if (ret < 0) {
            TDE_PRINT("line:%d,tde2_bitblit failed,ret=0x%x!\n", __LINE__, ret);
            ss_tde_cancel_job(handle);
            return;
        }
    }
    return;
}

static td_void circumrotate(td_u32 cur_on_show)
{
    td_s32 handle;
    ot_tde_opt opt = {0};
    td_u32 next_on_show;
    ot_tde_rect src_rect;
    td_s32 ret;
    ot_tde_single_src single_src = {0};

    next_on_show = !cur_on_show;

    opt.out_alpha_from = OT_TDE_OUT_ALPHA_FROM_BG;
    opt.colorkey_value.argb_colorkey.red.component_mask = 0xff;
    opt.colorkey_value.argb_colorkey.green.component_mask = 0xff;
    opt.colorkey_value.argb_colorkey.blue.component_mask = 0xff;
    opt.colorkey_mode = OT_TDE_COLORKEY_MODE_FG;
    opt.colorkey_value.argb_colorkey.alpha.is_component_ignore = TD_TRUE;

    src_rect.pos_x = 0;
    src_rect.pos_y = 0;
    src_rect.width = g_back_ground.width;
    src_rect.height = g_back_ground.height;

    /* 1. start job */
    handle = ss_tde_begin_job();
    if (handle == OT_ERR_TDE_INVALID_HANDLE) {
        TDE_PRINT("start job failed!\n");
        return;
    }

    /* 2. bitblt background to screen */
    single_src.src_surface = &g_back_ground;
    single_src.dst_surface = &g_screen[next_on_show];
    single_src.src_rect = &src_rect;
    single_src.dst_rect = &src_rect;
    ret = ss_tde_quick_copy(handle, &single_src);
    if (ret < 0) {
        TDE_PRINT("line:%d failed,ret=0x%x!\n", __LINE__, ret);
        ss_tde_cancel_job(handle);
        return;
    }

    sample_tde_bit_blit(handle, &src_rect, &opt, next_on_show);

    /* 5. submit job */
    ret = ss_tde_end_job(handle, TD_FALSE, TD_TRUE, 1000); /* 1000 time out */
    if (ret < 0) {
        TDE_PRINT("line:%d,tde2_end_job failed,ret=0x%x!\n", __LINE__, ret);
        ss_tde_cancel_job(handle);
        return;
    }
    g_frame_num++;
    return;
}

static td_s32 sample_init_images_surface()
{
    td_u32 i;

    /* allocate memory (720*576*2*N_IMAGES bytes) to save images' infornation */
    if (ss_mpi_sys_mmz_alloc(&(g_back_ground.phys_addr), ((void**)&g_back_ground_vir),
        TD_NULL, TD_NULL, 720 * 576 * 2 * N_IMAGES) == TD_FAILURE) { /* 720 576 2 720*576*2 */
        TDE_PRINT("allocate memory (720*576*2*N_IMAGES bytes) failed\n");
        return TD_FAILURE;
    }
    if (tde_create_surface_by_file(BACKGROUND_NAME, &g_back_ground, g_back_ground_vir) != TD_SUCCESS) {
        ss_mpi_sys_mmz_free(g_back_ground.phys_addr, g_back_ground_vir);
        g_back_ground_vir = TD_NULL;
        return TD_FAILURE;
    }

    g_img_sur[0].phys_addr = g_back_ground.phys_addr + (td_phys_addr_t)g_back_ground.stride *
        (td_phys_addr_t)g_back_ground.height;
    for (i = 0; i < N_IMAGES - 1; i++) {
        tde_create_surface_by_file(g_psz_image_names[i], &g_img_sur[i],
            g_back_ground_vir + (g_img_sur[i].phys_addr - g_back_ground.phys_addr));
        g_img_sur[i + 1].phys_addr = g_img_sur[i].phys_addr + (td_phys_addr_t)g_img_sur[i].stride *
            (td_phys_addr_t)g_img_sur[i].height;
    }
    if (tde_create_surface_by_file(g_psz_image_names[i], &g_img_sur[i],
        g_back_ground_vir + (g_img_sur[i].phys_addr - g_back_ground.phys_addr)) != TD_SUCCESS) {
        ss_mpi_sys_mmz_free(g_back_ground.phys_addr, g_back_ground_vir);
        g_back_ground_vir = TD_NULL;
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static td_s32 sample_init_surface(td_void)
{
    struct fb_fix_screeninfo fix;
    td_phys_addr_t phys_addr;

    if (ioctl(g_fd, FBIOGET_FSCREENINFO, &fix) < 0) {
        TDE_PRINT("process frame buffer device error\n");
        return TD_FAILURE;
    }

    g_size = fix.smem_len;
    phys_addr = fix.smem_start;
    g_sample_screen = mmap(TD_NULL, g_size, PROT_READ | PROT_WRITE, MAP_SHARED, g_fd, 0);
    if (g_sample_screen == TD_NULL) {
        TDE_PRINT("mmap fb0 failed!\n");
        return TD_FAILURE;
    }

    if (memset_s(g_sample_screen, g_size, 0x00, fix.smem_len) != EOK) {
        TDE_PRINT("%s:%d:memset_s failed\n", __FUNCTION__, __LINE__);
        (td_void)munmap(g_sample_screen, g_size);
        g_sample_screen = TD_NULL;
        return TD_FAILURE;
    }

    g_screen[0].color_format = PIXFMT;
    g_screen[0].phys_addr = phys_addr;
    g_screen[0].width = SCREEN_WIDTH;
    g_screen[0].height = SCREEN_HEIGHT;
    g_screen[0].stride = fix.line_length;
    g_screen[0].alpha_max_is_255 = TD_TRUE;
    g_screen[1] = g_screen[0];
    g_screen[1].phys_addr = g_screen[0].phys_addr + (td_phys_addr_t)g_screen[0].stride *
        (td_phys_addr_t)g_screen[0].height;

    if (sample_init_images_surface() != TD_SUCCESS) {
        (td_void)munmap(g_sample_screen, g_size);
        g_sample_screen = TD_NULL;
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static td_s32 sample_init_var(td_void)
{
    struct fb_var_screeninfo var;
    struct fb_bitfield r32 = {10, 5, 0};
    struct fb_bitfield g32 = {5, 5, 0};
    struct fb_bitfield b32 = {0, 5, 0};
    struct fb_bitfield a32 = {15, 1, 0};

    if (ioctl(g_fd, FBIOGET_VSCREENINFO, &var) < 0) {
        TDE_PRINT("get variable screen info failed!\n");
        return TD_FAILURE;
    }

    var.xres_virtual = SCREEN_WIDTH;
    var.yres_virtual = SCREEN_HEIGHT * 2; /* 2 2buf */
    var.xres = SCREEN_WIDTH;
    var.yres = SCREEN_HEIGHT;
    var.activate = FB_ACTIVATE_NOW;
    var.bits_per_pixel = 16; /* 16 2bytes */
    var.xoffset = 0;
    var.yoffset = 0;
    var.red = r32;
    var.green = g32;
    var.blue = b32;
    var.transp = a32;

    if (ioctl(g_fd, FBIOPUT_VSCREENINFO, &var) < 0) {
        TDE_PRINT("process frame buffer device error\n");
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static td_s32 sample_init_gfbg(td_void)
{
    td_bool is_compress;
    td_bool is_show;
    ot_fb_alpha alpha = {0};

    g_fd = open("/dev/fb0", O_RDWR);
    if (g_fd < 0) {
        TDE_PRINT("open frame buffer device error\n");
        return TD_FAILURE;
    }

    is_compress = TD_FALSE;
    if (ioctl(g_fd, FBIOPUT_COMPRESSION_GFBG, &is_compress) < 0) {
        TDE_PRINT(" FBIOPUT_COMPRESSION_GFBG failed!\n");
        close(g_fd);
        g_fd = -1;
        return TD_FAILURE;
    }

    alpha.alpha_chn_en = TD_FALSE;
    alpha.alpha_en = TD_FALSE;
    if (ioctl(g_fd, FBIOPUT_ALPHA_GFBG, &alpha) < 0) {
        TDE_PRINT("put alpha info failed!\n");
        close(g_fd);
        g_fd = -1;
        return TD_FAILURE;
    }

    /* init var */
    if (sample_init_var() != TD_SUCCESS) {
        close(g_fd);
        g_fd = -1;
        return TD_FAILURE;
    }

    /* init surface */
    if (sample_init_surface() != TD_SUCCESS) {
        close(g_fd);
        g_fd = -1;
        return TD_FAILURE;
    }

    is_show = TD_TRUE;
    if (ioctl(g_fd, FBIOPUT_SHOW_GFBG, &is_show) < 0) {
        fprintf(stderr, "couldn't show fb\n");
        close(g_fd);
        g_fd = -1;
        return TD_FAILURE;
    }
    return TD_SUCCESS;
}

static td_s32 sample_rotate_and_show()
{
    td_u32 times;
    struct fb_var_screeninfo var;

    if (ioctl(g_fd, FBIOGET_VSCREENINFO, &var) < 0) {
        TDE_PRINT("get variable screen info failed!\n");
        goto err;
    }

    g_frame_num = 0;
    for (times = 0; times < 20; times++) { /* 20 times */
        circumrotate(times % 2);
        var.yoffset = (times % 2) ? 0 : 576; /* 2 576 height */

        /* set frame buffer start position */
        if (ioctl(g_fd, FBIOPAN_DISPLAY, &var) < 0) {
            TDE_PRINT("process frame buffer device error\n");
            goto err;
        }
        if (g_sample_tde_exit == 1) {
            sample_tde_to_exit_signal();
        }
        sleep(1);
    }
    return TD_SUCCESS;
err:
    ss_mpi_sys_mmz_free(g_back_ground.phys_addr, g_back_ground_vir);
    g_back_ground_vir = TD_NULL;
    (td_void)munmap(g_sample_screen, g_size);
    g_sample_screen = TD_NULL;
    close(g_fd);
    g_fd = -1;
    return TD_FAILURE;
}

static td_s32 sample_tde_draw_graphic()
{
    td_s32 ret;
    /* 1. open tde device */
    ret = ss_tde_open();
    if (ret != TD_SUCCESS) {
        TDE_PRINT("tde_open failed:0x%x\n", ret);
        return ret;
    }
    /* 2. framebuffer operation */
    ret = sample_init_gfbg();
    if (ret != TD_SUCCESS) {
        TDE_PRINT("sample_init_gfbg failed:0x%x\n", ret);
        ss_tde_close();
        return TD_FAILURE;
    }
    /* 3. use tde and framebuffer to realize rotational effect */
    ret = sample_rotate_and_show();
    if (ret != TD_SUCCESS) {
        TDE_PRINT("sample_rotate_and_show failed:0x%x\n", ret);
        ss_tde_close();
        return TD_FAILURE;
    }

    return TD_SUCCESS;
}

static td_s32 sample_start_vo_by_intf_type(ot_vo_pub_attr *pub_attr)
{
    /* if it's displayed on HDMI, we should start HDMI */
    if (pub_attr->intf_type & OT_VO_INTF_HDMI) {
        g_int_type = OT_VO_INTF_HDMI;
        if (sample_comm_vo_hdmi_start(pub_attr->intf_sync) != TD_SUCCESS) {
            return TD_FAILURE;
        }
    }

     /* if it's displayed on bt1120, we should start bt1120 */
    if (pub_attr->intf_type & OT_VO_INTF_BT1120) {
        g_int_type = OT_VO_INTF_BT1120;
        if (sample_comm_vo_bt1120_start(VO_DEV, pub_attr) != TD_SUCCESS) {
            return TD_FAILURE;
        }
    }
    return TD_SUCCESS;
}

static td_void sample_tde_to_exit(td_void)
{
    td_char ch = 0;

    while (ch != 'q') {
        printf("\npress 'q' to exit this sample.\n");
        ch = (td_char)sample_tde_getchar();
        while (ch == '\n') {
            ch = (td_char)sample_tde_getchar();
        }
        if (ch == 'q') {
            continue;
        } else {
            printf("input invalid! please try again.\n");
        }
    }
    ss_mpi_sys_mmz_free(g_back_ground.phys_addr, g_back_ground_vir);
    g_back_ground_vir = TD_NULL;
    (td_void)munmap(g_sample_screen, g_size);
    g_sample_screen = TD_NULL;
    close(g_fd);
    g_fd = -1;
    ss_tde_close();
    return;
}

int main(int argc, char *argv[])
{
    ot_vo_pub_attr pub_attr = {0};
    ot_vb_cfg vb_conf = {0};
    sample_vo_cfg vo_config = {0};

#ifdef DRM_OPTION_ENABLE
    TDE_PRINT("sample DONOT support DRM!\n");
    return 0;
#endif

    if ((argc != 2) || (strlen(argv[1]) != 1)) { /* 2 varm nums */
        sample_tde_usage(argv[0]);
        return TD_FAILURE;
    }
    if (*argv[1] != '0' && *argv[1] != '1') {
        sample_tde_usage(argv[0]);
        return TD_FAILURE;
    }

    sample_sys_signal(&sample_tde_handle_sig);

    pub_attr.bg_color = 0x000000ff;
    pub_attr.intf_type = OT_VO_INTF_BT1120;
    pub_attr.intf_sync = OT_VO_OUT_1080P60;

    if ((argc > 1) && *argv[1] == '0') { /* '0': VO_INTF_VGA, else: HDMI */
        pub_attr.intf_type = OT_VO_INTF_HDMI;
    }

    vb_conf.max_pool_cnt = 16; /* 16 pool cnt */

#ifdef CONFIG_USER_SPACE
    SDK_init();
#endif

    /* 1 enable vo device HD first */
    if (sample_comm_sys_init(&vb_conf) != TD_SUCCESS) {
        return TD_FAILURE;
    }
    if (sample_comm_vo_start_dev(VO_DEV, &pub_attr, &vo_config.user_sync, vo_config.dev_frame_rate) !=
        TD_SUCCESS) {
        sample_comm_sys_exit();
        return TD_FAILURE;
    }

    if (sample_start_vo_by_intf_type(&pub_attr) != TD_SUCCESS) {
        goto err;
    }

    /* 2 run tde sample which draw grahpic on ot_fb memory */
    if (sample_tde_draw_graphic() != TD_SUCCESS) {
        goto err;
    }
    sample_tde_to_exit();
err:
    if (pub_attr.intf_type & OT_VO_INTF_HDMI) {
        sample_comm_vo_hdmi_stop();
    }
    ss_mpi_vo_disable(VO_DEV);
    sample_comm_sys_exit();

#ifdef CONFIG_USER_SPACE
    SDK_exit();
#endif
    return TD_SUCCESS;
}
