#ifndef __DATA_STRUCTURES_STATUS_H__ 
#define __DATA_STRUCTURES_STATUS_H__

typedef long DS_STATUS;

#define EXIT_STATUS_SUCCES                          ((DS_STATUS)(0x0000))
#define EXIT_STATUS_NOT_ENOUGH_RESOURCES            ((DS_STATUS)(0x0001))
#define EXIT_STATUS_NO_ELEMENTS_IN_LIST             ((DS_STATUS)(0x0002))
#define EXIT_STATUS_CORRUPTED_LIST                  ((DS_STATUS)(0x0003))
#define EXIT_STATUS_CREATE_THREAD_FAILED            ((DS_STATUS)(0x0004))
#define EXIT_STATUS_SHUTDOWN_PENDING                ((DS_STATUS)(0x0005))

#define DS_STATUS_SUCCESS(status)       (status == EXIT_STATUS_SUCCES)

#endif //__DATA_STRUCTURES_STATUS_H__