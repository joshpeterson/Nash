#ifndef __COLUMNWISEITERATOR_H
#define __COLUMNWISEITERATOR_H

#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/utility/enable_if.hpp>

template <class Value> 
class ColumnWiseIter 
    : public boost::iterator_adaptor<
    ColumnWiseIter<Value>, 
    Value, 
    boost::use_default, 
    boost::forward_traversal_tag>
{
private:
    struct enabler {};
    friend class boost::iterator_core_access;

    void increment()
    {
        Value temp(this->base());
        Value& it = this->base_reference();
        if (std::distance(it, m_it_end) > m_num_columns)
            it += m_num_columns;    // Skip to the same column, next row
        else if (++temp != m_it_end)
            it -= ((m_num_rows-1)*m_num_columns-1);  // Go to first row, next column
        else
            it = m_it_end;      // Finished, signal past the end
    }

public:
    Value m_it_end;
    int m_num_rows;
    int m_num_columns;

    ColumnWiseIter() {}

    explicit ColumnWiseIter(Value val, Value end, int num_rows, int num_columns) : 
        ColumnWiseIter::iterator_adaptor_(val),
        m_it_end(end),
        m_num_rows(num_rows),
        m_num_columns(num_columns)  {}

    template <class OtherValue>
    ColumnWiseIter(ColumnWiseIter<OtherValue> const& other, 
        typename boost::enable_if<
                boost::is_convertible<OtherValue, Value>, 
                        enabler>::type = enabler()) : 
        ColumnWiseIter::iterator_adaptor_(other.base()),
        m_it_end(other.m_it_end),
        m_num_rows(other.m_num_rows),
        m_num_columns(other.m_num_columns) {}
};

#endif //__COLUMNWISEITERATOR_H
