#include <iostream>
#include <iterator>
#include <ctime>
#include <tbb/task_scheduler_init.h>
#include <tbb/tick_count.h>
#include <boost/shared_ptr.hpp>
#include "CardinalStrategy.h"
#include "TRPMOStrategy.h"
#include "PrintStrategy.h"
#include "Game.h"
#include "RankOrder.h"
#include "TRPMOParallelIterator.h"
#include "TRPMONashCategorizationParallel.h"
#include "TRPMOSerialIterator.h"
#include "TRPMONashCategorization.h"
#include "NashMonteCarloSerial.h"
#include "NashMonteCarloParallel.h"
#include "TBBTimer.h"
#include "ProgramOptions.h"
#include "ISolutionMethod.h"
#include "TRPMODistributedParallelRunner.h"

using namespace std;
namespace po = boost::program_options;

int main(int argc, char* argv[])
{
    std::vector<ISolutionMethod*> solvers;

    TRPMONashCategorization nashCategorizationSerial;
    solvers.push_back(&nashCategorizationSerial);

    TRPMONashCategorizationParallel nashCategorizationParallel;
    solvers.push_back(&nashCategorizationParallel);

	TRPMODistributedParallelRunner nashCategorizationDistributedParallel;
    solvers.push_back(&nashCategorizationDistributedParallel);
    
    ProgramOptions options(argc, argv, solvers);

    for (std::vector<ISolutionMethod*>::const_iterator it = solvers.begin(); it != solvers.end(); ++it)
    {
        if (options.GetMethod() == (*it)->Name())
        {
            (*it)->Run(options, std::cout, argc, argv);
            break;
        }
    }
    
    return 0;
}
