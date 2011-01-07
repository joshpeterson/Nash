#include <cppunit/extensions/HelperMacros.h>

#include "../CardinalStrategy.h"
#include "../MinimalOrdinalStrategy.h"
#include "../RandomUniform.h"
#include "../RankOrder.h"

class MinimalOrdinalStrategyTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(MinimalOrdinalStrategyTest);
    CPPUNIT_TEST(testRowRankOrder);
    CPPUNIT_TEST(testColumnRankOrder);
    CPPUNIT_TEST(testIteratorConstructor);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp()
    {
    }

    void tearDown()
    {
    }

    void testRowRankOrder()
    {
	    double test[9] = {9., 5., 3., 2., 4., 7., 8., 1., 6.};
	    int verify[9] = {3, 2, 1, 1, 2, 3, 3, 1, 2};
	    CardinalStrategy strategy(3,3, test, test+9);

	    MinimalOrdinalStrategy<RowRankOrder> MOStrategy(strategy);

	    CPPUNIT_ASSERT(std::equal(verify, verify+9, MOStrategy.RowWiseBegin()));
    }

    void testColumnRankOrder()
    {
	    double test[9] = {9., 5., 3., 2., 4., 7., 8., 1., 6.};
	    int verify[9] = {3, 3, 1, 1, 2, 3, 2, 1, 2};
	    CardinalStrategy strategy(3,3, test, test+9);

	    MinimalOrdinalStrategy<ColumnRankOrder> MOStrategy(strategy);

	    CPPUNIT_ASSERT(std::equal(verify, verify+9, MOStrategy.RowWiseBegin()));
    }

    void testIteratorConstructor()
    {
	    int initStrategy[9] = {1,1,1,2,2,2,3,3,3};
	    MinimalOrdinalStrategy<ColumnRankOrder> testStrategy(3,3, initStrategy, initStrategy+9);

	    CPPUNIT_ASSERT(std::equal(testStrategy.RowWiseBegin(), testStrategy.RowWiseEnd(), initStrategy));

	    try
	    {
	        int initStrategy2[9] = {1,1,1,2,2,2,3,1,3};
	        MinimalOrdinalStrategy<ColumnRankOrder> testStrategy2(3,3, initStrategy2, initStrategy2+9);
	    }
	    catch(...)
	    {
	        try
	        {
		        int initStrategy3[9] = {1,1,1,2,2,4,3,3,3};
		        MinimalOrdinalStrategy<ColumnRankOrder> testStrategy3(3,3, initStrategy3, initStrategy3+9);
	        }
	        catch(...)
	        {
		        return;
	        }
	    }
	    CPPUNIT_ASSERT(0 && "MinimalOrdinalStrategy iterator constructor failed to catch invalid initialization.");
    }
};
