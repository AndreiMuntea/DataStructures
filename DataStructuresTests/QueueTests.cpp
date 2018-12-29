#include "stdafx.h"
#include "CppUnitTest.h"
#include "leaks_check.h"

#include "..\DataStructuresLibrary\Queue.h"
#include "..\DataStructuresLibrary\Queue.c"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#define QUEUE_ELMENT_TEST_TAG   'TEQ#'  //#QET

namespace DataStructuresTests
{
    typedef struct _QUEUE_ELEMENT_TEST
    {
        char* buffer;
    }QUEUE_ELEMENT_TEST, *PQUEUE_ELEMENT_TEST;

    VOID QueueFreeRoutine(PVOID Element)
    {
        PQUEUE_ELEMENT_TEST element = (PQUEUE_ELEMENT_TEST)(Element);
        DS_Free(element->buffer, QUEUE_ELMENT_TEST_TAG);
        DS_Free(element, QUEUE_ELMENT_TEST_TAG);
    }

    VOID InitializeQueueElement(PQUEUE_ELEMENT_TEST Element, unsigned int NoBytes)
    {
        Element->buffer = (char*)DS_Alloc(NoBytes, QUEUE_ELMENT_TEST_TAG);
        DS_Assert(Element->buffer != NULL);
        Ds_ZeroMemory(Element->buffer, NoBytes);
    }

    PQUEUE_ELEMENT_TEST AllocQueueElement()
    {
        PQUEUE_ELEMENT_TEST element = (PQUEUE_ELEMENT_TEST)DS_Alloc(sizeof(QUEUE_ELEMENT_TEST), QUEUE_ELMENT_TEST_TAG);
        DS_Assert(element != NULL);

        InitializeQueueElement(element, 700);
        return element;
    }

    TEST_CLASS(QueueTests)
    {
    public:
        TEST_METHOD(QueueInitializeTest)
        {
            QUEUE queue = { 0 };
            
            QueueInitialize(&queue, &QueueFreeRoutine);
            Assert::IsTrue(queue.QueueFreeRoutine == &QueueFreeRoutine);
        }

        TEST_METHOD(QueueDestroyTest)
        {
            QUEUE queue = { 0 };

            QueueInitialize(&queue, &QueueFreeRoutine);
            Assert::IsTrue(queue.QueueFreeRoutine == &QueueFreeRoutine);

            QueueDestroy(&queue);
            Assert::IsTrue(queue.QueueFreeRoutine == NULL);
        }

        TEST_METHOD(QueuePush1Test)
        {
            CrtCheckMemory __leaks;

            QUEUE queue = { 0 };

            QueueInitialize(&queue, &QueueFreeRoutine);
            Assert::IsTrue(queue.QueueFreeRoutine == &QueueFreeRoutine);

            PushN(&queue, 1);

            QueueDestroy(&queue);
            Assert::IsTrue(queue.QueueFreeRoutine == NULL);
        }

        TEST_METHOD(QueuePush1000Test)
        {
            CrtCheckMemory __leaks;

            QUEUE queue = { 0 };

            QueueInitialize(&queue, &QueueFreeRoutine);
            Assert::IsTrue(queue.QueueFreeRoutine == &QueueFreeRoutine);

            PushN(&queue, 1000);

            QueueDestroy(&queue);
            Assert::IsTrue(queue.QueueFreeRoutine == NULL);
        }

        TEST_METHOD(QueuePopEmptyTest)
        {
            CrtCheckMemory __leaks;

            QUEUE queue = { 0 };

            QueueInitialize(&queue, &QueueFreeRoutine);
            Assert::IsTrue(queue.QueueFreeRoutine == &QueueFreeRoutine);


            PQUEUE_ELEMENT_TEST element = NULL;
            DS_STATUS status = QueuePop(&queue, (PVOID*)(&element));
            Assert::IsTrue(status == DS_EXIT_STATUS_NO_ELEMENTS_IN_LIST);

            QueueDestroy(&queue);
            Assert::IsTrue(queue.QueueFreeRoutine == NULL);
        }

        TEST_METHOD(QueuePop1Test)
        {
            CrtCheckMemory __leaks;

            QUEUE queue = { 0 };

            QueueInitialize(&queue, &QueueFreeRoutine);
            Assert::IsTrue(queue.QueueFreeRoutine == &QueueFreeRoutine);

            PushN(&queue, 1);
            PopN(&queue, 1);

            QueueDestroy(&queue);
            Assert::IsTrue(queue.QueueFreeRoutine == NULL);
        }

        TEST_METHOD(QueuePop1000Test)
        {
            CrtCheckMemory __leaks;

            QUEUE queue = { 0 };

            QueueInitialize(&queue, &QueueFreeRoutine);
            Assert::IsTrue(queue.QueueFreeRoutine == &QueueFreeRoutine);

            PushN(&queue, 1000);
            PopN(&queue, 1000);

            QueueDestroy(&queue);
            Assert::IsTrue(queue.QueueFreeRoutine == NULL);
        }

        TEST_METHOD(QueuePush1000Pop500Test)
        {
            CrtCheckMemory __leaks;

            QUEUE queue = { 0 };

            QueueInitialize(&queue, &QueueFreeRoutine);
            Assert::IsTrue(queue.QueueFreeRoutine == &QueueFreeRoutine);

            PushN(&queue, 1000);
            PopN(&queue, 500);

            QueueDestroy(&queue);
            Assert::IsTrue(queue.QueueFreeRoutine == NULL);
        }

        void PushN(PQUEUE Queue, unsigned int N)
        {
            while (N--)
            {
                PQUEUE_ELEMENT_TEST element = AllocQueueElement();
                DS_STATUS status = QueuePush(Queue, (PVOID)element);
                Assert::IsTrue(DS_STATUS_SUCCESS(status));
            }
        }

        void PopN(PQUEUE Queue, unsigned int N)
        {
            while (N--)
            {
                PQUEUE_ELEMENT_TEST element = NULL;
                DS_STATUS status = QueuePop(Queue, (PVOID*)(&element));
                Assert::IsTrue(DS_STATUS_SUCCESS(status));

                QueueFreeRoutine((PVOID)(element));
            }
        }
    };
};