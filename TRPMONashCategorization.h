#ifndef __TRPMO_NASH_CATEGORIZATION_H
#define __TRPMO_NASH_CATEGORIZATION_H

#include <functional>
#include <iostream>
#include <sstream>
#include "MyAssert.h"
#include "TRPMOStrategy.h"
#include "CategorizeTRPMOGame.h"
#include "TRPMONashResults.h"
#include "ISolutionMethod.h"
#include "Game.h"
#include "ProgramOptions.h"
#include "TBBTimer.h"
#include "GameRunner.h"

class TRPMONashCategorization : public std::binary_function<TRPMOStrategy<ColumnRankOrder>, 
                                                            TRPMOStrategy<RowRankOrder>, void>, 
                                public TRPMONashResults,
                                public ISolutionMethod
{
public:
    TRPMONashCategorization() {}

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

    // ISolutionMethod
    void Run(const ProgramOptions& options, std::ostream& out)
    {
        MYASSERT(options.GetMethod() == this->Name(), "Invalid call to ISolutionMethod::Run");

        if (options.GetP1Strategy() == "" && options.GetP2Strategy() == "")
        {
            out << "Performing serial Nash solution categorization of all games of size " << options.GetRows() << "x" << options.GetColumns() <<"..." << std::endl;
            GameDoubleRunner< TRPMOSerialIterator<TRPMONashCategorization> >(options.GetRows(), options.GetColumns(), out);
        }
        else if (options.GetP1Strategy() != "")
        {
            out << "Performing serial Nash solution categorization of all games of size " << options.GetRows() << "x" << options.GetColumns() << " with the following player 1 strategy:\n" << std::endl;
            GameRunner< TRPMOStrategy<ColumnRankOrder>, 
                        TRPMOSerialIterator<TRPMONashCategorization> >
                        (options.GetRows(), options.GetColumns(), options.GetP1Strategy(), out);
        }
        else if (options.GetP2Strategy() != "")
        {
            out << "Performing serial Nash solution categorization of all games of size " << options.GetRows() << "x" << options.GetColumns() << " with the following player 2 strategy:\n" << std::endl;
            GameRunner< TRPMOStrategy<RowRankOrder>, 
                        TRPMOSerialIterator<TRPMONashCategorization> >
                        (options.GetRows(), options.GetColumns(), options.GetP2Strategy(), out);
        }
    }

    std::string Name() const
    {
        return "Nash Characterization Serial";
    };

};

#endif //__TRPMO_NASH_CATEGORIZATION_H
