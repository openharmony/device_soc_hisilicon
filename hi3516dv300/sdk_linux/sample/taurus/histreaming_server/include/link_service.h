

#ifndef __APPRESOURCE__H__
#define __APPRESOURCE__H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct LinkService {
    int (*get)(struct LinkService* ar, const char* property, char* value, int len);
    int (*modify)(struct LinkService* ar, const char* property, char* value, int len);
    const char* (*type)(struct LinkService* ar);
} LinkService;

/// for C++
#ifdef __cplusplus
}
#endif

#endif /*__APPRESOURCE__H__*/
