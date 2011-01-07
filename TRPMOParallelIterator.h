#ifndef __TRPMO_PARALLEL_ITERATOR
#define __TRPMO_PARALLEL_ITERATOR

#include <ostream>
#include "TRPMOStrategy.h"
#include "NashSolutionType.h"
#include <tbb/blocked_range.h>
#include <tbb/parallel_reduce.h>
#include <tbb/partitioner.h>

template <class ComparatorType>
class TRPMOParallelIterator
{
public:
    TRPMOParallelIterator(int rows, int columns) : 
        m_rows(rows), 
        m_columns(columns), 
        m_Comparator(rows, columns) {}
    
    TRPMOParallelIterator(const TRPMOStrategy<ColumnRankOrder>& p1CompareStrategy) : 
        m_rows(p1CompareStrategy.NumRows()), 
        m_columns(p1CompareStrategy.NumColumns()), 
        m_Comparator(m_rows, m_columns, p1CompareStrategy) {}

    TRPMOParallelIterator(const TRPMOStrategy<RowRankOrder>& p2CompareStrategy) : 
        m_rows(p2CompareStrategy.NumRows()), 
        m_columns(p2CompareStrategy.NumColumns()), 
        m_Comparator(m_rows, m_columns, p2CompareStrategy) {}

    void IterateStrategies()
    {
        TRPMOStrategy<ColumnRankOrder> p1Strategy(m_rows, m_columns);
        tbb::parallel_reduce(tbb::blocked_range<typename TRPMOStrategy<ColumnRankOrder>::strategyIterator>(
            p1Strategy.StrategyBegin(), p1Strategy.StrategyEnd()), m_Comparator, tbb::auto_partitioner()); 
    }

    void IterateP1Strategies()
    {
        TRPMOStrategy<ColumnRankOrder> p1Strategy(m_rows, m_columns);
        tbb::parallel_reduce(tbb::blocked_range<typename TRPMOStrategy<ColumnRankOrder>::strategyIterator>(
            p1Strategy.StrategyBegin(), p1Strategy.StrategyEnd()), m_Comparator, tbb::auto_partitioner());
    }

    void IterateP2Strategies()
    {
        TRPMOStrategy<RowRankOrder> p2Strategy(m_rows, m_columns);
        tbb::parallel_reduce(tbb::blocked_range<typename TRPMOStrategy<RowRankOrder>::strategyIterator>(
            p2Strategy.StrategyBegin(), p2Strategy.StrategyEnd()), m_Comparator, tbb::auto_partitioner());
    }

    const ComparatorType& Comparator() const
    {
        return m_Comparator;
    }

private:
    int m_rows;
    int m_columns;
    ComparatorType m_Comparator;
};

#endif // __TRPMO_PARALLEL_ITERATOR
