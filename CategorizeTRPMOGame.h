#ifndef __CATEGORIZE_TRPMO_GAME_H
#define __CATEGORIZE_TRPMO_GAME_H

#include "TRPMOStrategy.h"
#include "NashSolutionType.h"
#include "RankOrder.h"

NashSolutionType CategorizeTRPMOGame(const TRPMOStrategy<ColumnRankOrder>& p1CompareStrategy,
                    const TRPMOStrategy<RowRankOrder>& p2CompareStrategy)
{
    int numNashSolutions = 0;
    TRPMOStrategy<ColumnRankOrder>::const_RowWiseIterator p1Iter = p1CompareStrategy.RowWiseBegin();
    TRPMOStrategy<ColumnRankOrder>::const_RowWiseIterator p1End = p1CompareStrategy.RowWiseEnd();
    TRPMOStrategy<RowRankOrder>::const_RowWiseIterator p2Iter = p2CompareStrategy.RowWiseBegin();
    for (; p1Iter != p1End; ++p1Iter, ++p2Iter)
    {
        //TODO Change 1 to TRPValue enum
        if (*p1Iter == 1 && *p2Iter == 1)
            ++numNashSolutions;

        if (numNashSolutions > 1)
            return NonuniqueNash;
    }

    if (numNashSolutions == 1)
        return UniqueNash;
    else
        return NoNash;
}

#endif // __CATEGORIZE_TRPMO_GAME_H
