#include <cppunit/extensions/HelperMacros.h>

#include <vector>
#include <utility>
#include <functional>
#include <boost/noncopyable.hpp>
#include <mpi.h>

#include "../MpiParallelTask.h"
#include "../MpiAdapterInterface.h"

std::vector<std::pair<int, int>> one_partition(int, int, int);
std::vector<std::pair<int, int>> two_partitions(int, int, int);
std::vector<std::pair<int, int>> two_partitions_from_four_items(int, int, int);

class MpiParallelTaskTests : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(MpiParallelTaskTests);
    CPPUNIT_TEST(StartCallsPartitioningFunction);
    CPPUNIT_TEST(StartCallsPartitioningFunctionWithBeginInteger);
    CPPUNIT_TEST(StartCallsPartitioningFunctionWithEndInteger);
    CPPUNIT_TEST(StartCallsPartitioningFunctionWithNumberOfPartitions);
    CPPUNIT_TEST(StartCallsMpiSendOnceForOnePartition);
    CPPUNIT_TEST(StartCallsMpiSendTwiceForTwoPartitions);
    CPPUNIT_TEST(StartCallsMpiSendWithTheCorrectNumberOfEntriesForThePartitions);
    CPPUNIT_TEST(StartCallsMpiSendWithTheExpectedTag);
    CPPUNIT_TEST(StartCallsMpiSendWithTheIntegerRepresentationOfTheStartIteratorForThePartition);
    CPPUNIT_TEST(StartCallsMpiSendWithTheIntegerRepresentationOfTheEndIteratorForThePartition);
    CPPUNIT_TEST(StartCallsMpiSendWithRankDestinationZeroForTheFirstPartition);
    CPPUNIT_TEST(StartCallsMpiSendWithRankDestinationOneForTheSecondPartition);
    CPPUNIT_TEST(StartDoesNotCallMpiSendForNonRankZeroInstance);
    CPPUNIT_TEST(StartCallsMpiRecvForNonRankZeroInstances);
    CPPUNIT_TEST(StartCallsMpiRecvForNonRankZeroInstancesWithExpectedCount);
    CPPUNIT_TEST(StartCallsMpiRecvForNonRankZeroInstancesWithExpectedSource);
    CPPUNIT_TEST(StartCallsMpiRecvForNonRankZeroInstancesWithExpectedTag);
    CPPUNIT_TEST(StartDoesNotCallMpiRecvForRankZeroInstances);
    //CPPUNIT_TEST(StartCallsMapOnTaskWithProperBeginItereratorValue);
    //CPPUNIT_TEST(StartCallsMapOnTaskWithProperEndItereratorValue);
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

	void StartCallsMpiSendOnceForOnePartition()
	{
		MockTask task;
		MockMpiAdapater mpiAdapter;

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, 0, 1, one_partition, unused_number_of_partitions);

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MPI_Send method was not called the correct number of times based on the given number of partitions, which is not expected.", 1, mpiAdapter.GetNumberOfTimesMpiSendCalled());
	}

	void StartCallsMpiSendTwiceForTwoPartitions()
	{
		MockTask task;
		MockMpiAdapater mpiAdapter;

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, 0, 1, two_partitions, unused_number_of_partitions);

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MPI_Send method was not called the correct number of times based on the given number of partitions, which is not expected.", 2, mpiAdapter.GetNumberOfTimesMpiSendCalled());
	}

	void StartCallsMpiSendWithTheCorrectNumberOfEntriesForThePartitions()
	{
		MockTask task;
		MockMpiAdapater mpiAdapter;

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, 0, 1, one_partition, unused_number_of_partitions);

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MPI_Send method was not called with the correct count of elements based on the given number of partitions, which is not expected.", 2, mpiAdapter.GetCountInMpiSend());
	}

	void StartCallsMpiSendWithTheExpectedTag()
	{
		MockTask task;
		MockMpiAdapater mpiAdapter;

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, 0, 1, one_partition, unused_number_of_partitions);

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MPI_Send method was not called with the expected tag, which is not expected.", 0, mpiAdapter.GetTagInMpiSend());
	}

	void StartCallsMpiSendWithTheIntegerRepresentationOfTheStartIteratorForThePartition()
	{
		MockTask task;
		MockMpiAdapater mpiAdapter;

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, 67, 92, one_partition, unused_number_of_partitions);

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MPI_Send method was not called the correct first iterator value, which is not expected.", 67, mpiAdapter.GetFirstIteratorValueInMpiSend());
	}

	void StartCallsMpiSendWithTheIntegerRepresentationOfTheEndIteratorForThePartition()
	{
		MockTask task;
		MockMpiAdapater mpiAdapter;

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, 67, 92, one_partition, unused_number_of_partitions);

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MPI_Send method was not called the correct end iterator value, which is not expected.", 92, mpiAdapter.GetSecondIteratorValueInMpiSend());
	}

	void StartCallsMpiSendWithRankDestinationZeroForTheFirstPartition()
	{
		MockTask task;
		MockMpiAdapater mpiAdapter;

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, 67, 92, one_partition, unused_number_of_partitions);

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MPI_Send method was not called the correct destination, which is not expected.", 0, mpiAdapter.GetDestinationInMpiSend());
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

	void StartDoesNotCallMpiSendForNonRankZeroInstance()
	{
		MockTask task;
		MockMpiAdapater mpiAdapter(1);

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, 0, 1, one_partition, unused_number_of_partitions);

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MPI_Send method was called for a non-rank-zero intance, which is not expected.", 0, mpiAdapter.GetNumberOfTimesMpiSendCalled());
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
	/*
	void StartCallsMapOnTaskWithProperBeginItereratorValue()
	{
		std::vector<int> input;
		input.emplace_back(67);
		input.emplace_back(92);

		MockTask task;
		MockMpiAdapater mpiAdapter;

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, input.begin(), input.end(), one_partition, [](std::vector<int>::iterator iterator) { return *iterator; }, unused_number_of_partitions);

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The map method on the task was not called with the proper begin iterator value, which is not expected.", 67, task.GetBeginIteratorValue());
	}

	void StartCallsMapOnTaskWithProperEndItereratorValue()
	{
		std::vector<int> input;
		input.emplace_back(67);
		input.emplace_back(92);

		MockTask task;
		MockMpiAdapater mpiAdapter;

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<MockTask>(task, mpiAdapter, input.begin(), input.end(), one_partition, [](std::vector<int>::iterator iterator) { return *iterator; }, unused_number_of_partitions);

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The map method on the task was not called with the proper end iterator value, which is not expected.", 92, task.GetEndIteratorValue());
	}
	*/

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
		MockTask() : begin_iterator_value_(0), end_iterator_value_(0) {}

		void map(std::vector<int>::iterator begin, std::vector<int>::iterator end)
		{
			begin_iterator_value_ = *begin;
			end_iterator_value_ = *end;
		}
		
		int GetBeginIteratorValue() const { return begin_iterator_value_; }
		int GetEndIteratorValue() const { return end_iterator_value_; }

	private:
		int begin_iterator_value_;
		int end_iterator_value_;
	};

	class MockMpiAdapater : public MpiAdapterInterface
	{
	public:
		MockMpiAdapater() : rank_(0), number_of_times_MpiSend_called_(0), MpiRecv_called_(false)
		{}

		MockMpiAdapater(int rank) : rank_(rank), number_of_times_MpiSend_called_(0), MpiRecv_called_(false)
		{}

		virtual void MpiSend(const void* buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm) const
		{
			++number_of_times_MpiSend_called_;
			count_in_MpiSend_ = count;
			tag_in_MpiSend_ = tag;

			const int* iterator_values = (const int*)buf;

			first_iterator_value_MpiSend_ = iterator_values[0];
			second_iterator_value_MpiSend_ = iterator_values[1];

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
		}

		int GetNumberOfTimesMpiSendCalled() const { return number_of_times_MpiSend_called_; }
		int GetCountInMpiSend() const { return count_in_MpiSend_; }
		int GetTagInMpiSend() const { return tag_in_MpiSend_; }
		int GetFirstIteratorValueInMpiSend() const { return first_iterator_value_MpiSend_; }
		int GetSecondIteratorValueInMpiSend() const { return second_iterator_value_MpiSend_; }
		int GetDestinationInMpiSend() const {return destination_; }
		bool GetMpiRecvCalled() const { return MpiRecv_called_; }
		int GetCountInMpiRecv() const { return count_in_MpiRecv_; }
		int GetTagInMpiRecv() const { return tag_in_MpiRecv_; }
		int GetSourceInMpiRecv() const {return source_; }

	private:
		int rank_;
		mutable int number_of_times_MpiSend_called_;
		mutable int count_in_MpiSend_;
		mutable int tag_in_MpiSend_;
		mutable int first_iterator_value_MpiSend_;
		mutable int second_iterator_value_MpiSend_;
		mutable int destination_;
		mutable bool MpiRecv_called_;
		mutable int count_in_MpiRecv_;
		mutable int tag_in_MpiRecv_;
		mutable int source_;
	};
};

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

std::vector<std::pair<int, int>> two_partitions_from_four_items(int begin, int end, int number_of_partitions)
{
	std::vector<std::pair<int, int>> output;
	
	output.emplace_back(std::make_pair(begin, begin+1));
	output.emplace_back(std::make_pair(begin+2, begin+3));

	return output;
}