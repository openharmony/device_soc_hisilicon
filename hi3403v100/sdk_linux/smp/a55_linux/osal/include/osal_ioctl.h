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

#ifndef _OSAL_IOCTL_H
#define _OSAL_IOCTL_H

/* ioctl command encoding: 32 bits total, command in lower 16 bits,
 * size of the parameter structure in the lower 14 bits of the
 * upper 16 bits.
 * Encoding the size of the parameter structure in the ioctl request
 * is useful for catching programs compiled with old versions
 * and to avoid overwriting user space outside the user buffer area.
 * The highest 2 bits are reserved for indicating the ``access mode''.
 * NOTE: This limits the max parameter size to 16kB -1 !
 */
/*
 * The following is for compatibility across the various Linux
 * platforms.  The generic ioctl numbering scheme doesn't really enforce
 * a type field.  De facto, however, the top 8 bits of the lower 16
 * bits are indeed used as a type field, so we might just as well make
 * this explicit here.  Please be sure to use the decoding macros
 * below from now on.
 */
#ifndef OSAL_IOC_NRBITS
#define OSAL_IOC_NRBITS    8
#endif

#ifndef OSAL_IOC_TYPEBITS
#define OSAL_IOC_TYPEBITS  8
#endif

/*
 * Let any architecture override either of the following before
 * including this file.
 */
#ifndef OSAL_IOC_SIZEBITS
#define OSAL_IOC_SIZEBITS  14
#endif

#ifndef OSAL_IOC_DIRBITS
#define OSAL_IOC_DIRBITS   2
#endif

#ifndef OSAL_IOC_NRMASK
#define OSAL_IOC_NRMASK    ((1 << OSAL_IOC_NRBITS) - 1)
#endif

#ifndef OSAL_IOC_TYPEMASK
#define OSAL_IOC_TYPEMASK  ((1 << OSAL_IOC_TYPEBITS) - 1)
#endif

#ifndef OSAL_IOC_SIZEMASK
#define OSAL_IOC_SIZEMASK  ((1 << OSAL_IOC_SIZEBITS) - 1)
#endif

#ifndef OSAL_IOC_DIRMASK
#define OSAL_IOC_DIRMASK   ((1 << OSAL_IOC_DIRBITS) - 1)
#endif

#ifndef OSAL_IOC_NRSHIFT
#define OSAL_IOC_NRSHIFT    0
#endif

#ifndef OSAL_IOC_TYPESHIFT
#define OSAL_IOC_TYPESHIFT   (OSAL_IOC_NRSHIFT + OSAL_IOC_NRBITS)
#endif

#ifndef OSAL_IOC_SIZESHIFT
#define OSAL_IOC_SIZESHIFT   (OSAL_IOC_TYPESHIFT + OSAL_IOC_TYPEBITS)
#endif

#ifndef OSAL_IOC_DIRSHIFT
#define OSAL_IOC_DIRSHIFT    (OSAL_IOC_SIZESHIFT + OSAL_IOC_SIZEBITS)
#endif

/*
 * Direction bits, which any architecture can choose to override
 * before including this file.
 */
#ifndef OSAL_IOC_NONE
#define OSAL_IOC_NONE      0U
#endif

#ifndef OSAL_IOC_WRITE
#define OSAL_IOC_WRITE     1U
#endif

#ifndef OSAL_IOC_READ
#define OSAL_IOC_READ      2U
#endif

#ifndef OSAL_IOC
#define OSAL_IOC(dir, type, nr, size) \
    (((dir) << OSAL_IOC_DIRSHIFT) |   \
    ((type) << OSAL_IOC_TYPESHIFT) |  \
    ((nr) << OSAL_IOC_NRSHIFT) |      \
    ((size) << OSAL_IOC_SIZESHIFT))
#endif

