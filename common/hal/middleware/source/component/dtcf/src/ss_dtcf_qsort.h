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
#ifndef OT_DTCF_QSORT_H_
#define OT_DTCF_QSORT_H_

#include <stddef.h>

static inline void Swap(char *a, char *b, size_t n)
{
    size_t i = (n) / sizeof(long);
    register long *pi = (long *)(a);
    register long *pj = (long *)(b);
    do {
        register long t = *pi;
        *pi++ = *pj;
        *pj++ = t;
    } while (--i > 0);
}

static inline void VecSwap(char *a, char *b, int n)
{
    if (n > 0) {
        Swap(a, b, (size_t)n);
    }
}

static inline char *Med3(char *a, char *b, char *c, int (*cmp)())
{
    return cmp(a, b) < 0 ? (cmp(b, c) < 0 ? b : (cmp(a, c) < 0 ? c : a)) :
                           (cmp(b, c) > 0 ? b : (cmp(a, c) < 0 ? a : c));
}

static void SubSwap(char *a, size_t n, size_t es, int (*cmp)())
{
    char *pm = a + (n / 2) * es; // 2:get mid pointer
    if (n > 7) { // 7: sort list big than 7
        char *pl;
        char *pn;
        pl = a;
        pn = a + (n - 1) * es;
        if (n > 40) { // 40: sort list big than 40
            size_t d = (n / 8) * es; // 8: 1/8 of n as distance
            pl = Med3(pl, pl + d, pl + 2 * d, cmp); // 2: double distance
            pm = Med3(pm - d, pm, pm + d, cmp);
            pn = Med3(pn - 2 * d, pn - d, pn, cmp); // 2:doubule distance
        }
        pm = Med3(pl, pm, pn, cmp);
    }
    Swap(a, pm, es);
}

void SS_DTCF_Qsort(void *a, size_t n, size_t es, int (*cmp)())
{
    char *pa;
    char *pb;
    char *pc;
    char *pd;
    char *pn;
    int ret;
    int partitioning = 1;

    if (es == 0) {
        return;
    }

    SubSwap(a, n, es, cmp);

    pa = pb = a;
    pc = pd = (char *)a + (n - 1) * es;
    while (partitioning) {
        while (pb <= pc && (ret = cmp(pb, a)) <= 0) {
            if (ret == 0) {
                Swap(pa, pb, es);
                pa += es;
            }
            pb += es;
        }
        while (pc >= pb && (ret = cmp(pc, a)) >= 0) {
            if (ret == 0) {
                Swap(pc, pd, es);
                pd -= es;
            }
            pc -= es;
        }

        if (pb > pc) {
            partitioning = 0;
            continue;
        }
        Swap(pb, pc, es);
        pb += es;
        pc -= es;
    }

    pn = (char *)a + n * es;
    ret = MIN(pa - (char *)a, pb - pa);
    VecSwap(a, pb - ret, ret);
    ret = MIN(pd - pc, pn - pd - (int)es);
    VecSwap(pb, pn - ret, ret);
    if ((ret = pb - pa) > (int)es) {
        SS_DTCF_Qsort(a, ret / es, es, cmp);
    }

    if ((ret = pd - pc) > (int)es) {
        SS_DTCF_Qsort(pn - ret, ret / es, es, cmp);
    }
}

#endif /* OT_DTCF_QSORT_H_ */
