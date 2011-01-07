#ifndef __GAME_H
#define __GAME_H

#include "TRPMOStrategy.h"
#include "RankOrder.h"

class ostream;

template <class StrategyType, class GameIteratorType>
class Game {};

template <class GameIteratorType>
class Game<TRPMOStrategy<ColumnRankOrder>, GameIteratorType>
{
public:
    Game(const TRPMOStrategy<ColumnRankOrder>& p1Strategy) :
        m_gameIterator(p1Strategy) {}

    void IterateStrategies()
    {
        m_gameIterator.IterateP2Strategies();
    }

    void DisplayResults(std::ostream& out)
    {
        m_gameIterator.Comparator().DisplayResults(out);
    }

    const GameIteratorType& Iterator() const
    {
        return m_gameIterator;
    }

private:
    GameIteratorType m_gameIterator;
};

template <class GameIteratorType>
class Game<TRPMOStrategy<RowRankOrder>, GameIteratorType>
{
public:
    Game(const TRPMOStrategy<RowRankOrder>& p2Strategy) :
        m_gameIterator(p2Strategy) {}

    void IterateStrategies()
    {
        m_gameIterator.IterateP1Strategies();
    }

    void DisplayResults(std::ostream& out)
    {
        m_gameIterator.Comparator().DisplayResults(out);
    }

    const GameIteratorType& Iterator() const
    {
        return m_gameIterator;
    }

private:
    GameIteratorType m_gameIterator;
};


template <class GameIteratorType>
class GameDouble
{
public:
    GameDouble(int rows, int columns) :
        m_rows(rows),
        m_columns(columns),
        m_gameIterator(rows, columns)
    {
    }

    void IterateStrategies()
    {
        m_gameIterator.IterateStrategies();
    }

    const GameIteratorType& Iterator() const
    {
        return m_gameIterator;
    }

    void DisplayResults(std::ostream& out)
    {
        m_gameIterator.Comparator().DisplayResults(out);
    }

private:
    int m_rows;
    int m_columns;
    GameIteratorType m_gameIterator;
};

template <class GameIteratorType>
class GameRandom
{
public:
    GameRandom(int rows, int columns, RandomUniform* randGen) :
        m_rows(rows),
        m_columns(columns),
        m_CardinalStrategy(rows, columns, randGen),
        m_TRPMOStrategy(m_CardinalStrategy),
        m_gameIterator(m_TRPMOStrategy)
    {
    }

    void IterateStrategies()
    {
        m_gameIterator.IterateP2Strategies();
    }

    const GameIteratorType& Iterator() const
    {
        return m_gameIterator;
    }

    void DisplayResults(std::ostream& out)
    {
        m_gameIterator.Comparator().DisplayResults(out);
    }

private:
    int m_rows;
    int m_columns;
    CardinalStrategy m_CardinalStrategy;
    TRPMOStrategy<ColumnRankOrder> m_TRPMOStrategy;
    GameIteratorType m_gameIterator;
};

#endif  //__GAME_H
