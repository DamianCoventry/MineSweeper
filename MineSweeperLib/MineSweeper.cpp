#include "MineSweeper.h"

#include <sstream>

namespace MineSweeper
{

    MineSweeper::MineSweeper(const std::vector<MineField>& mineFields)
        : m_MineFields(mineFields)
    {
    }

    MineSweeper::MineSweeper(std::int32_t nColumnCount, std::int32_t nLineCount, std::int32_t nMineCount)
        : m_MineFields(1, MineField(nColumnCount, nLineCount, nMineCount))
    {
    }

    std::string MineSweeper::GenerateOutput() const
    {
        std::ostringstream output;
        int nIndex = 1; // Deliberate
        for (auto mineField : m_MineFields)
        {
            output
                << "Field #" << nIndex++ << ":\n"
                << mineField.GenerateOutput()
                << "\n";
        }
        return output.str();
    }


} // namespace MineSweeper
