// MineFieldWindow.cpp : implementation file
//

#include "stdafx.h"
#include "MineSweeperGui.h"
#include "MineFieldWindow.h"

#include "resource.h"

#include <stdexcept>

// CMineFieldWindow

IMPLEMENT_DYNAMIC(CMineFieldWindow, CWnd)

const _TCHAR* CMineFieldWindow::s_sClassName = _T("zldjvhfosaudfy298r74youafhldasf");

const COLORREF CMineFieldWindow::s_ColourLookup[9] =
{
    RGB(  0,   0,   0),         // No-op
    RGB(  0,   0, 255),         // The colour of number 1
    RGB(  0, 127,   0),         // The colour of number 2
    RGB(255,   0,   0),         // The colour of number 3
    RGB(  0,   0, 127),         // The colour of number 4
    RGB(127,   1,   2),         // The colour of number 5
    RGB(  0, 127, 127),         // The colour of number 6
    RGB( 16,  16,  16),         // The colour of number 7
    RGB(127, 127, 127),         // The colour of number 8
};

int CMineFieldWindow::m_BitmapResIds[CMineFieldWindow::BitmapCount] =
{
    IDB_LANDMINE, IDB_FLAG, IDB_EMPTYSQUARE, IDB_UNOPENEDSQUARE, IDB_LANDMINEEXPLODED, IDB_LANDMINEINCORRECT
};

CMineFieldWindow::CMineFieldWindow()
    : m_MineSweeper(nullptr)
{
    WNDCLASS windowClass;
    windowClass.style = CS_OWNDC | CS_DBLCLKS;
    windowClass.lpfnWndProc = ::DefWindowProc;
    windowClass.cbClsExtra = 0;
    windowClass.cbWndExtra = 0;
    windowClass.hInstance = AfxGetInstanceHandle();
    windowClass.hIcon = nullptr;
    windowClass.hCursor = AfxGetApp()->LoadCursor(IDC_ARROW);
    windowClass.hbrBackground = nullptr; // deliberately not painting the background
    windowClass.lpszMenuName = nullptr;
    windowClass.lpszClassName = s_sClassName;
    if (!AfxRegisterClass(&windowClass))
        throw std::runtime_error("Unable to register the window class");
}

CMineFieldWindow::~CMineFieldWindow()
{
}

#if defined _M_IX86
BOOL CMineFieldWindow::Create(int x, int y, int nWidth, int nHeight, HWND hWndParent, LONG nID)
#elif defined _M_X64
BOOL CMineFieldWindow::Create(int x, int y, int nWidth, int nHeight, HWND hWndParent, LONGLONG nID)
#endif
{
    if (!CreateEx(WS_EX_CLIENTEDGE, s_sClassName, L"", WS_CHILD | WS_VISIBLE | WS_TABSTOP,
        x, y, nWidth, nHeight, hWndParent, (HMENU)nID, this))
        return FALSE;

    m_NoGameSetBrush.CreateSysColorBrush(COLOR_ACTIVECAPTION);
    m_OpenedSquareBrush.CreateSysColorBrush(COLOR_INACTIVECAPTION);
    m_UnOpenedSquareBrush.CreateSolidBrush(RGB(0, 255, 0));

    for (int i = 0; i < BitmapCount; ++i)
    {
        m_Bitmaps[i].LoadBitmap(m_BitmapResIds[i]);
        m_Bitmaps[i].GetBitmap(&m_BitmapInfos[i]);
    }

    CDC* dc = GetDC();
    m_MemoryDC.CreateCompatibleDC(dc);

    HFONT hFont = (HFONT)::GetStockObject(DEFAULT_GUI_FONT);
    if (hFont == NULL)
        hFont = (HFONT)::GetStockObject(SYSTEM_FONT);

    LOGFONT logicalFont;
    GetObject(hFont, sizeof(LOGFONT), &logicalFont);
    _tcsncpy_s(logicalFont.lfFaceName, LF_FACESIZE, _T("Tahoma"), LF_FACESIZE);
    logicalFont.lfQuality = CLEARTYPE_NATURAL_QUALITY;
    logicalFont.lfWeight = FW_BOLD;
    logicalFont.lfHeight = -MulDiv(12, GetDeviceCaps(dc->GetSafeHdc(), LOGPIXELSY), 72);

    m_AdjacentSquareCountFont.CreateFontIndirect(&logicalFont);

    ReleaseDC(dc);
    return TRUE;
}

void CMineFieldWindow::StartNewGame(MineSweeper::MineSweeper * mineSweeper)
{
    m_MineSweeper = mineSweeper;

    m_MineSweeper->GetMineFields()[0].SetGameStartedHandler(std::bind(&CMineFieldWindow::OnGameStarted, this));
    m_MineSweeper->GetMineFields()[0].SetGameWonHandler(std::bind(&CMineFieldWindow::OnGameWon, this));
    m_MineSweeper->GetMineFields()[0].SetGameLostHandler(std::bind(&CMineFieldWindow::OnGameLost, this));

    InvalidateRect(nullptr);
}

