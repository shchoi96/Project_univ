#include "Init.h"
#include "Thread.h"
#include "Scheduler.h"


// 해당하는 thread 를 WaitingQueue 로 옮기기
void MoveToWaitingQueue(Thread* thread)
{
    thread->phNext = NULL;
    thread->phPrev = NULL;

    if (pWaitingQueueHead == NULL)
	{
		pWaitingQueueHead = thread;
		pWaitingQueueTail = thread;
	}
	else
	{
        pWaitingQueueTail->phNext = thread;
        thread->phPrev = pWaitingQueueTail;
        pWaitingQueueTail = thread;
	}
}
// 해당하는 thread 를 ReadyQueue 로 옮기기
void MoveToReadyQueue(Thread* thread)
{
    thread->phNext = NULL;
    thread->phPrev = NULL;
    
    if(pReadyQueueEnt[thread->priority].pHead == NULL)
    {
        pReadyQueueEnt[thread->priority].pHead = thread;
        pReadyQueueEnt[thread->priority].pTail = thread;
    }
    else
    {
        pReadyQueueEnt[thread->priority].pTail->phNext = thread;
        thread->phPrev = pReadyQueueEnt[thread->priority].pTail;
        pReadyQueueEnt[thread->priority].pTail = thread;
    }
    ++pReadyQueueEnt[thread->priority].queueCount;
}

// 해당하는 thread 를 ReadyQueue 에서 찾아서 지우기
int RemoveFromReadyQueue(Thread* thread)
{
    Thread* readyThread = pReadyQueueEnt[thread->priority].pHead;
    while (readyThread != NULL)
	{
		if (readyThread == thread)
		{
			if (thread == pReadyQueueEnt[thread->priority].pHead)
				pReadyQueueEnt[thread->priority].pHead = thread->phNext;
			if (thread == pReadyQueueEnt[thread->priority].pTail)
				pReadyQueueEnt[thread->priority].pTail = thread->phPrev;
			if (thread->phNext != NULL)
				thread->phNext->phPrev = thread->phPrev;
			if (thread->phPrev != NULL)
				thread->phPrev->phNext = thread->phNext;

            thread->phNext = NULL;
            thread->phPrev = NULL;
			pReadyQueueEnt[thread->priority].queueCount--;
            return 0;
		}
		readyThread = readyThread->phNext;
	}
    return -1;
}

// 해당하는 thread 를 WaitingQueue 에서 찾아서 지우기
int RemoveFromWaitingQueue(Thread* thread)
{
    Thread* waitThread = pWaitingQueueHead;
    while(waitThread != NULL)
    {
        if(waitThread == thread)
        {
            if (thread == pWaitingQueueHead)
				pWaitingQueueHead = pWaitingQueueHead->phNext;
			if (thread == pWaitingQueueTail)
				pWaitingQueueTail = pWaitingQueueTail->phPrev;
			if (thread->phNext != NULL)
				thread->phNext->phPrev = thread->phPrev;
			if (thread->phPrev != NULL)
				thread->phPrev->phNext = thread->phNext;

            thread->phNext = NULL;
            thread->phPrev = NULL;
            return 0;
        }
        waitThread = waitThread->phNext;
    }
    return -1;
}


// join handler
void Join_handler(int signo)
{
    thread_t tid = thread_self();
    Thread* thread = pThreadTblEnt[tid].pThread;
    pid_t pid;
    int priority = 255, i;
    
    if (pCurrentThread->status != THREAD_STATUS_ZOMBIE)
    {
        return;
    }
    // 부모 Thread를 Waiting Queue에서 제거 후 RunScheduler
    RemoveFromWaitingQueue(thread);
    thread->status = THREAD_STATUS_RUN;
    pCurrentThread = thread;
    RunScheduler();
}



int thread_create(thread_t *thread, thread_attr_t *attr, int priority, void *(*start_routine) (void *), void *arg)
{
    int i;
    Thread* newThread; // 새로운 TCB
    int flags = SIGCHLD|CLONE_FS|CLONE_FILES|CLONE_SIGHAND|CLONE_VM;
    char* pStack = malloc(STACK_SIZE);
    //Thread 생성 및 멈추기
    pid_t pid = clone(start_routine, (char*)pStack + STACK_SIZE, flags, arg);
    kill(pid, SIGSTOP); // waitpid(pid, null);  //kill(pid, SIGCONT); //나중에 재활성할때 쓸수도?
    // TCB 할당
    newThread = malloc(sizeof(Thread));
    // TCB 초기화   
    newThread->stackSize = STACK_SIZE;
    newThread->stackAddr = pStack; // 스택 사이즈 안더해도 되려나?
    newThread->exitCode = 0; // defalut로 몇해야하는지 강의 찾아보기
    newThread->pid = pid;
    newThread->priority = priority;
    newThread->phNext = NULL;
    newThread->phPrev = NULL;
    // pThreadTblEnt 에 thread 추가
    for(i=0;i<MAX_THREAD_NUM;++i)
    {
        if(!pThreadTblEnt[i].bUsed)
        {
            pThreadTblEnt[i].bUsed = 1;
            pThreadTblEnt[i].pThread = newThread;
            *thread = (thread_t)i; // *thread = i;
            break;
        }
    }
    // Decision Thread status
    if(pCurrentThread == NULL)
    {
        // pReadyQueueEnt[priority] 에 생성된 thread 추가
        newThread->status = THREAD_STATUS_READY;
        MoveToReadyQueue(newThread);
    }
    else
    {
        if( newThread->priority < pCurrentThread->priority)
        { // 우선 순위가 높은 newThread가 선점
            // pReadyQueueEnt[priority] 에 pCurrentThread 추가
            pCurrentThread->status = THREAD_STATUS_READY;
            MoveToReadyQueue(pCurrentThread);
            // newThread로 Context Switch
            pid = pCurrentThread->pid;
            pCurrentThread = newThread;
            newThread->status = THREAD_STATUS_RUN;
            __ContextSwitch(pid, newThread->pid);            
        }
        else
        {
            // pReadyQueueEnt[priority] 에 생성된 thread 추가
            newThread->status = THREAD_STATUS_READY;
            MoveToReadyQueue(newThread);
        }
    }
    return pid;
}

