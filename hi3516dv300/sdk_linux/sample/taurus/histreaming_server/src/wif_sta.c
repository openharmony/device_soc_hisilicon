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

static volatile sig_atomic_t g_interrupted = 0;
/**
	@berf Receive signal handle
	@param int sig: signal
*/
static void SigIntHandler(int sig)
{
    g_interrupted = 1;
}
/**
	@berf Client gets server status and value
	@param struct LinkService* ar: server struct
	@param const char* property: client property
	@param char* value: client value
	@param int len: client value len
*/
static int __doGET(struct LinkService* ar, const char* property, char* value, int len)
{
    printf("Receive property: %s(value=%s)\n", property, value);

    if (strcmp(property, "Status") == 0) {
        strcpy(value, "Opend");
    }

    /*
     * if Ok return StatusOk,
     * Otherwise, any error, return StatusFailure
     */
    return StatusOk;
}
/**
	@berf Client puts property status and value
	@param struct LinkService* ar: server struct
	@param const char* property: client property
	@param char* value: client value
	@param int len: client value len
*/
static int __doPUT(struct LinkService* ar, const char* property, char* value, int len)
{
    printf("Receive property: %s(value=%s)\n", property, value);

    /*
     * if Ok return StatusOk,
     * Otherwise, any error, return StatusFailure
     */
    return StatusOk;
}

/*
 * It is a Doorbell device
 */
static const char* g_wifista_type = "Light";
static const char* __doType(struct LinkService* ar)
{
    return g_wifista_type;
}

#ifdef __HuaweiLite__
int app_main(int argc, char *argv[])
#else
int main(int argc, char *argv[])
#endif
{
    LinkService* doorbell = 0;
    LinkPlatform* link = 0;

    //
    //Register SIGNT (Ctrl-C) handler
    //
    signal(SIGINT, SigIntHandler);
    signal(SIGKILL, SigIntHandler);
    signal(SIGTERM, SigIntHandler);

    /*
     *   Construct doorbell
     */
    doorbell = (LinkService*)malloc(sizeof(LinkService));
    if (!doorbell){
        printf("malloc Doorbell failure\n");
        goto ERROR;
    }

    doorbell->get    = __doGET;
    doorbell->modify = __doPUT;
    doorbell->type = __doType;

    link = LinkPlatformGet();
    if (!link) {
        printf("get link failure\n");
        goto ERROR;
    }
//	link->setDeviceType(link, "Light");
    link->addLinkService(link, doorbell, 1);
    link->open(link);

    while (!g_interrupted) {
        usleep(1000);
    }
    
    /* Release */
    link->close(link);

ERROR:

    if (link != 0){
        LinkPlatformFree(link);
    }

    return 0;
}

#endif
