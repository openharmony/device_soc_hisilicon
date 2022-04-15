/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2018. All rights reserved.
 * Description: Defined LinkAgent for Application
 * Author: caps.liu@hisilicion.com
 * Create: 2020/5/29
 */

#ifdef WITH_POSIX

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <memory.h>

/* Link Header Files */
#include "status_code.h"
#include "link_service.h"
#include "link_platform.h"
#include "link_agent.h"
#include "hisignalling.h"
//#include "link_log.h"

static const char *g_device_type = "Light";
static volatile sig_atomic_t g_interrupted = 0;

unsigned char light_on_off_buff[4] = {0,2,0,3};

static unsigned int histreaming_msg_send_switch_1(unsigned char *write_buffer);
int arrayToStr(unsigned char *buf, unsigned int buflen, char *out);
/**
	@berf Receive signal handle
	@param int sig: signal
*/
static void SigIntHandler(int sig)
{
    g_interrupted = 1;
}
/**
	@berf histreaming send message
	@param unsigned char *write_buffer: send data buff
*/
static unsigned int histreaming_msg_send_switch_1(unsigned char *write_buffer)
{
    switch_1_trigger_send_msg(write_buffer);
    return 0;
}
/**
	@berf hex2string
	@param unsigned char *buf: hex data buff
	@param unsigned int buflen: hex data length
	@param char *out: output string data
*/
int arrayToStr(unsigned char *buf, unsigned int buflen, char *out)
{
    char strBuf[33] = {0};
    char pbuf[32];
   
    for( unsigned int i = 0; i < buflen; i++) {
        sprintf(pbuf, "%02X", buf[i]);
        strncat(strBuf, pbuf, 2);
    }
    strncpy(out, strBuf, buflen * 2);
    printf("out = %s\n", out);
    return buflen * 2;
}
/**
	@berf main function
*/
int main(int argc, char** argv)
{
    LinkPlatform *link = 0;
    LinkAgent *linkAgent = 0;
    char send_buff[HISIGNALLING_MSG_BUFF_LEN]= {0};
    int send_buff_len = 0;
    int status = 0;
    unsigned int led_flag = 0;
    //
    //Register SIGNT (Ctrl-C) handler
    //
    signal(SIGINT, SigIntHandler);
    signal(SIGKILL, SigIntHandler);
    signal(SIGTERM, SigIntHandler);

    link = LinkPlatformGet();
    if (!link) {
        goto ERROR;
    }

    linkAgent = LinkAgentGet(link);
    if (!linkAgent){
        goto ERROR;
    }

    link->setDebuglevel(link, 7);//set histreaming client debug level
    link->open(link);

    while (!g_interrupted) {
        QueryResult *qres = linkAgent->query(linkAgent, g_device_type);//start histreaming client agent module
        while (qres == NULL && !g_interrupted) {
            sleep(5);
            qres = linkAgent->query(linkAgent, g_device_type);//client Waiting and server connection
			printf("begin to query %s again....\n", g_device_type);
        }

        if (qres != 0) {//Connect with the server
            printf("hisignalling_package_len = %d\r\n", hisignalling_package_len);
			printf("find %d Light devices\n", qres->count(qres));
            LinkServiceAgent *serviceAgent = qres->at(qres, 0);
            if (serviceAgent != NULL) {
                if (led_flag%2 == 0) {
                    light_on_off_buff[3] = 3;
                } else {
                    light_on_off_buff[3] = 4;
                }   
                histreaming_msg_send_switch_1(light_on_off_buff);//Send message to the server
                send_buff_len = arrayToStr(hisignalling_send_buf, hisignalling_package_len, send_buff);
				status = serviceAgent->modify(serviceAgent,  "status", send_buff, send_buff_len);
                if (StatusDeviceOffline == status) {
                    printf("%s is offline\n", g_device_type);
                    link->discover(link, g_device_type);
                }
                if (led_flag >= 0xffffffff) {
                    led_flag = 0;   
                }
                led_flag++;
                
            //    LinkServiceAgentFree(serviceAgent);
            }

            QueryResultFree(qres);//free histreaming agent module
        } else {
			printf("Agent query %s = NULL\n", g_device_type);
        }

        // qres = linkAgent->findService(linkAgent, "DoorbellService");
        // if (qres != NULL) {
        //     _DBG("find %d device by findService\n", qres->count(qres));
        //     QueryResultFree(qres);
        // }

        sleep(1);
    }

ERROR:

    if (link != NULL){
        LinkPlatformFree(link);
    }

    if (linkAgent != NULL){
        LinkAgentFree(linkAgent);
    }

    return 0;
}

#endif
