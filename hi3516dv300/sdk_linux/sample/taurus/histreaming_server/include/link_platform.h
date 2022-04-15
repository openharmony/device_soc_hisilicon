

#ifndef __LINKPLATFORM__H__
#define __LINKPLATFORM__H__

#include <link_service.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct LinkPlatform {
    int (*open)(struct LinkPlatform* linkP);
    int (*close)(struct LinkPlatform* linkP);
    int (*discover)(struct LinkPlatform* linkP, const char *type);
    int (*setDebuglevel)(struct LinkPlatform* linkP, int level);
    int (*addLinkService)(struct LinkPlatform* linkP, LinkService *service, int flag/*ownership*/);
} LinkPlatform;

LinkPlatform* LinkPlatformGet(void);
void LinkPlatformFree(LinkPlatform* linkP);

///for C++
#ifdef __cplusplus
}
#endif

#endif /*__LINKPLATFORM__H__*/
