#include <cppunit/extensions/HelperMacros.h>

#include <vector>
#include <utility>
#include <functional>
#include <climits>
#include <boost/noncopyable.hpp>
#include <mpi.h>

#include "../MpiParallelTask.h"
#include "../MpiAdapterInterface.h"

std::vector<std::pair<unsigned int, unsigned int>> one_partition(unsigned int, unsigned int, int);
std::vector<std::pair<unsigned int, unsigned int>> two_partitions(unsigned int, unsigned int, int);
std::vector<std::pair<unsigned int, unsigned int>> three_partitions(unsigned int, unsigned int, int);
std::vector<std::pair<unsigned int, unsigned int>> two_partitions_from_four_items(unsigned int, unsigned int, int);

class MpiParallelTaskTests : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(MpiParallelTaskTests);
    CPPUNIT_TEST(StartCallsPartitioningFunction);
    CPPUNIT_TEST(StartCallsPartitioningFunctionWithBeginInteger);
    CPPUNIT_TEST(StartCallsPartitioningFunctionWithEndInteger);
    CPPUNIT_TEST(StartCallsPartitioningFunctionWithNumberOfPartitions);
    CPPUNIT_TEST(StartCallsMpiSendOnceForTwoPartitions);
    CPPUNIT_TEST(StartCallsMpiSendTwiceForThreePartitions);
    CPPUNIT_TEST(StartCallsMpiSendWithTheCorrectNumberOfEntriesForThePartitions);
    CPPUNIT_TEST(StartCallsMpiSendWithTheExpectedTag);
    CPPUNIT_TEST(StartCallsMpiSendWithTheStartIntegerForThePartition);
    CPPUNIT_TEST(StartCallsMpiSendWithTheEndIntegerForThePartition);
    CPPUNIT_TEST(StartCallsMpiSendWithRankDestinationOneForTheSecondPartition);
    CPPUNIT_TEST(StartCallsMpiRecvForNonRankZeroInstances);
    CPPUNIT_TEST(StartCallsMpiRecvForNonRankZeroInstancesWithExpectedCount);
    CPPUNIT_TEST(StartCallsMpiRecvForNonRankZeroInstancesWithExpectedSource);
    CPPUNIT_TEST(StartCallsMpiRecvForNonRankZeroInstancesWithExpectedTag);
    CPPUNIT_TEST(StartDoesNotCallMpiRecvForRankZeroInstances);
    CPPUNIT_TEST(StartDoesNotCallMpiSendForRankZeroInstances);
    CPPUNIT_TEST(StartCallsMapOnProvidedTaskInstanceWithProperBeginValue);
    CPPUNIT_TEST(StartCallsMapOnProvidedTaskInstanceWithProperEndValue);
    CPPUNIT_TEST(StartCallsMapOnNewTaskInstanceWithProperBeginValueForSecondPartition);
    CPPUNIT_TEST(StartCallsMapOnNewTaskInstanceWithProperEndValueForSecondPartition);
    CPPUNIT_TEST(StartCallsReduceOnNewTaskInstanceWithZeroValueForNumberOfUniqueSolutions);
    CPPUNIT_TEST(StartCallsReduceOnNewTaskInstanceWithZeroValueForNumberOfNonuniqueSolutions);
    CPPUNIT_TEST(StartCallsReduceOnNewTaskInstanceWithZeroValueForNumberOfNoSolutions);
    CPPUNIT_TEST(StartCallsMpiSendForRankOneProcessWithNumberOfUniqueSolutions);
    CPPUNIT_TEST(StartCallsMpiSendForRankOneProcessWithNumberOfNonuniqueSolutions);
    CPPUNIT_TEST(StartCallsMpiSendForRankOneProcessWithNumberOfNoSolutions);
    CPPUNIT_TEST(StartCallsMpiSendForRankOneProcessWithACountOfThreeValues);
    CPPUNIT_TEST(StartCallsMpiSendForRankOneProcessWithADestinationOfZero);
    CPPUNIT_TEST(CompleteCallsMpiRecvForRankZeroProcessWithACountOfThree);
    CPPUNIT_TEST(CompleteDoesNotCallMpiRecvForANonRankZeroProcess);
    CPPUNIT_TEST(CompleteCallsMpiRecvForRankZeroProcessWithASourceForEachSlaveProcess);
    CPPUNIT_TEST(CompleteCallsReduceOnTheOriginalTaskForRankZeroProcessWithTheNumberOfUniqueSolutionsFromMpiRecvForEachSlaveProcess);
    CPPUNIT_TEST(CompleteCallsReduceOnTheOriginalTaskForRankZeroProcessWithTheNumberOfNonuniqueSolutionsFromMpiRecvForEachSlaveProcess);
    CPPUNIT_TEST(CompleteCallsReduceOnTheOriginalTaskForRankZeroProcessWithTheNumberOfNoSolutionsFromMpiRecvForEachSlaveProcess);
    CPPUNIT_TEST_SUITE_END();
	
