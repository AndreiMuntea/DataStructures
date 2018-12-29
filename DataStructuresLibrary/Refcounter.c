#include <Windows.h>
#include "Refcounter.h"
#include "data_structures_externals.h"


_Use_decl_annotations_
VOID
RefCounterInit(
    _Inout_ PREFCOUNTER                 RefCounter,
    _In_    REFCOUNTER_FREE_ROUTINE     FreeRoutine
)
{
    RefCounter->FreeRoutine = FreeRoutine;
    RefCounter->RefCounter = 1;
}

_Use_decl_annotations_
VOID
RefCounterReference(
    _Inout_ PREFCOUNTER RefCounter
)
{
    long long value = InterlockedIncrement64(&RefCounter->RefCounter);
    DS_Assert(value > 1);
}

_Use_decl_annotations_
VOID
RefCounterDereference(
    _Inout_ PREFCOUNTER RefCounter
)
{
    long long value = InterlockedDecrement64(&RefCounter->RefCounter);
    DS_Assert(value >= 0);

    if (value == 0)
    {
        RefCounter->FreeRoutine(RefCounter);
    }
}
