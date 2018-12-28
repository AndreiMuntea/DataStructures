#include "stdafx.h"
#include "CppUnitTest.h"
#include "leaks_check.h"

#include "..\DataStructuresLibrary\SynchronizedQueue.h"
#include "..\DataStructuresLibrary\SynchronizedQueue.c"

#include <Windows.h>
#include <vector>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace DataStructuresTests
{
    VOID SynchronizedQueueFreeRoutine(PVOID Element)
    {
        UNREFERENCED_PARAMETER(Element);
    }

    typedef struct _SYNCHRONIZED_QUEUE_THREAD_CONTEXT
    {
        PSYNCHRONIZED_QUEUE     Queue;
        std::vector<long long>* Elements;
        size_t                  NoElements;
    }SYNCHRONIZED_QUEUE_THREAD_CONTEXT, *PSYNCHRONIZED_QUEUE_THREAD_CONTEXT;


    DWORD WINAPI SynchronizedQueueTestThreadPushRoutine(PVOID Context)
    {
        auto context = (PSYNCHRONIZED_QUEUE_THREAD_CONTEXT)(Context);
        auto elements = context->Elements;

        for(size_t i = 0; i < context->NoElements; ++i)
        {
            DS_STATUS status = SynchronizedQueuePush(context->Queue, (PVOID)(&elements->at(i)));
            Assert::IsTrue(DS_STATUS_SUCCESS(status));
        }

        return 0;
    }

    DWORD WINAPI SynchronizedQueueTestThreadPopRoutine(PVOID Context)
    {
        auto context = (PSYNCHRONIZED_QUEUE_THREAD_CONTEXT)(Context);
        auto elements = context->Elements;
        long long* element = nullptr;

        for (size_t i = 0; i < context->NoElements; ++i)
        {
            auto status = SynchronizedQueuePop(context->Queue, (PVOID*)&element);
            Assert::IsTrue(DS_STATUS_SUCCESS(status));

            InterlockedIncrement64(&elements->at(*element));
        }
        return 0;
    }

    TEST_CLASS(SynchronizedQueueTests)
    {
    public:
        TEST_METHOD(QueuePushPop500From8ThreadsTest)
        {
            TestPushPop(8, 500, 10);
        }

        TEST_METHOD(QueuePushPop500From2ThreadsTest)
        {
            TestPushPop(2, 500, 10);
        }

        TEST_METHOD(QueuePushPop6000From16ThreadsTest)
        {
            TestPushPop(16, 6000, 10);
        }

        TEST_METHOD(QueuePushPop1000000From32ThreadsTest)
        {
           TestPushPop(32, 1000000, 10);
        }

        void 
        PushNFromXThreads(
            _Inout_ PSYNCHRONIZED_QUEUE Queue,
            _In_    unsigned int NoThreads,
            _In_    unsigned int NoElements,
            _Inout_ std::vector<long long>* Elements
        )
        {
            PushPopNFromXThreads(Queue, NoThreads, NoElements, Elements, SynchronizedQueueTestThreadPushRoutine);
        }

        void 
        PopNFromXThreads(
            _Inout_ PSYNCHRONIZED_QUEUE Queue,
            _In_    unsigned int NoThreads,
            _In_    unsigned int NoElements,
            _Inout_ std::vector<long long>* Elements
        )
        {
            PushPopNFromXThreads(Queue, NoThreads, NoElements, Elements, SynchronizedQueueTestThreadPopRoutine);
        }

        void 
        CheckResult(
            _In_  long long ExpectedCount, 
            _In_  const std::vector<long long>& Elements
        )
        {
            for (auto& element : Elements)
            {
                Assert::IsTrue(element == ExpectedCount);
            }
        }

        void 
        TestPushPop(
            _In_ unsigned int NoThreads, 
            _In_ unsigned int NoElements, 
            _In_ unsigned int Modulo
        )
        {
            CrtCheckMemory __leaks;

            std::vector<long long> elements(NoThreads * NoElements, 0);
            std::vector<long long> check(Modulo, 0);

            SYNCHRONIZED_QUEUE queue = { 0 };
            SynchronizedQueueInitialize(&queue, &SynchronizedQueueFreeRoutine);

            for (unsigned int i = 0; i < NoThreads * NoElements; ++i) elements[i] = i % Modulo;

            PushNFromXThreads(&queue, NoThreads, NoElements, &elements);
            PopNFromXThreads(&queue, NoThreads, NoElements, &check);
            CheckResult((NoElements * NoThreads) / Modulo, check);

            SynchronizedQueueDestroy(&queue);
        }

        void PushPopNFromXThreads(
            _Inout_ PSYNCHRONIZED_QUEUE Queue,
            _In_    unsigned int NoThreads,
            _In_    unsigned int NoElements,
            _Inout_ std::vector<long long>* Elements, 
            _In_    LPTHREAD_START_ROUTINE Routine
        )
        {
            std::vector<SYNCHRONIZED_QUEUE_THREAD_CONTEXT> contexts(NoThreads);
            std::vector<HANDLE> threads(NoThreads, nullptr);

            for (unsigned int i = 0; i < NoThreads; ++i)
            {
                contexts[i].Queue = Queue;
                contexts[i].Elements = Elements;
                contexts[i].NoElements = NoElements;

                threads[i] = CreateThread(nullptr, 0, Routine, &contexts[i], 0, nullptr);
                if (!threads[i])
                {
                    WaitForMultipleObjects(i, &threads[0], TRUE, INFINITE);
                    Assert::Fail();
                }
            }

            WaitForMultipleObjects(NoThreads, &threads[0], TRUE, INFINITE);
        }
    };
}