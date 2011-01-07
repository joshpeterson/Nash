#ifndef __RANDOMNUMBER_H
#define __RANDOMNUMBER_H

#include <ctime>
#include <boost/random.hpp>
#include <tbb/spin_mutex.h>

class RandomUniform : public std::unary_function<double&, void>
{
public:
    RandomUniform(double lowerBound, double upperBound, unsigned int seed = 0) :
        m_seed(seed ? seed : static_cast<unsigned int> (std::time(0))),
        m_generator(m_seed),
        m_dist(lowerBound, upperBound),
        m_rand(m_generator, m_dist) {}

    virtual ~RandomUniform() {}

    unsigned int GetSeed() const
    {
        return m_seed;
    }

    virtual void operator()(double& num)
    {
        num = m_rand();
    }

protected:
    unsigned int m_seed;
    boost::minstd_rand m_generator;
    boost::uniform_real<> m_dist;
    boost::variate_generator< boost::minstd_rand&, boost::uniform_real<> > m_rand;
};

class RandomUniformLocked : public RandomUniform
{
public:
    RandomUniformLocked(double lowerBound, double upperBound, unsigned int seed = 0) :
        RandomUniform(lowerBound, upperBound, seed) {}

    virtual void operator()(double& num)
    {
        tbb::spin_mutex::scoped_lock lock(m_randLock);
        num = m_rand();
    }

private:
    tbb::spin_mutex m_randLock;
};

#endif //__RANDOMNUMBER_H
