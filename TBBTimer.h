#ifndef __TBB_TIMER_H
#define __TBB_TIMER_H

#include <boost/noncopyable.hpp>
#include <tbb/tick_count.h>

#define TIME_OPERATION(timer, operation)\
    do {\
        timer.Start();\
        operation;\
        timer.Stop();\
    } while(0);

typedef void (*RunFunc)();

class TBBTimer : private boost::noncopyable
{
public:
    void Start()
    {
        m_Start = tbb::tick_count::now();
    }

    void Stop()
    {
        m_End = tbb::tick_count::now();
    }

    double ElapsedSeconds()
    {
        return (m_End - m_Start).seconds();
    }

private:
    RunFunc m_pFunc;
    tbb::tick_count m_Start;
    tbb::tick_count m_End;
};

#endif // __TBB_TIMER_H