#include <cppunit/extensions/HelperMacros.h>

#include "../CardinalStrategy.h"
#include "../RandomUniform.h"

class CardinalStrategyTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(CardinalStrategyTest);
    CPPUNIT_TEST(testRandomUniformRange);
    CPPUNIT_TEST(testRowWiseIterator);
    CPPUNIT_TEST(testColumnWiseIterator);
    CPPUNIT_TEST(testIteratorConstructor);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp()
    {
    }

    void tearDown()
    {
    }

    void testRandomUniformRange()
    {
        RandomUniform randGen(0.0, 1.0);
	    CardinalStrategy strategy4by4(4,4);
	    strategy4by4.RandomizeStrategy(&randGen);
	    CardinalStrategy::RowWiseIterator it;
	    for (it = strategy4by4.RowWiseBegin(); it != strategy4by4.RowWiseEnd();
		    ++it)
	        CPPUNIT_ASSERT((*it >= 0.0) && (*it < 1.0));
    }

    void testRowWiseIterator()
    {
	    CardinalStrategy strategy4by4(4,4);
	    std::vector<double> init(16);
	    RandomUniform randGen(0.0, 1.0);

	    std::for_each(init.begin(), init.end(), randGen);
	    std::copy(init.begin(), init.end(), strategy4by4.RowWiseBegin());
	    CPPUNIT_ASSERT(std::equal(init.begin(), init.end(), 
			    strategy4by4.RowWiseBegin()));
    }

    void testColumnWiseIterator()
    {
	    CardinalStrategy strategy3by3(3,3);
	    std::vector<double> rowWise(9), columnWise(9);

        RandomUniform randGen(0.0, 1.0);
	    strategy3by3.RandomizeStrategy(&randGen);

	    std::copy(strategy3by3.RowWiseBegin(), strategy3by3.RowWiseEnd(),
		        rowWise.begin());

	    std::copy(strategy3by3.ColumnWiseBegin(), strategy3by3.ColumnWiseEnd(),
		        columnWise.begin());

	    CPPUNIT_ASSERT(rowWise[0] == columnWise[0]);
	    CPPUNIT_ASSERT(rowWise[1] == columnWise[3]);
	    CPPUNIT_ASSERT(rowWise[2] == columnWise[6]);
	    CPPUNIT_ASSERT(rowWise[3] == columnWise[1]);
	    CPPUNIT_ASSERT(rowWise[4] == columnWise[4]);
	    CPPUNIT_ASSERT(rowWise[5] == columnWise[7]);
	    CPPUNIT_ASSERT(rowWise[6] == columnWise[2]);
	    CPPUNIT_ASSERT(rowWise[7] == columnWise[5]);
	    CPPUNIT_ASSERT(rowWise[8] == columnWise[8]);
    }

    void testIteratorConstructor()
    {
	    std::vector<double> test(16);
	    std::generate(test.begin(), test.end(), rand);
	    CardinalStrategy testStrategy(4, 4, test.begin(), test.end());
	    CPPUNIT_ASSERT(std::equal(testStrategy.RowWiseBegin(), testStrategy.RowWiseEnd(), 
			    test.begin()));
    }
};