private:
	class PartitioningTracker;
	class MockMpiAdapater;

public:
    void setUp()
    {
    }

    void tearDown()
    {
    }

	void StartCallsPartitioningFunction()
	{
		PartitioningTracker tracker;

		auto partitioning_method = std::bind(&PartitioningTracker::partition, &tracker, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

		MockTask task;
		MockMpiAdapater mpiAdapter;
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, 0U, 1U, partitioning_method, 1);

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The partitioning method was not called, which is not expected.", true, tracker.GetPartitioningMethodCalled());
	}

	void StartCallsPartitioningFunctionWithBeginInteger()
	{
		PartitioningTracker tracker;

		auto partitioning_method = std::bind(&PartitioningTracker::partition, &tracker, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

		const int begin = 17;

		MockTask task;
		MockMpiAdapater mpiAdapter;
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, begin, 1, partitioning_method, 1);

		runner.start();

		CPPUNIT_ASSERT_MESSAGE("The partitioning method was not called with the begin iterator, which is not expected.", begin == tracker.GetBeginInteger());
	}

	void StartCallsPartitioningFunctionWithEndInteger()
	{
		PartitioningTracker tracker;

		auto partitioning_method = std::bind(&PartitioningTracker::partition, &tracker, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

		const int end = 49;

		MockTask task;
		MockMpiAdapater mpiAdapter;
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, 0, end, partitioning_method, 1);

		runner.start();

		CPPUNIT_ASSERT_MESSAGE("The partitioning method was not called with the end iterator, which is not expected.", end == tracker.GetEndInteger());
	}

	void StartCallsPartitioningFunctionWithNumberOfPartitions()
	{
		PartitioningTracker tracker;

		auto partitioning_method = std::bind(&PartitioningTracker::partition, &tracker, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

		MockTask task;
		MockMpiAdapater mpiAdapter;
		const int number_of_partitions = 13;
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, 0U, 1U, partitioning_method, number_of_partitions);

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The partitioning method was not called with the number of partitions, which is not expected.",number_of_partitions, tracker.GetNumberOfPartitions());
	}

	void StartCallsMpiSendOnceForTwoPartitions()
	{
		MockTask task;
		MockMpiAdapater mpiAdapter;

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, 0U, 1U, two_partitions, unused_number_of_partitions);

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MPI_Send method was not called the correct number of times based on the given number of partitions, which is not expected.", 1, mpiAdapter.GetNumberOfTimesMpiSendCalled());
	}

	void StartCallsMpiSendTwiceForThreePartitions()
	{
		MockTask task;
		MockMpiAdapater mpiAdapter;

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, 0U, 1U, three_partitions, unused_number_of_partitions);

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MPI_Send method was not called the correct number of times based on the given number of partitions, which is not expected.", 2, mpiAdapter.GetNumberOfTimesMpiSendCalled());
	}

	void StartCallsMpiSendWithTheCorrectNumberOfEntriesForThePartitions()
	{
		MockTask task;
		MockMpiAdapater mpiAdapter;

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, 0U, 1U, two_partitions, unused_number_of_partitions);

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MPI_Send method was not called with the correct count of elements based on the given number of partitions, which is not expected.", 2, mpiAdapter.GetCountInMpiSend());
	}

	void StartCallsMpiSendWithTheExpectedTag()
	{
		MockTask task;
		MockMpiAdapater mpiAdapter;

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, 0U, 1U, two_partitions, unused_number_of_partitions);

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MPI_Send method was not called with the expected tag, which is not expected.", 0, mpiAdapter.GetTagInMpiSend());
	}

	void StartCallsMpiSendWithTheStartIntegerForThePartition()
	{
		MockTask task;
		MockMpiAdapater mpiAdapter;

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, 67U, 92U, two_partitions, unused_number_of_partitions);

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MPI_Send method was not called the correct first iterator value, which is not expected.", 67U, mpiAdapter.GetFirstIntegerValueInMpiSend());
	}

	void StartCallsMpiSendWithTheEndIntegerForThePartition()
	{
		MockTask task;
		MockMpiAdapater mpiAdapter;

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, 67U, 92U, two_partitions, unused_number_of_partitions);

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MPI_Send method was not called the correct end iterator value, which is not expected.", 92U, mpiAdapter.GetSecondIntegerValueInMpiSend());
	}

	void StartCallsMpiSendWithRankDestinationOneForTheSecondPartition()
	{
		MockTask task;
		MockMpiAdapater mpiAdapter;

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, 0U, 1U, two_partitions_from_four_items, unused_number_of_partitions);

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MPI_Send method was not called the correct destination, which is not expected.", 1, mpiAdapter.GetDestinationInMpiSend());
	}

	void StartCallsMpiRecvForNonRankZeroInstances()
	{
		MockTask task;
		MockMpiAdapater mpiAdapter(4);

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, 0U, 1U, one_partition, unused_number_of_partitions);

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MPI_Recv method was not called for a non-rank-zero intance, which is not expected.", true, mpiAdapter.GetMpiRecvCalled());
	}

	void StartCallsMpiRecvForNonRankZeroInstancesWithExpectedCount()
	{
		MockTask task;
		MockMpiAdapater mpiAdapter(4);

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, 0U, 1U, one_partition, unused_number_of_partitions);

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MPI_Recv method was not called for a non-rank-zero intance with the expected count, which is not expected.", 2, mpiAdapter.GetCountInMpiRecv());
	}

	void StartCallsMpiRecvForNonRankZeroInstancesWithExpectedSource()
	{
		MockTask task;
		MockMpiAdapater mpiAdapter(4);

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, 0U, 1U, one_partition, unused_number_of_partitions);

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MPI_Recv method was not called for a non-rank-zero intance with the expected source, which is not expected.", 0, mpiAdapter.GetSourceInMpiRecv());
	}

	void StartCallsMpiRecvForNonRankZeroInstancesWithExpectedTag()
	{
		MockTask task;
		MockMpiAdapater mpiAdapter(4);

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, 0U, 1U, one_partition, unused_number_of_partitions);

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MPI_Recv method was not called for a non-rank-zero intance with the expected tag, which is not expected.", 0, mpiAdapter.GetTagInMpiRecv());
	}

	void StartDoesNotCallMpiRecvForRankZeroInstances()
	{
		MockTask task;
		MockMpiAdapater mpiAdapter;

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, 0U, 1U, one_partition, unused_number_of_partitions);

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MPI_Recv method was called for a rank-zero intance, which is not expected.", false, mpiAdapter.GetMpiRecvCalled());
	}

	void StartDoesNotCallMpiSendForRankZeroInstances()
	{
		MockTask task;
		MockMpiAdapater mpiAdapter;

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, 0U, 1U, one_partition, unused_number_of_partitions);

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MPI_Send method was called for a rank-zero intance, which is not expected.", false, mpiAdapter.GetMpiSendCalled());
	}

	void StartCallsMapOnProvidedTaskInstanceWithProperBeginValue()
	{
		MockTask task;
		MockMpiAdapater mpiAdapter;

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, 67U, 92U, one_partition, unused_number_of_partitions);

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The map method on the task was not called with the proper begin iterator value, which is not expected.", 67U, task.GetBeginValue());
	}

	void StartCallsMapOnProvidedTaskInstanceWithProperEndValue()
	{
		MockTask task;
		MockMpiAdapater mpiAdapter;

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, 67U, 92U, one_partition, unused_number_of_partitions);

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The map method on the task was not called with the proper end iterator value, which is not expected.", 92U, task.GetEndValue());
	}

	void StartCallsMapOnNewTaskInstanceWithProperBeginValueForSecondPartition()
	{
		MockTask task;
		MockMpiAdapater mpiAdapter(1);

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, 0U, 1U, two_partitions, unused_number_of_partitions);

		task.ClearLatestValuesFromAnyInstance();

		runner.start();

		// The expected value is supplied directly from the mock MPI_Recv method.
		CPPUNIT_ASSERT_EQUAL_MESSAGE("The map method on the task was not called with the proper begin iterator value, which is not expected.", 67U, task.GetLatestBeginValueFromAnyInstance());
	}

	void StartCallsMapOnNewTaskInstanceWithProperEndValueForSecondPartition()
	{
		MockTask task;
		MockMpiAdapater mpiAdapter(1);

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, 0U, 1U, two_partitions, unused_number_of_partitions);

		task.ClearLatestValuesFromAnyInstance();

		runner.start();

		// The expected value is supplied directly from the mock MPI_Recv method.
		CPPUNIT_ASSERT_EQUAL_MESSAGE("The map method on the task was not called with the proper end iterator value, which is not expected.", 92U, task.GetLatestEndValueFromAnyInstance());
	}

	void StartCallsReduceOnNewTaskInstanceWithZeroValueForNumberOfUniqueSolutions()
	{
		MockTask task;
		MockMpiAdapater mpiAdapter(1);

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, 0U, 1U, two_partitions, unused_number_of_partitions);

		task.ClearLatestValuesFromAnyInstance();

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The reduce method on the slave task was not called with zero unique solutions, which is not expected.", 0U, task.GetLatestNumUniqueValueFromAnyInstance());
	}

	void StartCallsReduceOnNewTaskInstanceWithZeroValueForNumberOfNonuniqueSolutions()
	{
		MockTask task;
		MockMpiAdapater mpiAdapter(1);

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, 0U, 1U, two_partitions, unused_number_of_partitions);

		task.ClearLatestValuesFromAnyInstance();

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The reduce method on the slave task was not called with zero non-unique solutions, which is not expected.", 0U, task.GetLatestNumNonuniqueValueFromAnyInstance());
	}

	void StartCallsReduceOnNewTaskInstanceWithZeroValueForNumberOfNoSolutions()
	{
		MockTask task;
		MockMpiAdapater mpiAdapter(1);

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, 0U, 1U, two_partitions, unused_number_of_partitions);

		task.ClearLatestValuesFromAnyInstance();

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The reduce method on the slave task was not called with zero no solutions, which is not expected.", 0U, task.GetLatestNumNoValueFromAnyInstance());
	}

	void StartCallsMpiSendForRankOneProcessWithNumberOfUniqueSolutions()
	{
		MockTask task;
		MockMpiAdapater mpiAdapter(1);

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, 0U, 1U, two_partitions, unused_number_of_partitions);

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MpiSend method was not called with the number of unique solutions as the first value, which is not expected.", 42U, mpiAdapter.GetFirstIntegerValueInMpiSend());
	}

	void StartCallsMpiSendForRankOneProcessWithNumberOfNonuniqueSolutions()
	{
		MockTask task;
		MockMpiAdapater mpiAdapter(1);

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, 0U, 1U, two_partitions, unused_number_of_partitions);

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MpiSend method was not called with the number of non-unique solutions as the second value, which is not expected.", 43U, mpiAdapter.GetSecondIntegerValueInMpiSend());
	}

	void StartCallsMpiSendForRankOneProcessWithNumberOfNoSolutions()
	{
		MockTask task;
		MockMpiAdapater mpiAdapter(1);

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, 0U, 1U, two_partitions, unused_number_of_partitions);

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MpiSend method was not called with the number of no solutions as the third value, which is not expected.", 44U, mpiAdapter.GetThirdIntegerValueInMpiSend());
	}

	void StartCallsMpiSendForRankOneProcessWithACountOfThreeValues()
	{
		MockTask task;
		MockMpiAdapater mpiAdapter(1);

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, 0U, 1U, two_partitions, unused_number_of_partitions);

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MpiSend method was not called with a count of three values, which is not expected.", 3, mpiAdapter.GetCountInMpiSend());
	}

	void StartCallsMpiSendForRankOneProcessWithADestinationOfZero()
	{
		MockTask task;
		MockMpiAdapater mpiAdapter(1);

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, 0U, 1U, two_partitions, unused_number_of_partitions);

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MpiSend method was not called with a destination of zero, which is not expected.", 0, mpiAdapter.GetDestinationInMpiSend());
	}

	void CompleteCallsMpiRecvForRankZeroProcessWithACountOfThree()
	{
		MockTask task;
		MockMpiAdapater mpiAdapter;

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, 0U, 1U, one_partition, unused_number_of_partitions);

		runner.complete();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MpiRecv method was not called with a count of three, which is not expected.", 3, mpiAdapter.GetCountInMpiRecv());
	}

	void CompleteDoesNotCallMpiRecvForANonRankZeroProcess()
	{
		MockTask task;
		MockMpiAdapater mpiAdapter(3);

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, 0U, 1U, one_partition, unused_number_of_partitions);

		runner.complete();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MpiRecv method was called for a non-rank 0 process, which is not expected.", false, mpiAdapter.GetMpiRecvCalled());
	}

	void CompleteCallsMpiRecvForRankZeroProcessWithASourceForEachSlaveProcess()
	{
		MockTask task;
		MockMpiAdapater mpiAdapter;

		const int number_of_partitions = 6;
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, 0U, 1U, one_partition, number_of_partitions);

		runner.complete();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MpiRecv method was not called for five slave nodes, which is not expected.", 5, mpiAdapter.GetNumberOfTimesMpiRecvCalled());
		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MpiRecv method was not called for slave node 1, which is not expected.", 1, mpiAdapter.GetSourcesInMpiRecv()[0]);
		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MpiRecv method was not called for slave node 2, which is not expected.", 2, mpiAdapter.GetSourcesInMpiRecv()[1]);
		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MpiRecv method was not called for slave node 3, which is not expected.", 3, mpiAdapter.GetSourcesInMpiRecv()[2]);
		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MpiRecv method was not called for slave node 4, which is not expected.", 4, mpiAdapter.GetSourcesInMpiRecv()[3]);
		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MpiRecv method was not called for slave node 5, which is not expected.", 5, mpiAdapter.GetSourcesInMpiRecv()[4]);
	}

	void CompleteCallsReduceOnTheOriginalTaskForRankZeroProcessWithTheNumberOfUniqueSolutionsFromMpiRecvForEachSlaveProcess()
	{
		MockTask task;
		MockMpiAdapater mpiAdapter;

		const int number_of_partitions = 2;
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, 0U, 1U, one_partition, number_of_partitions);

		runner.complete();

		// The expected value comes directly from the MockMpiAdapater implementation.
		CPPUNIT_ASSERT_EQUAL_MESSAGE("The reduce method was not called with the correct number of unique solutions, which is not expected.", 67U, task.GetNumUniqueValue());
	}

	void CompleteCallsReduceOnTheOriginalTaskForRankZeroProcessWithTheNumberOfNonuniqueSolutionsFromMpiRecvForEachSlaveProcess()
	{
		MockTask task;
		MockMpiAdapater mpiAdapter;

		const int number_of_partitions = 2;
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, 0U, 1U, one_partition, number_of_partitions);

		runner.complete();

		// The expected value comes directly from the MockMpiAdapater implementation.
		CPPUNIT_ASSERT_EQUAL_MESSAGE("The reduce method was not called with the correct number of non-unique solutions, which is not expected.", 92U, task.GetNumNonuniqueValue());
	}

	void CompleteCallsReduceOnTheOriginalTaskForRankZeroProcessWithTheNumberOfNoSolutionsFromMpiRecvForEachSlaveProcess()
	{
		MockTask task;
		MockMpiAdapater mpiAdapter;

		const int number_of_partitions = 2;
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, 0U, 1U, one_partition, number_of_partitions);

		runner.complete();

		// The expected value comes directly from the MockMpiAdapater implementation.
		CPPUNIT_ASSERT_EQUAL_MESSAGE("The reduce method was not called with the correct number of no solutions, which is not expected.", 104U, task.GetNumNoValue());
	}

