#ifndef __MPI_MAP_REDUCE_H
#define __MPI_MAP_REDUCE_H

#include <functional>
#include <utility>
#include <vector>

#include "MpiAdapterInterface.h"

template <typename TaskType>
class MpiParallelTask
{
public:
	MpiParallelTask(TaskType& task, const MpiAdapterInterface& mpi_interface, int begin, int end,
			     std::function<std::vector<std::pair<int, int>>(int, int, int)> partitioning_method,
				 int number_of_threads) :
	task_(task), mpi_interface_(mpi_interface), begin_(begin), end_(end), partition_(partitioning_method), number_of_threads_(number_of_threads)
	{
	}

	void start()
	{
		auto rank = mpi_interface_.MpiCommRank(MPI_COMM_WORLD);

		if (rank == 0)
		{
			auto partitions = partition_(begin_, end_, number_of_threads_);
			for (size_t i = 1; i < partitions.size(); ++i)
			{
				int iterator_values[] = {partitions[i].first, partitions[i].second};
				mpi_interface_.MpiSend(iterator_values, 2, MPI_INT, i, 0, MPI_COMM_WORLD);
			}

			if (!partitions.empty())
				task_.map(partitions[0].first, partitions[0].second);
		}
		else
		{
			MPI_Status status;
			int iterator_values[2];
			mpi_interface_.MpiRecv(iterator_values, 2, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

			TaskType slave_task;
			slave_task.map(iterator_values[0], iterator_values[1]);

			unsigned int numUnique = 0;
			unsigned int numNonunique = 0;
			unsigned int numNo = 0;

			slave_task.reduce(numUnique, numNonunique, numNo);

			int result_values[] = {numUnique, numNonunique, numNo};
		    mpi_interface_.MpiSend(result_values, 3, MPI_INT, 0, 0, MPI_COMM_WORLD);
		}

	}
private:
	TaskType& task_;
	const MpiAdapterInterface& mpi_interface_;
	int begin_;
	int end_;
	std::function<std::vector<std::pair<int, int>>(int, int, int)> partition_;
	int number_of_threads_;
};

#endif // __MPI_MAP_REDUCE_H