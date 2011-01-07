#include <cppunit/extensions/HelperMacros.h>

#include "../CardinalStrategy.h"
#include "../MinimalOrdinalStrategy.h"
#include "../TRPMOStrategy.h"
#include "../RankOrder.h"

class TRPMOStrategyTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TRPMOStrategyTest);
    CPPUNIT_TEST(testRowRankOrder);
    CPPUNIT_TEST(testColumnRankOrder);
    CPPUNIT_TEST(testStrategyIteratorRowRankOrderIncrement);
    CPPUNIT_TEST(testStrategyIteratorRowRankOrderDecrement);
    CPPUNIT_TEST(testStrategyIteratorColumnRankOrderIncrement);
    CPPUNIT_TEST(testStrategyIteratorColumnRankOrderDecrement);
    CPPUNIT_TEST(testStrategyIteratorIdentity);
    CPPUNIT_TEST(testStrategyIteratorState);
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
	    int verify[9] = {0, 0, 1, 1, 0, 0, 0, 1, 0};
	    CardinalStrategy strategy(3,3, test, test+9);

	    TRPMOStrategy<RowRankOrder> TRPMOStrategy(strategy);

	    CPPUNIT_ASSERT(std::equal(verify, verify+9, TRPMOStrategy.RowWiseBegin()));
    }

    void testColumnRankOrder()
    {
	    double test[9] = {9., 5., 3., 2., 4., 7., 8., 1., 6.};
	    int verify[9] = {0, 0, 1, 1, 0, 0, 0, 1, 0};
	    CardinalStrategy strategy(3,3, test, test+9);

	    TRPMOStrategy<ColumnRankOrder> TRPMOStrategy(strategy);

	    CPPUNIT_ASSERT(std::equal(verify, verify+9, TRPMOStrategy.RowWiseBegin()));
    }

    void testStrategyIteratorRowRankOrderIncrement()
    {
	    int verify[4][4] = {{1,0,1,0}, {0,1,1,0}, {1,0,0,1}, {0,1,0,1}};
	    TRPMOStrategy<RowRankOrder> strategy(2,2);

	    int idx = 0;
	    for (TRPMOStrategy<RowRankOrder>::strategyIterator it = strategy.StrategyBegin();
		    it != strategy.StrategyEnd(); ++it, ++idx)
	        CPPUNIT_ASSERT(std::equal(verify[idx], verify[idx]+4, it->RowWiseBegin()));
    }

    void testStrategyIteratorRowRankOrderDecrement()
    {
	    int verify[4][4] = {{0,1,0,1}, {1,0,0,1},  {0,1,1,0}, {1,0,1,0}};
	    TRPMOStrategy<RowRankOrder> strategy(2,2);

	    TRPMOStrategy<RowRankOrder>::strategyIterator it = strategy.StrategyEnd();
	    --it;
	    CPPUNIT_ASSERT(std::equal(verify[0], verify[0]+4, it->RowWiseBegin()));
	    --it;
	    CPPUNIT_ASSERT(std::equal(verify[1], verify[1]+4, it->RowWiseBegin()));
	    --it;
	    CPPUNIT_ASSERT(std::equal(verify[2], verify[2]+4, it->RowWiseBegin()));
	    --it;
	    CPPUNIT_ASSERT(std::equal(verify[3], verify[3]+4, it->RowWiseBegin()));
    }

    void testStrategyIteratorColumnRankOrderIncrement()
    {
	    int verify[4][4] = {{1,1,0,0}, {0,1,1,0}, {1,0,0,1}, {0,0,1,1}};
	    TRPMOStrategy<ColumnRankOrder> strategy(2,2);

	    int idx = 0;
	    for (TRPMOStrategy<ColumnRankOrder>::strategyIterator it = strategy.StrategyBegin();
		    it != strategy.StrategyEnd(); ++it, ++idx)
	        CPPUNIT_ASSERT(std::equal(verify[idx], verify[idx]+4, it->RowWiseBegin()));
    }

    void testStrategyIteratorColumnRankOrderDecrement()
    {
	    int verify[4][4] = {{0,0,1,1}, {1,0,0,1},  {0,1,1,0}, {1,1,0,0}};
	    TRPMOStrategy<ColumnRankOrder> strategy(2,2);

	    TRPMOStrategy<ColumnRankOrder>::strategyIterator it = strategy.StrategyEnd();
	    --it;
	    CPPUNIT_ASSERT(std::equal(verify[0], verify[0]+4, it->RowWiseBegin()));
	    --it;
	    CPPUNIT_ASSERT(std::equal(verify[1], verify[1]+4, it->RowWiseBegin()));
	    --it;
	    CPPUNIT_ASSERT(std::equal(verify[2], verify[2]+4, it->RowWiseBegin()));
	    --it;
	    CPPUNIT_ASSERT(std::equal(verify[3], verify[3]+4, it->RowWiseBegin()));
    }

    void testStrategyIteratorIdentity()
    {
	    TRPMOStrategy<ColumnRankOrder> strategy(3,3);

	    TRPMOStrategy<ColumnRankOrder>::strategyIterator verify_it = strategy.StrategyBegin();

	    int idx = 0;
	    for (TRPMOStrategy<ColumnRankOrder>::strategyIterator it = strategy.StrategyBegin();
		    it != strategy.StrategyEnd(); ++it, ++idx)
	        CPPUNIT_ASSERT(std::equal(it->RowWiseBegin(), it->RowWiseEnd(), 
					                    (verify_it+idx)->RowWiseBegin()));

	    int initStrategy[9] = {0, 0, 1, 1, 0, 0, 0, 1, 0};
	    std::copy(initStrategy, initStrategy+9, strategy.RowWiseBegin());

	    TRPMOStrategy<ColumnRankOrder>::strategyIterator test_it(strategy);
	    CPPUNIT_ASSERT(std::equal(initStrategy, initStrategy+9, test_it->RowWiseBegin()));
    }

    void testStrategyIteratorState()
    {
	    TRPMOStrategy<ColumnRankOrder> strategy(3,3);
	    TRPMOStrategy<ColumnRankOrder>::strategyIterator it = strategy.StrategyBegin();
	    TRPMOStrategy<ColumnRankOrder>::strategyIterator test_it = it+4;
	    TRPMOStrategy<ColumnRankOrder>::strategyIterator verify_it = strategy.StrategyBegin();

	    CPPUNIT_ASSERT(std::equal(it->RowWiseBegin(), it->RowWiseEnd(), verify_it->RowWiseBegin()));
    }

    void testIteratorConstructor()
    {
	    int initStrategy[9] = {1,0,0,0,0,0,0,1,1};
	    TRPMOStrategy<ColumnRankOrder> testStrategy(3,3, initStrategy, initStrategy+9);

	    CPPUNIT_ASSERT(std::equal(testStrategy.RowWiseBegin(), testStrategy.RowWiseEnd(), initStrategy));

	    try
	    {
	        int initStrategy2[9] = {0,1,0,0,0,1,0,0,1};
	        MinimalOrdinalStrategy<ColumnRankOrder> testStrategy2(3,3, initStrategy2, initStrategy2+9);
	    }
	    catch(...)
	    {
	        return;
	    }
	    CPPUNIT_ASSERT(0 && "TRPMOStrategy iterator constructor failed to catch invalid initialization.");
    }
};
