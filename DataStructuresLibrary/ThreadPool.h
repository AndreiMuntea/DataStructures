#ifndef __THREAD_POOL_H__ 
#define __THREAD_POOL_H__

#include <Windows.h>
#include "SynchronizedQueue.h"

typedef struct _THREAD_POOL
{
    HANDLE*                 Threads;
    UINT8                   NoThreads;
    LPTHREAD_START_ROUTINE  Routine;
    SYNCHRONIZED_QUEUE      Payloads;
    BOOL                    IsShutdownPending;
}THREAD_POOL, *PTHREAD_POOL;

_Must_inspect_result_
DS_STATUS
ThreadPoolInit(
    _Inout_ PTHREAD_POOL           ThreadPool,
    _In_    UINT8                  NoThreads,
    _In_    LPTHREAD_START_ROUTINE Routine,
    _In_    QUEUE_FREE_ROUTINE     PayloadCleanupRoutine
);

VOID
ThreadPoolShutdown(
    _Inout_ PTHREAD_POOL    ThreadPool
);

_Must_inspect_result_
DS_STATUS
ThreadPoolEnqueue(
    _Inout_ PTHREAD_POOL   ThreadPool,
    _In_    PVOID          Payload
);

DWORD WINAPI
ThreadPooolWorker(
    _In_ PVOID Context
);

#endif //__THREAD_POOL_H__