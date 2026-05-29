#!/bin/sh
# Copyright (c) 2025 HiSilicon (Shanghai) Technologies Co., Ltd.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, see
# <https://www.gnu.org/licenses/>.

KERN_DIR=$1
VENDOR_DIR=$KERN_DIR/drivers/vendor
MPP_DIR=$VENDOR_DIR/mpp
CIPHER_DIR=$MPP_DIR/cipher
KLAD_DIR=$MPP_DIR/klad
OSAL_DIR=$VENDOR_DIR/osal
SYSCONFIG_DIR=$VENDOR_DIR/sysconfig

pushd $KERN_DIR;
    # vendor/Makefile
    echo "obj-y += osal/" >> drivers/vendor/Makefile
    echo "obj-y += sysconfig/" >> drivers/vendor/Makefile
    echo "obj-y += mpp/" >> drivers/vendor/Makefile

    # cipher
    sed -i '/__TIME__/s/CRYPTO/\/\/ CRYPTO/' $CIPHER_DIR/v3/src/drv/osal/drv_osal_init.c

    # mpp
    echo "EXTRA_CFLAGS += -Idrivers/vendor/mpp/include" > $MPP_DIR/Makefile
    echo "EXTRA_CFLAGS += -Idrivers/vendor/mpp/ext_inc" >> $MPP_DIR/Makefile
    echo "EXTRA_CFLAGS += -Idrivers/vendor/mpp/init/include" >> $MPP_DIR/Makefile
    echo "obj-y += mod_base.o init/src/base_init.o" >> $MPP_DIR/Makefile
    echo "obj-y += mod_sys.o init/src/sys_init.o" >> $MPP_DIR/Makefile
    echo "obj-y += cipher/" >> $MPP_DIR/Makefile
    echo "obj-y += klad/src/" >> $MPP_DIR/Makefile

    # osal
    echo "EXTRA_CFLAGS += -Idrivers/vendor/osal/ot_media" > $OSAL_DIR/Makefile
    echo "EXTRA_CFLAGS += -Idrivers/vendor/osal/include" >> $OSAL_DIR/Makefile
    echo "EXTRA_CFLAGS += -Idrivers/vendor/mpp/include" >> $OSAL_DIR/Makefile
    echo "obj-y := osal_fileops.o osal_vmalloc.o osal_addr.o osal_init.o osal_atomic.o osal_barrier.o osal_cache.o osal_debug.o osal_device.o osal_interrupt.o osal_math.o osal_mutex.o osal_proc.o osal_schedule.o osal_semaphore.o osal_spinlock.o osal_string.o osal_task.o osal_timer.o osal_wait.o osal_workqueue.o osal_notifier.o osal_platform.o ./ot_media/base.o ./ot_media/ot_media.o ./mmz/media_mem.o ./mmz/mmz_userdev.o ./mmz/ot_allocator.o" >> $OSAL_DIR/Makefile
    echo "obj-\$(CONFIG_CMA) += ./mmz/cma_allocator.o" >> $OSAL_DIR/Makefile

    sed -i '/int anony = 0;/{s/0/1/}' $OSAL_DIR/mmz/media_mem.c
    sed -i '/__initdata g_setup_zones/{s/__initdata //;s/{.*}/"anonymous,0,0x88000000,384M"/;}' $OSAL_DIR/mmz/media_mem.c
    sed -i '/__initdata g_setup_allocator/{s/__initdata //;s/;/ = "ot";/;}' $OSAL_DIR/mmz/media_mem.c
    sed -i '/__initdata g_mmap_zones/{s/__initdata //;s/ = {.*}//;}' $OSAL_DIR/mmz/media_mem.c
    sed -i '/"map_mmz="/{s/__setup/\/\/ __setup/;}' $OSAL_DIR/mmz/media_mem.c
    sed -i '/#ifdef MODULE/{d;}' $OSAL_DIR/mmz/media_mem.c
    sed -i -e '$!N;/\n.*subsys_initcall/!P;D' $OSAL_DIR/mmz/media_mem.c
    sed -i '/subsys_initcall/{n;d}' $OSAL_DIR/mmz/media_mem.c
    sed -i '/subsys_initcall/{d;}' $OSAL_DIR/mmz/media_mem.c

    sed -i '/static void mmz_flush/i\extern int svm_flush_cache(struct mm_struct *mm, unsigned long addr, size_t size);' $OSAL_DIR/mmz/mmz_userdev.c

    sed -i '/int valid_mmap_phys_addr_range/{s/int/static int/;}' $OSAL_DIR/osal_device.c
    sed -i '/owner == NULL/{s/ || (owner == NULL)//;}' $OSAL_DIR/ot_media/base.c
    sed -i '/owner == NULL/{s/ || (ot_media->owner == NULL)//;}' $OSAL_DIR/ot_media/ot_media.c

    # sysconfig
    echo "obj-y += sysconfig.o" > $SYSCONFIG_DIR/Makefile
    echo "sysconfig-y += sys_cfg.o clk_cfg.o sys_ctrl.o pin_mux.o" >> $SYSCONFIG_DIR/Makefile
    echo "EXTRA_CFLAGS += -Idrivers/vendor/sysconfig" >> $SYSCONFIG_DIR/Makefile
    echo "EXTRA_CFLAGS += -Idrivers/vendor/mpp/include" >> $SYSCONFIG_DIR/Makefile
popd
