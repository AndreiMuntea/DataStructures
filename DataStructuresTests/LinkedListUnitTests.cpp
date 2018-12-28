#include "stdafx.h"
#include "CppUnitTest.h"

#include "..\DataStructuresLibrary\LinkedList.h"
#include "..\DataStructuresLibrary\LinkedList.c"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace DataStructuresTests
{		
    typedef struct _LINKED_LIST_ENTRY_TEST
    {
        LINKED_LIST_ENTRY   Entry;
        size_t              Value;
    }LINKED_LIST_ENTRY_TEST, *PLINKED_LIST_ENTRY_TEST;

	TEST_CLASS(LinkedListTests)
	{
	public:
		TEST_METHOD(LinkedListInitializeTest)
		{
            LINKED_LIST list = { 0 };
            LinkedListInitialize(&list);

            Assert::IsTrue(list.Base.Blink == &list.Base);
            Assert::IsTrue(list.Base.Flink == &list.Base);
		}

        TEST_METHOD(LinkedListIsListEmptyTrueTest)
        {
            LINKED_LIST list = { 0 };
            LinkedListInitialize(&list);

            Assert::IsTrue(TRUE == LinkedListIsEmpty(&list));
        }

        TEST_METHOD(LinkedListIsListEmptyFalseTest)
        {
            LINKED_LIST list = { 0 };
            LinkedListInitialize(&list);

            LINKED_LIST_ENTRY_TEST entry1 = { 0 };
            auto status = LinkedListInsertTail(&list, &entry1.Entry);
            Assert::IsTrue(DS_STATUS_SUCCESS(status));

            Assert::IsTrue(FALSE == LinkedListIsEmpty(&list));
        }

        TEST_METHOD(LinkedListIsCorruptedFlinkAndBlinkTest)
        {
            LINKED_LIST list = { 0 };
            LINKED_LIST_ENTRY_TEST entry1 = { 0 };
            LINKED_LIST_ENTRY_TEST entry2 = { 0 };
            LINKED_LIST_ENTRY_TEST entry3 = { 0 };
            LINKED_LIST_ENTRY_TEST entry4 = { 0 };

            LinkedListInitialize(&list);

            auto status = LinkedListInsertTail(&list, &entry1.Entry);
            Assert::IsTrue(DS_STATUS_SUCCESS(status));
            Assert::IsFalse(TRUE == LinkedListIsCorrupted(&list));

            status = LinkedListInsertTail(&list, &entry2.Entry);
            Assert::IsTrue(DS_STATUS_SUCCESS(status));
            Assert::IsFalse(TRUE == LinkedListIsCorrupted(&list));

            status = LinkedListInsertTail(&list, &entry3.Entry);
            Assert::IsTrue(DS_STATUS_SUCCESS(status));
            Assert::IsFalse(TRUE == LinkedListIsCorrupted(&list));

            status = LinkedListInsertTail(&list, &entry4.Entry);
            Assert::IsTrue(DS_STATUS_SUCCESS(status));
            Assert::IsFalse(TRUE == LinkedListIsCorrupted(&list));

            entry1.Entry.Blink = &entry4.Entry;
            entry1.Entry.Flink = &entry3.Entry;

            Assert::IsTrue(TRUE == LinkedListIsCorrupted(&list));
        }

        TEST_METHOD(LinkedListIsCorruptedFlinkTest)
        {
            LINKED_LIST list = { 0 };
            LINKED_LIST_ENTRY_TEST entry1 = { 0 };
            LINKED_LIST_ENTRY_TEST entry2 = { 0 };
            LINKED_LIST_ENTRY_TEST entry3 = { 0 };
            LINKED_LIST_ENTRY_TEST entry4 = { 0 };

            LinkedListInitialize(&list);

            auto status = LinkedListInsertTail(&list, &entry1.Entry);
            Assert::IsTrue(DS_STATUS_SUCCESS(status));
            Assert::IsFalse(TRUE == LinkedListIsCorrupted(&list));

            status = LinkedListInsertTail(&list, &entry2.Entry);
            Assert::IsTrue(DS_STATUS_SUCCESS(status));
            Assert::IsFalse(TRUE == LinkedListIsCorrupted(&list));

            status = LinkedListInsertTail(&list, &entry3.Entry);
            Assert::IsTrue(DS_STATUS_SUCCESS(status));
            Assert::IsFalse(TRUE == LinkedListIsCorrupted(&list));

            status = LinkedListInsertTail(&list, &entry4.Entry);
            Assert::IsTrue(DS_STATUS_SUCCESS(status));
            Assert::IsFalse(TRUE == LinkedListIsCorrupted(&list));

            entry1.Entry.Flink = &entry4.Entry;

            Assert::IsTrue(TRUE == LinkedListIsCorrupted(&list));
        }

        TEST_METHOD(LinkedListIsCorruptedBlinkTest)
        {
            LINKED_LIST list = { 0 };
            LINKED_LIST_ENTRY_TEST entry1 = { 0 };
            LINKED_LIST_ENTRY_TEST entry2 = { 0 };
            LINKED_LIST_ENTRY_TEST entry3 = { 0 };
            LINKED_LIST_ENTRY_TEST entry4 = { 0 };

            LinkedListInitialize(&list);

            auto status = LinkedListInsertTail(&list, &entry1.Entry);
            Assert::IsTrue(DS_STATUS_SUCCESS(status));
            Assert::IsFalse(TRUE == LinkedListIsCorrupted(&list));

            status = LinkedListInsertTail(&list, &entry2.Entry);
            Assert::IsTrue(DS_STATUS_SUCCESS(status));
            Assert::IsFalse(TRUE == LinkedListIsCorrupted(&list));

            status = LinkedListInsertTail(&list, &entry3.Entry);
            Assert::IsTrue(DS_STATUS_SUCCESS(status));
            Assert::IsFalse(TRUE == LinkedListIsCorrupted(&list));

            status = LinkedListInsertTail(&list, &entry4.Entry);
            Assert::IsTrue(DS_STATUS_SUCCESS(status));
            Assert::IsFalse(TRUE == LinkedListIsCorrupted(&list));

            entry1.Entry.Blink = &entry3.Entry;

            Assert::IsTrue(TRUE == LinkedListIsCorrupted(&list));
        }

        TEST_METHOD(LinkedListInsertTail1Test)
        {
            LINKED_LIST list = { 0 };
            LINKED_LIST_ENTRY_TEST entry1 = { 0 };

            LinkedListInitialize(&list);
            
            auto status = LinkedListInsertTail(&list, &entry1.Entry);
            Assert::IsTrue(DS_STATUS_SUCCESS(status));

            Assert::IsTrue(entry1.Entry.Flink == &list.Base);
            Assert::IsTrue(entry1.Entry.Blink == &list.Base);
            Assert::IsTrue(&entry1.Entry == list.Base.Blink);
            Assert::IsTrue(&entry1.Entry == list.Base.Flink);
            Assert::IsFalse(TRUE == LinkedListIsCorrupted(&list));
        }

        TEST_METHOD(LinkedListInsertTail2Test)
        {
            LINKED_LIST list = { 0 };
            LINKED_LIST_ENTRY_TEST entry1 = { 0 };
            LINKED_LIST_ENTRY_TEST entry2 = { 0 };

            LinkedListInitialize(&list);

            auto status = LinkedListInsertTail(&list, &entry1.Entry);
            Assert::IsTrue(DS_STATUS_SUCCESS(status));
            Assert::IsFalse(TRUE == LinkedListIsCorrupted(&list));

            status = LinkedListInsertTail(&list, &entry2.Entry);
            Assert::IsTrue(DS_STATUS_SUCCESS(status));
            Assert::IsFalse(TRUE == LinkedListIsCorrupted(&list));

            Assert::IsTrue(entry1.Entry.Flink == &entry2.Entry);
            Assert::IsTrue(entry1.Entry.Blink == &list.Base);

            Assert::IsTrue(entry2.Entry.Flink == &list.Base);
            Assert::IsTrue(entry2.Entry.Blink == &entry1.Entry);

            Assert::IsTrue(list.Base.Flink == &entry1.Entry);
            Assert::IsTrue(list.Base.Blink == &entry2.Entry);
        }

        TEST_METHOD(LinkedListInsertTail500Test)
        {
            LINKED_LIST list = { 0 };
            LINKED_LIST_ENTRY_TEST entries[500] = { 0 };

            LinkedListInitialize(&list);

            for (size_t i = 0; i < 500; ++i)
            {
                auto status = LinkedListInsertTail(&list, &entries[i].Entry);
                Assert::IsTrue(DS_STATUS_SUCCESS(status));
                Assert::IsFalse(TRUE == LinkedListIsCorrupted(&list));
            }

            PLINKED_LIST_ENTRY current = list.Base.Flink;
            Assert::IsTrue(&entries[0].Entry == current);

            for (size_t i = 1; i < 499; ++i)
            {
                current = current->Flink;

                Assert::IsTrue(&entries[i].Entry == current);
                Assert::IsTrue(entries[i].Entry.Blink == &entries[i - 1].Entry);
                Assert::IsTrue(entries[i].Entry.Flink == &entries[i + 1].Entry);
            }

            Assert::IsTrue(&entries[499].Entry == current->Flink);

            Assert::IsTrue(entries[0].Entry.Blink == &list.Base);
            Assert::IsTrue(entries[499].Entry.Flink == &list.Base);

            Assert::IsTrue(list.Base.Flink == &entries[0].Entry);
            Assert::IsTrue(list.Base.Blink == &entries[499].Entry);
        }

        TEST_METHOD(LinkedListInsertHead1Test)
        {
            LINKED_LIST list = { 0 };
            LINKED_LIST_ENTRY_TEST entry1 = { 0 };

            LinkedListInitialize(&list);

            auto status = LinkedListInsertHead(&list, &entry1.Entry);
            Assert::IsFalse(TRUE == LinkedListIsCorrupted(&list));

            Assert::IsTrue(entry1.Entry.Flink == &list.Base);
            Assert::IsTrue(entry1.Entry.Blink == &list.Base);
            Assert::IsTrue(&entry1.Entry == list.Base.Blink);
            Assert::IsTrue(&entry1.Entry == list.Base.Flink);
            Assert::IsFalse(TRUE == LinkedListIsCorrupted(&list));
        }

        TEST_METHOD(LinkedListInsertHead2Test)
        {
            LINKED_LIST list = { 0 };
            LINKED_LIST_ENTRY_TEST entry1 = { 0 };
            LINKED_LIST_ENTRY_TEST entry2 = { 0 };

            LinkedListInitialize(&list);

            auto status = LinkedListInsertHead(&list, &entry1.Entry);
            Assert::IsTrue(DS_STATUS_SUCCESS(status));
            Assert::IsFalse(TRUE == LinkedListIsCorrupted(&list));

            status = LinkedListInsertHead(&list, &entry2.Entry);
            Assert::IsTrue(DS_STATUS_SUCCESS(status));
            Assert::IsFalse(TRUE == LinkedListIsCorrupted(&list));

            Assert::IsTrue(entry1.Entry.Flink == &list.Base);
            Assert::IsTrue(entry1.Entry.Blink == &entry2.Entry);

            Assert::IsTrue(entry2.Entry.Flink == &entry1.Entry);
            Assert::IsTrue(entry2.Entry.Blink == &list.Base);

            Assert::IsTrue(list.Base.Flink == &entry2.Entry);
            Assert::IsTrue(list.Base.Blink == &entry1.Entry);
        }

        TEST_METHOD(LinkedListInsertHead500Test)
        {
            LINKED_LIST list = { 0 };
            LINKED_LIST_ENTRY_TEST entries[500] = { 0 };

            LinkedListInitialize(&list);

            for (size_t i = 0; i < 500; ++i)
            {
                auto status = LinkedListInsertHead(&list, &entries[i].Entry);
                Assert::IsTrue(DS_STATUS_SUCCESS(status));
                Assert::IsFalse(TRUE == LinkedListIsCorrupted(&list));
            }

            PLINKED_LIST_ENTRY current = list.Base.Flink;
            Assert::IsTrue(&entries[499].Entry == current);

            for (size_t i = 498; i > 0; --i)
            {
                current = current->Flink;
                Assert::IsTrue(entries[i].Entry.Blink == &entries[i + 1].Entry);
                Assert::IsTrue(entries[i].Entry.Flink == &entries[i - 1].Entry);
            }

            Assert::IsTrue(&entries[0].Entry == current->Flink);

            Assert::IsTrue(entries[0].Entry.Flink == &list.Base);
            Assert::IsTrue(entries[499].Entry.Blink == &list.Base);

            Assert::IsTrue(list.Base.Flink == &entries[499].Entry);
            Assert::IsTrue(list.Base.Blink == &entries[0].Entry);
        }

        TEST_METHOD(LinkedListRemoveTailEmptyListTest)
        {
            LINKED_LIST list = { 0 };
            PLINKED_LIST_ENTRY entry = NULL;

            LinkedListInitialize(&list);

            auto status = LinkedListRemoveTail(&list, &entry);
            Assert::IsTrue(status == EXIT_STATUS_NO_ELEMENTS_IN_LIST);
            Assert::IsFalse(TRUE == LinkedListIsCorrupted(&list));
        }

        TEST_METHOD(LinkedListRemoveTail1Test)
        {
            LINKED_LIST list = { 0 };
            PLINKED_LIST_ENTRY entry = NULL;
            LINKED_LIST_ENTRY_TEST entry1 = { 0 };

            LinkedListInitialize(&list);


            auto status = LinkedListInsertTail(&list, &entry1.Entry);
            Assert::IsTrue(DS_STATUS_SUCCESS(status));
            Assert::IsFalse(TRUE == LinkedListIsCorrupted(&list));

            status = LinkedListRemoveTail(&list, &entry);
            Assert::IsTrue(DS_STATUS_SUCCESS(status));
            Assert::IsFalse(TRUE == LinkedListIsCorrupted(&list));

            status = LinkedListRemoveTail(&list, &entry);
            Assert::IsTrue(status == EXIT_STATUS_NO_ELEMENTS_IN_LIST);
            Assert::IsFalse(TRUE == LinkedListIsCorrupted(&list));
        }

        TEST_METHOD(LinkedListRemoveTail2Test)
        {
            LINKED_LIST list = { 0 };
            PLINKED_LIST_ENTRY entry = NULL;
            LINKED_LIST_ENTRY_TEST entry1 = { 0 }; entry1.Value = 1;
            LINKED_LIST_ENTRY_TEST entry2 = { 0 }; entry2.Value = 2;

            LinkedListInitialize(&list);

            auto status = LinkedListInsertTail(&list, &entry1.Entry);
            Assert::IsTrue(DS_STATUS_SUCCESS(status));
            Assert::IsFalse(TRUE == LinkedListIsCorrupted(&list));

            status = LinkedListInsertHead(&list, &entry2.Entry);
            Assert::IsTrue(DS_STATUS_SUCCESS(status));
            Assert::IsFalse(TRUE == LinkedListIsCorrupted(&list));

            status = LinkedListRemoveTail(&list, &entry);
            Assert::IsTrue(DS_STATUS_SUCCESS(status));
            Assert::IsTrue(((PLINKED_LIST_ENTRY_TEST)(entry))->Value == 1);
            Assert::IsFalse(TRUE == LinkedListIsCorrupted(&list));

            status = LinkedListRemoveTail(&list, &entry);
            Assert::IsTrue(DS_STATUS_SUCCESS(status));
            Assert::IsTrue(((PLINKED_LIST_ENTRY_TEST)(entry))->Value == 2);
            Assert::IsFalse(TRUE == LinkedListIsCorrupted(&list));

            status = LinkedListRemoveTail(&list, &entry);
            Assert::IsTrue(status == EXIT_STATUS_NO_ELEMENTS_IN_LIST);
            Assert::IsFalse(TRUE == LinkedListIsCorrupted(&list));
        }

        TEST_METHOD(LinkedListRemoveTail500Test)
        {
            LINKED_LIST list = { 0 };
            LINKED_LIST_ENTRY_TEST entries[500] = { 0 };
            PLINKED_LIST_ENTRY entry = NULL;

            LinkedListInitialize(&list);

            for (size_t i = 0; i < 500; ++i)
            {
                entries[i].Value = i + 1;
                auto status = LinkedListInsertTail(&list, &entries[i].Entry);
                Assert::IsTrue(DS_STATUS_SUCCESS(status));
                Assert::IsFalse(TRUE == LinkedListIsCorrupted(&list));
            }

            size_t current = 500;
            while (!LinkedListIsEmpty(&list))
            {
                auto status = LinkedListRemoveTail(&list, &entry);
                Assert::IsTrue(DS_STATUS_SUCCESS(status));
                Assert::IsTrue(((PLINKED_LIST_ENTRY_TEST)(entry))->Value == current);
                Assert::IsFalse(TRUE == LinkedListIsCorrupted(&list));

                current--;
            }

            auto status = LinkedListRemoveTail(&list, &entry);
            Assert::IsTrue(status == EXIT_STATUS_NO_ELEMENTS_IN_LIST);
            Assert::IsFalse(TRUE == LinkedListIsCorrupted(&list));
        }

        TEST_METHOD(LinkedListRemoveHeadEmptyListTest)
        {
            LINKED_LIST list = { 0 };
            PLINKED_LIST_ENTRY entry = NULL;

            LinkedListInitialize(&list);

            auto status = LinkedListRemoveHead(&list, &entry);
            Assert::IsTrue(status == EXIT_STATUS_NO_ELEMENTS_IN_LIST);
            Assert::IsFalse(TRUE == LinkedListIsCorrupted(&list));
        }

        TEST_METHOD(LinkedListRemoveHead1Test)
        {
            LINKED_LIST list = { 0 };
            PLINKED_LIST_ENTRY entry = NULL;
            LINKED_LIST_ENTRY_TEST entry1 = { 0 };

            LinkedListInitialize(&list);

            auto status = LinkedListInsertTail(&list, &entry1.Entry);
            Assert::IsTrue(DS_STATUS_SUCCESS(status));
            Assert::IsFalse(TRUE == LinkedListIsCorrupted(&list));

            status = LinkedListRemoveHead(&list, &entry);
            Assert::IsTrue(DS_STATUS_SUCCESS(status));
            Assert::IsFalse(TRUE == LinkedListIsCorrupted(&list));

            status = LinkedListRemoveHead(&list, &entry);
            Assert::IsTrue(status == EXIT_STATUS_NO_ELEMENTS_IN_LIST);
            Assert::IsFalse(TRUE == LinkedListIsCorrupted(&list));
        }

        TEST_METHOD(LinkedListRemoveHead2Test)
        {
            LINKED_LIST list = { 0 };
            PLINKED_LIST_ENTRY entry = NULL;
            LINKED_LIST_ENTRY_TEST entry1 = { 0 }; entry1.Value = 1;
            LINKED_LIST_ENTRY_TEST entry2 = { 0 }; entry2.Value = 2;

            LinkedListInitialize(&list);

            auto status = LinkedListInsertTail(&list, &entry1.Entry);
            Assert::IsTrue(DS_STATUS_SUCCESS(status));
            Assert::IsFalse(TRUE == LinkedListIsCorrupted(&list));

            status = LinkedListInsertHead(&list, &entry2.Entry);
            Assert::IsTrue(DS_STATUS_SUCCESS(status));
            Assert::IsFalse(TRUE == LinkedListIsCorrupted(&list));

            status = LinkedListRemoveHead(&list, &entry);
            Assert::IsTrue(DS_STATUS_SUCCESS(status));
            Assert::IsTrue(((PLINKED_LIST_ENTRY_TEST)(entry))->Value == 2);
            Assert::IsFalse(TRUE == LinkedListIsCorrupted(&list));

            status = LinkedListRemoveHead(&list, &entry);
            Assert::IsTrue(DS_STATUS_SUCCESS(status));
            Assert::IsTrue(((PLINKED_LIST_ENTRY_TEST)(entry))->Value == 1);
            Assert::IsFalse(TRUE == LinkedListIsCorrupted(&list));

            status = LinkedListRemoveHead(&list, &entry);
            Assert::IsTrue(status == EXIT_STATUS_NO_ELEMENTS_IN_LIST);
            Assert::IsFalse(TRUE == LinkedListIsCorrupted(&list));
        }

        TEST_METHOD(LinkedListRemoveHead500Test)
        {
            LINKED_LIST list = { 0 };
            LINKED_LIST_ENTRY_TEST entries[500] = { 0 };
            PLINKED_LIST_ENTRY entry = NULL;

            LinkedListInitialize(&list);

            for (size_t i = 0; i < 500; ++i)
            {
                entries[i].Value = i + 1;
                auto status = LinkedListInsertTail(&list, &entries[i].Entry);
                Assert::IsTrue(DS_STATUS_SUCCESS(status));
                Assert::IsFalse(TRUE == LinkedListIsCorrupted(&list));
            }

            size_t current = 1;
            while (!LinkedListIsEmpty(&list))
            {
                auto status = LinkedListRemoveHead(&list, &entry);
                Assert::IsTrue(DS_STATUS_SUCCESS(status));
                Assert::IsTrue(((PLINKED_LIST_ENTRY_TEST)(entry))->Value == current);
                Assert::IsFalse(TRUE == LinkedListIsCorrupted(&list));

                current++;
            }

            auto status = LinkedListRemoveHead(&list, &entry);
            Assert::IsTrue(status == EXIT_STATUS_NO_ELEMENTS_IN_LIST);
            Assert::IsFalse(TRUE == LinkedListIsCorrupted(&list));
        }
	};
}