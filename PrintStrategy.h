#ifndef __PRINTSTRATEGY_H
#define __PRINTSTRATEGY_H

#include <iostream>

template <class StrategyType>
void PrintStrategy(const StrategyType& strategy, std::ostream& out)
{
    typename StrategyType::const_RowWiseIterator it = strategy.RowWiseBegin();
    for (int row_idx = 0; row_idx < strategy.NumRows(); ++row_idx)
    {
        for (int col_idx = 0; col_idx < strategy.NumColumns(); ++col_idx)
        {
            out << *it;
            if (col_idx != strategy.NumColumns()-1)
                out << ",";
            ++it;
        }
        out << std::endl;
    }
}

#endif //__PRINTSTRATEGY_H
