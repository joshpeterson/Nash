#ifndef __TRPMO_NASH_RESULTS_H
#define __TRPMO_NASH_RESULTS_H

#include <boost/noncopyable.hpp>

class TRPMONashResults
{
public:
    TRPMONashResults()
    {
        m_results.numUniqueNash = 0;
        m_results.numNonuniqueNash = 0; 
        m_results.numNoNash = 0;
    }

    virtual ~TRPMONashResults() {}

    virtual void DisplayResults(std::ostream& out) const
    {
        OStreamHelper streamHelper(out, 2, std::ios::fixed);
        out << "Unique Nash: " << m_results.numUniqueNash << " (" 
            << PercentOfTotalGames(m_results.numUniqueNash) << "%)" << std::endl;
        out << "Nonunique Nash: " << m_results.numNonuniqueNash << " (" 
            << PercentOfTotalGames(m_results.numNonuniqueNash) << "%)" << std::endl;
        out << "No Nash: " << m_results.numNoNash << " (" 
            << PercentOfTotalGames(m_results.numNoNash) << "%)" << std::endl;
    }

    int GetNumUniqueNash() const
    {
        return m_results.numUniqueNash;
    }

    int GetNumNonuniqueNash() const
    {
        return m_results.numNonuniqueNash;
    }

    int GetNumNoNash() const
    {
        return m_results.numNoNash;
    }

protected:
    struct
    {
        unsigned int numUniqueNash;
        unsigned int numNonuniqueNash;
        unsigned int numNoNash;
    } m_results;

private:
    double PercentOfTotalGames(unsigned int numGames) const
    {
        unsigned int totalNumGames = m_results.numUniqueNash + m_results.numNonuniqueNash + m_results.numNoNash;
        return (static_cast<double>(numGames) / static_cast<double>(totalNumGames)) * 100.0;
    }

    class OStreamHelper : private boost::noncopyable
    {
    public:
        OStreamHelper(std::ostream& out, std::streamsize precision, std::ios_base::fmtflags flags) :
            m_StreamRef(out),
            m_prevPrecision(out.precision(precision)),
            m_prevFlags(out.setf(flags))
        {  
        }

        ~OStreamHelper()
        {
            m_StreamRef.precision(m_prevPrecision);
            m_StreamRef.setf(m_prevFlags);
        }
    private:
        std::ostream& m_StreamRef;
        std::streamsize m_prevPrecision;
        std::ios_base::fmtflags m_prevFlags;
    };
};

#endif //__TRPMO_NASH_RESULTS_H