int thread_suspend(thread_t tid)
{
    Thread* thread;
    // Thread 존재 여부 확인
    if(pThreadTblEnt[tid].bUsed)
        thread = pThreadTblEnt[tid].pThread;
    else
        return -1;
    
    // 좀비상태라면 -1 반환
    if(thread->status == THREAD_STATUS_ZOMBIE)
        return -1;
    // Ready Queue 또는 WaitingQueue 에서 제거
    if (RemoveFromReadyQueue(thread) == -1 && RemoveFromWaitingQueue(thread) == -1)
    { // 해당하는 Thread가 없으면 -1 반환
        return -1;
    }

    // Move TCB to waiting queue
	thread->status = THREAD_STATUS_WAIT;
    MoveToWaitingQueue(thread);
    
    return 0;
}

int thread_cancel(thread_t tid)
{
    Thread* thread;
    // Thread 존재 여부 확인
    if(pThreadTblEnt[tid].bUsed)
        thread = pThreadTblEnt[tid].pThread;
    else
        return -1;

    // 해당 Thread 종료
    kill(thread->pid, SIGKILL);
    // Ready Queue 또는 WaitingQueue 에서 제거
    if (RemoveFromReadyQueue(thread) == -1 && RemoveFromWaitingQueue(thread) == -1)
    { // 해당하는 Thread가 없으면 -1 반환
        return -1;
    }

    // Thread 정보 제거
    pThreadTblEnt[tid].bUsed = 0;
    free(thread);
    return 0;
}

int thread_resume(thread_t tid)
{
    Thread* thread;
    pid_t pid;
    // Thread 존재 여부 확인
    if(pThreadTblEnt[tid].bUsed)
        thread = pThreadTblEnt[tid].pThread;
    else
        return -1;

    // WaitingQueue 에서 해당 Thread를 제거. 해당 Thread가 없거나 좀비일 경우 -1
    if(thread->status == THREAD_STATUS_ZOMBIE || RemoveFromWaitingQueue(thread) == -1)
        return -1;
    // 우선순위 비교
    if(thread->priority < pCurrentThread->priority)
    {
        // pReadyQueueEnt[priority] 에 pCurrentThread 추가
        pCurrentThread->status = THREAD_STATUS_READY;
        MoveToReadyQueue(pCurrentThread);
        // newThread로 Context Switch
        pid = pCurrentThread->pid;
        pCurrentThread = thread;
        thread->status = THREAD_STATUS_RUN;
        __ContextSwitch(pid, thread->pid);
    }
    else
    {
        // pReadyQueueEnt[priority] 에 생성된 thread 추가
        thread->status =  THREAD_STATUS_READY;
        MoveToReadyQueue(thread);
    }
    return 0;
}

thread_t thread_self()
{
    int i;
    pid_t pid = getpid();
    for(i=0;i<MAX_THREAD_NUM;++i)
    {
        if(pThreadTblEnt[i].bUsed)
            if(pThreadTblEnt[i].pThread->pid == pid)
                return (thread_t)i;
    }
}

int thread_join(thread_t tid, void **retval)
{
    Thread* thread;
    struct sigaction act;
    // Thread 존재 여부 확인
    if(pThreadTblEnt[tid].bUsed)
        thread = pThreadTblEnt[tid].pThread;
    else
        return -1;

    if(thread->status != THREAD_STATUS_ZOMBIE)
    { // 현재 해당 Thread가 살아있다면 
        // 현재 Thread를 WaitingQueue로 보냄
        pCurrentThread->status = THREAD_STATUS_WAIT;
        MoveToWaitingQueue(pCurrentThread);
        
        // signal 등록
        memset(&act, 0, sizeof(struct sigaction));
        act.sa_handler = Join_handler;
        sigfillset(&act.sa_mask);
        act.sa_flags = SA_NOCLDSTOP;
        sigaction(SIGCHLD, &act, NULL);
        
        RunScheduler();
        while(thread->status != THREAD_STATUS_ZOMBIE)
        {
            pause();
        }
    }
    // 해당 Thread가 좀비상태라면 제거 
    *retval = &(thread->exitCode);
    thread_cancel(tid);
    
    return 0;
}

int thread_exit(void *retval)
{
    pCurrentThread->exitCode = *((int*)retval);
    pCurrentThread->status = THREAD_STATUS_ZOMBIE;
    MoveToWaitingQueue(pCurrentThread);
    exit(0);
}

