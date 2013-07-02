#include <cppunit/extensions/HelperMacros.h>

#include "../EvenPartitioningOfConsecutiveIntegers.h"

class EvenPartitioningOfConsecutiveIntegersTests : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(EvenPartitioningOfConsecutiveIntegersTests);
    CPPUNIT_TEST(ReturnsNoPartitionsWhenBeginAndEndAreTheSame);
    CPPUNIT_TEST(SplitsAnEvenNumberOfIntegersIntoAnEvenNumberOfPartitions);
    CPPUNIT_TEST(ReturnsTheCorrectFirstPartitionForAnEvenNumberOfIntegersAndAnEvenNumberOfPartitions);
    CPPUNIT_TEST(ReturnsTheCorrectSecondPartitionForAnEvenNumberOfIntegersAndAnEvenNumberOfPartitions);
    CPPUNIT_TEST(SplitsAnEvenNumberOfIntegersIntoAnOddNumberOfPartitions);
    CPPUNIT_TEST(ReturnsTheCorrectFirstPartitionForAnEvenNumberOfIntegersAndAnOddNumberOfPartitions);
    CPPUNIT_TEST(ReturnsTheCorrectSecondPartitionForAnEvenNumberOfIntegersAndAnOddNumberOfPartitions);
    CPPUNIT_TEST(ReturnsTheCorrectThirdPartitionForAnEvenNumberOfIntegersAndAnOddNumberOfPartitions);
    CPPUNIT_TEST(ReturnsTheCorrectThirdPartitionForAnOddNumberOfIntegersAndAnOddNumberOfPartitions);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp()
    {
    }

    void tearDown()
    {
    }

	void ReturnsNoPartitionsWhenBeginAndEndAreTheSame()
	{
		auto partitions = even_partitioning_of_consecutive_integers(42, 42, 12);

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The even partitioning returned at least one paritition when begin and end are the same, which is not expected.", 0U, partitions.size());
	}

	void SplitsAnEvenNumberOfIntegersIntoAnEvenNumberOfPartitions()
	{
		auto partitions = even_partitioning_of_consecutive_integers(1, 4, 2);

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The number of partitions is not 2, which is not expected.", 2U, partitions.size());
	}

	void ReturnsTheCorrectFirstPartitionForAnEvenNumberOfIntegersAndAnEvenNumberOfPartitions()
	{
		auto partitions = even_partitioning_of_consecutive_integers(1, 4, 2);

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The begin value of the first partition is not 1, which is not expected.", 1U, partitions[0].first);
		CPPUNIT_ASSERT_EQUAL_MESSAGE("The end value of the first partition is not 2, which is not expected.", 2U, partitions[0].second);
	}

	void ReturnsTheCorrectSecondPartitionForAnEvenNumberOfIntegersAndAnEvenNumberOfPartitions()
	{
		auto partitions = even_partitioning_of_consecutive_integers(10, 30, 4);

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The begin value of the second partition is not 15, which is not expected.", 15U, partitions[1].first);
		CPPUNIT_ASSERT_EQUAL_MESSAGE("The end value of the second partition is not 19, which is not expected.", 19U, partitions[1].second);
	}

	void SplitsAnEvenNumberOfIntegersIntoAnOddNumberOfPartitions()
	{
		auto partitions = even_partitioning_of_consecutive_integers(1, 10, 3);

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The number of partitions is not 3, which is not expected.", 3U, partitions.size());
	}

	void ReturnsTheCorrectFirstPartitionForAnEvenNumberOfIntegersAndAnOddNumberOfPartitions()
	{
		auto partitions = even_partitioning_of_consecutive_integers(1, 10, 3);

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The begin value of the first partition is not 1, which is not expected.", 1U, partitions[0].first);
		CPPUNIT_ASSERT_EQUAL_MESSAGE("The end value of the first partition is not 3, which is not expected.", 3U, partitions[0].second);
	}

	void ReturnsTheCorrectSecondPartitionForAnEvenNumberOfIntegersAndAnOddNumberOfPartitions()
	{
		auto partitions = even_partitioning_of_consecutive_integers(1, 10, 3);

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The begin value of the first partition is not 4, which is not expected.", 4U, partitions[1].first);
		CPPUNIT_ASSERT_EQUAL_MESSAGE("The end value of the first partition is not 6, which is not expected.", 6U, partitions[1].second);
	}

	void ReturnsTheCorrectThirdPartitionForAnEvenNumberOfIntegersAndAnOddNumberOfPartitions()
	{
		auto partitions = even_partitioning_of_consecutive_integers(1, 10, 3);

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The begin value of the first partition is not 7, which is not expected.", 7U, partitions[2].first);
		CPPUNIT_ASSERT_EQUAL_MESSAGE("The end value of the first partition is not 10, which is not expected.", 10U, partitions[2].second);
	}

		void ReturnsTheCorrectThirdPartitionForAnOddNumberOfIntegersAndAnOddNumberOfPartitions()
	{
		auto partitions = even_partitioning_of_consecutive_integers(1, 11, 3);

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The begin value of the first partition is not 7, which is not expected.", 7U, partitions[2].first);
		CPPUNIT_ASSERT_EQUAL_MESSAGE("The end value of the first partition is not 11, which is not expected.", 11U, partitions[2].second);
	}
};
	