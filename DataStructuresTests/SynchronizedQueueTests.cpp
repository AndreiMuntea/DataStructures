#include "stdafx.h"
#include "CppUnitTest.h"
#include "leaks_check.h"

#include "..\DataStructuresLibrary\SynchronizedQueue.h"
#include "..\DataStructuresLibrary\SynchronizedQueue.c"

#include <Windows.h>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#define QUEUE_ELMENT_TEST_TAG   'TEQ#'  //#QET

namespace DataStructuresTests
{
    VOID SynchronizedQueueFreeRoutine(PVOID Element)
    {
        UNREFERENCED_PARAMETER(Element);
    }

    typedef struct _SYNCHRONIZED_QUEUE_THREAD_CONTEXT
    {
        PSYNCHRONIZED_QUEUE Queue;
        volatile long long* Elements;
        unsigned int        NoElements;
    }SYNCHRONIZED_QUEUE_THREAD_CONTEXT, *PSYNCHRONIZED_QUEUE_THREAD_CONTEXT;


    DWORD WINAPI SynchronizedQueueTestThreadPushRoutine(PVOID Context)
    {
        PSYNCHRONIZED_QUEUE_THREAD_CONTEXT context = (PSYNCHRONIZED_QUEUE_THREAD_CONTEXT)(Context);
        for (unsigned int i = 0; i < context->NoElements; ++i)
        {
            DS_STATUS status = SynchronizedQueuePush(context->Queue, (PVOID)(&context->Elements[i]));
            Assert::IsTrue(DS_STATUS_SUCCESS(status));
        }

        return 0;
    }

    DWORD WINAPI SynchronizedQueueTestThreadPopRoutine(PVOID Context)
    {
        PSYNCHRONIZED_QUEUE_THREAD_CONTEXT context = (PSYNCHRONIZED_QUEUE_THREAD_CONTEXT)(Context);
        
        for (unsigned int i = 0; i < context->NoElements; ++i)
        {
            int* element = NULL;
            
            DS_STATUS status = SynchronizedQueuePop(context->Queue, (PVOID*)&element);
            Assert::IsTrue(DS_STATUS_SUCCESS(status));

            InterlockedIncrement64(&context->Elements[*element]);
        }

        return 0;
    }