#ifndef OSAL_IOC_TYPECHECK
#ifdef __CHECKER__
#define OSAL_IOC_TYPECHECK(t) (sizeof(t))
#else
/* provoke compile error for invalid uses of size argument */
extern unsigned int osal_invalid_size_argument_for_ioc;
#define OSAL_IOC_TYPECHECK(t)                    \
    (((sizeof(t) == sizeof(((t *)0)[1])) &&      \
        (sizeof(t) < (1 << OSAL_IOC_SIZEBITS)))  \
            ? sizeof(t)                          \
            : osal_invalid_size_argument_for_ioc)
#endif
#endif /* OSAL_IOC_TYPECHECK */

/* used to create numbers */
#ifndef OSAL_IO
#define OSAL_IO(type, nr)             OSAL_IOC(OSAL_IOC_NONE, (type), (nr), 0)
#endif

#ifndef OSAL_IOR
#define OSAL_IOR(type, nr, size)      OSAL_IOC(OSAL_IOC_READ, (type), (nr), (OSAL_IOC_TYPECHECK(size)))
#endif

#ifndef OSAL_IOW
#define OSAL_IOW(type, nr, size)      OSAL_IOC(OSAL_IOC_WRITE, (type), (nr), (OSAL_IOC_TYPECHECK(size)))
#endif

#ifndef OSAL_IOWR
#define OSAL_IOWR(type, nr, size)     OSAL_IOC(OSAL_IOC_READ | OSAL_IOC_WRITE, (type), (nr), (OSAL_IOC_TYPECHECK(size)))
#endif

#ifndef OSAL_IOR_BAD
#define OSAL_IOR_BAD(type, nr, size)  OSAL_IOC(OSAL_IOC_READ, (type), (nr), sizeof(size))
#endif

#ifndef OSAL_IOW_BAD
#define OSAL_IOW_BAD(type, nr, size)  OSAL_IOC(OSAL_IOC_WRITE, (type), (nr), sizeof(size))
#endif

#ifndef OSAL_IOWR_BAD
#define OSAL_IOWR_BAD(type, nr, size) OSAL_IOC(OSAL_IOC_READ | OSAL_IOC_WRITE, (type), (nr), sizeof(size))
#endif

/* used to decode ioctl numbers.. */
#ifndef OSAL_IOC_DIR
#define OSAL_IOC_DIR(nr)    (((nr) >> OSAL_IOC_DIRSHIFT) & OSAL_IOC_DIRMASK)
#endif

#ifndef OSAL_IOC_TYPE
#define OSAL_IOC_TYPE(nr)   (((nr) >> OSAL_IOC_TYPESHIFT) & OSAL_IOC_TYPEMASK)
#endif

#ifndef OSAL_IOC_NR
#define OSAL_IOC_NR(nr)     (((nr) >> OSAL_IOC_NRSHIFT) & OSAL_IOC_NRMASK)
#endif

#ifndef OSAL_IOC_SIZE
#define OSAL_IOC_SIZE(nr)   (((nr) >> OSAL_IOC_SIZESHIFT) & OSAL_IOC_SIZEMASK)
#endif

#ifndef IOC_IN
#define IOC_IN        (OSAL_IOC_WRITE << OSAL_IOC_DIRSHIFT)
#endif
#ifndef IOC_OUT
#define IOC_OUT       (OSAL_IOC_READ << OSAL_IOC_DIRSHIFT)
#endif

#ifndef IOC_INOUT
#define IOC_INOUT     ((OSAL_IOC_WRITE | OSAL_IOC_READ) << OSAL_IOC_DIRSHIFT)
#endif

#ifndef IOCSIZE_MASK
#define IOCSIZE_MASK  (OSAL_IOC_SIZEMASK << OSAL_IOC_SIZESHIFT)
#endif

#ifndef IOCSIZE_SHIFT
#define IOCSIZE_SHIFT (OSAL_IOC_SIZESHIFT)
#endif

#endif /* _OSAL_IOCTL_H */
