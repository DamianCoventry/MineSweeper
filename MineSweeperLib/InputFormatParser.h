#pragma once

#include "MineField.h"

#include <vector>

// The prescribed input format is:
//
// N M\n                N = number of lines, M = number of columns.
// ...*..\n             The next 'N' lines contain exactly 'M' characters.
// ......\n
// etc
// N M\n
// ...*..\n             'Mine' squares are reresented by a * character
// ......\n             'Safe' squares are reresented by a . character
// etc
// 0 0\n                The first field line where N = M = 0 represents the end of input and should not be processed.
//

namespace MineSweeper
{

    class InputFormatParser
    {
    public:
        static std::vector<MineField> Parse(const std::string& sMineFieldSpec);
    };


} // namespace MineSweeper
