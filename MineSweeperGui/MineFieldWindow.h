#pragma once

#include "../MineSweeperLib/MineSweeper.h"

// CMineFieldWindow

class CMineFieldWindow : public CWnd
{
	DECLARE_DYNAMIC(CMineFieldWindow)

public:
    static const LONG s_nSquareSize = 25;
    static const LONG s_nMineCountChanged = WM_USER + 1;
    static const LONG s_nGameStarted = WM_USER + 2;
    static const LONG s_nGameWon = WM_USER + 3;
    static const LONG s_nGameLost = WM_USER + 4;

public:
	CMineFieldWindow();
	virtual ~CMineFieldWindow();

public:
#if defined _M_IX86
    BOOL Create(int x, int y, int nWidth, int nHeight, HWND hWndParent, LONG nID);
#elif defined _M_X64
    BOOL Create(int x, int y, int nWidth, int nHeight, HWND hWndParent, LONGLONG nID);
#endif

public:
    void StartNewGame(MineSweeper::MineSweeper* mineSweeper);

private:
	DECLARE_MESSAGE_MAP()
    afx_msg void OnPaint();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnMButtonDblClk(UINT nFlags, CPoint point);

private:
    void DrawGameBoard(CPaintDC& dc);
    void DrawGameSquare(CPaintDC& dc, RECT* rect, MineSweeper::MineField& mindField, std::int32_t nColumn, std::int32_t nLine);
    void DrawAdjacentMineCountSquare(CPaintDC& dc, RECT* rect, int nCount);
    void DrawQuestionMarkSquare(CPaintDC& dc, RECT* rect);
    void DrawTextString(CPaintDC& dc, RECT* rect, const _TCHAR* textString, int nNumChars, COLORREF colour);
    void DrawImage(CPaintDC& dc, RECT* rect, CBitmap* bitmap, const BITMAP& bitmapInfo);

private:
    void ConvertClientCoordToMinefieldCoord(const CPoint& clientCoord, std::int32_t& nColumn, std::int32_t& nLine);
    void NotifyParent(int nNotifyCode);

private:
    void OnGameStarted();
    void OnGameWon();
    void OnGameLost();

private:
    static const _TCHAR* s_sClassName;
    static const COLORREF s_ColourLookup[9];

private:
    MineSweeper::MineSweeper* m_MineSweeper; // CMineSweeperGuiDlg owns the instance

private:
    CBrush m_NoGameSetBrush;
    CBrush m_OpenedSquareBrush;
    CBrush m_UnOpenedSquareBrush;
    CFont m_AdjacentSquareCountFont;
    CDC m_MemoryDC;

private:
    enum Bitmaps { LandMine = 0, Flag, EmptySquare, UnopenedSquare, LandMineExploded, LandMineIncorrect, BitmapCount };
    CBitmap m_Bitmaps[BitmapCount];
    BITMAP m_BitmapInfos[BitmapCount];
    static int m_BitmapResIds[BitmapCount];
};
