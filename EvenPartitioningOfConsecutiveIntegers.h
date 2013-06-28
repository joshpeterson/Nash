#ifndef __EVEN_PARTITIONING_OF_CONSECUTIVE_INTEGERS
#define __EVEN_PARTITIONING_OF_CONSECUTIVE_INTEGERS

#include <vector>
#include <utility>

std::vector<std::pair<unsigned int, unsigned int>> even_partitioning_of_consecutive_integers(unsigned int begin, unsigned int end, int number_of_partitions)
{
	if (begin != end)
	{
		auto full_range_size = end - begin + 1;
		auto partition_size = full_range_size / number_of_partitions;

		auto current_partition_begin = begin;

		auto partitions = std::vector<std::pair<unsigned int, unsigned int>>();
		for (auto i = 0; i < number_of_partitions; ++i)
		{
			auto current_partition_end = current_partition_begin + partition_size - 1;
			partitions.emplace_back(std::make_pair(current_partition_begin, current_partition_end));
			current_partition_begin = current_partition_begin + partition_size;
		}

		return partitions;
	}

	return std::vector<std::pair<unsigned int, unsigned int>>();
}

#endif // __EVEN_PARTITIONING_OF_CONSECUTIVE_INTEGERS