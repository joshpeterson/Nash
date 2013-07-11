#include <cppunit/extensions/HelperMacros.h>

#include <vector>
#include "../TRPMODistributedParallelTask.h"

class TRPMODistributedParallelTaskTests : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TRPMODistributedParallelTaskTests);
    CPPUNIT_TEST(MapUsesTheZerothP1StrategyAndComparesItToAllP2Strategies);
    CPPUNIT_TEST(MapUsesAllP1StrategiesBetweenTheBeginAndEndIntegersInclusiveAndComparesThemToAllP2StrategiesGetTheCorrectNumberOfUnqiueNashSolutions);
    CPPUNIT_TEST(MapUsesAllP1StrategiesBetweenTheBeginAndEndIntegersInclusiveAndComparesThemToAllP2StrategiesGetTheCorrectNumberOfNonunqiueNashSolutions);
    CPPUNIT_TEST(MapUsesAllP1StrategiesBetweenTheBeginAndEndIntegersInclusiveAndComparesThemToAllP2StrategiesGetTheCorrectNumberOfNoNashSolutions);
    CPPUNIT_TEST(ReduceAddsTheValueInItsZerothElementToTheNumberOfUniqueNashSolutions);
    CPPUNIT_TEST(ReduceAddsTheValueInItsZerothElementToTheNumberOfNonuniqueNashSolutions);
    CPPUNIT_TEST(ReduceAddsTheValueInItsZerothElementToTheNumberOfNoNashSolutions);
    CPPUNIT_TEST(ReduceReturnsAVectorWithTheValueInItsZerothElementEqualToTheNumberOfUniqueNashSolutions);
    CPPUNIT_TEST(ReduceReturnsAVectorWithTheValueInItsZerothElementEqualToTheNumberOfNonuniqueNashSolutions);
    CPPUNIT_TEST(ReduceReturnsAVectorWithTheValueInItsZerothElementEqualToTheNumberOfNoNashSolutions);
    CPPUNIT_TEST_SUITE_END();
	
public:
    void setUp()
    {
    }

    void tearDown()
    {
    }

	void MapUsesTheZerothP1StrategyAndComparesItToAllP2Strategies()
	{
		auto task = TRPMODistributedParallelTask(2, 2);
		task.map(0, 0);
		CPPUNIT_ASSERT_EQUAL_MESSAGE("The number of unique nash solutions is not correct.", 4, task.Comparator().GetNumUniqueNash());
	}

	void MapUsesAllP1StrategiesBetweenTheBeginAndEndIntegersInclusiveAndComparesThemToAllP2StrategiesGetTheCorrectNumberOfUnqiueNashSolutions()
	{
		auto task = TRPMODistributedParallelTask(2, 2);
		task.map(0, 3);
		CPPUNIT_ASSERT_EQUAL_MESSAGE("The number of unique nash solutions is not correct.", 12, task.Comparator().GetNumUniqueNash());
	}

	void MapUsesAllP1StrategiesBetweenTheBeginAndEndIntegersInclusiveAndComparesThemToAllP2StrategiesGetTheCorrectNumberOfNonunqiueNashSolutions()
	{
		auto task = TRPMODistributedParallelTask(2, 2);
		task.map(0, 3);
		CPPUNIT_ASSERT_EQUAL_MESSAGE("The number of non-unique nash solutions is not correct.", 2, task.Comparator().GetNumNonuniqueNash());
	}

	void MapUsesAllP1StrategiesBetweenTheBeginAndEndIntegersInclusiveAndComparesThemToAllP2StrategiesGetTheCorrectNumberOfNoNashSolutions()
	{
		auto task = TRPMODistributedParallelTask(2, 2);
		task.map(0, 3);
		CPPUNIT_ASSERT_EQUAL_MESSAGE("The number of no nash solutions is not correct.", 2, task.Comparator().GetNumNoNash());
	}

	void ReduceAddsTheValueInItsZerothElementToTheNumberOfUniqueNashSolutions()
	{
		auto task = TRPMODistributedParallelTask(2, 2);
		task.map(0, 3);

		std::vector<unsigned int> input;
		input.push_back(3);
		input.push_back(5);
		input.push_back(7);

		task.reduce(input);

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The number of unique nash solutions is not correct.", 15, task.Comparator().GetNumUniqueNash());
	}

	void ReduceAddsTheValueInItsZerothElementToTheNumberOfNonuniqueNashSolutions()
	{
		auto task = TRPMODistributedParallelTask(2, 2);
		task.map(0, 3);

		std::vector<unsigned int> input;
		input.push_back(3);
		input.push_back(5);
		input.push_back(7);

		task.reduce(input);

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The number of non-unique nash solutions is not correct.", 7, task.Comparator().GetNumNonuniqueNash());
	}

	void ReduceAddsTheValueInItsZerothElementToTheNumberOfNoNashSolutions()
	{
		auto task = TRPMODistributedParallelTask(2, 2);
		task.map(0, 3);

		std::vector<unsigned int> input;
		input.push_back(3);
		input.push_back(5);
		input.push_back(7);

		task.reduce(input);

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The number of no nash solutions is not correct.", 9, task.Comparator().GetNumNoNash());
	}

	void ReduceReturnsAVectorWithTheValueInItsZerothElementEqualToTheNumberOfUniqueNashSolutions()
	{
		auto task = TRPMODistributedParallelTask(2, 2);
		task.map(0, 3);

		std::vector<unsigned int> input;
		input.push_back(3);
		input.push_back(5);
		input.push_back(7);

		auto output = task.reduce(input);

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The number of unique nash solutions is not correct.", 15U, output[0]);
	}

	void ReduceReturnsAVectorWithTheValueInItsZerothElementEqualToTheNumberOfNonuniqueNashSolutions()
	{
		auto task = TRPMODistributedParallelTask(2, 2);
		task.map(0, 3);

		std::vector<unsigned int> input;
		input.push_back(3);
		input.push_back(5);
		input.push_back(7);

		auto output = task.reduce(input);

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The number of non-unique nash solutions is not correct.", 7U, output[1]);
	}

	void ReduceReturnsAVectorWithTheValueInItsZerothElementEqualToTheNumberOfNoNashSolutions()
	{
		auto task = TRPMODistributedParallelTask(2, 2);
		task.map(0, 3);

		std::vector<unsigned int> input;
		input.push_back(3);
		input.push_back(5);
		input.push_back(7);

		auto output = task.reduce(input);

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The number of no nash solutions is not correct.", 9U, output[2]);
	}
};