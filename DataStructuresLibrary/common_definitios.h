#ifndef __COMMON_DEFINITIONS_H__ 
#define __COMMON_DEFINITIONS_H__ 

#include <sal.h>

#include "data_structures_status.h"

typedef unsigned __int8     BYTE,  *PBYTE;
typedef unsigned __int16    WORD,  *PWORD;
typedef unsigned __int32    DWORD, *PDWORD;
typedef unsigned __int64    QWORD, *PQWORD;

typedef signed __int8       INT8,  *PINT8;
typedef signed __int16      INT16, *PINT16;
typedef signed __int32      INT32, *PINT32;
typedef signed __int64      INT64, *PINT64;

typedef unsigned __int8     UINT8,  *PUINT8;
typedef unsigned __int16    UINT16, *PUINT16;
typedef unsigned __int32    UINT32, *PUINT32;
typedef unsigned __int64    UINT64, *PUINT64;

typedef unsigned __int8     BOOL, *PBOOL;
typedef unsigned __int8     BOOLEAN, *PBOOLEAN;

typedef void                *PVOID;

#ifndef VOID 
#define VOID    void 
#endif

#ifndef NULL
#define NULL    0
#endif

#ifndef FALSE
#define FALSE   0
#endif

#ifndef TRUE
#define TRUE    1
#endif

#ifndef UNREFERENCED_PARAMETER
#define UNREFERENCED_PARAMETER(x) ((void)(x))
#endif

#endif //__COMMON_DEFINITIONS_H__