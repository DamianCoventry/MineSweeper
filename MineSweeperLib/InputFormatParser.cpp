#include "InputFormatParser.h"

#include <sstream>

namespace MineSweeper
{

    std::vector<MineField> InputFormatParser::Parse(const std::string & sMineFieldSpec)
    {
        std::vector<MineField> mineFields;

        std::string sLine, sSquares;
        int nLineCount, nColumnCount, nNumReadLines;

        enum class State { Dimensions, Squares };
        State state = State::Dimensions;

        bool bReadSentinel = false;
        std::istringstream input(sMineFieldSpec);
        while (std::getline(input, sLine) && !bReadSentinel)
        {
            switch (state)
            {
            case State::Dimensions:
            {
                std::istringstream dimensions(sLine);
                dimensions >> nColumnCount >> nLineCount;
                if (nColumnCount == 0 && nLineCount == 0)
                    bReadSentinel = true;
                else
                {
                    // TODO: validate the values held in nLineCount and nColumnCount
                    state = State::Squares;
                    nNumReadLines = 0;
                    sSquares.clear();
                }
                break;
            }
            case State::Squares:
            {
                // TODO: Check that nColumnCount == sLine.size()
                sSquares += sLine;
                if (++nNumReadLines == nLineCount)
                {
                    mineFields.push_back(MineField(sSquares, nColumnCount, nLineCount));
                    state = State::Dimensions;
                }
                break;
            }
            }
        }

        return mineFields;
    }


} // namespace MineSweeper