private:
	class PartitioningTracker
	{
	public:
		PartitioningTracker() : partitioning_method_called_(false)
		{
		}

		std::vector<std::pair<unsigned int, unsigned int>> partition(unsigned int begin, unsigned int end, int number_of_partitions)
		{
			partitioning_method_called_ = true;
			begin_integer_ = begin;
			end_integer_ = end;
			number_of_partitions_ = number_of_partitions;
			return std::vector<std::pair<unsigned int, unsigned int>>();
		}

		bool GetPartitioningMethodCalled() const
		{
			return partitioning_method_called_;
		}

		int GetBeginInteger() const
		{
			return begin_integer_;
		}

		int GetEndInteger() const
		{
			return end_integer_;
		}

		int GetNumberOfPartitions() const
		{
			return number_of_partitions_;
		}

	private:
		bool partitioning_method_called_;
		int begin_integer_;
		int end_integer_;
		int number_of_partitions_;
	};

	class MockTask
	{
	public:
		MockTask() : begin_value_(0), end_value_(0), numUnique_value_(0), numNonunique_value_(0), numNo_value_(0) {}

		void map(unsigned int begin, unsigned int end)
		{
			begin_value_ = begin;
			end_value_ = end;

			latest_begin_value_from_any_instance_ = begin;
			latest_end_value_from_any_instance_ = end;
		}

		std::vector<int> reduce(const std::vector<unsigned int>& input)
		{
			numUnique_value_ = input.size() == 3 ? input[0] : 0;
			numNonunique_value_ = input.size() == 3 ? input[1] : 0;
			numNo_value_ = input.size() == 3 ? input[2] : 0;

			latest_numUnique_value_from_any_instance_ = input.size() == 3 ? input[0] : 0;
			latest_numNonunique_value_from_any_instance_ = input.size() == 3 ? input[1] : 0;
			latest_numNo_value_from_any_instance_ = input.size() == 3 ? input[2] : 0;

			std::vector<int> result;
			result.emplace_back(42);
			result.emplace_back(43);
			result.emplace_back(44);

			return result;
		}
		
		unsigned int GetBeginValue() const { return begin_value_; }
		unsigned int GetEndValue() const { return end_value_; }

		unsigned int GetNumUniqueValue() const { return numUnique_value_; }
		unsigned int GetNumNonuniqueValue() const { return numNonunique_value_; }
		unsigned int GetNumNoValue() const { return numNo_value_; }

		unsigned int GetLatestBeginValueFromAnyInstance() const { return latest_begin_value_from_any_instance_; }
		unsigned int GetLatestEndValueFromAnyInstance() const { return latest_end_value_from_any_instance_; }
		
		unsigned int GetLatestNumUniqueValueFromAnyInstance() const { return latest_numUnique_value_from_any_instance_; }
		unsigned int GetLatestNumNonuniqueValueFromAnyInstance() const { return latest_numNonunique_value_from_any_instance_; }
		unsigned int GetLatestNumNoValueFromAnyInstance() const { return latest_numNo_value_from_any_instance_; }

		static void ClearLatestValuesFromAnyInstance() 
		{
			latest_begin_value_from_any_instance_ = 0;
			latest_end_value_from_any_instance_ = 0;

			latest_numUnique_value_from_any_instance_ = UINT_MAX;
			latest_numNonunique_value_from_any_instance_ = UINT_MAX;
			latest_numNo_value_from_any_instance_ = UINT_MAX;
		}

	private:
		unsigned int begin_value_;
		unsigned int end_value_;

		unsigned int numUnique_value_;
		unsigned int numNonunique_value_;
		unsigned int numNo_value_;

		static unsigned int latest_begin_value_from_any_instance_;
		static unsigned int latest_end_value_from_any_instance_;

		static unsigned int latest_numUnique_value_from_any_instance_;
		static unsigned int latest_numNonunique_value_from_any_instance_;
		static unsigned int latest_numNo_value_from_any_instance_;
	};

	class MockMpiAdapater : public MpiAdapterInterface
	{
	public:
		MockMpiAdapater() : rank_(0), number_of_times_MpiSend_called_(0), count_in_MpiSend_(0), tag_in_MpiSend_(0), first_integer_value_in_MpiSend_(0),
			second_integer_value_in_MpiSend_(0), third_integer_value_in_MpiSend_(0), destination_(INT_MAX), MpiRecv_called_(false), count_in_MpiRecv_(0), tag_in_MpiRecv_(0)
		{}

		MockMpiAdapater(int rank) : rank_(rank), number_of_times_MpiSend_called_(0), count_in_MpiSend_(0), tag_in_MpiSend_(0), first_integer_value_in_MpiSend_(0),
			second_integer_value_in_MpiSend_(0), third_integer_value_in_MpiSend_(0), destination_(INT_MAX), MpiRecv_called_(false), count_in_MpiRecv_(0), tag_in_MpiRecv_(0)
		{}

		virtual void MpiSend(const void* buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm) const
		{
			++number_of_times_MpiSend_called_;
			count_in_MpiSend_ = count;
			tag_in_MpiSend_ = tag;

			auto iterator_values = (const unsigned int*)buf;

			first_integer_value_in_MpiSend_ = iterator_values[0];
			second_integer_value_in_MpiSend_ = iterator_values[1];

			if (count == 3)
				third_integer_value_in_MpiSend_ = iterator_values[2];

			destination_ = dest;
		}

		virtual int MpiCommRank(MPI_Comm comm) const
		{
			return rank_;
		}

	    virtual void MpiRecv(void* buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Status* status) const
		{
			MpiRecv_called_ = true;
			count_in_MpiRecv_ = count;
			tag_in_MpiRecv_ = tag;

			sources_.push_back(source);

			auto output_values = (unsigned int*)buf;
			output_values[0] = 67;
			output_values[1] = 92;

			if (count == 3)
				output_values[2] = 104;
		}

		int GetNumberOfTimesMpiSendCalled() const { return number_of_times_MpiSend_called_; }
		int GetCountInMpiSend() const { return count_in_MpiSend_; }
		int GetTagInMpiSend() const { return tag_in_MpiSend_; }
		unsigned int GetFirstIntegerValueInMpiSend() const { return first_integer_value_in_MpiSend_; }
		unsigned int GetSecondIntegerValueInMpiSend() const { return second_integer_value_in_MpiSend_; }
		unsigned int GetThirdIntegerValueInMpiSend() const { return third_integer_value_in_MpiSend_; }
		int GetDestinationInMpiSend() const {return destination_; }
		bool GetMpiSendCalled() const { return number_of_times_MpiSend_called_ > 0; }
		bool GetMpiRecvCalled() const { return MpiRecv_called_; }
		int GetCountInMpiRecv() const { return count_in_MpiRecv_; }
		int GetTagInMpiRecv() const { return tag_in_MpiRecv_; }
		int GetSourceInMpiRecv() const {return sources_[0]; }
		int GetNumberOfTimesMpiRecvCalled() const {return sources_.size(); }
		std::vector<int> GetSourcesInMpiRecv() const {return sources_; }

	private:
		int rank_;
		mutable int number_of_times_MpiSend_called_;
		mutable int count_in_MpiSend_;
		mutable int tag_in_MpiSend_;
		mutable unsigned int first_integer_value_in_MpiSend_;
		mutable unsigned int second_integer_value_in_MpiSend_;
		mutable unsigned int third_integer_value_in_MpiSend_;
		mutable int destination_;
		mutable bool MpiRecv_called_;
		mutable int count_in_MpiRecv_;
		mutable int tag_in_MpiRecv_;
		mutable std::vector<int> sources_;
	};
};

