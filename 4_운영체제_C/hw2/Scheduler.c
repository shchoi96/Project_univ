#include "Init.h"
#include "Thread.h"
#include "Scheduler.h"

int RunScheduler( void )
{
    int i;
    int priority = -1;
    Thread* thread;
    pid_t pid;
    // ReadyQueue 에 Thread가 존재하는 우선순위를 찾음
    priority = -1;
    for(i=0;i<MAX_READYQUEUE_NUM;++i)
    {
        if(pReadyQueueEnt[i].queueCount != 0)
        {
            priority = i;
            break;
        }
    }
    
    // ReadyQueue 에 아무것도 없음  || ReadyQueue 에 존재하는 Thread 보다 현재 Thread 가 우선순위가 높음
    if (priority == -1)
    {
        alarm(TIMESLICE);
        return 0;
    }
    if (pCurrentThread != NULL && pCurrentThread->status != THREAD_STATUS_WAIT && pCurrentThread->priority < priority)
    {
        alarm(TIMESLICE);
        return 0;
    }
    if(pCurrentThread != NULL && pCurrentThread->status != THREAD_STATUS_WAIT)
    {
        // pReadyQueueEnt[pCurrentThread->priority] 에 pCurrentThread 추가 : MoveToReadyQueue(pCurrentThread)
        pCurrentThread->status = THREAD_STATUS_READY;

        pCurrentThread->phNext = NULL;
        pCurrentThread->phPrev = NULL;
        if(pReadyQueueEnt[pCurrentThread->priority].pHead == NULL)
        {
            pReadyQueueEnt[pCurrentThread->priority].pHead = pCurrentThread;
            pReadyQueueEnt[pCurrentThread->priority].pTail = pCurrentThread;
        }
        else
        {
            pReadyQueueEnt[pCurrentThread->priority].pTail->phNext = pCurrentThread;
            pCurrentThread->phPrev = pReadyQueueEnt[pCurrentThread->priority].pTail;
            pReadyQueueEnt[pCurrentThread->priority].pTail = pCurrentThread;
        }
        ++pReadyQueueEnt[pCurrentThread->priority].queueCount;
    }
    
    // pReadyQueueEnt[priority]에서 새로운 thread 꺼내기
    thread = pReadyQueueEnt[priority].pHead;
    if (thread == pReadyQueueEnt[priority].pHead)
        pReadyQueueEnt[priority].pHead = thread->phNext;
    if (thread == pReadyQueueEnt[priority].pTail)
        pReadyQueueEnt[priority].pTail = thread->phPrev;
    if (thread->phNext != NULL)
        thread->phNext->phPrev = thread->phPrev;
    if (thread->phPrev != NULL)
        thread->phPrev->phNext = thread->phNext;
    pReadyQueueEnt[priority].queueCount--;
    // 새로운 thread 초기화
    thread->phNext = NULL;
    thread->phPrev = NULL;
    thread->status = THREAD_STATUS_RUN;
    if(pCurrentThread == NULL)
    {
        pCurrentThread = thread;
        kill(thread->pid, SIGCONT);
        alarm(TIMESLICE); 
    }
    else
    {
        pid = pCurrentThread->pid;
        pCurrentThread = thread;
        __ContextSwitch(pid, thread->pid);
    }
    
    return 0;
}


void __ContextSwitch(int curpid, int newpid)
{
    thread_t tid = thread_self();
    Thread* thread = pThreadTblEnt[tid].pThread;
    kill(newpid, SIGCONT);
    alarm(TIMESLICE);
    if(curpid != getpid()) 
        kill(curpid, SIGSTOP);
    else
    {
        if(thread->status == THREAD_STATUS_WAIT)
        {
            pause();
        } 
        else kill(curpid, SIGSTOP);
    }
}
