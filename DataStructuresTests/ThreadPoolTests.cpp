#include "stdafx.h"
#include "CppUnitTest.h"
#include "leaks_check.h"

#include "..\DataStructuresLibrary\ThreadPool.h"
#include "..\DataStructuresLibrary\ThreadPool.c"

#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace DataStructuresTests
{
    typedef struct _THREAD_POOL_TEST_CONTEXT
    {
        std::vector<int>* Input;
    }THREAD_POOL_TEST_CONTEXT, *PTHREAD_POOL_TEST_CONTEXT;


    VOID PayloadFreeRoutine(PVOID Element)
    {
        UNREFERENCED_PARAMETER(Element);
    }

    DWORD WINAPI ThreadPoolWorkerTestSortRoutine(PVOID Context)
    {
        PTHREAD_POOL_TEST_CONTEXT context = (PTHREAD_POOL_TEST_CONTEXT)Context;
        std::sort(context->Input->begin(), context->Input->end());
        
        return 0;
    }

    DWORD WINAPI ThreadPoolWorkerTestCheckRoutine(PVOID Context)
    {
        PTHREAD_POOL_TEST_CONTEXT context = (PTHREAD_POOL_TEST_CONTEXT)Context;
        for (size_t idx = 1; idx < context->Input->size(); ++idx)
        {
            Assert::IsTrue(context->Input->at(idx) >= context->Input->at(idx - 1));
        }

        return 0;
    }


    std::vector<int> RandomizeVector(size_t Size)
    {
        std::vector<int> vector(Size, 0);
        for (size_t i = 0; i < Size; ++i) vector[i] = static_cast<int>(i);
        std::random_shuffle(vector.begin(), vector.end());
        return vector;
    }

    TEST_CLASS(ThreadPoolTests)
    {
    public:
        TEST_METHOD(ThreadPool2Threads500ElementsBucketTest)
        {
            TestThreadPool(2, 500);
        }

        TEST_METHOD(ThreadPool8Threads50000ElementsBucketTest)
        {
            TestThreadPool(8, 50000);
        }

        TEST_METHOD(ThreadPool16Threads50000ElementsBucketTest)
        {
            TestThreadPool(16, 50000);
        }

        void TestThreadPool(uint8_t NoThreads, size_t NoElements)
        {
            CrtCheckMemory __leaks;
            
            std::vector<std::vector<int>> input;
            std::vector<THREAD_POOL_TEST_CONTEXT> partition;
            for (size_t i = 0; i < NoThreads; ++i)
            {
                input.push_back(RandomizeVector(NoElements));
                partition.push_back({ &input[input.size() - 1] });
            }

            THREAD_POOL threadPool = { 0 };
            DS_STATUS status = ThreadPoolInit(&threadPool, NoThreads, ThreadPoolWorkerTestSortRoutine, PayloadFreeRoutine);
            Assert::IsTrue(DS_STATUS_SUCCESS(status));

            for (auto& element : partition)
            {
                status = ThreadPoolEnqueue(&threadPool, (PVOID)(&element));
                Assert::IsTrue(DS_STATUS_SUCCESS(status));
            }

            ThreadPoolShutdown(&threadPool);

            status = ThreadPoolInit(&threadPool, NoThreads, ThreadPoolWorkerTestCheckRoutine, PayloadFreeRoutine);
            Assert::IsTrue(DS_STATUS_SUCCESS(status));

            for (auto& element : partition)
            {
                status = ThreadPoolEnqueue(&threadPool, (PVOID)(&element));
                Assert::IsTrue(DS_STATUS_SUCCESS(status));
            }

            ThreadPoolShutdown(&threadPool);
        }
    };
}