unsigned int MpiParallelTaskTests::MockTask::latest_begin_value_from_any_instance_ = 0;
unsigned int MpiParallelTaskTests::MockTask::latest_end_value_from_any_instance_ = 0;
unsigned int MpiParallelTaskTests::MockTask::latest_numUnique_value_from_any_instance_ = UINT_MAX;
unsigned int MpiParallelTaskTests::MockTask::latest_numNonunique_value_from_any_instance_ = UINT_MAX;
unsigned int MpiParallelTaskTests::MockTask::latest_numNo_value_from_any_instance_ = UINT_MAX;

std::vector<std::pair<unsigned int, unsigned int>> one_partition(unsigned int begin, unsigned int end, int number_of_partitions)
{
	std::vector<std::pair<unsigned int, unsigned int>> output;
    output.emplace_back(std::make_pair(begin, end));

	return output;
}

std::vector<std::pair<unsigned int, unsigned int>> two_partitions(unsigned int begin, unsigned int end, int number_of_partitions)
{
	std::vector<std::pair<unsigned int, unsigned int>> output;
	output.emplace_back(std::make_pair(begin, end));
	output.emplace_back(std::make_pair(begin, end));
	return output;
}

std::vector<std::pair<unsigned int, unsigned int>> three_partitions(unsigned int begin, unsigned int end, int number_of_partitions)
{
	std::vector<std::pair<unsigned int, unsigned int>> output;
	output.emplace_back(std::make_pair(begin, end));
	output.emplace_back(std::make_pair(begin, end));
	output.emplace_back(std::make_pair(begin, end));
	return output;
}

std::vector<std::pair<unsigned int, unsigned int>> two_partitions_from_four_items(unsigned int begin, unsigned int end, int number_of_partitions)
{
	std::vector<std::pair<unsigned int, unsigned int>> output;
	
	output.emplace_back(std::make_pair(begin, begin+1));
	output.emplace_back(std::make_pair(begin+2, begin+3));

	return output;
}