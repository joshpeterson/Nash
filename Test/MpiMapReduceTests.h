#include <cppunit/extensions/HelperMacros.h>

#include <vector>
#include <utility>
#include <functional>
#include <boost/noncopyable.hpp>
#include <mpi.h>

#include "../MpiMapReduce.h"
#include "../MpiAdapterInterface.h"

std::vector<std::pair<std::vector<int>::iterator, std::vector<int>::iterator>> one_partition(std::vector<int>::iterator, std::vector<int>::iterator, int);
std::vector<std::pair<std::vector<int>::iterator, std::vector<int>::iterator>> two_partitions(std::vector<int>::iterator, std::vector<int>::iterator, int);
std::vector<std::pair<std::vector<int>::iterator, std::vector<int>::iterator>> two_partitions_from_four_items(std::vector<int>::iterator begin, std::vector<int>::iterator end, int number_of_partitions);

class MpiMapReduceTests : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(MpiMapReduceTests);
    CPPUNIT_TEST(MapCallsPartitioningFunction);
    CPPUNIT_TEST(MapCallsPartitioningFunctionWithBeginIterator);
    CPPUNIT_TEST(MapCallsPartitioningFunctionWithEndIterator);
    CPPUNIT_TEST(MapCallsPartitioningFunctionWithNumberOfPartitions);
    CPPUNIT_TEST(MapCallsMpiSendOnceForOnePartition);
    CPPUNIT_TEST(MapCallsMpiSendTwiceForTwoPartitions);
    CPPUNIT_TEST(MapCallsMpiSendWithTheCorrectNumberOfEntriesForThePartitions);
    CPPUNIT_TEST(MapCallsMpiSendWithTheIntegerRepresentationOfTheStartIteratorForThePartition);
    CPPUNIT_TEST(MapCallsMpiSendWithTheIntegerRepresentationOfTheEndIteratorForThePartition);
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

	void MapCallsPartitioningFunction()
	{
		PartitioningTracker tracker;

		auto partitioning_method = std::bind(&PartitioningTracker::partition, &tracker, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

		std::vector<int> input;

		MockMpiAdapater mpiAdapter;
		auto runner = MpiMapReduce<std::vector<int>::iterator>(mpiAdapter, input.begin(), input.end(), partitioning_method, [](std::vector<int>::iterator iterator) { return 0; }, 1);

		runner.map();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The partitioning method was not called, which is not expected.", true, tracker.GetPartitioningMethodCalled());
	}

	void MapCallsPartitioningFunctionWithBeginIterator()
	{
		PartitioningTracker tracker;

		auto partitioning_method = std::bind(&PartitioningTracker::partition, &tracker, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

		std::vector<int> input;

		MockMpiAdapater mpiAdapter;
		auto runner = MpiMapReduce<std::vector<int>::iterator>(mpiAdapter, input.begin(), input.end(), partitioning_method, [](std::vector<int>::iterator iterator) { return 0; }, 1);

		runner.map();

		CPPUNIT_ASSERT_MESSAGE("The partitioning method was not called with the begin iterator, which is not expected.", input.begin() == tracker.GetBeginInterator());
	}

	void MapCallsPartitioningFunctionWithEndIterator()
	{
		PartitioningTracker tracker;

		auto partitioning_method = std::bind(&PartitioningTracker::partition, &tracker, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

		std::vector<int> input;

		MockMpiAdapater mpiAdapter;
		auto runner = MpiMapReduce<std::vector<int>::iterator>(mpiAdapter, input.begin(), input.end(), partitioning_method, [](std::vector<int>::iterator iterator) { return 0; }, 1);

		runner.map();

		CPPUNIT_ASSERT_MESSAGE("The partitioning method was not called with the end iterator, which is not expected.", input.end() == tracker.GetEndInterator());
	}

	void MapCallsPartitioningFunctionWithNumberOfPartitions()
	{
		PartitioningTracker tracker;

		auto partitioning_method = std::bind(&PartitioningTracker::partition, &tracker, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

		std::vector<int> input;

		MockMpiAdapater mpiAdapter;
		const int number_of_partitions = 13;
		auto runner = MpiMapReduce<std::vector<int>::iterator>(mpiAdapter, input.begin(), input.end(), partitioning_method, [](std::vector<int>::iterator iterator) { return 0; }, number_of_partitions);

		runner.map();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The partitioning method was not called with the number of partitions, which is not expected.",number_of_partitions, tracker.GetNumberOfPartitions());
	}

	void MapCallsMpiSendOnceForOnePartition()
	{
		std::vector<int> input;

		MockMpiAdapater mpiAdapter;

		const int unused_number_of_partitions = 13;
		auto runner = MpiMapReduce<std::vector<int>::iterator>(mpiAdapter, input.begin(), input.end(), one_partition, [](std::vector<int>::iterator iterator) { return 0; }, unused_number_of_partitions);

		runner.map();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MPI_Send method was not called the correct number of times based on the given number of partitions, which is not expected.", 1, mpiAdapter.GetNumberOfTimesMpiSendCalled());
	}

	void MapCallsMpiSendTwiceForTwoPartitions()
	{
		std::vector<int> input;

		MockMpiAdapater mpiAdapter;

		const int unused_number_of_partitions = 13;
		auto runner = MpiMapReduce<std::vector<int>::iterator>(mpiAdapter, input.begin(), input.end(), two_partitions, [](std::vector<int>::iterator iterator) { return 0; }, unused_number_of_partitions);

		runner.map();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MPI_Send method was not called the correct number of times based on the given number of partitions, which is not expected.", 2, mpiAdapter.GetNumberOfTimesMpiSendCalled());
	}

	void MapCallsMpiSendWithTheCorrectNumberOfEntriesForThePartitions()
	{
		std::vector<int> input;

		MockMpiAdapater mpiAdapter;

		const int unused_number_of_partitions = 13;
		auto runner = MpiMapReduce<std::vector<int>::iterator>(mpiAdapter, input.begin(), input.end(), one_partition, [](std::vector<int>::iterator iterator) { return 0; }, unused_number_of_partitions);

		runner.map();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MPI_Send method was not called with the correct count of elements based on the given number of partitions, which is not expected.", 2, mpiAdapter.GetCountInMpiSend());
	}

	void MapCallsMpiSendWithTheIntegerRepresentationOfTheStartIteratorForThePartition()
	{
		std::vector<int> input;
		input.emplace_back(67);
		input.emplace_back(92);

		MockMpiAdapater mpiAdapter;

		const int unused_number_of_partitions = 13;
		auto runner = MpiMapReduce<std::vector<int>::iterator>(mpiAdapter, input.begin(), input.end(), one_partition, [](std::vector<int>::iterator iterator) { return *iterator; }, unused_number_of_partitions);

		runner.map();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MPI_Send method was not called the correct first iterator value, which is not expected.", 67, mpiAdapter.GetFirstIteratorValueInMpiSend());
	}

	void MapCallsMpiSendWithTheIntegerRepresentationOfTheEndIteratorForThePartition()
	{
		std::vector<int> input;
		input.emplace_back(67);
		input.emplace_back(92);

		MockMpiAdapater mpiAdapter;

		const int unused_number_of_partitions = 13;
		auto runner = MpiMapReduce<std::vector<int>::iterator>(mpiAdapter, input.begin(), input.end(), one_partition, [](std::vector<int>::iterator iterator) { return *iterator; }, unused_number_of_partitions);

		runner.map();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The MPI_Send method was not called the correct end iterator value, which is not expected.", 92, mpiAdapter.GetSecondIteratorValueInMpiSend());
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
		MockMpiAdapater() : number_of_times_MPI_Send_called_(0)
		{}

		virtual void MpiSend(const void* buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm) const
		{
			++number_of_times_MPI_Send_called_;
			count_in_MPI_Send_ = count;

			const int* iterator_values = (const int*)buf;

			first_iterator_value_MPI_Send_ = iterator_values[0];
			second_iterator_value_MPI_Send_ = iterator_values[1];
		}

		int GetNumberOfTimesMpiSendCalled() const { return number_of_times_MPI_Send_called_; }
		int GetCountInMpiSend() const { return count_in_MPI_Send_; }
		int GetFirstIteratorValueInMpiSend() const { return first_iterator_value_MPI_Send_; }
		int GetSecondIteratorValueInMpiSend() const { return second_iterator_value_MPI_Send_; }

	private:
		mutable int number_of_times_MPI_Send_called_;
		mutable int count_in_MPI_Send_;
		mutable int first_iterator_value_MPI_Send_;
		mutable int second_iterator_value_MPI_Send_;
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