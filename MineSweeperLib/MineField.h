#pragma once

#include "Square.h"

#include <chrono>
#include <cstdint>
#include <functional>
#include <string>
#include <vector>

// The prescribed output format is:
//
// Field #x:\n          Where 'x' is the number of the field, 1 based.
//     0111\n           Where the numbers are the count of mines adjacent to the square.
//     01*1\n           Where * is still the mine.
//     etc
// \n
// Field #x:\n
//     0111\n
//     01*1\n
//     etc
// \n
// etc
//

namespace MineSweeper
{

    class MineField
    {
    public:
        static const std::string::value_type MINE;
        static const std::int32_t s_nMinColumnCount = 10;
        static const std::int32_t s_nMaxColumnCount = 70;
        static const std::int32_t s_nMinLineCount = 10;
        static const std::int32_t s_nMaxLineCount = 40;

    public:
        MineField(const std::string& sMineField, std::int32_t nColumnCount, std::int32_t nLineCount);
        MineField(std::int32_t nColumnCount, std::int32_t nLineCount, std::int32_t nMineCount);

    public:
        void SetGameStartedHandler(const std::function<void()>& handler) { m_fnGameStartedHandler = handler; }
        void SetGameWonHandler(const std::function<void()>& handler) { m_fnGameWonHandler = handler; }
        void SetGameLostHandler(const std::function<void()>& handler) { m_fnGameLostHandler = handler; }

    public:
        bool IsSquareAMine(std::int32_t nColumn, std::int32_t nLine) const;
        std::int32_t GetAdjacentMineCount(std::int32_t nColumn, std::int32_t nLine) const;
        std::int32_t GetAdjacentMarkedAsMineCount(std::int32_t nColumn, std::int32_t nLine) const;

    public:
        static std::int32_t GetMinMineCountForSize(std::int32_t nColumnCount, std::int32_t nLineCount);
        static std::int32_t GetMaxMineCountForSize(std::int32_t nColumnCount, std::int32_t nLineCount);

    public:
        std::string GenerateOutput() const;
        std::int32_t GetColumnCount() const { return static_cast<std::int32_t>(m_MineField.size()); }
        std::int32_t GetLineCount() const { if (m_MineField.empty()) return -1; return static_cast<std::int32_t>(m_MineField[0].size()); }

    public:
        bool OpenSquare(std::int32_t nColumn, std::int32_t nLine);
        bool AutoOpenAdjacentSquares(std::int32_t nColumn, std::int32_t nLine);
        bool IsSquareOpened(std::int32_t nColumn, std::int32_t nLine) const;

    public:
        void MarkSquareAsMine(std::int32_t nColumn, std::int32_t nLine);
        void MarkSquareAsUnknown(std::int32_t nColumn, std::int32_t nLine);
        void UnmarkSquare(std::int32_t nColumn, std::int32_t nLine);

    public:
        bool IsSquareMarkedAsMine(std::int32_t nColumn, std::int32_t nLine) const;
        bool IsSquareMarkedAsUnknown(std::int32_t nColumn, std::int32_t nLine) const;
        bool IsSquareUnmarked(std::int32_t nColumn, std::int32_t nLine) const;

    public:
        bool IsSquareExploded(std::int32_t nColumn, std::int32_t nLine) const { return IsSquareOpened(nColumn, nLine) && IsSquareAMine(nColumn, nLine); }
        bool IsSquareIncorrectlyMarked(std::int32_t nColumn, std::int32_t nLine) const { return IsSquareMarkedAsMine(nColumn, nLine) && !IsSquareAMine(nColumn, nLine); }

    private:
        enum class GameState { FirstMove, Playing, Won, Lost };
    public:
        bool IsGameFirstMove() const { return m_GameState == GameState::FirstMove; }
        bool IsGamePlaying() const { return m_GameState == GameState::Playing; }
        bool IsGameWon() const { return m_GameState == GameState::Won; }
        bool IsGameLost() const { return m_GameState == GameState::Lost; }

    public:
        std::int32_t GetMinesRemainingCount() const;

        std::chrono::system_clock::duration GetElapsedTime() const { return std::chrono::system_clock::now() - m_StartTime; }

    private:
        bool IsOutOfBounds(std::int32_t nColumn, std::int32_t nLine) const;
        void Mark(std::int32_t nColumn, std::int32_t nLine, Square::Mark mark);
        bool CompareMark(std::int32_t nColumn, std::int32_t nLine, Square::Mark mark) const;

    private:
        void GenerateMineLocations(std::int32_t nPlayerClickColumn, std::int32_t nPlayerClickLine);

    private:
        typedef std::vector<std::pair<std::int32_t, std::int32_t>> SquareLocations;

    private:
        void OpenAndRecurse(std::int32_t nColumn, std::int32_t nLine, SquareLocations& alreadyOpened);
        void OpenAllNeighbours(std::int32_t nColumn, std::int32_t nLine);
        void OpenNeighbour(std::int32_t nColumn, std::int32_t nLine);
        SquareLocations CollectZeroCountAdjacentSquares(std::int32_t nColumn, std::int32_t nLine, SquareLocations& alreadyOpened);
        void CheckAndAddAdjacentSquare(SquareLocations& adjacentSquares, std::int32_t nColumn, std::int32_t nLine, SquareLocations& alreadyOpened);
        bool AreAllNonMineSquaresOpen() const;

    private:
        std::string GenerateLineOutput(std::int32_t nLine) const;

    private:
        typedef std::vector<std::vector<Square>> Container;
        Container m_MineField;
        GameState m_GameState;

    private:
        std::int32_t m_nMineCount;
        std::chrono::system_clock::time_point m_StartTime;

    private:
        std::function<void()> m_fnGameStartedHandler;
        std::function<void()> m_fnGameWonHandler;
        std::function<void()> m_fnGameLostHandler;
    };


} // namespace MineSweeper
