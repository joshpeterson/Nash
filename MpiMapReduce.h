#ifndef __MPI_MAP_REDUCE_H
#define __MPI_MAP_REDUCE_H

#include <functional>
#include <utility>
#include <vector>

#include "MpiAdapterInterface.h"

template <typename IteratorType>
class MpiMapReduce
{
public:
	MpiMapReduce(const MpiAdapterInterface& mpi_interface, IteratorType begin, IteratorType end,
			     std::function<std::vector<std::pair<IteratorType, IteratorType>>(IteratorType, IteratorType, int)> partitioning_method,
				 std::function<int(IteratorType)> convert_to_integer,
				 int number_of_threads) :
	mpi_interface_(mpi_interface), begin_(begin), end_(end), partition_(partitioning_method), convert_to_integer_(convert_to_integer),
	number_of_threads_(number_of_threads)
	{
	}

	void map()
	{
		auto partitions = partition_(begin_, end_, number_of_threads_);
		for (std::pair<IteratorType, IteratorType> partition : partitions)
		{
			int iterator_values[] = {convert_to_integer_(partition.first), convert_to_integer_(partition.second)};
			mpi_interface_.MpiSend(iterator_values, 2, MPI_INT, 0, 0, MPI_COMM_WORLD);
		}

	}
private:
	const MpiAdapterInterface& mpi_interface_;
	IteratorType begin_;
	IteratorType end_;
	std::function<std::vector<std::pair<IteratorType, IteratorType>>(IteratorType, IteratorType, int)> partition_;
	std::function<int(IteratorType)> convert_to_integer_;
	int number_of_threads_;
};

#endif // __MPI_MAP_REDUCE_H