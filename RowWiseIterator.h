#ifndef __ROWWISEITERATOR_H
#define __ROWWISEITERATOR_H

#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/utility/enable_if.hpp>

template <class Value>
class RowWiseIter 
    : public boost::iterator_adaptor<
    RowWiseIter<Value>, 
    Value, 
    boost::use_default, 
    boost::forward_traversal_tag>
{
private:
    struct enabler {};
    friend class boost::iterator_core_access;
    void increment() { ++this->base_reference(); }

public:
    RowWiseIter() {}
    explicit RowWiseIter(Value val) : RowWiseIter::iterator_adaptor_(val) {}
    template <class OtherValue>
    RowWiseIter(RowWiseIter<OtherValue> const& other, 
        typename boost::enable_if<
            boost::is_convertible<OtherValue, Value>, 
                        enabler>::type = enabler()) 
    : RowWiseIter::iterator_adaptor_(other.base()) {}
};

#endif //__ROWWISEITERATOR_H
