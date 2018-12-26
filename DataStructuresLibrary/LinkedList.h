#ifndef __LINKED_LIST_H__
#define __LINKED_LIST_H__


#include "LinkedListEntry.h"
#include "common_definitios.h"

typedef struct _LINKED_LIST
{
    LINKED_LIST_ENTRY Base;
}LINKED_LIST, *PLINKED_LIST;

VOID
LinkedListInitialize(
    _Inout_ PLINKED_LIST LinkedList
);

_Must_inspect_result_
DS_STATUS
LinkedListInsertTail(
    _Inout_ PLINKED_LIST        LinkedList,
    _In_    PLINKED_LIST_ENTRY  Entry
);

_Must_inspect_result_
DS_STATUS
LinkedListInsertHead(
    _Inout_ PLINKED_LIST        LinkedList,
    _In_    PLINKED_LIST_ENTRY  Entry
);

_Must_inspect_result_
DS_STATUS
LinkedListRemoveHead(
    _Inout_ PLINKED_LIST        LinkedList,
    _Out_   PLINKED_LIST_ENTRY* Entry
);

_Must_inspect_result_
DS_STATUS
LinkedListRemoveTail(
    _Inout_ PLINKED_LIST        LinkedList,
    _Out_   PLINKED_LIST_ENTRY* Entry
);

_Must_inspect_result_
DS_STATUS
LinkedListRemoveEntry(
    _Inout_ PLINKED_LIST        LinkedList,
    _In_    PLINKED_LIST_ENTRY  Entry
);

BOOL
LinkedListIsEmpty(
    _In_    PLINKED_LIST        LinkedList
);

BOOL
LinkedListIsCorrupted(
    _In_    PLINKED_LIST        LinkedList
);

BOOL
LinkedListEntryIsCorrupted(
    _In_    PLINKED_LIST_ENTRY  Entry
);

#endif //__LINKED_LIST_H__