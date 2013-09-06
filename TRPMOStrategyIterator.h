#ifndef __TRPMOSTRATEGYITERATOR_H
#define __TRPMOSTRATEGYITERATOR_H

#include <boost/iterator/iterator_facade.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/utility/enable_if.hpp>
#include "MyAssert.h"
#include "RankOrder.h"
#include "StrategySize.h"

template <class Value>
class TRPMOStrategyIter 
    : public boost::iterator_facade<
    TRPMOStrategyIter<Value>, 
    Value, 
    boost::random_access_traversal_tag,
    Value&,
    int>
{
private:
    struct enabler {};
    friend class boost::iterator_core_access;
    template <class> friend class TRPMOStrategyIter;

    void advance(int n)
    {
        //TODO verify increment of m_curIndex is valid
        MYASSERT((m_curIndex + n >= 0), "Invalid iterator increment: before the beginning.");
        MYASSERT((m_curIndex + n <= StrategySize<Value>(m_curStrategy)), 
            "Invalid iterator increment: more than one past the end.");
        m_curIndex += n;
        SetCurStrategyFromCurIndex();
    }

    void increment() { advance(1); }

    void decrement() { advance(-1); }

    template <class OtherValue>
    bool equal(TRPMOStrategyIter<OtherValue> const& other) const
    {
        return m_curIndex == other.m_curIndex;
    }

    Value& dereference() const
    {
        return m_curStrategy;
    }
    
    template <class OtherValue>
    int distance_to(TRPMOStrategyIter<OtherValue> const& other) const
    {
        return other.m_curIndex - m_curIndex;
    }

    void SetCurStrategyFromCurIndex()
    {
        int numRows = m_curStrategy.NumRows();
        int numColumns = m_curStrategy.NumColumns();
        std::vector<int> initStrategy(numRows*numColumns);

        // Only compute the strategy if m_curIndex is valid.
        // Otherwise, make the strategy all zeros, which represents
        // end iterator.
        if (m_curIndex < StrategySize<Value>(m_curStrategy))
        {
            int lclIndex = m_curIndex;
            for (int idx = 0; idx < numRows; ++idx)
            {
            //TODO replace 1 with TRPValue enum
            initStrategy[(idx * numColumns) + (lclIndex % numColumns)] = 1;
            lclIndex /= numColumns;
            }
        }

        if (m_curStrategy.RankOrder() == RowRankOrder)
            std::copy(initStrategy.begin(), initStrategy.end(), 
                m_curStrategy.RowWiseBegin());
        else
            std::copy(initStrategy.begin(), initStrategy.end(), 
                m_curStrategy.ColumnWiseBegin());
    }

    void SetCurIndexFromCurStrategy()
    {
        int numRows = m_curStrategy.NumRows();
        int numColumns = m_curStrategy.NumColumns();

        int idx = 0;
        m_curIndex = 0;
        for (typename Value::const_RowWiseIterator it = m_curStrategy.RowWiseBegin(); 
            it != m_curStrategy.RowWiseEnd(); ++it)
        {
            //TODO replace 1 with TRPValue enum
            if (*it == 1)
            {
            int curColumn = idx % numColumns;
            int curRow = idx % numRows;

            m_curIndex += curColumn * static_cast<int>(pow(static_cast<double>(numColumns), 
                                                            static_cast<double>(curRow)));
            }
            idx++;
        }
    }

    void SetCurIndexFromCurStrategyColumnRankOrder()
    {
        int numRows = m_curStrategy.NumRows();
        int numColumns = m_curStrategy.NumColumns();

        int idx = 0;
        m_curIndex = 0;
        for (typename Value::const_ColumnWiseIterator it = m_curStrategy.ColumnWiseBegin(); 
            it != m_curStrategy.ColumnWiseEnd(); ++it)
        {
            //TODO replace 1 with TRPValue enum
            if (*it == 1)
            {
            int curColumn = idx % numColumns;
            int curRow = idx % numRows;

            m_curIndex += curColumn * static_cast<int>(pow(static_cast<double>(numColumns), 
                                                            static_cast<double>(curRow)));
            }
            idx++;
        }
    }

    mutable int m_curIndex;
    mutable Value m_curStrategy;

public:
    explicit TRPMOStrategyIter(Value val) : m_curIndex(0), m_curStrategy(val)
    {
        if (m_curStrategy.RankOrder() == RowRankOrder)
            SetCurIndexFromCurStrategy();
        else
            SetCurIndexFromCurStrategyColumnRankOrder();
    }

    TRPMOStrategyIter(int curIndex, int numRows, int numColumns) : 
        m_curIndex(curIndex), 
        m_curStrategy(numRows, numColumns)
    {
        SetCurStrategyFromCurIndex();
    }

    template <class OtherValue>
    TRPMOStrategyIter(TRPMOStrategyIter<OtherValue> const& other, 
        typename boost::enable_if<
            boost::is_convertible<OtherValue, Value>, 
                        enabler>::type = enabler()) 
    : m_curIndex(other.m_curIndex), m_curStrategy(other.m_curStrategy) {}

	int GetIntegerRepresentation() const
	{
		return m_curIndex;
	}
};

#endif //__TRPMOSTRATEGYITERATOR_H
