/*
# Copyright (C) 2024 HiHope Open Source Organization .
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
*/


#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "securec.h"
#include "ohos_sle_common.h"
#include "ohos_sle_errcode.h"
#include "ohos_sle_ssap_server.h"
#include "ohos_sle_ssap_client.h"
#include "ohos_sle_device_discovery.h"
#include "ohos_sle_connection_manager.h"

#define SLE_OH_LOG "[SLE_OH_LOG]"

//设置本地设备地址，返回1设置成功，返回0设置失败
bool  SetLocalSleAddress(SleAddr *addr){
    ErrCodeType errcode = sle_set_local_addr(addr);
	  if(errcode == 0){
      printf("%s SetLocalSleAddress_Error:%d\r\n", SLE_OH_LOG,errcode);
		  return 1;
	  }else{
      printf("%s SetLocalSleAddress_Error:%d\r\n", SLE_OH_LOG,errcode);
		  return 0 ;
	  }	
}

//注册 SSAP client
ErrCodeType  SsapcRegister(SleUuid *appUuid, uint8_t *clientId){
  sle_uuid_t *sle_uuid = (sle_uuid_t *)malloc(sizeof(sle_uuid_t)); 
  sle_uuid->len = appUuid->len;
  memcpy(sle_uuid->uuid,appUuid->uuid,appUuid->len);
  ErrCodeType errcode = ssapc_register_client(appUuid,clientId);
  free(sle_uuid);
 	return errcode;
}	

//注销 SSAP client
ErrCodeType  SsapcRegisterUnregister(uint8_t clientId){
	return ssapc_unregister_client(clientId);
}

//向对端发送写请求。明确下发数据的速率要大于连接间隔30%
ErrCodeType SsapcWriteReq(uint8_t clientId, uint16_t connId, SsapcWriteParam *param){
  ssapc_write_param_t *ssapc_write_param = (ssapc_write_param_t *)malloc(sizeof(ssapc_write_param_t)); 
  ssapc_write_param->handle = param->handle;
  ssapc_write_param->type = param->type;
  ssapc_write_param->data_len = param->dataLen;
  memcpy(ssapc_write_param->data,param->data,param->dataLen);
  ErrCodeType errcode = ssapc_write_req(clientId,connId,param);
  free(ssapc_write_param);
 	return errcode;
}

//注册SSAP server
ErrCodeType SsapsRegisterServer(SleUuid *appUuid, uint8_t *serverId){
  sle_uuid_t *sle_uuid = (sle_uuid_t *)malloc(sizeof(sle_uuid_t)); 
  sle_uuid->len = appUuid->len;
  memcpy(sle_uuid->uuid,appUuid->uuid,appUuid->len);
  ErrCodeType errcode = ssaps_register_server(appUuid,serverId);
  free(sle_uuid);
 	return errcode;
 }

//添加SSAP server
ErrCodeType SsapsAddService(uint8_t serviceId, SleUuid *serviceUuid, bool isPrimary, uint16_t *handle){
   (void)handle;
   sle_uuid_t *sle_uuid = (sle_uuid_t *)malloc(sizeof(sle_uuid_t)); 
   sle_uuid->len = serviceUuid->len;
   memcpy(sle_uuid->uuid,serviceUuid->uuid,serviceUuid->len);
   ErrCodeType errcode = ssaps_add_service(serviceId, serviceUuid, isPrimary);
   free(sle_uuid);
  return errcode;
}

ErrCodeType  SsapsAddProperty(uint8_t serviceId, uint16_t serviceHandle, SsapsPropertyInfo *property, uint16_t *handle){
  (void)handle;
  ssaps_property_info_t *sle_property = (ssaps_property_info_t *)malloc(sizeof(ssaps_property_info_t)); 
  sle_property->uuid.len = property->uuid.len;
  memcpy(sle_property->uuid.uuid,property->uuid.uuid,property->uuid.len);
  sle_property->permissions = property->permissions;
  sle_property->operate_indication = property->operateIndication;
  sle_property->value_len = property->valueLen;
  sle_property->value = property->value;
  ErrCodeType errcode = ssaps_add_property(serviceId, serviceHandle, property);
  free(sle_property);
  return errcode;
}

ErrCodeType SsapsAddDescriptor(uint8_t serviceId, uint16_t serviceHandle, uint16_t propertyHandle,
    SsapsDescInfo *descriptor){
  ssaps_desc_info_t *ssaps_desc_info = (ssaps_desc_info_t *)malloc(sizeof(ssaps_desc_info_t)); 
  ssaps_desc_info->uuid.len = descriptor->uuid.len;
  memcpy(ssaps_desc_info->uuid.uuid,descriptor->uuid.uuid,descriptor->uuid.len);
  ssaps_desc_info->permissions = descriptor->permissions;
  ssaps_desc_info->operate_indication = descriptor->operateIndication;
  ssaps_desc_info->value_len = descriptor->valueLen;
  ssaps_desc_info->value = descriptor->value;
  ErrCodeType errcode = ssaps_add_descriptor(serviceId, serviceHandle, propertyHandle,descriptor);
  free(ssaps_desc_info);
  return errcode;
}