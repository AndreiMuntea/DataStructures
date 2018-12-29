#ifndef __REFCOUNTER_H__ 
#define __REFCOUNTER_H__

#include "common_definitios.h"

typedef VOID(*REFCOUNTER_FREE_ROUTINE)(PVOID Element);

typedef struct _REFCOUNTER
{
    volatile long long      RefCounter;
    REFCOUNTER_FREE_ROUTINE FreeRoutine;
}REFCOUNTER, *PREFCOUNTER;


VOID
RefCounterInit(
    _Inout_ PREFCOUNTER                 RefCounter,
    _In_    REFCOUNTER_FREE_ROUTINE     FreeRoutine
);

VOID
RefCounterReference(
    _Inout_ PREFCOUNTER RefCounter
);

VOID
RefCounterDereference(
    _Inout_ PREFCOUNTER RefCounter
);

#endif //__REFCOUNTER_H__