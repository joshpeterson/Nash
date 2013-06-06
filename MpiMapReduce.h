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
				 int number_of_threads) :
	mpi_interface_(mpi_interface), begin_(begin), end_(end), partitioning_method_(partitioning_method), number_of_threads_(number_of_threads)
	{
	}

	void map()
	{
		auto partitions = partitioning_method_(begin_, end_, number_of_threads_);
		for (auto partition : partitions)
			mpi_interface_.MpiSend(NULL, 0, MPI_INT, 0, 0, MPI_COMM_WORLD);

	}
private:
	const MpiAdapterInterface& mpi_interface_;
	IteratorType begin_;
	IteratorType end_;
	std::function<std::vector<std::pair<IteratorType, IteratorType>>(IteratorType, IteratorType, int)> partitioning_method_;
	int number_of_threads_;
};

#endif // __MPI_MAP_REDUCE_H