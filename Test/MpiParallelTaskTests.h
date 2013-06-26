#include <cppunit/extensions/HelperMacros.h>

#include <vector>
#include <utility>
#include <functional>
#include <climits>
#include <boost/noncopyable.hpp>
#include <mpi.h>

#include "../MpiParallelTask.h"
#include "../MpiAdapterInterface.h"

std::vector<std::pair<int, int>> one_partition(int, int, int);
std::vector<std::pair<int, int>> two_partitions(int, int, int);
std::vector<std::pair<int, int>> three_partitions(int, int, int);
std::vector<std::pair<int, int>> two_partitions_from_four_items(int, int, int);

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
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, 0, 1, partitioning_method, 1);

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
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, 0, 1, partitioning_method, number_of_partitions);

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The partitioning method was not called with the number of partitions, which is not expected.",number_of_partitions, tracker.GetNumberOfPartitions());
	}

	void StartCallsMpiSendOnceForTwoPartitions()
	{
		MockTask task;
		MockMpiAdapater mpiAdapter;

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, 0, 1, two_partitions, unused_number_of_partitions);

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MPI_Send method was not called the correct number of times based on the given number of partitions, which is not expected.", 1, mpiAdapter.GetNumberOfTimesMpiSendCalled());
	}

	void StartCallsMpiSendTwiceForThreePartitions()
	{
		MockTask task;
		MockMpiAdapater mpiAdapter;

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, 0, 1, three_partitions, unused_number_of_partitions);

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MPI_Send method was not called the correct number of times based on the given number of partitions, which is not expected.", 2, mpiAdapter.GetNumberOfTimesMpiSendCalled());
	}

	void StartCallsMpiSendWithTheCorrectNumberOfEntriesForThePartitions()
	{
		MockTask task;
		MockMpiAdapater mpiAdapter;

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, 0, 1, two_partitions, unused_number_of_partitions);

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MPI_Send method was not called with the correct count of elements based on the given number of partitions, which is not expected.", 2, mpiAdapter.GetCountInMpiSend());
	}

	void StartCallsMpiSendWithTheExpectedTag()
	{
		MockTask task;
		MockMpiAdapater mpiAdapter;

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, 0, 1, two_partitions, unused_number_of_partitions);

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MPI_Send method was not called with the expected tag, which is not expected.", 0, mpiAdapter.GetTagInMpiSend());
	}

	void StartCallsMpiSendWithTheStartIntegerForThePartition()
	{
		MockTask task;
		MockMpiAdapater mpiAdapter;

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, 67, 92, two_partitions, unused_number_of_partitions);

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MPI_Send method was not called the correct first iterator value, which is not expected.", 67, mpiAdapter.GetFirstIntegerValueInMpiSend());
	}

	void StartCallsMpiSendWithTheEndIntegerForThePartition()
	{
		MockTask task;
		MockMpiAdapater mpiAdapter;

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, 67, 92, two_partitions, unused_number_of_partitions);

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MPI_Send method was not called the correct end iterator value, which is not expected.", 92, mpiAdapter.GetSecondIntegerValueInMpiSend());
	}

	void StartCallsMpiSendWithRankDestinationOneForTheSecondPartition()
	{
		MockTask task;
		MockMpiAdapater mpiAdapter;

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, 0, 1, two_partitions_from_four_items, unused_number_of_partitions);

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MPI_Send method was not called the correct destination, which is not expected.", 1, mpiAdapter.GetDestinationInMpiSend());
	}

	void StartCallsMpiRecvForNonRankZeroInstances()
	{
		MockTask task;
		MockMpiAdapater mpiAdapter(4);

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, 0, 1, one_partition, unused_number_of_partitions);

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MPI_Recv method was not called for a non-rank-zero intance, which is not expected.", true, mpiAdapter.GetMpiRecvCalled());
	}

	void StartCallsMpiRecvForNonRankZeroInstancesWithExpectedCount()
	{
		MockTask task;
		MockMpiAdapater mpiAdapter(4);

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, 0, 1, one_partition, unused_number_of_partitions);

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MPI_Recv method was not called for a non-rank-zero intance with the expected count, which is not expected.", 2, mpiAdapter.GetCountInMpiRecv());
	}

	void StartCallsMpiRecvForNonRankZeroInstancesWithExpectedSource()
	{
		MockTask task;
		MockMpiAdapater mpiAdapter(4);

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, 0, 1, one_partition, unused_number_of_partitions);

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MPI_Recv method was not called for a non-rank-zero intance with the expected source, which is not expected.", 0, mpiAdapter.GetSourceInMpiRecv());
	}

	void StartCallsMpiRecvForNonRankZeroInstancesWithExpectedTag()
	{
		MockTask task;
		MockMpiAdapater mpiAdapter(4);

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, 0, 1, one_partition, unused_number_of_partitions);

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MPI_Recv method was not called for a non-rank-zero intance with the expected tag, which is not expected.", 0, mpiAdapter.GetTagInMpiRecv());
	}

	void StartDoesNotCallMpiRecvForRankZeroInstances()
	{
		MockTask task;
		MockMpiAdapater mpiAdapter;

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, 0, 1, one_partition, unused_number_of_partitions);

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MPI_Recv method was called for a rank-zero intance, which is not expected.", false, mpiAdapter.GetMpiRecvCalled());
	}

	void StartDoesNotCallMpiSendForRankZeroInstances()
	{
		MockTask task;
		MockMpiAdapater mpiAdapter;

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, 0, 1, one_partition, unused_number_of_partitions);

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MPI_Send method was called for a rank-zero intance, which is not expected.", false, mpiAdapter.GetMpiSendCalled());
	}

	void StartCallsMapOnProvidedTaskInstanceWithProperBeginValue()
	{
		MockTask task;
		MockMpiAdapater mpiAdapter;

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, 67, 92, one_partition, unused_number_of_partitions);

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The map method on the task was not called with the proper begin iterator value, which is not expected.", 67, task.GetBeginValue());
	}

	void StartCallsMapOnProvidedTaskInstanceWithProperEndValue()
	{
		MockTask task;
		MockMpiAdapater mpiAdapter;

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, 67, 92, one_partition, unused_number_of_partitions);

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The map method on the task was not called with the proper end iterator value, which is not expected.", 92, task.GetEndValue());
	}

	void StartCallsMapOnNewTaskInstanceWithProperBeginValueForSecondPartition()
	{
		MockTask task;
		MockMpiAdapater mpiAdapter(1);

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, 0, 1, two_partitions, unused_number_of_partitions);

		task.ClearLatestValuesFromAnyInstance();

		runner.start();

		// The expected value is supplied directly from the mock MPI_Recv method.
		CPPUNIT_ASSERT_EQUAL_MESSAGE("The map method on the task was not called with the proper begin iterator value, which is not expected.", 67, task.GetLatestBeginValueFromAnyInstance());
	}

	void StartCallsMapOnNewTaskInstanceWithProperEndValueForSecondPartition()
	{
		MockTask task;
		MockMpiAdapater mpiAdapter(1);

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, 0, 1, two_partitions, unused_number_of_partitions);

		task.ClearLatestValuesFromAnyInstance();

		runner.start();

		// The expected value is supplied directly from the mock MPI_Recv method.
		CPPUNIT_ASSERT_EQUAL_MESSAGE("The map method on the task was not called with the proper end iterator value, which is not expected.", 92, task.GetLatestEndValueFromAnyInstance());
	}

	void StartCallsReduceOnNewTaskInstanceWithZeroValueForNumberOfUniqueSolutions()
	{
		MockTask task;
		MockMpiAdapater mpiAdapter(1);

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, 0, 1, two_partitions, unused_number_of_partitions);

		task.ClearLatestValuesFromAnyInstance();

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The reduce method on the slave task was not called with zero unique solutions, which is not expected.", 0, task.GetLatestNumUniqueValueFromAnyInstance());
	}

	void StartCallsReduceOnNewTaskInstanceWithZeroValueForNumberOfNonuniqueSolutions()
	{
		MockTask task;
		MockMpiAdapater mpiAdapter(1);

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, 0, 1, two_partitions, unused_number_of_partitions);

		task.ClearLatestValuesFromAnyInstance();

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The reduce method on the slave task was not called with zero non-unique solutions, which is not expected.", 0, task.GetLatestNumNonuniqueValueFromAnyInstance());
	}

	void StartCallsReduceOnNewTaskInstanceWithZeroValueForNumberOfNoSolutions()
	{
		MockTask task;
		MockMpiAdapater mpiAdapter(1);

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, 0, 1, two_partitions, unused_number_of_partitions);

		task.ClearLatestValuesFromAnyInstance();

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The reduce method on the slave task was not called with zero no solutions, which is not expected.", 0, task.GetLatestNumNoValueFromAnyInstance());
	}

	void StartCallsMpiSendForRankOneProcessWithNumberOfUniqueSolutions()
	{
		MockTask task;
		MockMpiAdapater mpiAdapter(1);

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, 0, 1, two_partitions, unused_number_of_partitions);

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MpiSend method was not called with the number of unique solutions as the first value, which is not expected.", 42, mpiAdapter.GetFirstIntegerValueInMpiSend());
	}

	void StartCallsMpiSendForRankOneProcessWithNumberOfNonuniqueSolutions()
	{
		MockTask task;
		MockMpiAdapater mpiAdapter(1);

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, 0, 1, two_partitions, unused_number_of_partitions);

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MpiSend method was not called with the number of non-unique solutions as the second value, which is not expected.", 43, mpiAdapter.GetSecondIntegerValueInMpiSend());
	}

	void StartCallsMpiSendForRankOneProcessWithNumberOfNoSolutions()
	{
		MockTask task;
		MockMpiAdapater mpiAdapter(1);

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, 0, 1, two_partitions, unused_number_of_partitions);

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MpiSend method was not called with the number of no solutions as the third value, which is not expected.", 44, mpiAdapter.GetThirdIntegerValueInMpiSend());
	}

	void StartCallsMpiSendForRankOneProcessWithACountOfThreeValues()
	{
		MockTask task;
		MockMpiAdapater mpiAdapter(1);

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, 0, 1, two_partitions, unused_number_of_partitions);

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MpiSend method was not called with a count of three values, which is not expected.", 3, mpiAdapter.GetCountInMpiSend());
	}

	void StartCallsMpiSendForRankOneProcessWithADestinationOfZero()
	{
		MockTask task;
		MockMpiAdapater mpiAdapter(1);

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, 0, 1, two_partitions, unused_number_of_partitions);

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MpiSend method was not called with a destination of zero, which is not expected.", 0, mpiAdapter.GetDestinationInMpiSend());
	}

