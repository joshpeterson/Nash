#include <cppunit/extensions/HelperMacros.h>

#include <vector>
#include "../TRPMONashCategorizationDistributedParallel.h"

class TRPMONashCategorizationDistributedParallelTests : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TRPMONashCategorizationDistributedParallelTests);
    CPPUNIT_TEST(AccumulateResultsTreatsAnEmptyVectorAsAVectorOfZeros);
    CPPUNIT_TEST_SUITE_END();
	
public:
    void setUp()
    {
    }

    void tearDown()
    {
    }

	void AccumulateResultsTreatsAnEmptyVectorAsAVectorOfZeros()
	{
		TRPMONashCategorizationDistributedParallel comparator;
		comparator.AccumulateResults(std::vector<unsigned int>());

		CPPUNIT_ASSERT_EQUAL_MESSAGE("The number of unique nash solutions is not correct.", 0, comparator.GetNumUniqueNash());
		CPPUNIT_ASSERT_EQUAL_MESSAGE("The number of nonunique nash solutions is not correct.", 0, comparator.GetNumNonuniqueNash ());
		CPPUNIT_ASSERT_EQUAL_MESSAGE("The number of no nash solutions is not correct.", 0, comparator.GetNumNoNash());
	}
};