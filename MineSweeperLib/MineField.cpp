#include "MineField.h"

#include <algorithm>
#include <random>
#include <set>
#include <stdexcept>

namespace MineSweeper
{

    const std::string::value_type MineField::MINE = '*';

    MineField::MineField(const std::string& sMineField, std::int32_t nColumnCount, std::int32_t nLineCount)
        : m_GameState(GameState::FirstMove)
    {
        if (nColumnCount < 1 || nLineCount < 1)
            throw std::runtime_error("Invalid mine field size");

        m_MineField.reserve(nColumnCount);

        for (std::int32_t nColumn = 0; nColumn < nColumnCount; ++nColumn)
        {
            std::vector<Square> column;
            column.reserve(nLineCount);

            for (std::int32_t nLine = 0; nLine < nLineCount; ++nLine)
            {
                std::string::value_type c = sMineField[nLine * nColumnCount + nColumn];
                column.push_back(Square(c == MINE));
            }

            m_MineField.push_back(column);
        }

        m_nMineCount = GetMinesRemainingCount();
        m_GameState = GameState::Playing;
        m_StartTime = std::chrono::system_clock::now();
        if (m_fnGameStartedHandler)
            m_fnGameStartedHandler();
    }

    MineField::MineField(std::int32_t nColumnCount, std::int32_t nLineCount, std::int32_t nMineCount)
        : m_GameState(GameState::FirstMove)
    {
        if (nColumnCount < 1 || nLineCount < 1)
            throw std::runtime_error("Invalid mine field size");

        if (nMineCount < 1 || nMineCount > GetMaxMineCountForSize(nColumnCount, nLineCount))
            throw std::runtime_error("Invalid mine count");

        // Allocate and initialise to the specified size.
        std::vector<Square> column(nLineCount, Square(false));
        m_MineField.assign(nColumnCount, column);

        m_nMineCount = nMineCount;

        // When the user first clicks the board, we'll generate the mine positions. This allows
        // us to avoid generating a mine at that click location. This improves gameplay.
    }

    bool MineField::IsSquareAMine(std::int32_t nColumn, std::int32_t nLine) const
    {
        if (IsOutOfBounds(nColumn, nLine))
            return false;
        return m_MineField[nColumn][nLine].IsMine();
    }

    std::int32_t MineField::GetAdjacentMineCount(std::int32_t nColumn, std::int32_t nLine) const
    {
        std::int32_t nAdjacentSquareCount = 0;
        // Test the 8 adjacent squares. IsSquareAMine returns false for 'out of bounds' squares.
        if (IsSquareAMine(nColumn - 1, nLine - 1)) ++nAdjacentSquareCount;
        if (IsSquareAMine(nColumn - 1, nLine)) ++nAdjacentSquareCount;
        if (IsSquareAMine(nColumn - 1, nLine + 1)) ++nAdjacentSquareCount;
        if (IsSquareAMine(nColumn, nLine - 1)) ++nAdjacentSquareCount;
        if (IsSquareAMine(nColumn, nLine + 1)) ++nAdjacentSquareCount;
        if (IsSquareAMine(nColumn + 1, nLine - 1)) ++nAdjacentSquareCount;
        if (IsSquareAMine(nColumn + 1, nLine)) ++nAdjacentSquareCount;
        if (IsSquareAMine(nColumn + 1, nLine + 1)) ++nAdjacentSquareCount;
        return nAdjacentSquareCount;
    }

    std::int32_t MineField::GetAdjacentMarkedAsMineCount(std::int32_t nColumn, std::int32_t nLine) const
    {
        std::int32_t nMarkedAsMineCount = 0;
        // Test the 8 adjacent squares. IsSquareMarkedAsMine returns false for 'out of bounds' squares.
        if (IsSquareMarkedAsMine(nColumn - 1, nLine - 1)) ++nMarkedAsMineCount;
        if (IsSquareMarkedAsMine(nColumn - 1, nLine)) ++nMarkedAsMineCount;
        if (IsSquareMarkedAsMine(nColumn - 1, nLine + 1)) ++nMarkedAsMineCount;
        if (IsSquareMarkedAsMine(nColumn, nLine - 1)) ++nMarkedAsMineCount;
        if (IsSquareMarkedAsMine(nColumn, nLine + 1)) ++nMarkedAsMineCount;
        if (IsSquareMarkedAsMine(nColumn + 1, nLine - 1)) ++nMarkedAsMineCount;
        if (IsSquareMarkedAsMine(nColumn + 1, nLine)) ++nMarkedAsMineCount;
        if (IsSquareMarkedAsMine(nColumn + 1, nLine + 1)) ++nMarkedAsMineCount;
        return nMarkedAsMineCount;
    }

