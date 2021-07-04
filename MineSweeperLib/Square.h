#pragma once

#include <cstdint>

namespace MineSweeper
{

    class Square
    {
    public:
        Square(bool bMine)
            : m_bMine(bMine)
            , m_bOpened(false)
            , m_Mark(Mark::Unmarked)
            , m_nAdjacentMineCount(0) {}

    public:
        bool IsMine() const { return m_bMine; }

    public:
        void Open() { m_bOpened = true; }
        bool IsOpened() const { return m_bOpened; }

    public:
        enum class Mark { Unmarked, Mine, Unknown };
        void SetMark(Mark mark) { m_Mark = mark; }
        Mark GetMark() const { return m_Mark; }

    public:
        // Caching the adjacent mine count here makes drawing the game board quicker
        void SetAdjacentMineCount(std::int32_t nAdjacentMineCount) { m_nAdjacentMineCount = nAdjacentMineCount; }
        std::int32_t GetAdjacentMineCount() const { return m_nAdjacentMineCount; }

    private:
        bool m_bMine;
        bool m_bOpened;

    private:
        Mark m_Mark;
        std::int32_t m_nAdjacentMineCount;
    };


} // namespace MineSweeper
