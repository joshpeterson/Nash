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
	MpiParallelTask(TaskType& task, const MpiAdapterInterface& mpi_interface, unsigned int begin, unsigned int end,
			     std::function<std::vector<std::pair<unsigned int, unsigned int>>(unsigned int, unsigned int, int)> partitioning_method,
				 int number_of_partitions) :
	task_(task), mpi_interface_(mpi_interface), begin_(begin), end_(end), partition_(partitioning_method), number_of_partitions_(number_of_partitions)
	{
	}

	void start()
	{
		auto rank = mpi_interface_.MpiCommRank(MPI_COMM_WORLD);

		if (rank == 0)
		{
			auto partitions = partition_(begin_, end_, number_of_partitions_);
			for (size_t i = 1; i < partitions.size(); ++i)
			{
				unsigned int iterator_values[] = {partitions[i].first, partitions[i].second};
				mpi_interface_.MpiSend(iterator_values, 2, MPI_UINT32_T, i, 0, MPI_COMM_WORLD);
			}

			if (!partitions.empty())
				task_.map(partitions[0].first, partitions[0].second);
		}
		else
		{
			MPI_Status status;
			std::vector<unsigned int> iterator_values(2);
			mpi_interface_.MpiRecv(&iterator_values[0], 2, MPI_UINT32_T, 0, 0, MPI_COMM_WORLD, &status);

			TaskType slave_task;
			slave_task.map(iterator_values[0], iterator_values[1]);

			auto result = slave_task.reduce(std::vector<unsigned int>());

			mpi_interface_.MpiSend(&result[0], result.size(), MPI_UINT32_T, 0, 0, MPI_COMM_WORLD);
		}
	}

	void complete()
	{
		auto rank = mpi_interface_.MpiCommRank(MPI_COMM_WORLD);

		if (rank == 0)
		{
			for (int slave_process_rank = 1; slave_process_rank < number_of_partitions_; ++slave_process_rank)
			{
				MPI_Status status;
				std::vector<unsigned int> output(3);
				mpi_interface_.MpiRecv(&output[0], 3, MPI_UINT32_T, slave_process_rank, 0, MPI_COMM_WORLD, &status);

				task_.reduce(output);
			}
		}
	}

private:
	TaskType& task_;
	const MpiAdapterInterface& mpi_interface_;
	int begin_;
	int end_;
	std::function<std::vector<std::pair<unsigned int, unsigned int>>(unsigned int, unsigned int, int)> partition_;
	int number_of_partitions_;
};

#endif // __MPI_MAP_REDUCE_H