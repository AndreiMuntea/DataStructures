#include "SynchronizedQueue.h"

_Use_decl_annotations_
VOID
SynchronizedQueueInitialize(
    _Inout_ PSYNCHRONIZED_QUEUE SynchronizedQueue,
    _In_    QUEUE_FREE_ROUTINE  FreeRoutine
)
{
    InitializeCriticalSection(&SynchronizedQueue->CriticalSection);
    QueueInitialize(&SynchronizedQueue->Queue, FreeRoutine);
}

_Use_decl_annotations_
VOID
SynchronizedQueueDestroy(
    _Inout_ PSYNCHRONIZED_QUEUE SynchronizedQueue
)
{
    EnterCriticalSection(&SynchronizedQueue->CriticalSection);
    QueueDestroy(&SynchronizedQueue->Queue);
    LeaveCriticalSection(&SynchronizedQueue->CriticalSection);
    DeleteCriticalSection(&SynchronizedQueue->CriticalSection);
}

_Use_decl_annotations_
DS_STATUS
SynchronizedQueuePush(
    _Inout_ PSYNCHRONIZED_QUEUE SynchronizedQueue,
    _In_    PVOID               Element
)
{
    DS_STATUS status = DS_EXIT_STATUS_SUCCES;

    EnterCriticalSection(&SynchronizedQueue->CriticalSection);
    status = QueuePush(&SynchronizedQueue->Queue, Element);
    LeaveCriticalSection(&SynchronizedQueue->CriticalSection);

    return status;
}

_Use_decl_annotations_
DS_STATUS
SynchronizedQueuePop(
    _Inout_ PSYNCHRONIZED_QUEUE SynchronizedQueue,
    _Out_   PVOID*              Element
)
{
    DS_STATUS status = DS_EXIT_STATUS_SUCCES;

    EnterCriticalSection(&SynchronizedQueue->CriticalSection);
    status = QueuePop(&SynchronizedQueue->Queue, Element);
    LeaveCriticalSection(&SynchronizedQueue->CriticalSection);

    return status;
}

_Use_decl_annotations_
BOOL
SynchronizedQueueIsEmpty(
    _In_ PSYNCHRONIZED_QUEUE SynchronizedQueue
)
{
    BOOL result = FALSE;

    EnterCriticalSection(&SynchronizedQueue->CriticalSection);
    result = QueueIsEmpty(&SynchronizedQueue->Queue);
    LeaveCriticalSection(&SynchronizedQueue->CriticalSection);

    return result;
}