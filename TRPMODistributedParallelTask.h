#ifndef __TRPMO_DISTRIBUTED_PARALLEL_TASK
#define __TRPMO_DISTRIBUTED_PARALLEL_TASK

#include <vector>
#include "TRPMONashCategorizationDistributedParallel.h"
#include "TRPMOStrategy.h"

class TRPMODistributedParallelTask
{
public:
	TRPMODistributedParallelTask(int rows, int columns) : rows_(rows), columns_(columns) {}
	
	TRPMODistributedParallelTask(const TRPMODistributedParallelTask& other) : rows_(other.rows_), columns_(other.columns_) {}

	void map(unsigned int begin, unsigned int end)
	{
		TRPMOStrategy<ColumnRankOrder> p1_strategy_start(rows_, columns_);
		auto begin_iterator = p1_strategy_start.StrategyBegin(begin);
		auto end_iterator = p1_strategy_start.StrategyEnd(end + 1);

		for (auto p1_strategy = begin_iterator; p1_strategy != end_iterator; ++p1_strategy)
		{
			TRPMOStrategy<RowRankOrder> p2Strategy(rows_, columns_);
			for (auto p2_strategy = p2Strategy.StrategyBegin(); p2_strategy != p2Strategy.StrategyEnd(); ++p2_strategy)
			{
				comparator_(*p1_strategy, *p2_strategy);
			}
		}
	}

	std::vector<unsigned int> reduce(const std::vector<unsigned int>& input)
	{
		comparator_.AccumulateResults(input);

		auto output = std::vector<unsigned int>();
		output.push_back(comparator_.GetNumUniqueNash());
		output.push_back(comparator_.GetNumNonuniqueNash());
		output.push_back(comparator_.GetNumNoNash());

		return output;
	}

	const TRPMONashCategorizationDistributedParallel& Comparator() const
    {
        return comparator_;
    }

private:
	int rows_;
	int columns_;
	TRPMONashCategorizationDistributedParallel comparator_;
};

#endif // __TRPMO_DISTRIBUTED_PARALLEL_TASK