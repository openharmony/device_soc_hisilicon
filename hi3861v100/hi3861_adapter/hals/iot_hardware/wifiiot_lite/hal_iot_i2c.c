/*
 * Copyright (c) 2020 Huawei Device Co., Ltd.
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

#include "iot_errno.h"
#include "iot_i2c.h"
#include "hi_i2c.h"

unsigned int IoTI2cWrite(unsigned int id, unsigned short deviceAddr, const unsigned char *data, unsigned int dataLen)
{
    hi_i2c_data i2cData;
    i2cData.receive_buf = NULL;
    i2cData.receive_len = 0;
    i2cData.send_buf = data;
    i2cData.send_len = dataLen;

    return hi_i2c_write((hi_i2c_idx)id, deviceAddr, &i2cData);
}

unsigned int IoTI2cRead(unsigned int id, unsigned short deviceAddr, unsigned char *data, unsigned int dataLen)
{
    hi_i2c_data i2cData;
    i2cData.receive_buf = data;
    i2cData.receive_len = dataLen;
    i2cData.send_buf = NULL;
    i2cData.send_len = 0;

    return hi_i2c_read((hi_i2c_idx)id, deviceAddr, &i2cData);
}

unsigned int IoTI2cInit(unsigned int id, unsigned int baudrate)
{
    return hi_i2c_init((hi_i2c_idx)id, baudrate);
}

unsigned int IoTI2cDeinit(unsigned int id)
{
    return hi_i2c_deinit((hi_i2c_idx)id);
}

unsigned int IoTI2cSetBaudrate(unsigned int id, unsigned int baudrate)
{
    return hi_i2c_set_baudrate((hi_i2c_idx)id, baudrate);
}
