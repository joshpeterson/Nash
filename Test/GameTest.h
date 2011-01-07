#include <cppunit/extensions/HelperMacros.h>

#include <tbb/task_scheduler_init.h>
#include "../TRPMOStrategy.h"
#include "../Game.h"
#include "../TRPMOSerialIterator.h"
#include "../RankOrder.h"
#include "../TRPMONashCategorization.h"
#include "../TRPMOParallelIterator.h"
#include "../TRPMONashCategorizationParallel.h"

class GameTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(GameTest);
    CPPUNIT_TEST(testTRPMOSerialIteratorP1Strategy);
    CPPUNIT_TEST(testTRPMOParallelIteratorP1Strategy);
    CPPUNIT_TEST(testTRPMOSerialIteratorP2Strategy);
    CPPUNIT_TEST(testTRPMOParallelIteratorP2Strategy);
    CPPUNIT_TEST(testGameDoubleSerial);
    CPPUNIT_TEST(testGameDoubleParallel);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp()
    {
    }

    void tearDown()
    {
    }

    void testTRPMOSerialIteratorP1Strategy()
    {
	    int initStrategy[9] = {1,0,0,0,0,0,0,1,1};
	    TRPMOStrategy<ColumnRankOrder> p1Strategy(3,3, initStrategy, initStrategy+9);

	    Game<TRPMOStrategy<ColumnRankOrder>, TRPMOSerialIterator<TRPMONashCategorization> > game(p1Strategy);

	    game.IterateStrategies();

	    CPPUNIT_ASSERT(game.Iterator().Comparator().GetNumUniqueNash() == 15);
	    CPPUNIT_ASSERT(game.Iterator().Comparator().GetNumNonuniqueNash() == 6);
	    CPPUNIT_ASSERT(game.Iterator().Comparator().GetNumNoNash() == 6);
    }

    void testTRPMOParallelIteratorP1Strategy()
    {
        tbb::task_scheduler_init init;
	    int initStrategy[9] = {1,0,0,0,0,0,0,1,1};
	    TRPMOStrategy<ColumnRankOrder> p1Strategy(3,3, initStrategy, initStrategy+9);

	    Game<TRPMOStrategy<ColumnRankOrder>, TRPMOParallelIterator<TRPMONashCategorizationParallel> > game(p1Strategy);

	    game.IterateStrategies();

	    CPPUNIT_ASSERT(game.Iterator().Comparator().GetNumUniqueNash() == 15);
	    CPPUNIT_ASSERT(game.Iterator().Comparator().GetNumNonuniqueNash() == 6);
	    CPPUNIT_ASSERT(game.Iterator().Comparator().GetNumNoNash() == 6);
    }

    void testTRPMOSerialIteratorP2Strategy()
    {
	    int initStrategy[9] = {1,0,0,1,0,0,0,0,1};
	    TRPMOStrategy<RowRankOrder> p2Strategy(3,3, initStrategy, initStrategy+9);

	    Game<TRPMOStrategy<RowRankOrder>, TRPMOSerialIterator<TRPMONashCategorization> > game(p2Strategy);

	    game.IterateStrategies();

	    CPPUNIT_ASSERT(game.Iterator().Comparator().GetNumUniqueNash() == 15);
	    CPPUNIT_ASSERT(game.Iterator().Comparator().GetNumNonuniqueNash() == 6);
	    CPPUNIT_ASSERT(game.Iterator().Comparator().GetNumNoNash() == 6);
    }

    void testTRPMOParallelIteratorP2Strategy()
    {
        tbb::task_scheduler_init init;
	    int initStrategy[9] = {1,0,0,1,0,0,0,0,1};
	    TRPMOStrategy<RowRankOrder> p2Strategy(3,3, initStrategy, initStrategy+9);

	    Game<TRPMOStrategy<RowRankOrder>, TRPMOParallelIterator<TRPMONashCategorizationParallel> > game(p2Strategy);

	    game.IterateStrategies();

	    CPPUNIT_ASSERT(game.Iterator().Comparator().GetNumUniqueNash() == 15);
	    CPPUNIT_ASSERT(game.Iterator().Comparator().GetNumNonuniqueNash() == 6);
	    CPPUNIT_ASSERT(game.Iterator().Comparator().GetNumNoNash() == 6);
    }

    void testGameDoubleSerial()
    {
        GameDouble< TRPMOSerialIterator<TRPMONashCategorization> > game2x2(2,2);

	    game2x2.IterateStrategies();

	    CPPUNIT_ASSERT(game2x2.Iterator().Comparator().GetNumUniqueNash() == 12);
	    CPPUNIT_ASSERT(game2x2.Iterator().Comparator().GetNumNonuniqueNash() == 2);
	    CPPUNIT_ASSERT(game2x2.Iterator().Comparator().GetNumNoNash() == 2);

        GameDouble< TRPMOSerialIterator<TRPMONashCategorization> > game3x3(3,3);

	    game3x3.IterateStrategies();

	    CPPUNIT_ASSERT(game3x3.Iterator().Comparator().GetNumUniqueNash() == 423);
	    CPPUNIT_ASSERT(game3x3.Iterator().Comparator().GetNumNonuniqueNash() == 150);
	    CPPUNIT_ASSERT(game3x3.Iterator().Comparator().GetNumNoNash() == 156);
    }

    void testGameDoubleParallel()
    {
        tbb::task_scheduler_init init;
        GameDouble< TRPMOParallelIterator<TRPMONashCategorizationParallel> > game2x2(2,2);

	    game2x2.IterateStrategies();

	    CPPUNIT_ASSERT(game2x2.Iterator().Comparator().GetNumUniqueNash() == 12);
	    CPPUNIT_ASSERT(game2x2.Iterator().Comparator().GetNumNonuniqueNash() == 2);
	    CPPUNIT_ASSERT(game2x2.Iterator().Comparator().GetNumNoNash() == 2);

        GameDouble< TRPMOParallelIterator<TRPMONashCategorizationParallel> > game3x3(3,3);

	    game3x3.IterateStrategies();

	    CPPUNIT_ASSERT(game3x3.Iterator().Comparator().GetNumUniqueNash() == 423);
	    CPPUNIT_ASSERT(game3x3.Iterator().Comparator().GetNumNonuniqueNash() == 150);
	    CPPUNIT_ASSERT(game3x3.Iterator().Comparator().GetNumNoNash() == 156);
    }
};
