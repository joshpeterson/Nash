#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

#include "CardinalStrategyTest.h"
#include "MinimalOrdinalStrategyTest.h"
#include "TRPMOStrategyTest.h"
#include "RandomUniformTest.h"
#include "GameTest.h"
#include "NashMonteCarloTest.h"
#include "MpiParallelTaskTests.h"
#include "EvenPartitioningOfConsecutiveIntegersTests.h"
#include "TRPMODistributedParallelTaskTests.h"
#include "TRPMONashCategorizationDistributedParallelTests.h"

CPPUNIT_TEST_SUITE_REGISTRATION(CardinalStrategyTest);
CPPUNIT_TEST_SUITE_REGISTRATION(MinimalOrdinalStrategyTest);
CPPUNIT_TEST_SUITE_REGISTRATION(TRPMOStrategyTest);
CPPUNIT_TEST_SUITE_REGISTRATION(RandomUniformTest);
CPPUNIT_TEST_SUITE_REGISTRATION(GameTest);
CPPUNIT_TEST_SUITE_REGISTRATION(NashMonteCarloTest);
CPPUNIT_TEST_SUITE_REGISTRATION(MpiParallelTaskTests);
CPPUNIT_TEST_SUITE_REGISTRATION(EvenPartitioningOfConsecutiveIntegersTests);
CPPUNIT_TEST_SUITE_REGISTRATION(TRPMODistributedParallelTaskTests);
CPPUNIT_TEST_SUITE_REGISTRATION(TRPMONashCategorizationDistributedParallelTests);

int main(int argc, char** argv)
{
    CppUnit::TextUi::TestRunner runner;
    CppUnit::TestFactoryRegistry &registry = 
	CppUnit::TestFactoryRegistry::getRegistry();

    runner.addTest(registry.makeTest());
    bool testStatus = runner.run("", false);

    return testStatus ? 0 : 1;
}
