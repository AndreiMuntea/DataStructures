#include "Queue.h"

_Use_decl_annotations_
VOID
QueueInitialize(
    _Inout_ PQUEUE              Queue,
    _In_    QUEUE_FREE_ROUTINE  FreeRoutine
)
{
    LinkedListInitialize(&Queue->ElementsList);
    Queue->QueueFreeRoutine = FreeRoutine;
}

_Use_decl_annotations_
VOID
QueueDestroy(
    _Inout_ PQUEUE              Queue
)
{
    PLINKED_LIST_ENTRY element = NULL;
    PQUEUE_CONTEXT queueElementContext = NULL;
    DS_STATUS status = EXIT_STATUS_SUCCES;

    while (!LinkedListIsEmpty(&Queue->ElementsList))
    {
        status = LinkedListRemoveTail(&Queue->ElementsList, &element);
        DS_Assert(DS_STATUS_SUCCESS(status));

        queueElementContext = (PQUEUE_CONTEXT)(element);
        Queue->QueueFreeRoutine((PVOID)(queueElementContext->Element));
        DS_Free(queueElementContext, QUEUE_ELEMENT_CONTEXT_TAG);
    }

    Queue->QueueFreeRoutine = NULL;
}

_Use_decl_annotations_
DS_STATUS
QueuePush(
    _Inout_ PQUEUE Queue,
    _In_    PVOID  Element
)
{
    DS_STATUS status = EXIT_STATUS_SUCCES;
    PQUEUE_CONTEXT element = NULL;

    element = (PQUEUE_CONTEXT)DS_Alloc(sizeof(QUEUE_CONTEXT), QUEUE_ELEMENT_CONTEXT_TAG);
    if (!element)
    {
        return EXIT_STATUS_NOT_ENOUGH_RESOURCES;
    }
    
    Ds_ZeroMemory(element, sizeof(QUEUE_CONTEXT));
    element->Element = Element;

    status = LinkedListInsertTail(&Queue->ElementsList, &element->ListEntry);
    if (!DS_STATUS_SUCCESS(status))
    {
        DS_Free(Element, QUEUE_ELEMENT_CONTEXT_TAG);
    }

    return status;
}

_Use_decl_annotations_
DS_STATUS
QueuePop(
    _Inout_ PQUEUE  Queue,
    _Out_   PVOID*  Element
)
{
    PQUEUE_CONTEXT element = NULL;
    PLINKED_LIST_ENTRY listEntry;
    DS_STATUS status = EXIT_STATUS_SUCCES;

    *Element = NULL;

    status = LinkedListRemoveHead(&Queue->ElementsList, &listEntry);
    if (!DS_STATUS_SUCCESS(status))
    {
        return status;
    }

    element = (PQUEUE_CONTEXT)(listEntry);
    *Element = element->Element;
    DS_Free(element, QUEUE_ELEMENT_CONTEXT_TAG);

    return EXIT_STATUS_SUCCES;
}

_Use_decl_annotations_
BOOL
QueueIsEmpty(
    _In_ PQUEUE              Queue
)
{
    return LinkedListIsEmpty(&Queue->ElementsList);
}