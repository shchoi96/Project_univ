#include "Init.h"
#include "Thread.h"
#include "Scheduler.h"

void Init(void)
{
    int i;
    // pThreadTblEnt
    for(i=0;i<MAX_THREAD_NUM;++i)
    {
        pThreadTblEnt[i].bUsed = 0;
    }
    // pReadyQueueEnt
    for(i=0;i<MAX_READYQUEUE_NUM;++i)
    {
        pReadyQueueEnt[i].queueCount = 0;
        pReadyQueueEnt[i].pHead = NULL;
        pReadyQueueEnt[i].pTail = NULL;
    }
    // pWaitingQueueHead & pWaitingQueueTail
    pWaitingQueueHead = NULL;
    pWaitingQueueTail = NULL;
    // pCurrentThread
    pCurrentThread = NULL;

    signal(SIGALRM, RunScheduler);
}
