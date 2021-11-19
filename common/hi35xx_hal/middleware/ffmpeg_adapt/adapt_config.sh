# Copyright (c) Hisilicon Technologies Co., Ltd. 2021-2022. All rights reserved.
#!/bin/sh

function cancel_opt
{
    pattern=$1
    sed -i "s/$pattern 1/$pattern 0/g" config.h
    sed -i "s/$pattern/!$pattern/g" ffbuild/config.mak
}

function effect_opt
{
    pattern=$1
    echo $pattern
    sed -i "s/$pattern 0/$pattern 1/g" config.h
    sed -i "s/!$pattern/$pattern/g" ffbuild/config.mak
}

function adapt_liteos
{
echo "adapt ffmpeg for liteos"

sed -i "/^CFLAGS=/ s/$/ \$(LITEOS_MACRO) \$(LITEOS_OSDRV_INCLUDE) \$(LITEOS_USR_INCLUDE) \$(LITEOS_CMACRO) \$(LITEOS_INCLUDE) \-I\$(LITEOSTOPDIR)\/lib\/cxxstl\/gccinclude/g" ffbuild/config.mak

sed -i "s:atomics\/gcc:atomics\/pthread:g" ffbuild/config.mak

sed -i "/getenv/ s:^://:g" config.h

effect_opt HAVE_UNISTD_H
effect_opt HAVE_CBRT
effect_opt HAVE_CBRTF
effect_opt HAVE_COPYSIGN
effect_opt HAVE_ERF
effect_opt HAVE_HYPOT
effect_opt HAVE_RINT
effect_opt HAVE_LRINT
effect_opt HAVE_LRINTF
effect_opt HAVE_ROUND
effect_opt HAVE_ROUNDF
effect_opt HAVE_TRUNC
effect_opt HAVE_TRUNCF
effect_opt HAVE_GMTIME_R
effect_opt HAVE_LOCALTIME_R
effect_opt HAVE_PTHREAD_CANCEL
effect_opt HAVE_PTHREADS
effect_opt HAVE_SIMD_ALIGN_16

cancel_opt HAVE_SYSCONF
cancel_opt HAVE_SYSCTL
cancel_opt HAVE_ATOMICS_NATIVE
cancel_opt HAVE_LLRINTF
cancel_opt HAVE_MMAP
cancel_opt HAVE_LZO1X_999_COMPRESS
cancel_opt HAVE_GETHRTIME
cancel_opt HAVE_CABS
cancel_opt HAVE_CEXP
cancel_opt HAVE_VALGRIND_VALGRIND_H
cancel_opt HAVE_SYMVER_ASM_LABEL
#cancel_opt CONFIG_MANPAGES
#cancel_opt CONFIG_PODPAGES
#cancel_opt CONFIG_FAAN
#cancel_opt CONFIG_FAANDCT
#cancel_opt CONFIG_FAANIDCT
#cancel_opt CONFIG_FDCTDSP

make clean

echo "already adapt ffmpeg to liteos"
}
CURDIR=$(pwd)
adapt_liteos
