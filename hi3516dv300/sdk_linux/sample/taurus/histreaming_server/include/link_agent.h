#ifndef __LINK_AGENT_H__
#define __LINK_AGENT_H__

#if defined(__cplusplus)
extern "C" {
#endif

#include <link_service_agent.h>
#include <link_platform.h>

typedef struct QueryResult {
    int (*count)(struct QueryResult *q);
    struct LinkServiceAgent *(*at)(struct QueryResult *q, int pos);
} QueryResult;

typedef struct LinkAgent {
    struct QueryResult *(*query)(struct LinkAgent *agent, const char *type);
} LinkAgent;

struct LinkAgent *LinkAgentGet(struct LinkPlatform *p);
void LinkAgentFree(struct LinkAgent *agent);
void QueryResultFree(struct QueryResult *q);

#if defined(__cplusplus)
}
#endif

#endif /*__LINK_AGENT_H__*/