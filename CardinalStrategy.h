#ifndef __CARDINALSTRATEGY_H
#define __CARDINALSTRATEGY_H

#include <vector>
#include "MyAssert.h"
#include "RowWiseIterator.h"
#include "ColumnWiseIterator.h"
#include "RandomUniform.h"

class CardinalStrategy :
    RowWiseIter<std::vector<double>::iterator>,
    RowWiseIter<std::vector<double>::const_iterator>,
    ColumnWiseIter<std::vector<double>::iterator>,
    ColumnWiseIter<std::vector<double>::const_iterator>
{
public:
    CardinalStrategy(int rows, int columns) :
        m_Strategy(std::vector<double>(rows*columns)), m_rows(rows), m_columns(columns){}

    CardinalStrategy(int rows, int columns, RandomUniform* randGen) :
        m_Strategy(std::vector<double>(rows*columns)), m_rows(rows), m_columns(columns)
    {
        RandomizeStrategy(randGen);
    }

    template<class FwdIter>
    CardinalStrategy(int rows, int columns, FwdIter begin, FwdIter end) :
        m_Strategy(std::vector<double>(rows*columns)), m_rows(rows), m_columns(columns)
    {
        std::vector<double> temp;

        std::copy(begin, end, std::back_inserter(temp));

        MYASSERT(temp.size() == rows*columns, "Invalid input iterators for CardinalStrategy.");

        temp.swap(m_Strategy);
    }

    int NumRows() const { return m_rows; }
    int NumColumns() const { return m_columns; }

    typedef RowWiseIter<std::vector<double>::iterator> RowWiseIterator;
    typedef RowWiseIter<
        std::vector<double>::const_iterator> const_RowWiseIterator;
    typedef ColumnWiseIter<std::vector<double>::iterator> ColumnWiseIterator;
    typedef ColumnWiseIter<
        std::vector<double>::const_iterator> const_ColumnWiseIterator;

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

    const_RowWiseIterator RowWiseBegin() const
    {
        return const_RowWiseIterator(m_Strategy.begin());
    }

    const_RowWiseIterator RowWiseEnd() const
    {
        return const_RowWiseIterator(m_Strategy.end());
    }

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

    
    unsigned int RandomizeStrategy(RandomUniform* randGen)
    {
        // Assign random values to the vector
        for_each(this->RowWiseBegin(), this->RowWiseEnd(), *randGen);

        // Allow the caller to save the random seed to allow the generation of the 
        // same random number sequence.
        return randGen->GetSeed();
    }

private:
    std::vector<double> m_Strategy;
    int m_rows;
    int m_columns;
};

#endif //__CARDINALSTRAEGY_H