private:
	class PartitioningTracker
	{
	public:
		PartitioningTracker() : partitioning_method_called_(false)
		{
		}

		std::vector<std::pair<int, int>> partition(int begin, int end, int number_of_partitions)
		{
			partitioning_method_called_ = true;
			begin_integer_ = begin;
			end_integer_ = end;
			number_of_partitions_ = number_of_partitions;
			return std::vector<std::pair<int, int>>();
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
		MockTask() : begin_value_(0), end_value_(0) {}

		void map(int begin, int end)
		{
			begin_value_ = begin;
			end_value_ = end;

			latest_begin_value_from_any_instance_ = begin;
			latest_end_value_from_any_instance_ = end;
		}

		std::vector<int> reduce(const std::vector<int> input)
		{
			latest_numUnique_value_from_any_instance_ = input.size() == 3 ? input[0] : 0;
			latest_numNonunique_value_from_any_instance_ = input.size() == 3 ? input[1] : 0;;
			latest_numNo_value_from_any_instance_ = input.size() == 3 ? input[2] : 0;;

			std::vector<int> result;
			result.emplace_back(42);
			result.emplace_back(43);
			result.emplace_back(44);

			return result;
		}
		
		int GetBeginValue() const { return begin_value_; }
		int GetEndValue() const { return end_value_; }

		int GetLatestBeginValueFromAnyInstance() const { return latest_begin_value_from_any_instance_; }
		int GetLatestEndValueFromAnyInstance() const { return latest_end_value_from_any_instance_; }
		
		int GetLatestNumUniqueValueFromAnyInstance() const { return latest_numUnique_value_from_any_instance_; }
		int GetLatestNumNonuniqueValueFromAnyInstance() const { return latest_numNonunique_value_from_any_instance_; }
		int GetLatestNumNoValueFromAnyInstance() const { return latest_numNo_value_from_any_instance_; }

		static void ClearLatestValuesFromAnyInstance() 
		{
			latest_begin_value_from_any_instance_ = 0;
			latest_end_value_from_any_instance_ = 0;

			latest_numUnique_value_from_any_instance_ = UINT_MAX;
			latest_numNonunique_value_from_any_instance_ = UINT_MAX;
			latest_numNo_value_from_any_instance_ = UINT_MAX;
		}

	private:
		int begin_value_;
		int end_value_;

		static int latest_begin_value_from_any_instance_;
		static int latest_end_value_from_any_instance_;

		static unsigned int latest_numUnique_value_from_any_instance_;
		static unsigned int latest_numNonunique_value_from_any_instance_;
		static unsigned int latest_numNo_value_from_any_instance_;
	};

	class MockMpiAdapater : public MpiAdapterInterface
	{
	public:
		MockMpiAdapater() : rank_(0), number_of_times_MpiSend_called_(0), count_in_MpiSend_(0), tag_in_MpiSend_(0), first_integer_value_in_MpiSend_(0),
			second_integer_value_in_MpiSend_(0), third_integer_value_in_MpiSend_(0), destination_(INT_MAX), MpiRecv_called_(false), count_in_MpiRecv_(0), tag_in_MpiRecv_(0),
			source_(INT_MAX)
		{}

		MockMpiAdapater(int rank) : rank_(rank), number_of_times_MpiSend_called_(0), count_in_MpiSend_(0), tag_in_MpiSend_(0), first_integer_value_in_MpiSend_(0),
			second_integer_value_in_MpiSend_(0), third_integer_value_in_MpiSend_(0), destination_(INT_MAX), MpiRecv_called_(false), count_in_MpiRecv_(0), tag_in_MpiRecv_(0),
			source_(INT_MAX)
		{}

		virtual void MpiSend(const void* buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm) const
		{
			++number_of_times_MpiSend_called_;
			count_in_MpiSend_ = count;
			tag_in_MpiSend_ = tag;

			const int* iterator_values = (const int*)buf;

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

			source_ = source;

			auto output_values = (int*)buf;
			output_values[0] = 67;
			output_values[1] = 92;
		}

		int GetNumberOfTimesMpiSendCalled() const { return number_of_times_MpiSend_called_; }
		int GetCountInMpiSend() const { return count_in_MpiSend_; }
		int GetTagInMpiSend() const { return tag_in_MpiSend_; }
		int GetFirstIntegerValueInMpiSend() const { return first_integer_value_in_MpiSend_; }
		int GetSecondIntegerValueInMpiSend() const { return second_integer_value_in_MpiSend_; }
		int GetThirdIntegerValueInMpiSend() const { return third_integer_value_in_MpiSend_; }
		int GetDestinationInMpiSend() const {return destination_; }
		bool GetMpiSendCalled() const { return number_of_times_MpiSend_called_ > 0; }
		bool GetMpiRecvCalled() const { return MpiRecv_called_; }
		int GetCountInMpiRecv() const { return count_in_MpiRecv_; }
		int GetTagInMpiRecv() const { return tag_in_MpiRecv_; }
		int GetSourceInMpiRecv() const {return source_; }

	private:
		int rank_;
		mutable int number_of_times_MpiSend_called_;
		mutable int count_in_MpiSend_;
		mutable int tag_in_MpiSend_;
		mutable int first_integer_value_in_MpiSend_;
		mutable int second_integer_value_in_MpiSend_;
		mutable int third_integer_value_in_MpiSend_;
		mutable int destination_;
		mutable bool MpiRecv_called_;
		mutable int count_in_MpiRecv_;
		mutable int tag_in_MpiRecv_;
		mutable int source_;
	};
};

