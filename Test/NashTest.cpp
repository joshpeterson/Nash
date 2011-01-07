#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

#include "CardinalStrategyTest.h"
#include "MinimalOrdinalStrategyTest.h"
#include "TRPMOStrategyTest.h"
#include "RandomUniformTest.h"
#include "GameTest.h"
#include "NashMonteCarloTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION(CardinalStrategyTest);
CPPUNIT_TEST_SUITE_REGISTRATION(MinimalOrdinalStrategyTest);
CPPUNIT_TEST_SUITE_REGISTRATION(TRPMOStrategyTest);
CPPUNIT_TEST_SUITE_REGISTRATION(RandomUniformTest);
CPPUNIT_TEST_SUITE_REGISTRATION(GameTest);
CPPUNIT_TEST_SUITE_REGISTRATION(NashMonteCarloTest);

int main(int argc, char** argv)
{
    CppUnit::TextUi::TestRunner runner;
    CppUnit::TestFactoryRegistry &registry = 
	CppUnit::TestFactoryRegistry::getRegistry();

    runner.addTest(registry.makeTest());
    bool testStatus = runner.run("", false);

    return testStatus ? 0 : 1;
}
