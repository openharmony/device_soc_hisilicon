

#ifndef __LinkServiceAgent__H__
#define __LinkServiceAgent__H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct LinkServiceAgent {
    /*
     * this funtion to get device's property value.
     * s : ServiceAgent
     * property : device's property name
     * value : a json string of device's property value
     * len : value's length
     * rerurn code:
     * StatusOk : get value successfully
     * StatusFailure : failure to get value
     * StatusDeviceOfffline : device had offline
     */
    int (*get)(struct LinkServiceAgent* s, const char* property, char* value, int len);
    /*
     * this funtion to modify device's property
     * s : ServiceAgent
     * property : device's property name
     * value : a json string of device's property
     * len : value's length
     * rerurn code:
     * StatusOk : moified value successfully
     * StatusFailure : failure to moified value
     * StatusDeviceOfffline : device had offline
     */
    int (*modify)(struct LinkServiceAgent* s, const char* property, char* value, int len);
    /*
     * get device's service type
     * return code:
     * if type is not null, then return a string,otherwise,
     * a null string return
     */
    const char* (*type)(struct LinkServiceAgent* s);
    /*
     * get address of device's service
     */
    const char* (*addr)(struct LinkServiceAgent* s);
    /*
     * the id of device's service
     */
    int (*id)(struct LinkServiceAgent* s);
} LinkServiceAgent;

/*
 * free ServiceAgent that malloced by LinkAgent
 */
void LinkServiceAgentFree(LinkServiceAgent* s);

/// for C++
#ifdef __cplusplus
}
#endif

#endif /*__LinkServiceAgent__H__*/