BEGIN_MESSAGE_MAP(CMineFieldWindow, CWnd)
    ON_WM_PAINT()
    ON_WM_LBUTTONDOWN()
    ON_WM_RBUTTONDOWN()
    ON_WM_LBUTTONDBLCLK()
    ON_WM_MBUTTONDOWN()
    ON_WM_LBUTTONDBLCLK()
    ON_WM_MBUTTONDBLCLK()
    ON_WM_RBUTTONDBLCLK()
END_MESSAGE_MAP()

// CMineFieldWindow message handlers

void CMineFieldWindow::OnPaint()
{
    CPaintDC dc(this);

    if (m_MineSweeper && m_MineSweeper->GetMineFields().size() == 1)
    {
        DrawGameBoard(dc);
        return;
    }

    // Erase the background
    RECT rect;
    GetClientRect(&rect);
    dc.FillRect(&rect, &m_NoGameSetBrush);
}

void CMineFieldWindow::DrawGameBoard(CPaintDC& dc)
{
    MineSweeper::MineField& mineField = m_MineSweeper->GetMineFields()[0]; // Only care about the first one

    RECT rect;
    rect.left = rect.top = 0;
    rect.right = rect.bottom = s_nSquareSize;

    for (std::int32_t nLine = 0; nLine < mineField.GetLineCount(); ++nLine)
    {
        rect.left = 0;
        rect.right = s_nSquareSize;

        for (std::int32_t nColumn = 0; nColumn < mineField.GetColumnCount(); ++nColumn)
        {
            DrawGameSquare(dc, &rect, mineField, nColumn, nLine);

            rect.left += s_nSquareSize;
            rect.right += s_nSquareSize;
        }

        rect.top += s_nSquareSize;
        rect.bottom += s_nSquareSize;
    }
}

void CMineFieldWindow::DrawGameSquare(CPaintDC& dc, RECT* rect, MineSweeper::MineField& mineField, std::int32_t nColumn, std::int32_t nLine)
{
    bool bSquareDrawn = false;

    if (mineField.IsGameLost())
    {
        if (mineField.IsSquareExploded(nColumn, nLine))
        {
            DrawImage(dc, rect, &m_Bitmaps[LandMineExploded], m_BitmapInfos[LandMineExploded]);
            bSquareDrawn = true;
        }
        else if (mineField.IsSquareIncorrectlyMarked(nColumn, nLine))
        {
            DrawImage(dc, rect, &m_Bitmaps[LandMineIncorrect], m_BitmapInfos[LandMineIncorrect]);
            bSquareDrawn = true;
        }
        else if (mineField.IsSquareAMine(nColumn, nLine))
        {
            DrawImage(dc, rect, &m_Bitmaps[LandMine], m_BitmapInfos[LandMine]);
            bSquareDrawn = true;
        }
    }

    if (!bSquareDrawn)
    {
        if (mineField.IsSquareOpened(nColumn, nLine))
        {
            if (mineField.IsSquareAMine(nColumn, nLine))
                DrawImage(dc, rect, &m_Bitmaps[LandMine], m_BitmapInfos[LandMine]);
            else
            {
                auto nAdjacentMineCount = mineField.GetAdjacentMineCount(nColumn, nLine);
                if (nAdjacentMineCount > 0)
                    DrawAdjacentMineCountSquare(dc, rect, nAdjacentMineCount);
                else
                    DrawImage(dc, rect, &m_Bitmaps[EmptySquare], m_BitmapInfos[EmptySquare]);
            }
        }
        else if (mineField.IsSquareMarkedAsMine(nColumn, nLine))
            DrawImage(dc, rect, &m_Bitmaps[Flag], m_BitmapInfos[Flag]);
        else if (mineField.IsSquareMarkedAsUnknown(nColumn, nLine))
            DrawQuestionMarkSquare(dc, rect);
        else if (mineField.IsSquareUnmarked(nColumn, nLine))
            DrawImage(dc, rect, &m_Bitmaps[UnopenedSquare], m_BitmapInfos[UnopenedSquare]);
        else
            DrawImage(dc, rect, &m_Bitmaps[EmptySquare], m_BitmapInfos[EmptySquare]); // Shouldn't happen
    }
}

void CMineFieldWindow::DrawAdjacentMineCountSquare(CPaintDC & dc, RECT* rect, int nCount)
{
    DrawImage(dc, rect, &m_Bitmaps[EmptySquare], m_BitmapInfos[EmptySquare]);

    _TCHAR buffer[16];
    _itot_s(nCount, buffer, 10);
    DrawTextString(dc, rect, buffer, 1, s_ColourLookup[nCount]);  // Count will be 0 - 8
}

void CMineFieldWindow::DrawQuestionMarkSquare(CPaintDC & dc, RECT* rect)
{
    DrawImage(dc, rect, &m_Bitmaps[UnopenedSquare], m_BitmapInfos[UnopenedSquare]);
    DrawTextString(dc, rect, _T("?"), 1, RGB(0, 0, 0));
}

