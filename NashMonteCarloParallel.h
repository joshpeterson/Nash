#ifndef __NASH_MONTE_CARLO_PARALLEL_H
#define __NASH_MONTE_CARLO_PARALLEL_H

#include <tbb/blocked_range.h>
#include "Game.h"
#include "TRPMONashResults.h"

class NashMonteCarloParallel : public TRPMONashResults
{
public:
    NashMonteCarloParallel(int rows, int columns, int numSamples,
                            double lowerBound, double upperBound, unsigned int seed = 0) :
        m_rows(rows),
        m_columns(columns),
        m_numSamples(numSamples),
        m_randGen(lowerBound, upperBound, seed ? seed : static_cast<unsigned int> (std::time(0)))
    {}

    void Run()
    {
        ParallelRunner simulation(m_rows, m_columns, &m_randGen);
        tbb::parallel_reduce(tbb::blocked_range<int>(0, m_numSamples), simulation, tbb::auto_partitioner());
        m_results.numUniqueNash = simulation.GetNumUniqueNash();
        m_results.numNonuniqueNash = simulation.GetNumNonuniqueNash();
        m_results.numNoNash = simulation.GetNumNoNash();
    }

    void DisplayResults(std::ostream& out) const
    {
        unsigned int totalNumGames = m_results.numUniqueNash + m_results.numNonuniqueNash + m_results.numUniqueNash;
        out << "Number of samples: " << m_numSamples << " (" << totalNumGames << " games)" << std::endl;
        out << "Random seed: " << m_randGen.GetSeed() << std::endl;
        TRPMONashResults::DisplayResults(out);
    }

private:
    int m_rows;
    int m_columns;
    int m_numSamples;
    RandomUniformLocked m_randGen;

    class ParallelRunner: public TRPMONashResults
    {
    public:
        ParallelRunner(int rows, int columns, RandomUniformLocked* pRandGen) :
            m_rows(rows),
            m_columns(columns),
            m_pRandGen(pRandGen)
        {}

        ParallelRunner(const ParallelRunner& other, tbb::split) :
            m_rows(other.m_rows),
            m_columns(other.m_columns),
            m_pRandGen(other.m_pRandGen)
        {}

        void operator()(const tbb::blocked_range<int>& r)
        {
            for (int idx = r.begin(); idx < r.end(); ++idx)
            {
                GameRandom< TRPMOSerialIterator<TRPMONashCategorization> > gameRandom(m_rows, m_columns, m_pRandGen);
                gameRandom.IterateStrategies();

                m_results.numUniqueNash += gameRandom.Iterator().Comparator().GetNumUniqueNash();
                m_results.numNonuniqueNash += gameRandom.Iterator().Comparator().GetNumNonuniqueNash();
                m_results.numNoNash += gameRandom.Iterator().Comparator().GetNumNoNash();
            }
        }

        void join(const ParallelRunner& other)
        {
            m_results.numUniqueNash += other.m_results.numUniqueNash;
            m_results.numNonuniqueNash += other.m_results.numNonuniqueNash;
            m_results.numNoNash += other.m_results.numNoNash;
        }
    private:
        int m_rows;
        int m_columns;
        RandomUniformLocked* m_pRandGen;
    };
};

#endif  //__NASH_MONTE_CARLO_PARALLEL_H