int MpiParallelTaskTests::MockTask::latest_begin_value_from_any_instance_ = 0;
int MpiParallelTaskTests::MockTask::latest_end_value_from_any_instance_ = 0;
unsigned int MpiParallelTaskTests::MockTask::latest_numUnique_value_from_any_instance_ = UINT_MAX;
unsigned int MpiParallelTaskTests::MockTask::latest_numNonunique_value_from_any_instance_ = UINT_MAX;
unsigned int MpiParallelTaskTests::MockTask::latest_numNo_value_from_any_instance_ = UINT_MAX;

std::vector<std::pair<int, int>> one_partition(int begin, int end, int number_of_partitions)
{
	std::vector<std::pair<int, int>> output;
    output.emplace_back(std::make_pair(begin, end));

	return output;
}

std::vector<std::pair<int, int>> two_partitions(int begin, int end, int number_of_partitions)
{
	std::vector<std::pair<int, int>> output;
	output.emplace_back(std::make_pair(begin, end));
	output.emplace_back(std::make_pair(begin, end));
	return output;
}

std::vector<std::pair<int, int>> three_partitions(int begin, int end, int number_of_partitions)
{
	std::vector<std::pair<int, int>> output;
	output.emplace_back(std::make_pair(begin, end));
	output.emplace_back(std::make_pair(begin, end));
	output.emplace_back(std::make_pair(begin, end));
	return output;
}

std::vector<std::pair<int, int>> two_partitions_from_four_items(int begin, int end, int number_of_partitions)
{
	std::vector<std::pair<int, int>> output;
	
	output.emplace_back(std::make_pair(begin, begin+1));
	output.emplace_back(std::make_pair(begin+2, begin+3));

	return output;
}