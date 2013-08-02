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
    
    /*tbb::task_scheduler_init init;

    TBBTimer timer;
    NashMonteCarloParallel simulationP(3, 3, 1000000, 0.0, 1.0, 1208107925);
    NashMonteCarloSerial simulationS(3, 3, 1000000, 0.0, 1.0, 1208107925);

    TIME_OPERATION(timer, simulationP.Run())
    simulationP.DisplayResults(cout);
    cout << timer.ElapsedSeconds() << endl;

    TIME_OPERATION(timer, simulationS.Run())
    simulationS.DisplayResults(cout);
    cout << timer.ElapsedSeconds() << endl;*/

    //int size = 5;
    ////for (int size = 2; size < 5; size++)
    //{
    //    cout << "Size: " << size << endl << endl;
    //    cout << "Serial:" << endl;
    //    GameDouble< TRPMOSerialIterator<TRPMONashCategorization> > gameSerial(size,size);

    //    tbb::tick_count t0Serial = tbb::tick_count::now();
    //    gameSerial.IterateStrategies();
    //    tbb::tick_count t1Serial = tbb::tick_count::now();

    //    gameSerial.DisplayResults(cout);
    //    cout << "Run took " << (t1Serial-t0Serial).seconds() << " seconds." << endl;

    //    cout << endl;

    //    /*cout << "Parallel with TBB:" << endl;
    //    GameDouble< TRPMOParallelIterator<TRPMONashCategorizationParallel> > game(size,size);

    //    tbb::tick_count t0 = tbb::tick_count::now();
    //    game.IterateStrategies();
    //    tbb::tick_count t1 = tbb::tick_count::now();

    //    game.DisplayResults(cout);
    //    cout << "Run took " << (t1-t0).seconds() << " seconds." << endl;

    //    cout << endl;*/
    //}
    return 0;
}
