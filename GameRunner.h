#ifndef __GAME_RUNNER_H
#define __GAME_RUNNER_H

#include <iosfwd>
#include "Game.h"
#include "CardinalStrategy.h"

template <class StrategyType, class IteratorType>
void GameRunner(int rows, int columns, std::string inStrategy, std::ostream& out)
{
    std::stringstream ssStrategy;
    ssStrategy << inStrategy;

    CardinalStrategy cardinalStrategy(rows, columns,
                                        std::istream_iterator<double>(ssStrategy),
                                        std::istream_iterator<double>());

    StrategyType strategy(cardinalStrategy);
    Game<StrategyType, IteratorType> game(strategy);

    TBBTimer timer;
    TIME_OPERATION(timer, game.IterateStrategies());

    game.DisplayResults(out);
    out << "Run took " << timer.ElapsedSeconds() << " seconds." << std::endl;
}

template<class IteratorType>
void GameDoubleRunner(int rows, int columns, std::ostream& out)
{
    GameDouble<IteratorType> game(rows, columns);

    TBBTimer timer;
    TIME_OPERATION(timer, game.IterateStrategies());

    game.DisplayResults(out);
    out << "Run took " << timer.ElapsedSeconds() << " seconds." << std::endl;
}
#endif //__GAME_RUNNER_H