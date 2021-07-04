#include "MineSweeperFactory.h"
#include "InputFormatParser.h"

#include <fstream>
#include <sstream>
#include <stdexcept>

namespace MineSweeper
{
    MineSweeper MineSweeperFactory::CreateFromFile(const std::string & sFileName)
    {
        std::ifstream file(sFileName);
        if (!file)
            throw std::runtime_error("Unable to open the file");

        std::string sMineFieldSpec(
            (std::istreambuf_iterator<char>(file)), // most vexing parse
            std::istreambuf_iterator<char>());

        return CreateFromText(sMineFieldSpec);
    }

    MineSweeper MineSweeperFactory::CreateFromText(const std::string& sMineFieldSpec)
    {
        return MineSweeper(InputFormatParser::Parse(sMineFieldSpec));
    }

    MineSweeper MineSweeperFactory::CreateFromMetrics(std::int32_t nColumnCount, std::int32_t nLineCount, std::int32_t nMineCount)
    {
        return MineSweeper(nColumnCount, nLineCount, nMineCount);
    }


} // namespace MineSweeper