    std::int32_t MineField::GetMinMineCountForSize(std::int32_t nColumnCount, std::int32_t nLineCount)
    {
        // Only allow the mine field to be min 15% full
        return (nColumnCount * nLineCount * 15) / 100;
    }

    std::int32_t MineField::GetMaxMineCountForSize(std::int32_t nColumnCount, std::int32_t nLineCount)
    {
        // Only allow the mine field to be max 70% full
        return (nColumnCount * nLineCount * 70) / 100;
    }

    std::string MineField::GenerateOutput() const
    {
        std::string sOutput;
        std::int32_t nLineCount = static_cast<std::int32_t>(m_MineField.front().size()); // Use any column to get the line count

        for (std::int32_t nLine = 0; nLine < nLineCount; ++nLine)
            sOutput += GenerateLineOutput(nLine);
        return sOutput;
    }

    bool MineField::OpenSquare(std::int32_t nColumn, std::int32_t nLine)
    {
        if (IsOutOfBounds(nColumn, nLine))
            return false;

        if (m_GameState == GameState::FirstMove)
        {
            m_GameState = GameState::Playing;
            m_StartTime = std::chrono::system_clock::now();
            GenerateMineLocations(nColumn, nLine);
            if (m_fnGameStartedHandler)
                m_fnGameStartedHandler();
        }
        else if (m_GameState != GameState::Playing)
            return false;

        if (m_MineField[nColumn][nLine].IsOpened() || m_MineField[nColumn][nLine].GetMark() == Square::Mark::Mine)
            return false;

        m_MineField[nColumn][nLine].Open();
        m_MineField[nColumn][nLine].SetMark(Square::Mark::Unmarked);

        if (m_MineField[nColumn][nLine].IsMine())
        {
            m_GameState = GameState::Lost;
            if (m_fnGameLostHandler)
                m_fnGameLostHandler();
            return true;
        }

        if(m_MineField[nColumn][nLine].GetAdjacentMineCount() == 0)
        {
            SquareLocations alreadyOpened;
            OpenAndRecurse(nColumn, nLine, alreadyOpened);
        }

        if (AreAllNonMineSquaresOpen())
        {
            m_GameState = GameState::Won;
            if (m_fnGameWonHandler)
                m_fnGameWonHandler();
        }

        return true;
    }

    bool MineField::AutoOpenAdjacentSquares(std::int32_t nColumn, std::int32_t nLine)
    {
        if (IsOutOfBounds(nColumn, nLine) || !IsGamePlaying())
            return false;

        if (m_MineField[nColumn][nLine].IsMine())
            return OpenSquare(nColumn, nLine);

        if (GetAdjacentMineCount(nColumn, nLine) == GetAdjacentMarkedAsMineCount(nColumn, nLine))
        {
            OpenSquare(nColumn - 1, nLine - 1);
            OpenSquare(nColumn - 1, nLine);
            OpenSquare(nColumn - 1, nLine + 1);
            OpenSquare(nColumn, nLine - 1);
            OpenSquare(nColumn, nLine + 1);
            OpenSquare(nColumn + 1, nLine - 1);
            OpenSquare(nColumn + 1, nLine);
            OpenSquare(nColumn + 1, nLine + 1);
        }

        return true;
    }

    bool MineField::IsSquareOpened(std::int32_t nColumn, std::int32_t nLine) const
    {
        if (IsOutOfBounds(nColumn, nLine))
            return false;
        return m_MineField[nColumn][nLine].IsOpened();
    }

    void MineField::MarkSquareAsMine(std::int32_t nColumn, std::int32_t nLine)
    {
        Mark(nColumn, nLine, Square::Mark::Mine);
    }

    void MineField::MarkSquareAsUnknown(std::int32_t nColumn, std::int32_t nLine)
    {
        Mark(nColumn, nLine, Square::Mark::Unknown);
    }

