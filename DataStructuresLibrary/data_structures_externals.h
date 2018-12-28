#ifndef __DATA_STRUCTURES_EXTERNAL_H__ 
#define __DATA_STRUCTURES_EXTERNAL_H__

void*
DS_Alloc(
    unsigned int Size,
    unsigned long Tag
);

void
DS_Free(
    void* Block,
    unsigned long Tag
);

void
DS_Assert(
    int Condition
);

void
Ds_ZeroMemory(
    void* Block,
    unsigned int Size
);

#endif //__DATA_STRUCTURES_EXTERNAL_H__