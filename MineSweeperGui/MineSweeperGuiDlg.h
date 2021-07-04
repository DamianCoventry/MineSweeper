
// MineSweeperGuiDlg.h : header file
//

#pragma once

#include "NewGameParameters.h"
#include "MineFieldWindow.h"

#include "../MineSweeperLib/MineSweeper.h"

#include <memory>
#include "afxwin.h"

// CMineSweeperGuiDlg dialog
class CMineSweeperGuiDlg : public CDialogEx
{
// Construction
public:
	CMineSweeperGuiDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MINESWEEPERGUI };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
    virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

protected:
    DECLARE_MESSAGE_MAP()
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnGameNew();
    afx_msg void OnGameRestart();
    afx_msg void OnGameExit();
    afx_msg void OnBnClickedRestartgame();
    afx_msg void OnTimer(UINT_PTR nIDEvent);

private:
    void StartNewGame(const NewGameParameters& newGameParameters);
    void ResizeWindowAndControls(const NewGameParameters& gameParameters);
    void UpdateRemainingMineCount();

private:
    static const NewGameParameters s_BeginnerGameParameters;
    static const NewGameParameters s_IntermediateGameParameters;
    static const NewGameParameters s_ExpertGameParameters;
    static const int s_nPadding = 12;
    static const int s_nMineFieldWindowId = 1000;

private:
    CStatic m_MinesRemainingLabel;
    CStatic m_ElapsedTimeLabel;
    CStatic m_GameStatusLabel;
    CButton m_RestartGameButton;
    CFont m_LargerFont;

private:
    std::unique_ptr<MineSweeper::MineSweeper> m_MineSweeper;
    std::unique_ptr<CMineFieldWindow> m_MineFieldWindow;
    NewGameParameters m_CurrentParameters;
};
