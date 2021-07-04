#pragma once

#include "MineField.h"

#include <vector>

namespace MineSweeper
{

    class MineSweeper
    {
    public:
        friend class MineSweeperFactory;

    private:
        MineSweeper(const std::vector<MineField>& mineFields); // Created via MineSweeperFactory
        MineSweeper(std::int32_t nColumnCount, std::int32_t nLineCount, std::int32_t nMineCount);

    public:
        std::vector<MineField>& GetMineFields() { return m_MineFields; }

    public:
        std::string GenerateOutput() const;

    private:
        std::vector<MineField> m_MineFields;
    };


} // namespace MineSweeper
