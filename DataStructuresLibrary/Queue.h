#ifndef __QUEUE_H__
#define __QUEUE_H__

#include "data_structures_common_definitios.h"
#include "data_structures_tags.h"
#include "data_structures_externals.h"
#include "LinkedList.h"

typedef VOID(*QUEUE_FREE_ROUTINE)(PVOID Element);

typedef struct _QUEUE_CONTEXT
{
    LINKED_LIST_ENTRY ListEntry;
    PVOID             Element;
}QUEUE_CONTEXT, *PQUEUE_CONTEXT;

typedef struct _QUEUE
{
    LINKED_LIST         ElementsList;
    QUEUE_FREE_ROUTINE  QueueFreeRoutine;
}QUEUE, *PQUEUE;


VOID
QueueInitialize(
    _Inout_ PQUEUE              Queue,
    _In_    QUEUE_FREE_ROUTINE  FreeRoutine
);

VOID
QueueDestroy(
    _Inout_ PQUEUE              Queue
);

_Must_inspect_result_
DS_STATUS
QueuePush(
    _Inout_ PQUEUE Queue,
    _In_    PVOID  Element
);

_Must_inspect_result_
DS_STATUS
QueuePop(
    _Inout_ PQUEUE  Queue,
    _Out_   PVOID*  Element
);

BOOL
QueueIsEmpty(
    _In_ PQUEUE              Queue
);

#endif //__QUEUE_H__