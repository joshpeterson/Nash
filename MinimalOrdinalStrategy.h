#ifndef __MINIMALORDINALSTRATEGY_H
#define __MINIMALORDINALSTRATEGY_H

#include <vector>
#include <iterator>
#include "MyAssert.h"
#include "CardinalStrategy.h"
#include "RowWiseIterator.h"
#include "ColumnWiseIterator.h"
#include "RankOrder.h"

template <RankOrderType rank_order>
class MinimalOrdinalStrategy :
    RowWiseIter<std::vector<int>::iterator>,
    RowWiseIter<std::vector<int>::const_iterator>,
    ColumnWiseIter<std::vector<int>::iterator>,
    ColumnWiseIter<std::vector<int>::const_iterator>
{
public:
    MinimalOrdinalStrategy(int rows, int columns) :
        m_Strategy(std::vector<int>(rows*columns)), m_rows(rows), m_columns(columns) {}

    template <class FwdIter>
    MinimalOrdinalStrategy(int rows, int columns, FwdIter begin, FwdIter end) :
        m_Strategy(std::vector<int>(rows*columns)), m_rows(rows), m_columns(columns)
    {
        std::vector<int> temp;
        std::copy(begin, end, std::back_inserter(temp));

        MYASSERT(temp.size() == rows*columns, "Invalid input iterators for MinimalOrdinalStrategy.");
        
        temp.swap(m_Strategy);

        MYASSERT(CheckStrategyValid(), "Invalid MinimalOrdinalStrategy.");
    }

    MinimalOrdinalStrategy(const CardinalStrategy& strategy) :
        m_Strategy(std::vector<int>(strategy.NumRows()*strategy.NumColumns())), 
        m_rows(strategy.NumRows()), m_columns(strategy.NumColumns())
    {
        if (rank_order == RowRankOrder)
            RankOrder<CardinalStrategy::const_RowWiseIterator,
                typename MinimalOrdinalStrategy<rank_order>::RowWiseIterator>(strategy.RowWiseBegin(),
                                        RowWiseBegin(),
                                        strategy.NumColumns());
        else if (rank_order == ColumnRankOrder)
            RankOrder<CardinalStrategy::const_ColumnWiseIterator,
                typename MinimalOrdinalStrategy<rank_order>::ColumnWiseIterator>(strategy.ColumnWiseBegin(),
                                        ColumnWiseBegin(),
                                        strategy.NumRows());
        else
            MYASSERT(false, "Invalid rank order type specified");
    }

    int NumRows() const { return m_rows; }
    int NumColumns() const { return m_rows; }

    typedef RowWiseIter<std::vector<int>::iterator> RowWiseIterator;
    typedef RowWiseIter<std::vector<int>::const_iterator> const_RowWiseIterator;
    typedef ColumnWiseIter<std::vector<int>::iterator> ColumnWiseIterator;
    typedef ColumnWiseIter<std::vector<int>::const_iterator> const_ColumnWiseIterator;

    RowWiseIterator RowWiseBegin() { return RowWiseIterator(m_Strategy.begin()); }
    RowWiseIterator RowWiseEnd() { return RowWiseIterator(m_Strategy.end()); }
    ColumnWiseIterator ColumnWiseBegin()
    {
        return ColumnWiseIterator(m_Strategy.begin(), m_Strategy.end(), 
                        m_rows, m_columns); 
    }
    ColumnWiseIterator ColumnWiseEnd()
    {
        return ColumnWiseIterator(m_Strategy.end(), m_Strategy.end(), 
                        m_rows, m_columns);
    }

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

private:
    std::vector<int> m_Strategy;
    int m_rows;
    int m_columns;

    class MinimalOrdinalComparison : public std::binary_function<double, double, bool>
    {
    public:
        bool operator() (double left, double right)
        {
            if (left == 0.0)
                return false;
            else if (right == 0.0)
                return true;
            else
                return left < right;
        }
    };

    template <class InputIteratorType, class OutputIteratorType>
    void RankOrder(InputIteratorType strategy_it, 
            OutputIteratorType output_it, 
            int nElements)
    {
        for (int i = 0; i < nElements; ++i)
        {
            std::vector< std::pair<double, int> > copy_row;
            std::vector<double> sorted_row;
            for (int i = 0; i < nElements; ++i, ++strategy_it)
            {
                copy_row.push_back(std::make_pair(*strategy_it, 0));
                sorted_row.push_back(*strategy_it);
            }

            std::sort(sorted_row.begin(), sorted_row.end(), MinimalOrdinalComparison());

            int rank = 1;
            for (std::vector<double>::const_iterator it = sorted_row.begin(); 
                    it != sorted_row.end(); ++it, ++rank)
            {
                std::vector< std::pair<double, int> >::iterator found_it = 
                            find_if(copy_row.begin(), copy_row.end(), 
                                bind2nd(std::equal_to< std::pair<double, int> >(), 
                                    std::make_pair<double, int>(*it, 0)));
                found_it->second = rank;
            }

            for (std::vector< std::pair<double, int> >::const_iterator it = copy_row.begin();
                it != copy_row.end(); ++it)
            {
                *output_it = it->second;
                ++output_it;
            }
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
        int strategySize = m_rows*m_columns;

        for (int idx = 0; idx < strategySize; idx += checkInterval)
        {
            FwdIter localBegin = begin;
            std::advance(localBegin, idx);

            FwdIter localEnd = localBegin;
            std::advance(localEnd, checkInterval);

            if (localEnd != std::find_if(localBegin, localEnd, std::bind2nd(std::greater<int>(), checkInterval)))
                return false;

            for (FwdIter it = localBegin; it != localEnd; ++it)
            {
                FwdIter start = it;
                std::advance(start, 1);
                if (localEnd != std::find_if(start, localEnd, std::bind2nd(std::equal_to<int>(), *it)))
                    return false;
            }
        }

        return true;
    }
};

#endif //__MINIMALORDINALSTRAEGY_H