    void MineField::UnmarkSquare(std::int32_t nColumn, std::int32_t nLine)
    {
        Mark(nColumn, nLine, Square::Mark::Unmarked);
    }

    bool MineField::IsSquareMarkedAsMine(std::int32_t nColumn, std::int32_t nLine) const
    {
        return CompareMark(nColumn, nLine, Square::Mark::Mine);
    }

    bool MineField::IsSquareMarkedAsUnknown(std::int32_t nColumn, std::int32_t nLine) const
    {
        return CompareMark(nColumn, nLine, Square::Mark::Unknown);
    }

    bool MineField::IsSquareUnmarked(std::int32_t nColumn, std::int32_t nLine) const
    {
        return CompareMark(nColumn, nLine, Square::Mark::Unmarked);
    }

    std::int32_t MineField::GetMinesRemainingCount() const
    {
        if (m_GameState == GameState::FirstMove)
            return m_nMineCount;

        std::int32_t nRemainingCount = 0;
        for (auto& column : m_MineField)
        {
            for (auto& square : column)
            {
                if (square.IsMine() && !square.IsOpened() && square.GetMark() != Square::Mark::Mine)
                    ++nRemainingCount;
            }
        }
        return nRemainingCount;
    }

    bool MineField::IsOutOfBounds(std::int32_t nColumn, std::int32_t nLine) const
    {
        if (nColumn < 0 || static_cast<Container::size_type>(nColumn) >= m_MineField.size())
            return true;

        return (nLine < 0 || static_cast<Container::size_type>(nLine) >= m_MineField[nColumn].size());
    }

    void MineField::Mark(std::int32_t nColumn, std::int32_t nLine, Square::Mark mark)
    {
        if (IsGamePlaying())
        {
            if (!IsOutOfBounds(nColumn, nLine) && !IsSquareOpened(nColumn, nLine))
                m_MineField[nColumn][nLine].SetMark(mark);
        }
    }

    bool MineField::CompareMark(std::int32_t nColumn, std::int32_t nLine, Square::Mark mark) const
    {
        if (IsOutOfBounds(nColumn, nLine))
            return false;
        return m_MineField[nColumn][nLine].GetMark() == mark;
    }

    void MineField::GenerateMineLocations(std::int32_t nPlayerClickColumn, std::int32_t nPlayerClickLine)
    {
        // Insert mines into the field at random locations. To prevent the same location being chosen
        // more than once, all locations are inserted into a container and a random item is removed from
        // the container.

        std::set<std::pair<std::int32_t, std::int32_t>> locations;
        for (std::int32_t nColumn = 0; nColumn < static_cast<int32_t>(m_MineField.size()); ++nColumn)
        {
            for (std::int32_t nLine = 0; nLine < static_cast<int32_t>(m_MineField[0].size()); ++nLine)
                locations.insert(std::make_pair(nColumn, nLine));
        }

        std::random_device seed;
        std::default_random_engine rng(seed());

        auto nMineCount = m_nMineCount;
        while (nMineCount)
        {
            std::uniform_int_distribution<std::size_t> randomRange(0, locations.size() - 1);
            std::size_t nLocationIndex = randomRange(rng);

            auto itor = locations.begin();
            std::advance(itor, nLocationIndex);

            auto location = *itor;
            if (location.first != nPlayerClickColumn || location.second != nPlayerClickLine) // Don't put a mine here.
            {
                locations.erase(itor);

                m_MineField[location.first][location.second] = Square(true);
                --nMineCount;
            }
        }

        // Cache the adjacent mine count for each square so that it's easy to
        // retrieve when painting the board
        for (std::int32_t nColumn = 0; nColumn < static_cast<int32_t>(m_MineField.size()); ++nColumn)
        {
            for (std::int32_t nLine = 0; nLine < static_cast<int32_t>(m_MineField[0].size()); ++nLine)
                m_MineField[nColumn][nLine].SetAdjacentMineCount(GetAdjacentMineCount(nColumn, nLine));
        }
    }

