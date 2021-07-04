#pragma once

#include "MineSweeper.h"

namespace MineSweeper
{

    class MineSweeperFactory
    {
    public:
        // The file is assumed to be encoded with ASCII
        static MineSweeper CreateFromFile(const std::string& sFileName); // throws std::runtime_error
        static MineSweeper CreateFromText(const std::string& sMineFieldSpec);
        static MineSweeper CreateFromMetrics(std::int32_t nColumnCount, std::int32_t nLineCount, std::int32_t nMineCount);
    };


} // namespace MineSweeper
