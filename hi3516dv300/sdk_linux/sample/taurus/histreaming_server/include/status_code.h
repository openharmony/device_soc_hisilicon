

#ifndef __STATUSCODE__H__
#define __STATUSCODE__H__

typedef enum StatusCode
{
    StatusFailure = -1,
    StatusOk = 0,
    StatusTimeOut,
    StatusURIError,
    StatusIOError,
    StatusIOReady,
    StatusNoInitialize,
    StatusDeviceOffline,
    StatusNoDevice,
    StatusNoVideoDevices,
    StatusNewFramed,
    StatusFramedEnd,
    StatusComposedData,
    StatusNoSession,
    StatusTargetExist,
    StatusTargetNoExist,
    StatusQEmpty,
    StatusLostFrame,
} StatusCode;

#endif /*__STATUSCODE__H__*/
