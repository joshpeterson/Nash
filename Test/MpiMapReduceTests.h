#include <cppunit/extensions/HelperMacros.h>

#include <vector>
#include <utility>
#include <functional>
#include <boost/noncopyable.hpp>

#include "../MpiMapReduce.h"

class MpiMapReduceTests : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(MpiMapReduceTests);
    CPPUNIT_TEST(MapCallsPartitioningFunction);
    CPPUNIT_TEST(MapCallsPartitioningFunctionWithBeginIterator);
    CPPUNIT_TEST(MapCallsPartitioningFunctionWithEndIterator);
    CPPUNIT_TEST(MapCallsPartitioningFunctionWithNumberOfPartitions);
    CPPUNIT_TEST_SUITE_END();

private:
	class PartitioningTracker;

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

		auto runner = MpiMapReduce<std::vector<int>::iterator>(input.begin(), input.end(), partitioning_method, 1);

		runner.map();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The partitioning method was not called, which is not expected.", true, tracker.GetPartitioningMethodCalled());
	}

	void MapCallsPartitioningFunctionWithBeginIterator()
	{
		PartitioningTracker tracker;

		auto partitioning_method = std::bind(&PartitioningTracker::partition, &tracker, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

		std::vector<int> input;

		auto runner = MpiMapReduce<std::vector<int>::iterator>(input.begin(), input.end(), partitioning_method, 1);

		runner.map();

		CPPUNIT_ASSERT_MESSAGE("The partitioning method was not called with the begin iterator, which is not expected.", input.begin() == tracker.GetBeginInterator());
	}

	void MapCallsPartitioningFunctionWithEndIterator()
	{
		PartitioningTracker tracker;

		auto partitioning_method = std::bind(&PartitioningTracker::partition, &tracker, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

		std::vector<int> input;

		auto runner = MpiMapReduce<std::vector<int>::iterator>(input.begin(), input.end(), partitioning_method, 1);

		runner.map();

		CPPUNIT_ASSERT_MESSAGE("The partitioning method was not called with the end iterator, which is not expected.", input.end() == tracker.GetEndInterator());
	}

	void MapCallsPartitioningFunctionWithNumberOfPartitions()
	{
		PartitioningTracker tracker;

		auto partitioning_method = std::bind(&PartitioningTracker::partition, &tracker, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

		std::vector<int> input;

		const int number_of_partitions = 13;
		auto runner = MpiMapReduce<std::vector<int>::iterator>(input.begin(), input.end(), partitioning_method, number_of_partitions);

		runner.map();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The partitioning method was not called with the number of partitions, which is not expected.",number_of_partitions, tracker.GetNumberOfPartitions());
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
};
