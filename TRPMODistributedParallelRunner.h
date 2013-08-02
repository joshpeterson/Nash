#ifndef __TRPMO_DISTRIBUTED_PARALLEL_RUNNER_H
#define __TRPMO_DISTRIBUTED_PARALLEL_RUNNER_H

#include "MyAssert.h"
#include "ISolutionMethod.h"
#include "MpiAdapter.h"
#include "MpiParallelTask.h"
#include "TRPMODistributedParallelTask.h"
#include "EvenPartitioningOfConsecutiveIntegers.h"
#include "ProgramOptions.h"

class TRPMODistributedParallelRunner : public ISolutionMethod
{
	// ISolutionMethod
    void Run(const ProgramOptions& options, std::ostream& out, int argc, char* argv[])
    {
        MYASSERT(options.GetMethod() == this->Name(), "Invalid call to ISolutionMethod::Run");

		MPI_Init(&argc, &argv);

        if (options.GetP1Strategy() == "" && options.GetP2Strategy() == "")
        {
            out << "Performing distributed parallel Nash solution categorization of all games of size " << options.GetRows() << "x" << options.GetColumns() <<"..." << std::endl;

			TRPMODistributedParallelTask task(options.GetRows(), options.GetColumns());
			MpiAdapter mpi;
            auto runner = MpiParallelTask<TRPMODistributedParallelTask>(task, mpi, 0, NumStrategies(options.GetRows(), options.GetColumns()), even_partitioning_of_consecutive_integers, 2);

			runner.start();
			runner.complete();
        }

		MPI_Finalize();
    }

    std::string Name() const
    {
        return "Nash Characterization Distributed Parallel";
    }
};

#endif // __TRPMO_DISTRIBUTED_PARALLEL_RUNNER_H