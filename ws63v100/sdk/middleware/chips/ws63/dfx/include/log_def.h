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
 * Description:  common logging producer interface - need to change name of log.h in all the protocol core files
 */

#ifndef _LOG_DEF_H_
#define _LOG_DEF_H_

#include "core.h"

#ifdef BTH_SYS_PART
#include "log_def_bth.h"
#else
#include "log_def_bt.h"
#endif

#include "log_def_pf.h"
#include "log_def_dsp.h"
#ifdef GNSS_SYS_PART
#include "log_def_gnss.h"
#endif

#endif
