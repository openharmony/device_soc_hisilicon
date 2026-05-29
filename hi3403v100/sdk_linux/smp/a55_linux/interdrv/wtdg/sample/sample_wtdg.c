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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "ot_wtdg.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define DEV_FILE "/dev/watchdog"

void print_help(void)
{
    (void)printf("\n"
        "Usage: ./sample_wtdg [options] ...\n"
        "Options: \n"
        "    -s(set)  e.g. -s timeout 30\n"
        "    -g(get)  e.g. -g timeout\n"
        "    -f(feed) e.g. -f\n");
}

static int set_func_test(int fd, int argc, char *argv[])
{
    int ret;
    int timeout;

    if (strcmp("timeout", argv[2]) == 0) { /* 2: The second argument */
        timeout = atoi(argv[3]); /* 3: The third argument */
        ret = ioctl(fd, WDIOC_SETTIMEOUT, &timeout);
        if (ret != 0) {
            fprintf(stderr, "[Error]%s(%d): ioctl ret =%d\n", __FUNCTION__, __LINE__, ret);
            return -1;
        }
    } else if (strcmp("option", argv[2]) == 0) { /* 2: The second argument */
        timeout = atoi(argv[3]); /* 3: The third argument */
        printf("set new option :%d \n", timeout);
        ret = ioctl(fd, WDIOC_SETOPTIONS, &timeout);
        if (ret != 0) {
            fprintf(stderr, "[Error]%s(%d): ioctl ret =%d\n", __FUNCTION__, __LINE__, ret);
            return -1;
        }
    } else {
        print_help();
    }
    return 0;
}

static int get_func_test(int fd, int argc, char *argv[])
{
    int ret;
    int timeout = 0;

    if (strcmp("timeout", argv[2]) == 0) { /* 2: The second argument */
        ret = ioctl(fd, WDIOC_GETTIMEOUT, &timeout);
        if (ret != 0) {
            fprintf(stderr, "[Error]%s(%d): ioctl ret =%d\n", __FUNCTION__, __LINE__, ret);
            return -1;
        } else {
            fprintf(stdout, "Resp: timeout=%d\n", timeout);
        }
    } else if (strcmp("option", argv[2]) == 0) { /* 2: The second argument */
        printf("Line:%d \n", __LINE__);
        ret = ioctl(fd, WDIOC_GETSTATUS, &timeout);
        if (ret != 0) {
            fprintf(stderr, "[Error]%s(%d): ioctl ret =%d\n", __FUNCTION__, __LINE__, ret);
            return -1;
        } else {
            fprintf(stdout, "Resp: option=%d\n", timeout);
        }
    } else {
        print_help();
    }
    return 0;
}

static int parse_args_and_proc(int fd, int argc, char *argv[])
{
    int ret = 0;

    if (argc == 1) {
        print_help();
        return -1;
    }
    int i = argc - 1;
    while (i > 0) {
        printf("%d: %s  ", i, argv[i]);
        i--;
    }
    printf("\n");
    if ((strcmp("-s", argv[1]) == 0) && (argc == 4)) { /* 4 args like './test -s timeout 30' */
        ret = set_func_test(fd, argc, argv);
        if (ret != 0) {
            return -1;
        }
    } else if ((strcmp("-g", argv[1]) == 0) && (argc == 3)) { /* 3 args like './test -g timeout' */
        ret = get_func_test(fd, argc, argv);
        if (ret != 0) {
            return -1;
        }
    } else if ((strcmp("-f", argv[1]) == 0) && (argc == 2)) { /* 2 args like './test -f' */
        ret = ioctl(fd, WDIOC_KEEPALIVE);
        if (ret != 0) {
            fprintf(stderr, "[Error]%s(%d): ioctl ret =%d\n", __FUNCTION__, __LINE__, ret);
            return -1;
        }
    } else {
        print_help();
    }
    return ret;
}

#ifdef __LITEOS__
int app_main(int argc, char *argv[])
#else
int main(int argc, char *argv[])
#endif
{
    int fd  = open(DEV_FILE, O_RDWR);
    if (fd < 0) {
        fprintf(stderr, "fail to open file:%s\n", DEV_FILE);
        return -1;
    }

    parse_args_and_proc(fd, argc, argv);

    close(fd);
    return 0;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