    void MineField::OpenAndRecurse(std::int32_t nColumn, std::int32_t nLine, SquareLocations& alreadyOpened)
    {
        // Be sure that we only Open each square once
        auto squareLocation = std::make_pair(nColumn, nLine);
        if (std::find(alreadyOpened.begin(), alreadyOpened.end(), squareLocation) != alreadyOpened.end())
            return;

        // Open it and save this fact
        OpenAllNeighbours(nColumn, nLine);
        alreadyOpened.push_back(squareLocation);

        // Collect all adjacent squares that themselves have 0 adjacent squares and haven't been Opened yet
        auto adjacentSquares = CollectZeroCountAdjacentSquares(nColumn, nLine, alreadyOpened);
        for (auto adjacentSquare : adjacentSquares)
            OpenAndRecurse(adjacentSquare.first, adjacentSquare.second, alreadyOpened);
    }

    void MineField::OpenAllNeighbours(std::int32_t nColumn, std::int32_t nLine)
    {
        OpenNeighbour(nColumn - 1, nLine - 1);
        OpenNeighbour(nColumn - 1, nLine);
        OpenNeighbour(nColumn - 1, nLine + 1);
        OpenNeighbour(nColumn, nLine - 1);
        OpenNeighbour(nColumn, nLine + 1);
        OpenNeighbour(nColumn + 1, nLine - 1);
        OpenNeighbour(nColumn + 1, nLine);
        OpenNeighbour(nColumn + 1, nLine + 1);
    }

    void MineField::OpenNeighbour(std::int32_t nColumn, std::int32_t nLine)
    {
        if (!IsOutOfBounds(nColumn, nLine))
        {
            m_MineField[nColumn][nLine].Open();
            m_MineField[nColumn][nLine].SetMark(Square::Mark::Unmarked);
        }
    }

    MineField::SquareLocations MineField::CollectZeroCountAdjacentSquares(std::int32_t nColumn, std::int32_t nLine, SquareLocations& alreadyOpened)
    {
        SquareLocations adjacentSquares;

        CheckAndAddAdjacentSquare(adjacentSquares, nColumn - 1, nLine - 1, alreadyOpened);
        CheckAndAddAdjacentSquare(adjacentSquares, nColumn - 1, nLine, alreadyOpened);
        CheckAndAddAdjacentSquare(adjacentSquares, nColumn - 1, nLine + 1, alreadyOpened);
        CheckAndAddAdjacentSquare(adjacentSquares, nColumn, nLine - 1, alreadyOpened);
        CheckAndAddAdjacentSquare(adjacentSquares, nColumn, nLine + 1, alreadyOpened);
        CheckAndAddAdjacentSquare(adjacentSquares, nColumn + 1, nLine - 1, alreadyOpened);
        CheckAndAddAdjacentSquare(adjacentSquares, nColumn + 1, nLine, alreadyOpened);
        CheckAndAddAdjacentSquare(adjacentSquares, nColumn + 1, nLine + 1, alreadyOpened);

        return adjacentSquares;
    }

    void MineField::CheckAndAddAdjacentSquare(SquareLocations& adjacentSquares, std::int32_t nColumn, std::int32_t nLine, SquareLocations& alreadyOpened)
    {
        if (IsOutOfBounds(nColumn, nLine) || m_MineField[nColumn][nLine].IsMine())
            return;

        auto squareLocation = std::make_pair(nColumn, nLine);
        if (std::find(alreadyOpened.begin(), alreadyOpened.end(), squareLocation) != alreadyOpened.end())
            return;

        if (GetAdjacentMineCount(nColumn, nLine) == 0)
            adjacentSquares.push_back(std::make_pair(nColumn, nLine));
    }

    bool MineField::AreAllNonMineSquaresOpen() const
    {
        for (auto& column : m_MineField)
        {
            for (auto& square : column)
            {
                if (!square.IsMine() && !square.IsOpened())
                    return false;
            }
        }
        return true;
    }

    std::string MineField::GenerateLineOutput(std::int32_t nLine) const
    {
        std::string sOutput(4, ' '); // Indentation

        std::int32_t nColumnCount = static_cast<std::int32_t>(m_MineField.size());

        for (std::int32_t nColumn = 0; nColumn < nColumnCount; ++nColumn)
        {
            if (m_MineField[nColumn][nLine].IsMine())
                sOutput += std::string(1, MINE);
            else
                sOutput += std::to_string(GetAdjacentMineCount(nColumn, nLine));
        }
        return sOutput + "\n";
    }


} // namespace MineSweeper
