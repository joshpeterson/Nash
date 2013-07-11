#ifndef __TRPMOSTRATEGY_H
#define __TRPMOSTRATEGY_H

#include <vector>
#include "MyAssert.h"
#include "MinimalOrdinalStrategy.h"
#include "RowWiseIterator.h"
#include "ColumnWiseIterator.h"
#include "TRPMOStrategyIterator.h"
#include "RankOrder.h"

class CardinalStrategy;

template <RankOrderType rank_order>
class TRPMOStrategy :
    RowWiseIter<std::vector<int>::iterator>,
    RowWiseIter<std::vector<int>::const_iterator>,
    ColumnWiseIter<std::vector<int>::iterator>,
    ColumnWiseIter<std::vector<int>::const_iterator>
{
public:
    TRPMOStrategy() : m_Strategy(), m_rows(0), m_columns(0) {}

    TRPMOStrategy(int rows, int columns) :
        m_Strategy(std::vector<int>(rows*columns)), m_rows(rows), m_columns(columns) {}

    template <class FwdIter>
    TRPMOStrategy(int rows, int columns, FwdIter begin, FwdIter end) :
        m_Strategy(std::vector<int>(rows*columns)), m_rows(rows), m_columns(columns)
    {
        std::vector<int> temp;
        std::copy(begin, end, std::back_inserter(temp));

        MYASSERT(temp.size() == rows*columns, "Invalid input iterators for TRPMOStrategy.");
        
        temp.swap(m_Strategy);

        MYASSERT(CheckStrategyValid(), "Invalid TRPMOStrategy.")
    }

    TRPMOStrategy(const TRPMOStrategy<rank_order>& strategy) :
        m_Strategy(std::vector<int>(strategy.NumRows()*strategy.NumColumns())), 
        m_rows(strategy.NumRows()),
        m_columns(strategy.NumColumns())
    {
        std::copy(strategy.RowWiseBegin(), strategy.RowWiseEnd(),
                m_Strategy.begin());
    }

    TRPMOStrategy(const CardinalStrategy& strategy) :
        m_Strategy(std::vector<int>(strategy.NumRows()*strategy.NumColumns())), 
        m_rows(strategy.NumRows()),
        m_columns(strategy.NumColumns())
    {
        MinimalOrdinalStrategy<rank_order> MOStrategy(strategy);
        ReduceToTRPs(MOStrategy);
    }

    TRPMOStrategy(const MinimalOrdinalStrategy<rank_order>& strategy) :
        m_Strategy(std::vector<int>(strategy.NumRows()*strategy.NumColumns())), 
        m_rows(strategy.NumRows()),
        m_columns(strategy.NumColumns())
    {
        ReduceToTRPs(strategy);
    }

    int NumRows() const { return m_rows; }
    int NumColumns() const { return m_columns; }
    RankOrderType RankOrder() const { return rank_order; }

    typedef RowWiseIter<std::vector<int>::iterator> RowWiseIterator;
    typedef RowWiseIter<std::vector<int>::const_iterator> const_RowWiseIterator;
    typedef ColumnWiseIter<std::vector<int>::iterator> ColumnWiseIterator;
    typedef ColumnWiseIter<std::vector<int>::const_iterator> const_ColumnWiseIterator;
    typedef TRPMOStrategyIter<TRPMOStrategy<rank_order> > strategyIterator;


    RowWiseIterator RowWiseBegin() { return RowWiseIterator(m_Strategy.begin()); }

    RowWiseIterator RowWiseEnd() { return RowWiseIterator(m_Strategy.end()); }

    ColumnWiseIterator ColumnWiseBegin() 
    {
        return ColumnWiseIterator(m_Strategy.begin(), m_Strategy.end(), m_rows, m_columns); 
    }

    ColumnWiseIterator ColumnWiseEnd()
    {
        return ColumnWiseIterator(m_Strategy.end(), m_Strategy.end(), m_rows, m_columns);
    }

    strategyIterator StrategyBegin() { return StrategyBegin(0); }
    
	strategyIterator StrategyBegin(int strategyIndex) { return strategyIterator(strategyIndex, m_rows, m_columns); }

    strategyIterator StrategyEnd() { return StrategyEnd(StrategySize<TRPMOStrategy<rank_order> >(*this)); }
    
	strategyIterator StrategyEnd(int strategyIndex) { return strategyIterator(strategyIndex, m_rows, m_columns); }

    const_RowWiseIterator RowWiseBegin() const { return const_RowWiseIterator(m_Strategy.begin()); }

    const_RowWiseIterator RowWiseEnd() const { return const_RowWiseIterator(m_Strategy.end()); }

    const_ColumnWiseIterator ColumnWiseBegin() const 
    {
        return const_ColumnWiseIterator(m_Strategy.begin(), m_Strategy.end(), 
                        m_rows, m_columns); 
    }
    const_ColumnWiseIterator ColumnWiseEnd() const
    {
        return const_ColumnWiseIterator(m_Strategy.end(), m_Strategy.end(), 
                        m_rows, m_columns);
    }

    enum TRP_value_type
    {
        DontCareValue = 0,
        TRPValue = 1
    };

private:
    std::vector<int> m_Strategy;
    int m_rows;
    int m_columns;

    void ReduceToTRPs(const MinimalOrdinalStrategy<rank_order>& strategy)
    {
        typename MinimalOrdinalStrategy<rank_order>::const_RowWiseIterator input_it = strategy.RowWiseBegin();
        std::vector<int>::iterator this_it = m_Strategy.begin();
        for (; this_it != m_Strategy.end(); ++this_it, ++input_it)
        {
            *this_it  = (*input_it == TRPValue) ? TRPValue : DontCareValue;
        }
    }

    bool CheckStrategyValid()
    {
        if (rank_order == ColumnRankOrder)
            return StrategyValid(this->ColumnWiseBegin(), this->ColumnWiseEnd(), m_rows);
        else
            return StrategyValid(this->RowWiseBegin(), this->RowWiseEnd(), m_columns);
    }

    template <class FwdIter>
    bool StrategyValid(FwdIter begin, FwdIter end, int checkInterval)
    {
        bool bTRPFound = false;
        int idx = 0;
        for (FwdIter it = begin; it != end; ++it, ++idx)
        {
            if (idx % checkInterval == 0)
            bTRPFound = false;

            if (*it == TRPValue)
            if (!bTRPFound)
                bTRPFound = true;
            else
                return false;
        }
        return true;
    }
};

#endif //__TRPMOSTRAEGY_H
