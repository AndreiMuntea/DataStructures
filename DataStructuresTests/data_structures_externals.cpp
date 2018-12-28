#include "..\DataStructuresLibrary\data_structures_externals.h"
#include "..\DataStructuresLibrary\\common_definitios.h"

#include "stdafx.h"
#include <malloc.h>
#include <Windows.h>
#include <assert.h>

void*
DS_Alloc(
    unsigned int Size,
    unsigned long Tag
)
{
    UNREFERENCED_PARAMETER(Tag);
    return malloc(Size);
}

void
DS_Free(
    void* Block,
    unsigned long Tag
)
{
    UNREFERENCED_PARAMETER(Tag);
    free(Block);
}

void
DS_Assert(
    int Condition
)
{
    assert(Condition);
}

void
Ds_ZeroMemory(
    void* Block,
    unsigned int Size
)
{
    ZeroMemory(Block, Size);
}
