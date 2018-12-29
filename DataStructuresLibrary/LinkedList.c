#include "LinkedList.h"

_Use_decl_annotations_
VOID
LinkedListInitialize(
    _Inout_ PLINKED_LIST LinkedList
)
{
    LinkedList->Base.Flink = &LinkedList->Base;
    LinkedList->Base.Blink = &LinkedList->Base;
    LinkedList->Size = 0;
}

_Use_decl_annotations_
DS_STATUS
LinkedListInsertTail(
    _Inout_ PLINKED_LIST        LinkedList,
    _In_    PLINKED_LIST_ENTRY  Entry
)
{
    if (LinkedListIsCorrupted(LinkedList))
    {
        return DS_EXIT_STATUS_CORRUPTED_LIST;
    }

    Entry->Blink = LinkedList->Base.Blink;
    Entry->Flink = &LinkedList->Base;

    LinkedList->Base.Blink->Flink = Entry;
    LinkedList->Base.Blink = Entry;
    
    LinkedList->Size++;

    return DS_EXIT_STATUS_SUCCES;
}

_Use_decl_annotations_
DS_STATUS
LinkedListInsertHead(
    _Inout_ PLINKED_LIST        LinkedList,
    _In_    PLINKED_LIST_ENTRY  Entry
)
{
    if (LinkedListIsCorrupted(LinkedList))
    {
        return DS_EXIT_STATUS_CORRUPTED_LIST;
    }

    Entry->Flink = LinkedList->Base.Flink;
    Entry->Blink = &LinkedList->Base;

    LinkedList->Base.Flink->Blink = Entry;
    LinkedList->Base.Flink = Entry;

    LinkedList->Size++;

    return DS_EXIT_STATUS_SUCCES;
}

_Use_decl_annotations_
DS_STATUS
LinkedListRemoveHead(
    _Inout_ PLINKED_LIST        LinkedList,
    _Out_   PLINKED_LIST_ENTRY* Entry
)
{
    *Entry = LinkedList->Base.Flink;
    return LinkedListRemoveEntry(LinkedList, *Entry);
}

_Use_decl_annotations_
DS_STATUS
LinkedListRemoveTail(
    _Inout_ PLINKED_LIST        LinkedList,
    _Out_   PLINKED_LIST_ENTRY* Entry
)
{
    *Entry = LinkedList->Base.Blink;
    return LinkedListRemoveEntry(LinkedList, *Entry);
}

_Use_decl_annotations_
DS_STATUS
LinkedListRemoveEntry(
    _Inout_ PLINKED_LIST        LinkedList,
    _In_    PLINKED_LIST_ENTRY  Entry
)
{
    if (LinkedListIsCorrupted(LinkedList))
    {
        return DS_EXIT_STATUS_CORRUPTED_LIST;
    }

    if (LinkedListEntryIsCorrupted(Entry))
    {
        return DS_EXIT_STATUS_CORRUPTED_LIST;
    }

    if (LinkedListIsEmpty(LinkedList))
    {
        return DS_EXIT_STATUS_NO_ELEMENTS_IN_LIST;
    }

    PLINKED_LIST_ENTRY next = Entry->Flink;
    PLINKED_LIST_ENTRY prev = Entry->Blink;

    next->Blink = prev;
    prev->Flink = next;

    LinkedList->Size--;

    return DS_EXIT_STATUS_SUCCES;
}

_Use_decl_annotations_
BOOL
LinkedListIsEmpty(
    _In_    PLINKED_LIST        LinkedList
)
{
    return LinkedList->Base.Flink == &LinkedList->Base &&
           LinkedList->Base.Blink == &LinkedList->Base;
}

_Use_decl_annotations_
BOOL
LinkedListEntryIsCorrupted(
    _In_    PLINKED_LIST_ENTRY  Entry
)
{
    return Entry->Flink->Blink != Entry ||
           Entry->Blink->Flink != Entry;
}

_Use_decl_annotations_
QWORD
LinkedListGetSize(
    _In_    PLINKED_LIST        LinkedList
)
{
    return LinkedList->Size;
}

_Use_decl_annotations_
BOOL
LinkedListIsCorrupted(
    _In_    PLINKED_LIST        LinkedList
)
{
    return LinkedListEntryIsCorrupted(LinkedList->Base.Flink) ||
           LinkedListEntryIsCorrupted(LinkedList->Base.Blink);
}