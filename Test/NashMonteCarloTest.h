#include <cppunit/extensions/HelperMacros.h>

#include <tbb/task_scheduler_init.h>
#include "../NashMonteCarloSerial.h"
#include "../NashMonteCarloParallel.h"

class NashMonteCarloTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(NashMonteCarloTest);
    CPPUNIT_TEST(testNashMonteCarloSerial);
    CPPUNIT_TEST(testNashMonteCarloParallel);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp() {}
    void tearDown() {}

    void testNashMonteCarloSerial()
    {
        NashMonteCarloSerial simulation(3, 3, 1000, 0.0, 1.0, 1208107925);
        simulation.Run();

        CPPUNIT_ASSERT(simulation.GetNumUniqueNash() == 15705);
        CPPUNIT_ASSERT(simulation.GetNumNonuniqueNash() == 5533);
        CPPUNIT_ASSERT(simulation.GetNumNoNash() == 5762);
    }

    void testNashMonteCarloParallel()
    {
        tbb::task_scheduler_init init;

        NashMonteCarloParallel simulation(3, 3, 1000, 0.0, 1.0, 1208107925);
        simulation.Run();

        CPPUNIT_ASSERT(simulation.GetNumUniqueNash() == 15705);
        CPPUNIT_ASSERT(simulation.GetNumNonuniqueNash() == 5533);
        CPPUNIT_ASSERT(simulation.GetNumNoNash() == 5762);
    }
};
