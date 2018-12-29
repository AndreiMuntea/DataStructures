#ifndef __DATA_STRUCTURES_COMMON_DEFINITIONS_H__ 
#define __DATA_STRUCTURES_COMMON_DEFINITIONS_H__ 

#include <sal.h>

#include "data_structures_status.h"

typedef unsigned char       BYTE,  *PBYTE;
typedef unsigned short      WORD,  *PWORD;
typedef unsigned long       DWORD, *PDWORD;
typedef unsigned __int64    QWORD, *PQWORD;

typedef signed char         INT8,  *PINT8;
typedef signed short        INT16, *PINT16;
typedef signed int          INT32, *PINT32;
typedef signed __int64      INT64, *PINT64;

typedef unsigned char       UINT8,  *PUINT8;
typedef unsigned short      UINT16, *PUINT16;
typedef unsigned int        UINT32, *PUINT32;
typedef unsigned __int64    UINT64, *PUINT64;
         
typedef UINT8               BOOLEAN, *PBOOLEAN;
typedef int                 BOOL, *PBOOL;

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

#endif //__DATA_STRUCTURES_COMMON_DEFINITIONS_H__