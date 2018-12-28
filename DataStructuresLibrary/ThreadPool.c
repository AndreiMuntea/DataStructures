#include "ThreadPool.h"

#define THREAD_POOL_DEFAULT_POLLING_TIME    5000

_Use_decl_annotations_
DS_STATUS
ThreadPoolInit(
    _Inout_ PTHREAD_POOL           ThreadPool,
    _In_    UINT8                  NoThreads,
    _In_    LPTHREAD_START_ROUTINE Routine,
    _In_    QUEUE_FREE_ROUTINE     PayloadCleanupRoutine
)
{
    HANDLE* threads = (HANDLE*)DS_Alloc(NoThreads * sizeof(HANDLE), THREAD_POOL_TAG);
    if (NULL == threads)
    {
        return EXIT_STATUS_NOT_ENOUGH_RESOURCES;
    }
    Ds_ZeroMemory(threads, NoThreads * sizeof(HANDLE));

    ThreadPool->IsShutdownPending = FALSE;
    ThreadPool->NoThreads = NoThreads;
    ThreadPool->Routine = Routine;
    ThreadPool->Threads = threads;

    SynchronizedQueueInitialize(&ThreadPool->Payloads, PayloadCleanupRoutine);

    for (UINT8 i = 0; i < NoThreads; ++i)
    {
        threads[i] = CreateThread(NULL, 0, ThreadPooolWorker, ThreadPool, 0, NULL);
        if (!threads[i])
        {
            ThreadPool->IsShutdownPending = TRUE;
            WaitForMultipleObjects(i, threads, TRUE, INFINITE);
            DS_Free(threads, THREAD_POOL_TAG);
            return EXIT_STATUS_CREATE_THREAD_FAILED;
        }
    }

    return EXIT_STATUS_SUCCES;
}

_Use_decl_annotations_
VOID
ThreadPoolShutdown(
    _Inout_ PTHREAD_POOL    ThreadPool
)
{
    ThreadPool->IsShutdownPending = TRUE;

    WaitForMultipleObjects(ThreadPool->NoThreads, ThreadPool->Threads, TRUE, INFINITE);
    DS_Free(ThreadPool->Threads, THREAD_POOL_TAG);

    SynchronizedQueueDestroy(&ThreadPool->Payloads);
}

_Use_decl_annotations_
DS_STATUS
ThreadPoolEnqueue(
    _Inout_ PTHREAD_POOL   ThreadPool,
    _In_    PVOID          Payload
)
{
    if (ThreadPool->IsShutdownPending)
    {
        return EXIT_STATUS_SHUTDOWN_PENDING;
    }

    return SynchronizedQueuePush(&ThreadPool->Payloads, Payload);
}

_Use_decl_annotations_
DWORD WINAPI
ThreadPooolWorker(
    _In_ PVOID Context
)
{
    PTHREAD_POOL threadPool = (PTHREAD_POOL)(Context);
    PVOID payload = NULL;
    DS_STATUS status = EXIT_STATUS_SUCCES;

    while (!threadPool->IsShutdownPending)
    {
        status = SynchronizedQueuePop(&threadPool->Payloads, &payload);
        if (status == EXIT_STATUS_NO_ELEMENTS_IN_LIST)
        {
            Sleep(THREAD_POOL_DEFAULT_POLLING_TIME);
            continue;
        }

        if (!DS_STATUS_SUCCESS(status))
        {
            continue;
        }

        threadPool->Routine(payload);
    }

    return EXIT_STATUS_SUCCES;
}