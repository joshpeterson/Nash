#ifndef __STRATEGYSIZE_H
#define __STRATEGYSIZE_H

#include <math.h>

template <class StrategyType>
int StrategySize(const StrategyType& strategy)
{
    return static_cast<int>(pow(static_cast<double>(strategy.NumRows()), 
                                static_cast<double>(strategy.NumColumns())));
}

int NumStrategies(int rows, int columns)
{
    return static_cast<int>(pow(static_cast<double>(rows), 
                                static_cast<double>(columns)));
}

#endif  //__STRATEGYSIZE_H
