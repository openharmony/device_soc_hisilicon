/**
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
 *
 * Description: Multi-precision integer implementation. \n
 *
 * History: \n
 * 2024-01-27, Create file. \n
 */

#include "app_call.h"
#include "hilink_sal_mpi.h"

HiLinkMpi HILINK_SAL_MpiInit(void)
{
    return app_call0(APP_CALL_HILINK_SAL_MPI_INIT, HiLinkMpi);
}

void HILINK_SAL_MpiFree(HiLinkMpi mpi)
{
    app_call1(APP_CALL_HILINK_SAL_MPI_FREE, void, HiLinkMpi, mpi);
}

int HILINK_SAL_MpiExpMod(HiLinkMpi x, HiLinkMpi a, HiLinkMpi e, HiLinkMpi n)
{
    return app_call4(APP_CALL_HILINK_SAL_MPI_EXP_MOD, int, HiLinkMpi, x, HiLinkMpi, a, HiLinkMpi, e, HiLinkMpi, n);
}

int HILINK_SAL_MpiCmpInt(HiLinkMpi x, int64_t z)
{
    return app_call2(APP_CALL_HILINK_SAL_MPI_CMP_INT, int, HiLinkMpi, x, int64_t, z);
}

int HILINK_SAL_MpiSubInt(HiLinkMpi x, HiLinkMpi a, int64_t b)
{
    return app_call3(APP_CALL_HILINK_SAL_MPI_SUB_INT, int, HiLinkMpi, x, HiLinkMpi, a, int64_t, b);
}

int HILINK_SAL_MpiCmpMpi(HiLinkMpi x, HiLinkMpi y)
{
    return app_call2(APP_CALL_HILINK_SAL_MPI_CMP_MPI, int, HiLinkMpi, x, HiLinkMpi, y);
}

int HILINK_SAL_MpiReadString(HiLinkMpi mpi, unsigned char radix, const char *s)
{
    return app_call3(APP_CALL_HILINK_SAL_MPI_READ_STRING, int, HiLinkMpi, mpi, unsigned char, radix, const char *, s);
}

int HILINK_SAL_MpiWriteString(HiLinkMpi mpi, unsigned int radix, char *buf, unsigned int *bufLen)
{
    return app_call4(APP_CALL_HILINK_SAL_MPI_WRITE_STRING, int, HiLinkMpi, mpi, unsigned int, radix,
        char *, buf, unsigned int *, bufLen);
}

int HILINK_SAL_MpiReadBinary(HiLinkMpi mpi, const unsigned char *buf, unsigned int bufLen)
{
    return app_call3(APP_CALL_HILINK_SAL_MPI_READ_BINARY, int,
        HiLinkMpi, mpi, const unsigned char *, buf, unsigned int, bufLen);
}

int HILINK_SAL_MpiWriteBinary(HiLinkMpi mpi, unsigned char *buf, unsigned int bufLen)
{
    return app_call3(APP_CALL_HILINK_SAL_MPI_WRITE_BINARY, int,
        HiLinkMpi, mpi, unsigned char *, buf, unsigned int, bufLen);
}