void CMineFieldWindow::DrawTextString(CPaintDC & dc, RECT * rect, const _TCHAR * textString, int nNumChars, COLORREF colour)
{
    int nOldMode = dc.SetBkMode(TRANSPARENT);
    COLORREF oldColour = dc.SetTextColor(colour);
    CGdiObject* previousFont = dc.SelectObject(&m_AdjacentSquareCountFont);
    dc.DrawText(textString, nNumChars, rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
    dc.SelectObject(previousFont);
    dc.SetTextColor(oldColour);
    dc.SetBkMode(nOldMode);
}

void CMineFieldWindow::DrawImage(CPaintDC& dc, RECT* rect, CBitmap* bitmap, const BITMAP& bitmapInfo)
{
    CBitmap* previousBitmap = m_MemoryDC.SelectObject(bitmap);
    dc.StretchBlt(
        rect->left, rect->top,
        rect->right - rect->left, rect->bottom - rect->top,
        &m_MemoryDC,
        0, 0,
        bitmapInfo.bmWidth, bitmapInfo.bmHeight,
        SRCCOPY);
    m_MemoryDC.SelectObject(previousBitmap);
}

void CMineFieldWindow::ConvertClientCoordToMinefieldCoord(const CPoint & clientCoord, std::int32_t & nColumn, std::int32_t & nLine)
{
    nColumn = clientCoord.x / s_nSquareSize;
    nLine = clientCoord.y / s_nSquareSize;
}

void CMineFieldWindow::NotifyParent(int nNotifyCode)
{
    NMHDR notify;
    notify.code = nNotifyCode;
    notify.idFrom = GetDlgCtrlID();
    notify.hwndFrom = GetSafeHwnd();
    GetParent()->SendMessage(WM_NOTIFY, notify.idFrom, (LPARAM)&notify);
}

void CMineFieldWindow::OnGameStarted()
{
    NotifyParent(s_nGameStarted);
}

void CMineFieldWindow::OnGameWon()
{
    NotifyParent(s_nGameWon);
}

void CMineFieldWindow::OnGameLost()
{
    NotifyParent(s_nGameLost);
}

void CMineFieldWindow::OnLButtonDown(UINT nFlags, CPoint point)
{
    SetFocus();

    std::int32_t nColumn, nLine;
    ConvertClientCoordToMinefieldCoord(point, nColumn, nLine);

    m_MineSweeper->GetMineFields()[0].OpenSquare(nColumn, nLine);
    InvalidateRect(nullptr);

    NotifyParent(s_nMineCountChanged);

    CWnd::OnLButtonDown(nFlags, point);
}

void CMineFieldWindow::OnRButtonDown(UINT nFlags, CPoint point)
{
    SetFocus();

    std::int32_t nColumn, nLine;
    ConvertClientCoordToMinefieldCoord(point, nColumn, nLine);

    if (nFlags & MK_LBUTTON)
        m_MineSweeper->GetMineFields()[0].AutoOpenAdjacentSquares(nColumn, nLine);
    else
    {
        if (m_MineSweeper->GetMineFields()[0].IsSquareUnmarked(nColumn, nLine))
            m_MineSweeper->GetMineFields()[0].MarkSquareAsMine(nColumn, nLine);
        else if (m_MineSweeper->GetMineFields()[0].IsSquareMarkedAsMine(nColumn, nLine))
            m_MineSweeper->GetMineFields()[0].MarkSquareAsUnknown(nColumn, nLine);
        else if (m_MineSweeper->GetMineFields()[0].IsSquareMarkedAsUnknown(nColumn, nLine))
            m_MineSweeper->GetMineFields()[0].UnmarkSquare(nColumn, nLine);
    }

    NotifyParent(s_nMineCountChanged);

    InvalidateRect(nullptr);

    CWnd::OnRButtonDown(nFlags, point);
}

void CMineFieldWindow::OnMButtonDown(UINT nFlags, CPoint point)
{
    SetFocus();

    std::int32_t nColumn, nLine;
    ConvertClientCoordToMinefieldCoord(point, nColumn, nLine);

    m_MineSweeper->GetMineFields()[0].AutoOpenAdjacentSquares(nColumn, nLine);

    InvalidateRect(nullptr);

    CWnd::OnRButtonDown(nFlags, point);
}

void CMineFieldWindow::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    CMineFieldWindow::OnLButtonDown(nFlags, point);

    CWnd::OnLButtonDblClk(nFlags, point);
}

void CMineFieldWindow::OnRButtonDblClk(UINT nFlags, CPoint point)
{
    CMineFieldWindow::OnRButtonDown(nFlags, point);

    CWnd::OnRButtonDblClk(nFlags, point);
}

void CMineFieldWindow::OnMButtonDblClk(UINT nFlags, CPoint point)
{
    CMineFieldWindow::OnMButtonDown(nFlags, point);

    CWnd::OnMButtonDblClk(nFlags, point);
}

