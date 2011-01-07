#include <cppunit/extensions/HelperMacros.h>

#include "../RandomUniform.h"

class RandomUniformTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(RandomUniformTest);
    CPPUNIT_TEST(testRandomness);
    CPPUNIT_TEST(testRange);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp()
    {
    }

    void tearDown()
    {
    }

    void testRandomness()
    {
	    double lastRandomNumnber;
	    RandomUniform randGen(0.0, 100.0);
	    randGen(lastRandomNumnber);
	    for (int i = 0; i < 100; ++i)
	    {
	        double newRandomUniform;
	        randGen(newRandomUniform);
	        CPPUNIT_ASSERT(lastRandomNumnber != newRandomUniform);
	        lastRandomNumnber = newRandomUniform;
	    }
    }

    void testRange()
    {
	    double lowerBound = 0.0;
	    double upperBound = 100.0;
	    double randomNumnber;
	    RandomUniform randGen(lowerBound, upperBound);
	    for (int i = 0; i < 100; ++i)
	    {
	        randGen(randomNumnber);
	        CPPUNIT_ASSERT(randomNumnber >= lowerBound && randomNumnber < upperBound);
	    }
    }
};
