#include <cppunit/extensions/HelperMacros.h>

#include <vector>
#include <utility>
#include <functional>
#include <boost/noncopyable.hpp>
#include <mpi.h>

#include "../MpiParallelTask.h"
#include "../MpiAdapterInterface.h"

std::vector<std::pair<std::vector<int>::iterator, std::vector<int>::iterator>> one_partition(std::vector<int>::iterator, std::vector<int>::iterator, int);
std::vector<std::pair<std::vector<int>::iterator, std::vector<int>::iterator>> two_partitions(std::vector<int>::iterator, std::vector<int>::iterator, int);
std::vector<std::pair<std::vector<int>::iterator, std::vector<int>::iterator>> two_partitions_from_four_items(std::vector<int>::iterator begin, std::vector<int>::iterator end, int number_of_partitions);

class MpiParallelTaskTests : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(MpiParallelTaskTests);
    CPPUNIT_TEST(StartCallsPartitioningFunction);
    CPPUNIT_TEST(StartCallsPartitioningFunctionWithBeginIterator);
    CPPUNIT_TEST(StartCallsPartitioningFunctionWithEndIterator);
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

		std::vector<int> input;

		MockMpiAdapater mpiAdapter;
		auto runner = MpiParallelTask<std::vector<int>::iterator>(mpiAdapter, input.begin(), input.end(), partitioning_method, [](std::vector<int>::iterator iterator) { return 0; }, 1);

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The partitioning method was not called, which is not expected.", true, tracker.GetPartitioningMethodCalled());
	}

	void StartCallsPartitioningFunctionWithBeginIterator()
	{
		PartitioningTracker tracker;

		auto partitioning_method = std::bind(&PartitioningTracker::partition, &tracker, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

		std::vector<int> input;

		MockMpiAdapater mpiAdapter;
		auto runner = MpiParallelTask<std::vector<int>::iterator>(mpiAdapter, input.begin(), input.end(), partitioning_method, [](std::vector<int>::iterator iterator) { return 0; }, 1);

		runner.start();

		CPPUNIT_ASSERT_MESSAGE("The partitioning method was not called with the begin iterator, which is not expected.", input.begin() == tracker.GetBeginInterator());
	}

	void StartCallsPartitioningFunctionWithEndIterator()
	{
		PartitioningTracker tracker;

		auto partitioning_method = std::bind(&PartitioningTracker::partition, &tracker, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

		std::vector<int> input;

		MockMpiAdapater mpiAdapter;
		auto runner = MpiParallelTask<std::vector<int>::iterator>(mpiAdapter, input.begin(), input.end(), partitioning_method, [](std::vector<int>::iterator iterator) { return 0; }, 1);

		runner.start();

		CPPUNIT_ASSERT_MESSAGE("The partitioning method was not called with the end iterator, which is not expected.", input.end() == tracker.GetEndInterator());
	}

	void StartCallsPartitioningFunctionWithNumberOfPartitions()
	{
		PartitioningTracker tracker;

		auto partitioning_method = std::bind(&PartitioningTracker::partition, &tracker, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

		std::vector<int> input;

		MockMpiAdapater mpiAdapter;
		const int number_of_partitions = 13;
		auto runner = MpiParallelTask<std::vector<int>::iterator>(mpiAdapter, input.begin(), input.end(), partitioning_method, [](std::vector<int>::iterator iterator) { return 0; }, number_of_partitions);

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The partitioning method was not called with the number of partitions, which is not expected.",number_of_partitions, tracker.GetNumberOfPartitions());
	}

	void StartCallsMpiSendOnceForOnePartition()
	{
		std::vector<int> input;

		MockMpiAdapater mpiAdapter;

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<std::vector<int>::iterator>(mpiAdapter, input.begin(), input.end(), one_partition, [](std::vector<int>::iterator iterator) { return 0; }, unused_number_of_partitions);

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MPI_Send method was not called the correct number of times based on the given number of partitions, which is not expected.", 1, mpiAdapter.GetNumberOfTimesMpiSendCalled());
	}

	void StartCallsMpiSendTwiceForTwoPartitions()
	{
		std::vector<int> input;

		MockMpiAdapater mpiAdapter;

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<std::vector<int>::iterator>(mpiAdapter, input.begin(), input.end(), two_partitions, [](std::vector<int>::iterator iterator) { return 0; }, unused_number_of_partitions);

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MPI_Send method was not called the correct number of times based on the given number of partitions, which is not expected.", 2, mpiAdapter.GetNumberOfTimesMpiSendCalled());
	}

	void StartCallsMpiSendWithTheCorrectNumberOfEntriesForThePartitions()
	{
		std::vector<int> input;

		MockMpiAdapater mpiAdapter;

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<std::vector<int>::iterator>(mpiAdapter, input.begin(), input.end(), one_partition, [](std::vector<int>::iterator iterator) { return 0; }, unused_number_of_partitions);

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MPI_Send method was not called with the correct count of elements based on the given number of partitions, which is not expected.", 2, mpiAdapter.GetCountInMpiSend());
	}

	void StartCallsMpiSendWithTheExpectedTag()
	{
		std::vector<int> input;

		MockMpiAdapater mpiAdapter;

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<std::vector<int>::iterator>(mpiAdapter, input.begin(), input.end(), one_partition, [](std::vector<int>::iterator iterator) { return 0; }, unused_number_of_partitions);

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MPI_Send method was not called with the expected tag, which is not expected.", 0, mpiAdapter.GetTagInMpiSend());
	}

	void StartCallsMpiSendWithTheIntegerRepresentationOfTheStartIteratorForThePartition()
	{
		std::vector<int> input;
		input.emplace_back(67);
		input.emplace_back(92);

		MockMpiAdapater mpiAdapter;

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<std::vector<int>::iterator>(mpiAdapter, input.begin(), input.end(), one_partition, [](std::vector<int>::iterator iterator) { return *iterator; }, unused_number_of_partitions);

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MPI_Send method was not called the correct first iterator value, which is not expected.", 67, mpiAdapter.GetFirstIteratorValueInMpiSend());
	}

	void StartCallsMpiSendWithTheIntegerRepresentationOfTheEndIteratorForThePartition()
	{
		std::vector<int> input;
		input.emplace_back(67);
		input.emplace_back(92);

		MockMpiAdapater mpiAdapter;

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<std::vector<int>::iterator>(mpiAdapter, input.begin(), input.end(), one_partition, [](std::vector<int>::iterator iterator) { return *iterator; }, unused_number_of_partitions);

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MPI_Send method was not called the correct end iterator value, which is not expected.", 92, mpiAdapter.GetSecondIteratorValueInMpiSend());
	}

	void StartCallsMpiSendWithRankDestinationZeroForTheFirstPartition()
	{
		std::vector<int> input;
		input.emplace_back(67);
		input.emplace_back(92);

		MockMpiAdapater mpiAdapter;

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<std::vector<int>::iterator>(mpiAdapter, input.begin(), input.end(), one_partition, [](std::vector<int>::iterator iterator) { return *iterator; }, unused_number_of_partitions);

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MPI_Send method was not called the correct destination, which is not expected.", 0, mpiAdapter.GetDestinationInMpiSend());
	}

	void StartCallsMpiSendWithRankDestinationOneForTheSecondPartition()
	{
		std::vector<int> input;
		input.emplace_back(67);
		input.emplace_back(92);
		input.emplace_back(2);
		input.emplace_back(89);

		MockMpiAdapater mpiAdapter;

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<std::vector<int>::iterator>(mpiAdapter, input.begin(), input.end(), two_partitions_from_four_items, [](std::vector<int>::iterator iterator) { return *iterator; }, unused_number_of_partitions);

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MPI_Send method was not called the correct destination, which is not expected.", 1, mpiAdapter.GetDestinationInMpiSend());
	}

	void StartDoesNotCallMpiSendForNonRankZeroInstance()
	{
		std::vector<int> input;

		MockMpiAdapater mpiAdapter(1);

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<std::vector<int>::iterator>(mpiAdapter, input.begin(), input.end(), one_partition, [](std::vector<int>::iterator iterator) { return 0; }, unused_number_of_partitions);

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MPI_Send method was called for a non-rank-zero intance, which is not expected.", 0, mpiAdapter.GetNumberOfTimesMpiSendCalled());
	}

	void StartCallsMpiRecvForNonRankZeroInstances()
	{
		std::vector<int> input;

		MockMpiAdapater mpiAdapter(4);

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<std::vector<int>::iterator>(mpiAdapter, input.begin(), input.end(), one_partition, [](std::vector<int>::iterator iterator) { return 0; }, unused_number_of_partitions);

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MPI_Recv method was not called for a non-rank-zero intance, which is not expected.", true, mpiAdapter.GetMpiRecvCalled());
	}

	void StartCallsMpiRecvForNonRankZeroInstancesWithExpectedCount()
	{
		std::vector<int> input;

		MockMpiAdapater mpiAdapter(4);

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<std::vector<int>::iterator>(mpiAdapter, input.begin(), input.end(), one_partition, [](std::vector<int>::iterator iterator) { return 0; }, unused_number_of_partitions);

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MPI_Recv method was not called for a non-rank-zero intance with the expected count, which is not expected.", 2, mpiAdapter.GetCountInMpiRecv());
	}

	void StartCallsMpiRecvForNonRankZeroInstancesWithExpectedSource()
	{
		std::vector<int> input;

		MockMpiAdapater mpiAdapter(4);

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<std::vector<int>::iterator>(mpiAdapter, input.begin(), input.end(), one_partition, [](std::vector<int>::iterator iterator) { return 0; }, unused_number_of_partitions);

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MPI_Recv method was not called for a non-rank-zero intance with the expected source, which is not expected.", 0, mpiAdapter.GetSourceInMpiRecv());
	}

	void StartCallsMpiRecvForNonRankZeroInstancesWithExpectedTag()
	{
		std::vector<int> input;

		MockMpiAdapater mpiAdapter(4);

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<std::vector<int>::iterator>(mpiAdapter, input.begin(), input.end(), one_partition, [](std::vector<int>::iterator iterator) { return 0; }, unused_number_of_partitions);

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MPI_Recv method was not called for a non-rank-zero intance with the expected tag, which is not expected.", 0, mpiAdapter.GetTagInMpiRecv());
	}

	void StartDoesNotCallMpiRecvForRankZeroInstances()
	{
		std::vector<int> input;

		MockMpiAdapater mpiAdapter;

		const int unused_number_of_partitions = 13;
		auto runner = MpiParallelTask<std::vector<int>::iterator>(mpiAdapter, input.begin(), input.end(), one_partition, [](std::vector<int>::iterator iterator) { return 0; }, unused_number_of_partitions);

		runner.start();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MPI_Recv method was called for a rank-zero intance, which is not expected.", false, mpiAdapter.GetMpiRecvCalled());
	}

private:
	class PartitioningTracker
	{
	public:
		PartitioningTracker() : partitioning_method_called_(false)
		{
		}

		std::vector<std::pair<std::vector<int>::iterator, std::vector<int>::iterator>> partition(std::vector<int>::iterator begin, std::vector<int>::iterator end, int number_of_partitions)
		{
			partitioning_method_called_ = true;
			begin_iterator_ = begin;
			end_iterator_ = end;
			number_of_partitions_ = number_of_partitions;
			return std::vector<std::pair<std::vector<int>::iterator, std::vector<int>::iterator>>();
		}

		bool GetPartitioningMethodCalled() const
		{
			return partitioning_method_called_;
		}

		std::vector<int>::iterator GetBeginInterator() const
		{
			return begin_iterator_;
		}

		std::vector<int>::iterator GetEndInterator() const
		{
			return end_iterator_;
		}

		int GetNumberOfPartitions() const
		{
			return number_of_partitions_;
		}

	private:
		bool partitioning_method_called_;
		std::vector<int>::iterator begin_iterator_;
		std::vector<int>::iterator end_iterator_;
		int number_of_partitions_;
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

std::vector<std::pair<std::vector<int>::iterator, std::vector<int>::iterator>> one_partition(std::vector<int>::iterator begin, std::vector<int>::iterator end, int number_of_partitions)
{
	std::vector<std::pair<std::vector<int>::iterator, std::vector<int>::iterator>> output;
	if (begin != end)
		output.emplace_back(std::make_pair(begin, end-1));
	else
		output.emplace_back(std::make_pair(begin, begin));

	return output;
}

std::vector<std::pair<std::vector<int>::iterator, std::vector<int>::iterator>> two_partitions(std::vector<int>::iterator begin, std::vector<int>::iterator end, int number_of_partitions)
{
	std::vector<std::pair<std::vector<int>::iterator, std::vector<int>::iterator>> output;
	output.emplace_back(std::make_pair(begin, end));
	output.emplace_back(std::make_pair(begin, end));
	return output;
}

std::vector<std::pair<std::vector<int>::iterator, std::vector<int>::iterator>> two_partitions_from_four_items(std::vector<int>::iterator begin, std::vector<int>::iterator end, int number_of_partitions)
{
	std::vector<std::pair<std::vector<int>::iterator, std::vector<int>::iterator>> output;
	
	output.emplace_back(std::make_pair(begin, begin+1));
	output.emplace_back(std::make_pair(begin+2, begin+3));

	return output;
}