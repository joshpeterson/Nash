#ifndef __NASH_MONTE_CARLO_SERIAL_H
#define __NASH_MONTE_CARLO_SERIAL_H

#include "Game.h"
#include "TRPMONashResults.h"

class NashMonteCarloSerial : public TRPMONashResults
{
public:
    NashMonteCarloSerial(int rows, int columns, int numSamples,
                            double lowerBound, double upperBound, unsigned int seed = 0) :
        m_rows(rows),
        m_columns(columns),
        m_numSamples(numSamples),
        m_randGen(lowerBound, upperBound, seed ? seed : static_cast<unsigned int> (std::time(0)))
    {}

    void Run()
    {
        for (int idx = 0; idx < m_numSamples; ++idx)
        {
            GameRandom< TRPMOSerialIterator<TRPMONashCategorization> > gameRandom(m_rows, m_columns, &m_randGen);
            gameRandom.IterateStrategies();

            m_results.numUniqueNash += gameRandom.Iterator().Comparator().GetNumUniqueNash();
            m_results.numNonuniqueNash += gameRandom.Iterator().Comparator().GetNumNonuniqueNash();
            m_results.numNoNash += gameRandom.Iterator().Comparator().GetNumNoNash();
        }
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
    RandomUniform m_randGen;
};

#endif  //__NASH_MONTE_CARLO_SERIAL_H
