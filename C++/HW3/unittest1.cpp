#include "CppUnitTest.h"
#include "../HW3_AllPublic/Queue.h"
#include "../HW3_AllPublic/Simulate.h"

#define __TRY_LIMIT 10000000
#define __DEFAULT_Q_SIZE 5

const wchar_t* err1 = L"err1";
const wchar_t* err2 = L"err2";
const wchar_t* err3 = L"err3";
const wchar_t* err4 = L"err4";

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace HW3_UnitTest //all public version of Simulate was used for testing
{
	TEST_CLASS(UnitTestQueue)
	{
	public:

		TEST_METHOD(TestConstructor)
		{
			srand(time(NULL));
			auto funPtr1 = [this] {  Queue::Queue(-1);  };
			Assert::ExpectException<const std::invalid_argument&>(funPtr1);

			Queue myQ(rand() % 1000 + 1);
			Assert::AreEqual(0, myQ.getSize());

			auto funPtr2 = [myQ] {  myQ.peek();  };
			Assert::ExpectException<const std::underflow_error&>(funPtr2);

			Assert::AreNotEqual(0, myQ.getCapacity());
			Assert::AreNotEqual(0, myQ.getServiceTime());
			Assert::AreNotEqual(0, myQ.getSerial());
			Assert::IsTrue(myQ.getServiceTime() >= 1 && myQ.getServiceTime() <= 10);

			for (int i = 0; i < __TRY_LIMIT / 10; ++i) {
				Queue myQ(1, 20, 200);
				Assert::IsTrue(myQ.getServiceTime() >= 20 && myQ.getServiceTime() <= 200);
			}

		}

		TEST_METHOD(TestCopyConstructor)
		{
			srand(time(NULL));
			Queue myQ(__DEFAULT_Q_SIZE);
			Queue myQCopy = myQ;

			Assert::AreEqual(myQ.getCapacity(), myQCopy.getCapacity());
			Assert::AreEqual(myQ.getSize(), myQCopy.getSize());
			Assert::AreEqual(myQ.getServiceTime(), myQCopy.getServiceTime());
			Assert::IsTrue(myQCopy.isEmpty());
			Assert::IsFalse(myQCopy.isFull());

			myQCopy.enQueue(0);
			myQCopy.enQueue(1);
			myQCopy.enQueue(2);
			myQCopy.enQueue(3);
			myQCopy.enQueue(4);

			Assert::AreEqual(myQ.getCapacity(), myQCopy.getCapacity());
			Assert::AreNotEqual(myQ.getSize(), myQCopy.getSize());
			Assert::AreEqual(myQ.getServiceTime(), myQCopy.getServiceTime());
			Assert::IsFalse(myQCopy.isEmpty());
			Assert::IsTrue(myQCopy.isFull());

			Queue myQ2(myQCopy);

			Assert::AreEqual(myQ2.getCapacity(), myQCopy.getCapacity());
			Assert::AreEqual(myQ2.getSize(), myQCopy.getSize());
			Assert::AreEqual(myQ2.getServiceTime(), myQCopy.getServiceTime());
			Assert::IsFalse(myQ2.isEmpty());
			Assert::IsTrue(myQ2.isFull());

			for (int i = 0; i < 5; ++i) {
				Assert::AreEqual(i, myQ2.deQueue());
			}
			Assert::IsTrue(myQ2.isEmpty());
		}

		TEST_METHOD(TestFullEmpty)
		{
			srand(time(NULL));
			Queue myQ(__DEFAULT_Q_SIZE);

			Assert::IsTrue(myQ.isEmpty());
			Assert::IsFalse(myQ.isFull());

			myQ.enQueue(1);

			Assert::IsFalse(myQ.isFull());
			Assert::IsFalse(myQ.isEmpty());

			myQ.enQueue(1);

			Assert::IsFalse(myQ.isFull());
			Assert::IsFalse(myQ.isEmpty());

			myQ.enQueue(1);

			Assert::IsFalse(myQ.isFull());
			Assert::IsFalse(myQ.isEmpty());

			myQ.enQueue(1);

			Assert::IsFalse(myQ.isFull());
			Assert::IsFalse(myQ.isEmpty());

			myQ.enQueue(1);

			Assert::IsTrue(myQ.isFull());
			Assert::IsFalse(myQ.isEmpty());

			for (int i = 0; i < __TRY_LIMIT / 1000; ++i) {
				int randLen = rand() % 100 + 1;
				Assert::IsTrue(randLen > 0 && randLen <= 100);
				Queue randLenQueue(randLen);
				Assert::IsTrue(randLenQueue.isEmpty());
				for (int j = 0; j < randLen - 1; ++j) {
					randLenQueue.enQueue(1);
					Assert::IsFalse(randLenQueue.isEmpty(), err1);
					Assert::IsFalse(randLenQueue.isFull(), err2);
				}
				randLenQueue.enQueue(1);
				Assert::IsTrue(randLenQueue.isFull(), err3);

				for (int j = 0; j < randLen - 1; ++j) {
					randLenQueue.deQueue();
					Assert::IsFalse(randLenQueue.isEmpty(), err1);
					Assert::IsFalse(randLenQueue.isFull(), err2);
				}
				randLenQueue.deQueue();
				Assert::IsTrue(randLenQueue.isEmpty(), err3);

			}

		}

		TEST_METHOD(TestGetSerial) {
			srand(time(NULL));
			Queue myQ1(10);
			Queue myQ2(10);
			Queue myQ3(10);
			Queue myQ4(10);
			Queue myQ5(10);

			int q1Serial = myQ1.getSerial();
			//note - static counter is not initialized to 1 but to 0 in these tests on account of .lib compilation
			Assert::AreEqual(++q1Serial, myQ2.getSerial());
			Assert::AreEqual(++q1Serial, myQ3.getSerial());
			Assert::AreEqual(++q1Serial, myQ4.getSerial());
			Assert::AreEqual(++q1Serial, myQ5.getSerial());

			~myQ5;
			Queue myQ6(10);
			Assert::AreEqual(++q1Serial, myQ6.getSerial());

		}

		TEST_METHOD(TestGetServiceTime) {
			srand(time(NULL));
			auto funPtr1 = [this] {  Queue::Queue(5, -10, 2);  };
			Assert::ExpectException<const std::invalid_argument&>(funPtr1);

			auto funPtr2 = [this] {  Queue::Queue(5, 10, -2);  };
			Assert::ExpectException<const std::invalid_argument&>(funPtr2);

			for (int i = 0; i < __TRY_LIMIT / 100; ++i) {
				int randMinService = rand() % 100 + 1;
				int randMaxService = randMinService + (rand() % 100 + 1);
				Queue myQ(1, randMinService, randMaxService);
				Assert::IsTrue(myQ.getServiceTime() >= randMinService && myQ.getServiceTime() <= randMaxService);
			}
		}

		TEST_METHOD(TestPeek) {
			srand(time(NULL));
			for (int i = 0; i < __TRY_LIMIT / 100000; ++i) {
				int randLen = rand() % 100 + 1;
				Queue myQ(randLen);
				for (int j = 0; j < randLen; ++j) {
					myQ.enQueue(j);
				}
				for (int j = 0; j < randLen; ++j) {
					Assert::AreEqual(j, myQ.peek());
					myQ.deQueue();
				}
				Assert::IsTrue(myQ.isEmpty());
			}
		}

		TEST_METHOD(TestEnqueueDequeue) {
			srand(time(NULL));
			Queue myQ(100);
			Assert::AreEqual(0, myQ.getSize());

			for (int i = 0; i < 100; ++i) {
				myQ.enQueue(i);
				Assert::AreEqual(i + 1, myQ.getSize());
			}

			Assert::IsTrue(myQ.isFull());

			for (int i = 0; i < 100; ++i) {
				Assert::AreEqual(i, myQ.deQueue());
				Assert::AreEqual(100 - i - 1, myQ.getSize());
			}

			Assert::IsTrue(myQ.isEmpty());
		}

		TEST_METHOD(TestOperatorInt) {
			srand(time(NULL));
			for (int i = 0; i < __TRY_LIMIT / 1000; ++i) {
				int randServiceTime = rand() % 100 + 1;
				int randAdd = rand() % 50 + 1;
				Queue myQ(10, randServiceTime, randServiceTime + randAdd);
				for (int j = 0; j < 10; ++j) {
					Assert::IsTrue((int)myQ == myQ.getSize()*myQ.getServiceTime());
					myQ.enQueue(1);
					Assert::IsTrue((int)myQ <= myQ.getCapacity()*myQ.getServiceTime());
				}
			}
		}

		TEST_METHOD(TestClickTime) {
			srand(time(NULL));
			int randomCustomerAmount = rand() % 100 + 1;
			Queue myQ(randomCustomerAmount);
			int myQserviceTime = myQ.getServiceTime();
			for (int i = 0; i < randomCustomerAmount; ++i) {
				myQ.enQueue(1);
			}

			Assert::IsTrue(myQ.isFull());

			for (int i = randomCustomerAmount; i > 0; --i) {
				for (int j = 0; j < myQserviceTime - 1; ++j) {
					myQ.clickTime();
					Assert::IsTrue(myQ.getSize() == i);
				}
				myQ.clickTime();
				Assert::IsTrue(myQ.getSize() == i - 1);
			}
			Assert::IsTrue(myQ.isEmpty());
		}

		TEST_METHOD(TestOperatorEquals) {
			srand(time(NULL));
			Queue myQ1(10), myQ2(10);
			Assert::IsFalse(&myQ1 == &myQ2);

			myQ1.enQueue(1);
			myQ1.enQueue(-23);

			myQ2.enQueue(15);
			myQ2.enQueue(78);
			myQ2.enQueue(900);

			Assert::AreNotEqual(myQ1.getSize(), myQ2.getSize());
			Assert::AreNotEqual(myQ1.getSerial(), myQ2.getSerial());

			myQ1 = myQ2;

			Assert::AreEqual(myQ1.getSize(), myQ2.getSize());
			Assert::AreEqual(3, myQ1.getSize(), err1);
			Assert::AreNotEqual(myQ1.getSerial(), myQ2.getSerial());
			Assert::AreEqual(myQ1.peek(), myQ2.peek());

			int temp = myQ1.getSize();
			for (int i = temp/*myQ1.getSize()*/; i > 0; --i) {
				Assert::AreEqual(myQ1.deQueue(), myQ2.deQueue());
			}

			Assert::IsTrue(myQ1.isEmpty(), err2);
			Assert::IsTrue(myQ2.isEmpty(), err3);
		}
	};

	TEST_CLASS(UnitTestSimulate)
	{
	public:

		TEST_METHOD(TestConstructor_Shortest_q10_10) {
			srand(time(NULL));
			Simulate mySim(1, shortest, "q10_10");
			Assert::AreEqual(1, mySim.getInterval());
			Assert::AreEqual(0, strcmp("q10_10", mySim.getQType()));
			Assert::AreEqual(0, mySim.getInlineCustomers());
			Assert::AreEqual(0, mySim.getMaxInlineCustomers());
			Assert::AreEqual(0, mySim.getSadCustomers());
			const Queue * const * const qArr = mySim.getQArr();

			for (int i = 0; i < mySim.getNumOfQs(); ++i) {
				Assert::IsTrue(qArr[i]->getServiceTime() >= 50 && qArr[i]->getServiceTime() <= 150);
			}

		}

		TEST_METHOD(TestGetSadCustomers) {
			srand(time(NULL));
			for (int j = 1; j < 2; ++j) {
				for (int i = 0; i < 101; ++i) {
					Simulate mySim(1, longest, "q100_1");
					Assert::AreEqual(0, mySim.getSadCustomers());
					mySim.startSimulation(i);
					Assert::AreEqual(0, mySim.getSadCustomers());
				}

				for (int i = 0; i < 11; ++i) {
					Simulate mySim(1, longest, "q10_10");
					Assert::AreEqual(0, mySim.getSadCustomers());
					mySim.startSimulation(i);
					Assert::AreEqual(0, mySim.getSadCustomers());
				}

				for (int i = 0; i < 11; ++i) {
					Simulate mySim(1, random, "q10_R1_19");
					Assert::AreEqual(0, mySim.getSadCustomers());
					mySim.startSimulation(i);
					Assert::AreEqual(0, mySim.getSadCustomers());
				}

				Simulate mySim1(1, longest, "q10_10");
				Simulate mySim3(1, longest, "q10_R1_19");

				mySim1.startSimulation(10000);
				mySim3.startSimulation(10000);

				Assert::AreNotEqual(0, mySim1.getSadCustomers(), L"banana_1");
				Assert::AreNotEqual(0, mySim3.getSadCustomers(), L"banana_3");
			}
		}

		TEST_METHOD(TestGetInlineCustomers) {
			srand(time(NULL));
			int inLine = 0;
			Simulate mySim(10, random, "q100_1");
			Assert::AreEqual(0, mySim.getInlineCustomers());
			mySim.addCustomer(); mySim.addCustomer(); mySim.addCustomer();
			Assert::AreEqual(3, mySim.getInlineCustomers());
			for (int i = 0; i <= Simulate::MAX_SERVICE_TIME; ++i) {
				mySim.deQFinishing();
				inLine = mySim.getInlineCustomers();
			}
			Assert::AreEqual(0, mySim.getInlineCustomers());

		}

		TEST_METHOD(TestGetMaxInlineCustomers) {

		}

		TEST_METHOD(TestGetInterval) {

		}

		TEST_METHOD(TestStartSimulation) {

		}

		TEST_METHOD(TestGetNumOfQs) {
			Simulate mySim1(1, shortest, "q10_10");
			Simulate mySim2(1, shortest, "q100_1");
			Simulate mySim3(1, shortest, "q10_R1_19");

			Assert::AreEqual(10, mySim1.getNumOfQs());
			Assert::AreEqual(100, mySim2.getNumOfQs());
			Assert::AreEqual(10, mySim3.getNumOfQs());

			mySim1.startSimulation(1000);
			mySim2.startSimulation(1000);
			mySim3.startSimulation(1000);

			Assert::AreEqual(10, mySim1.getNumOfQs());
			Assert::AreEqual(100, mySim2.getNumOfQs());
			Assert::AreEqual(10, mySim3.getNumOfQs());

		}

		TEST_METHOD(TestFindFirstQAviliable) {
			Simulate mySim(1, shortest, "q10_10");

			int index = 0;
			for (int i = 0; i < 100; ++i) {
				index = mySim.findFirstQAviliable();
				for (int j = 0; j < index; ++j) {
					Assert::IsTrue(mySim.m_qArr[j]->isFull());
				}
				Assert::IsFalse(mySim.m_qArr[index]->isFull());
				mySim.addCustomer();
			}

			Simulate mySim2(1, longest, "q10_10");

			index = 0;
			for (int i = 0; i < 100; ++i) {
				index = mySim2.findFirstQAviliable();
				for (int j = 0; j < index; ++j) {
					Assert::IsTrue(mySim2.m_qArr[j]->isFull());
				}
				Assert::IsFalse(mySim2.m_qArr[index]->isFull());
				mySim2.addCustomer();
			}

			Simulate mySim3(1, random, "q100_1");

			for (int i = 0; i < 100; ++i) {
				index = mySim3.findFirstQAviliable();
				for (int j = 0; j < index; ++j) {
					Assert::IsTrue(mySim3.m_qArr[j]->isFull());
				}
				Assert::AreEqual(i, index);
				mySim3.m_qArr[i]->enQueue(1);
			}
		}

	};
}