    TEST_CLASS(SynchronizedQueueTests)
    {
    public:
        TEST_METHOD(QueuePushPop500From8ThreadsTest)
        {
            CrtCheckMemory __leaks;

            SYNCHRONIZED_QUEUE queue = { 0 };
            SynchronizedQueueInitialize(&queue, &SynchronizedQueueFreeRoutine);
            
            const unsigned int noThreads = 8;
            const unsigned int noElements = 500;
            const unsigned int modulo = 10;

            volatile long long Elements[noThreads * noElements] = { 0 };
            volatile long long Check[modulo] = { 0 };

            for (unsigned int i = 0; i < noThreads * noElements; ++i) Elements[i] = i % modulo;

            PushNFromXThreads(&queue, noElements, noThreads, Elements);
            PopNFromXThreads(&queue, noElements, noThreads, Check);
            CheckResult((noElements* noThreads) / modulo, Check, modulo);

            SynchronizedQueueDestroy(&queue);
        }

        TEST_METHOD(QueuePushPop500From2ThreadsTest)
        {
            CrtCheckMemory __leaks;

            SYNCHRONIZED_QUEUE queue = { 0 };
            SynchronizedQueueInitialize(&queue, &SynchronizedQueueFreeRoutine);

            const unsigned int noThreads = 2;
            const unsigned int noElements = 500;
            const unsigned int modulo = 10;

            volatile long long Elements[noThreads * noElements] = { 0 };
            volatile long long Check[modulo] = { 0 };

            for (unsigned int i = 0; i < noThreads * noElements; ++i) Elements[i] = i % modulo;

            PushNFromXThreads(&queue, noElements, noThreads, Elements);
            PopNFromXThreads(&queue, noElements, noThreads, Check);
            CheckResult((noElements* noThreads) / modulo, Check, modulo);

            SynchronizedQueueDestroy(&queue);
        }

        TEST_METHOD(QueuePushPop6000From16ThreadsTest)
        {
            CrtCheckMemory __leaks;

            SYNCHRONIZED_QUEUE queue = { 0 };
            SynchronizedQueueInitialize(&queue, &SynchronizedQueueFreeRoutine);

            const unsigned int noThreads = 16;
            const unsigned int noElements = 6000;
            const unsigned int modulo = 10;

            volatile long long Elements[noThreads * noElements] = { 0 };
            volatile long long Check[modulo] = { 0 };

            for (unsigned int i = 0; i < noThreads * noElements; ++i) Elements[i] = i % modulo;

            PushNFromXThreads(&queue, noElements, noThreads, Elements);
            PopNFromXThreads(&queue, noElements, noThreads, Check);
            CheckResult((noElements* noThreads) / modulo, Check, modulo);

            SynchronizedQueueDestroy(&queue);
        }

        TEST_METHOD(QueuePushPop1000000From32ThreadsTest)
        {
            CrtCheckMemory __leaks;

            SYNCHRONIZED_QUEUE queue = { 0 };
            SynchronizedQueueInitialize(&queue, &SynchronizedQueueFreeRoutine);

            const unsigned int noThreads = 32;
            const unsigned int noElements = 1000000;
            const unsigned int modulo = 10;

            volatile long long* Elements = (volatile long long*)DS_Alloc(noThreads * noElements * sizeof(volatile long long), QUEUE_ELMENT_TEST_TAG);
            volatile long long Check[modulo] = { 0 };

            Assert::IsTrue(Elements != NULL);
            for (unsigned int i = 0; i < noThreads * noElements; ++i) Elements[i] = i % modulo;

            PushNFromXThreads(&queue, noElements, noThreads, Elements);
            PopNFromXThreads(&queue, noElements, noThreads, Check);
            CheckResult((noElements* noThreads) / modulo, Check, modulo);

            SynchronizedQueueDestroy(&queue);
            DS_Free((void*)Elements, QUEUE_ELMENT_TEST_TAG);
        }

        void PushNFromXThreads(PSYNCHRONIZED_QUEUE Queue, unsigned int N, unsigned int X, volatile long long* Elements)
        {
            PushPopNFromXThreads(Queue, N, X, Elements, SynchronizedQueueTestThreadPushRoutine);
        }

        void PopNFromXThreads(PSYNCHRONIZED_QUEUE Queue, unsigned int N, unsigned int X, volatile long long* Elements)
        {
            PushPopNFromXThreads(Queue, N, X, Elements, SynchronizedQueueTestThreadPopRoutine);
        }

        void CheckResult(int ExpectedCount, volatile long long* Elements, unsigned int ElementsCount)
        {
            for (unsigned int i = 0; i < ElementsCount; ++i)
            {
                Assert::IsTrue(Elements[i] == ExpectedCount);
            }
        }

        void PushPopNFromXThreads(PSYNCHRONIZED_QUEUE Queue, unsigned int N, unsigned int X, volatile long long* Elements, LPTHREAD_START_ROUTINE Routine)
        {
            PSYNCHRONIZED_QUEUE_THREAD_CONTEXT contexts = NULL;
            HANDLE* threads = NULL;
            bool shouldFail = true;

            contexts = (PSYNCHRONIZED_QUEUE_THREAD_CONTEXT)DS_Alloc(X * sizeof(_SYNCHRONIZED_QUEUE_THREAD_CONTEXT), QUEUE_ELMENT_TEST_TAG);
            if (NULL == contexts)
            {
                goto CleanUp;
            }

            threads = (HANDLE*)DS_Alloc(X * sizeof(HANDLE), QUEUE_ELMENT_TEST_TAG);
            if (NULL == threads)
            {
                goto CleanUp;
            }
            Ds_ZeroMemory(threads, X * sizeof(HANDLE));

            for (unsigned int i = 0; i < X; ++i)
            {
                contexts[i].Queue = Queue;
                contexts[i].NoElements = N;
                contexts[i].Elements = Elements;

                threads[i] = CreateThread(NULL, 0, Routine, &contexts[i], 0, NULL);
                if (NULL == threads[i])
                {
                    WaitForMultipleObjects(i, threads, TRUE, INFINITE);
                    goto CleanUp;
                }
            }

            WaitForMultipleObjects(X, threads, TRUE, INFINITE);
            shouldFail = false;

        CleanUp:
            if (threads != NULL)
            {
                DS_Free(threads, QUEUE_ELMENT_TEST_TAG);
            }

            if (contexts != NULL)
            {
                DS_Free(contexts, QUEUE_ELMENT_TEST_TAG);
            }

            if (shouldFail)
            {
                Assert::Fail();
            }
        }
    };
}