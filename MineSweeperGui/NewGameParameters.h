#pragma once

struct NewGameParameters
{
    int m_nColumnCount;
    int m_nLineCount;
    int m_nMineCount;

    bool operator==(const NewGameParameters& other) const
    {
        return ((m_nColumnCount == other.m_nColumnCount) && (m_nLineCount == other.m_nLineCount) && (m_nMineCount == other.m_nMineCount));
    }
};
