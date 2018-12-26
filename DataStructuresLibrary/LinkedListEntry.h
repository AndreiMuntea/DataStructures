#ifndef __LINKED_LIST_ENTRY_H__ 
#define __LINKED_LIST_ENTRY_H__

typedef struct _LINKED_LIST_ENTRY
{
    struct _LINKED_LIST_ENTRY* Flink;
    struct _LINKED_LIST_ENTRY* Blink;
}LINKED_LIST_ENTRY, *PLINKED_LIST_ENTRY;

#endif //__LINKED_LIST_ENTRY_H__