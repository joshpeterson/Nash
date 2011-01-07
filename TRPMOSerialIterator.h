#ifndef __TRPMO_SERIAL_ITERATOR
#define __TRPMO_SERIAL_ITERATOR

#include <ostream>
#include "TRPMOStrategy.h"
#include "NashSolutionType.h"

template <class ComparatorType>
class TRPMOSerialIterator
{
public:
    TRPMOSerialIterator(int rows, int columns) : 
        m_rows(rows), 
        m_columns(columns), 
        m_Comparator() {}

    TRPMOSerialIterator(const TRPMOStrategy<ColumnRankOrder>& p1CompareStrategy) : 
        m_rows(p1CompareStrategy.NumRows()), 
        m_columns(p1CompareStrategy.NumColumns()), 
        m_p1CompareStrategy(p1CompareStrategy),
        m_Comparator() {}

    TRPMOSerialIterator(const TRPMOStrategy<RowRankOrder>& p2CompareStrategy) : 
        m_rows(p2CompareStrategy.NumRows()), 
        m_columns(p2CompareStrategy.NumColumns()), 
        m_p2CompareStrategy(p2CompareStrategy),
        m_Comparator() {}

    void IterateStrategies()
    {
        TRPMOStrategy<ColumnRankOrder> p1Strategy(m_rows, m_columns);
        TRPMOStrategy<ColumnRankOrder>::strategyIterator endIterP1 = p1Strategy.StrategyEnd();
        for (TRPMOStrategy<ColumnRankOrder>::strategyIterator itP1 = p1Strategy.StrategyBegin(); 
            itP1 != endIterP1; ++itP1)
        {
            TRPMOStrategy<RowRankOrder> p2Strategy(m_rows, m_columns);
            TRPMOStrategy<RowRankOrder>::strategyIterator endIterP2 = p2Strategy.StrategyEnd();
            for (TRPMOStrategy<RowRankOrder>::strategyIterator itP2 = p2Strategy.StrategyBegin(); 
                itP2 != endIterP2; ++itP2)
            {
                m_Comparator(*itP1, *itP2);
            }
        }
    }

    void IterateP1Strategies()
    {
        TRPMOStrategy<ColumnRankOrder> p1Strategy(m_p2CompareStrategy.NumRows(), 
                                                    m_p2CompareStrategy.NumColumns());
        TRPMOStrategy<ColumnRankOrder>::strategyIterator endIter = p1Strategy.StrategyEnd();
        for (TRPMOStrategy<ColumnRankOrder>::strategyIterator it = p1Strategy.StrategyBegin(); 
            it != endIter; ++it)
        {
            m_Comparator(*it, m_p2CompareStrategy);
        }
    }

    void IterateP2Strategies()
    {
        TRPMOStrategy<RowRankOrder> p2Strategy(m_p1CompareStrategy.NumRows(), 
                                                m_p1CompareStrategy.NumColumns());
        TRPMOStrategy<RowRankOrder>::strategyIterator endIter = p2Strategy.StrategyEnd();
        for (TRPMOStrategy<RowRankOrder>::strategyIterator it = p2Strategy.StrategyBegin(); 
            it != endIter; ++it)
        {
            m_Comparator(m_p1CompareStrategy, *it);
        }
    }

    const ComparatorType& Comparator() const
    {
        return m_Comparator;
    }

private:

    int m_rows;
    int m_columns;
    TRPMOStrategy<ColumnRankOrder> m_p1CompareStrategy;
    TRPMOStrategy<RowRankOrder> m_p2CompareStrategy;
    ComparatorType m_Comparator;
};

#endif // __TRPMO_SERIAL_ITERATOR
