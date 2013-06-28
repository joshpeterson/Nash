#include <cppunit/extensions/HelperMacros.h>

#include "../EvenPartitioningOfConsecutiveIntegers.h"

class EvenPartitioningOfConsecutiveIntegersTests : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(EvenPartitioningOfConsecutiveIntegersTests);
    CPPUNIT_TEST(ReturnsNoPartitionsWhenBeginAndEndAreTheSame);
    CPPUNIT_TEST(SplitsAnEvenNumberOfIntegersIntoAnEvenNumberOfPartitions);
    CPPUNIT_TEST(ReturnsTheCorrectFirstPartitionForAnEvenNumberOfIntegersAndAnEvenNumberOfPartitions);
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
		CPPUNIT_ASSERT_EQUAL_MESSAGE("The end value of the first partition is not 1, which is not expected.", 2U, partitions[0].second);
	}
};
	