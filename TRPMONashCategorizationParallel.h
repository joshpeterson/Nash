#ifndef __TRPMO_NASH_CATEGORIZATION_PARALLEL_H
#define __TRPMO_NASH_CATEGORIZATION_PARALLEL_H

#include <functional>
#include <iostream>
#include <tbb/blocked_range.h>
#include "MyAssert.h"
#include "TRPMOStrategy.h"
#include "CategorizeTRPMOGame.h"
#include "TRPMONashResults.h"
#include "ISolutionMethod.h"
#include "ProgramOptions.h"
#include "TBBTimer.h"
#include "GameRunner.h"

class TRPMONashCategorizationParallel : public TRPMONashResults, public ISolutionMethod
{
public:
    TRPMONashCategorizationParallel() {}

    TRPMONashCategorizationParallel(int rows, int columns) : 
        m_rows(rows), 
        m_columns(columns), 
        m_bIterateBoth(true) {}

    TRPMONashCategorizationParallel(int rows, int columns, const TRPMOStrategy<ColumnRankOrder>& compareStrategy) : 
        m_rows(rows), 
        m_columns(columns), 
        m_bIterateBoth(false), 
        m_P1CompareStrategy(compareStrategy) {}

    TRPMONashCategorizationParallel(int rows, int columns, const TRPMOStrategy<RowRankOrder>& compareStrategy) : 
        m_rows(rows), 
        m_columns(columns), 
        m_bIterateBoth(false), 
        m_P2CompareStrategy(compareStrategy) {}
    
    TRPMONashCategorizationParallel(const TRPMONashCategorizationParallel& other, tbb::split) :
        m_rows(other.m_rows),
        m_columns(other.m_columns),
        m_bIterateBoth(other.m_bIterateBoth),
        m_P1CompareStrategy(other.m_P1CompareStrategy),
        m_P2CompareStrategy(other.m_P2CompareStrategy) {}

    void operator()(const tbb::blocked_range<TRPMOStrategy<ColumnRankOrder>::strategyIterator>& r)
    {
        if (m_bIterateBoth)
            IterateBoth(r);
        else
            IterateOne(r);
    }

    void operator()(const tbb::blocked_range<TRPMOStrategy<RowRankOrder>::strategyIterator>& r)
    {
        if (m_bIterateBoth)
        {
            MYASSERT(0, "Invalid attempt to use Nash Parallel comparator to iterate both with P2 strategies.");
        }
        else
            IterateOne(r);
    }

    void join(const TRPMONashCategorizationParallel& other)
    {
        m_results.numUniqueNash += other.m_results.numUniqueNash;
        m_results.numNonuniqueNash += other.m_results.numNonuniqueNash;
        m_results.numNoNash += other.m_results.numNoNash;
    }

    void DisplayResults(std::ostream& out) const
    {
        out << "Total number of games: " << 
            m_results.numUniqueNash + m_results.numNonuniqueNash + m_results.numNoNash << std::endl;
        TRPMONashResults::DisplayResults(out);
    }

    // ISolutionMethod
    void Run(const ProgramOptions& options, std::ostream& out)
    {
        MYASSERT(options.GetMethod() == this->Name(), "Invalid call to ISolutionMethod::Run");

        tbb::task_scheduler_init init(options.GetThreads());

        if (options.GetP1Strategy() == "" && options.GetP2Strategy() == "")
        {
            out << "Performing parallel Nash solution categorization of all games of size " << options.GetRows() << "x" << options.GetColumns() <<"..." << std::endl;
            GameDoubleRunner< TRPMOParallelIterator<TRPMONashCategorizationParallel> >(options.GetRows(), options.GetColumns(), out);
        }
        else if (options.GetP1Strategy() != "")
        {
            out << "Performing parallel Nash solution categorization of all games of size " << options.GetRows() << "x" << options.GetColumns() << " with the following player 1 strategy:\n" << std::endl;
            GameRunner< TRPMOStrategy<ColumnRankOrder>, 
                        TRPMOParallelIterator<TRPMONashCategorizationParallel> >
                        (options.GetRows(), options.GetColumns(), options.GetP1Strategy(), out);
        }
        else if (options.GetP2Strategy() != "")
        {
            out << "Performing parallel Nash solution categorization of all games of size " << options.GetRows() << "x" << options.GetColumns() << " with the following player 2 strategy:\n" << std::endl;
            GameRunner< TRPMOStrategy<RowRankOrder>, 
                        TRPMOParallelIterator<TRPMONashCategorizationParallel> >
                        (options.GetRows(), options.GetColumns(), options.GetP2Strategy(), out);
        }
    }

    std::string Name() const
    {
        return "Nash Characterization Parallel";
    };


private:

    int m_rows;
    int m_columns;
    bool m_bIterateBoth;
    TRPMOStrategy<ColumnRankOrder> m_P1CompareStrategy;
    TRPMOStrategy<RowRankOrder> m_P2CompareStrategy;

    void IterateBoth(const tbb::blocked_range<TRPMOStrategy<ColumnRankOrder>::strategyIterator>& r)
    {
        TRPMOStrategy<RowRankOrder> p2Strategy(m_rows, m_columns);
        for (TRPMOStrategy<ColumnRankOrder>::strategyIterator itP1 = r.begin(); itP1 != r.end(); ++itP1)
        {
            TRPMOStrategy<RowRankOrder>::strategyIterator endIterP2 = p2Strategy.StrategyEnd();
            for (TRPMOStrategy<RowRankOrder>::strategyIterator itP2 = p2Strategy.StrategyBegin(); 
                itP2 != endIterP2; ++itP2)
            {
                NashSolutionType solution = CategorizeTRPMOGame(*itP1, *itP2);
                if (solution == UniqueNash)
                    ++m_results.numUniqueNash;
                else if (solution == NonuniqueNash)
                    ++m_results.numNonuniqueNash;
                else if (solution == NoNash)
                    ++m_results.numNoNash;
                else
                    MYASSERT(false, "Invalid solution type returned by CategorizeTRPMOGame.");
            }
        }
    }

    void IterateOne(const tbb::blocked_range<TRPMOStrategy<ColumnRankOrder>::strategyIterator>& r)
    {
        for (TRPMOStrategy<ColumnRankOrder>::strategyIterator itP1 = r.begin(); itP1 != r.end(); ++itP1)
        {
            NashSolutionType solution = CategorizeTRPMOGame(*itP1, m_P2CompareStrategy);
            if (solution == UniqueNash)
                ++m_results.numUniqueNash;
            else if (solution == NonuniqueNash)
                ++m_results.numNonuniqueNash;
            else if (solution == NoNash)
                ++m_results.numNoNash;
            else
                MYASSERT(false, "Invalid solution type returned by CategorizeTRPMOGame.");
        }
    }

    void IterateOne(const tbb::blocked_range<TRPMOStrategy<RowRankOrder>::strategyIterator>& r)
    {
        for (TRPMOStrategy<RowRankOrder>::strategyIterator itP2 = r.begin(); itP2 != r.end(); ++itP2)
        {
            NashSolutionType solution = CategorizeTRPMOGame(m_P1CompareStrategy, *itP2);
            if (solution == UniqueNash)
                ++m_results.numUniqueNash;
            else if (solution == NonuniqueNash)
                ++m_results.numNonuniqueNash;
            else if (solution == NoNash)
                ++m_results.numNoNash;
            else
                MYASSERT(false, "Invalid solution type returned by CategorizeTRPMOGame.");
        }
    }
};

#endif //__TRPMO_NASH_CATEGORIZATION_PARALLEL_H
