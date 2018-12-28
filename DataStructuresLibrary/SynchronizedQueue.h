#ifndef __SYNCHRONIZED_QUEUE_H__ 
#define __SYNCHRONIZED_QUEUE_H__

#include <Windows.h>
#include "Queue.h"

typedef struct _SYNCHRONIZED_QUEUE
{
    QUEUE               Queue;
    CRITICAL_SECTION    CriticalSection;
}SYNCHRONIZED_QUEUE, *PSYNCHRONIZED_QUEUE;


VOID
SynchronizedQueueInitialize(
    _Inout_ PSYNCHRONIZED_QUEUE SynchronizedQueue,
    _In_    QUEUE_FREE_ROUTINE  FreeRoutine
);

VOID
SynchronizedQueueDestroy(
    _Inout_ PSYNCHRONIZED_QUEUE SynchronizedQueue
);

_Must_inspect_result_
DS_STATUS
SynchronizedQueuePush(
    _Inout_ PSYNCHRONIZED_QUEUE SynchronizedQueue,
    _In_    PVOID               Element
);

_Must_inspect_result_
DS_STATUS
SynchronizedQueuePop(
    _Inout_ PSYNCHRONIZED_QUEUE SynchronizedQueue,
    _Out_   PVOID*              Element
);


#endif //__SYNCHRONIZED_QUEUE_H__