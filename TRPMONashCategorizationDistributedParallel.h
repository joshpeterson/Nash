#ifndef __TRPMO_NASH_CATEGORIZATION_DISTRIBUTED_PARALLEL
#define __TRPMO_NASH_CATEGORIZATION_DISTRIBUTED_PARALLEL

#include "MyAssert.h"
#include "TRPMOStrategy.h"
#include "TRPMONashResults.h"

class TRPMODistributedParallelTask;

class TRPMONashCategorizationDistributedParallel : public TRPMONashResults
{
public:
    TRPMONashCategorizationDistributedParallel() {}

    void operator()(const TRPMOStrategy<ColumnRankOrder>& p1CompareStrategy,
                    const TRPMOStrategy<RowRankOrder>& p2CompareStrategy)
    {
        NashSolutionType solution = CategorizeTRPMOGame(p1CompareStrategy, p2CompareStrategy);
        if (solution == UniqueNash)
            ++m_results.numUniqueNash;
        else if (solution == NonuniqueNash)
            ++m_results.numNonuniqueNash;
        else if (solution == NoNash)
            ++m_results.numNoNash;
        else
            MYASSERT(false, "Invalid solution type returned by CategorizeTRPMOGame.");
    }

    void DisplayResults(std::ostream& out) const
    {
        out << "Total number of games: " << 
            m_results.numUniqueNash + m_results.numNonuniqueNash + m_results.numNoNash << std::endl;
        TRPMONashResults::DisplayResults(out);
    }

	void AccumulateResults(const std::vector<unsigned int>& otherResults)
	{
		m_results.numUniqueNash += otherResults.size() == 3 ? otherResults[0] : 0;
		m_results.numNonuniqueNash += otherResults.size() == 3 ? otherResults[1] : 0;
		m_results.numNoNash += otherResults.size() == 3 ? otherResults[2] : 0;
	}
};

#endif // __TRPMO_NASH_CATEGORIZATION_DISTRIBUTED_PARALLEL