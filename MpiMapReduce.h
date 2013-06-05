#ifndef __MPI_MAP_REDUCE_H
#define __MPI_MAP_REDUCE_H

#include <functional>
#include <utility>
#include <vector>

template <typename IteratorType>
class MpiMapReduce
{
public:
	MpiMapReduce(IteratorType begin, IteratorType end,
			     std::function<std::vector<std::pair<IteratorType, IteratorType>>(IteratorType, IteratorType, int)> partitioning_method,
				 int number_of_threads) :
	_begin(begin), _end(end), _partitioning_method(partitioning_method), _number_of_threads(number_of_threads)
	{
	}

	void map()
	{
		_partitioning_method(_begin, _end, _number_of_threads);
	}
private:
	IteratorType _begin;
	IteratorType _end;
	std::function<std::vector<std::pair<IteratorType, IteratorType>>(IteratorType, IteratorType, int)> _partitioning_method;
	int _number_of_threads;
};

#endif // __MPI_MAP_REDUCE_H