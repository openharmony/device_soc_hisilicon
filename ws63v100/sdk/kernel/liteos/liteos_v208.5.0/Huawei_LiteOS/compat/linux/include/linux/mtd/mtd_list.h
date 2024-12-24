/*
 * Copyright (c) 2020 HiSilicon (Shanghai) Technologies CO., LIMITED.
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
#ifndef _LINUX_MTD_LIST_H
#define _LINUX_MTD_LIST_H

#include "linux/mtd/mtd.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct Node {
    char *type;           /* flash type */
    int status;           /* flash status */
    struct mtd_info *mtd; /* mtd_info struct */
    struct Node *next;    /* next mtd_info struct */
} Lnode, *linklist;

extern linklist head;
struct mtd_info* get_mtd(const char *type);
int get_mtd_info(const char *type);
int free_mtd(struct mtd_info *mtd);
int del_mtd_list(struct mtd_info *mtd);
void add_mtd_list(char *type, struct mtd_info *mtd);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _LINUX_MTD_LIST_H */
