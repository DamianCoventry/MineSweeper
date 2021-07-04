
// MineSweeperGuiDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MineSweeperGui.h"
#include "MineSweeperGuiDlg.h"
#include "NewGameDialog.h"
#include "afxdialogex.h"

#include "../MineSweeperLib/MineSweeperFactory.h"

#include <stdexcept>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMineSweeperGuiDlg dialog

const NewGameParameters CMineSweeperGuiDlg::s_BeginnerGameParameters = { 15, 10, 22 };          // Each of these are approx ¼ full of mines
const NewGameParameters CMineSweeperGuiDlg::s_IntermediateGameParameters = { 25, 18, 65 };
const NewGameParameters CMineSweeperGuiDlg::s_ExpertGameParameters = { 65, 35, 560 };

CMineSweeperGuiDlg::CMineSweeperGuiDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MINESWEEPERGUI, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMineSweeperGuiDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_MINESREMAINING, m_MinesRemainingLabel);
    DDX_Control(pDX, IDC_ELAPSEDTIME, m_ElapsedTimeLabel);
    DDX_Control(pDX, IDC_RESTARTGAME, m_RestartGameButton);
    DDX_Control(pDX, IDC_GAMESTATE, m_GameStatusLabel);
}

BEGIN_MESSAGE_MAP(CMineSweeperGuiDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_COMMAND(ID_GAME_NEW, &CMineSweeperGuiDlg::OnGameNew)
    ON_COMMAND(ID_GAME_RESTART, &CMineSweeperGuiDlg::OnGameRestart)
    ON_COMMAND(ID_GAME_EXIT, &CMineSweeperGuiDlg::OnGameExit)
    ON_BN_CLICKED(IDC_RESTARTGAME, &CMineSweeperGuiDlg::OnBnClickedRestartgame)
    ON_WM_TIMER()
END_MESSAGE_MAP()


// CMineSweeperGuiDlg message handlers

BOOL CMineSweeperGuiDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

    CDC* dc = GetDC();
    HFONT hFont = (HFONT)::GetStockObject(DEFAULT_GUI_FONT);
    if (hFont == NULL)
        hFont = (HFONT)::GetStockObject(SYSTEM_FONT);

    LOGFONT logicalFont;
    GetObject(hFont, sizeof(LOGFONT), &logicalFont);
    _tcsncpy_s(logicalFont.lfFaceName, LF_FACESIZE, _T("Tahoma"), LF_FACESIZE);
    logicalFont.lfQuality = CLEARTYPE_NATURAL_QUALITY;
    logicalFont.lfWeight = FW_BOLD;
    logicalFont.lfHeight = -MulDiv(12, GetDeviceCaps(dc->GetSafeHdc(), LOGPIXELSY), 72);

    m_LargerFont.CreateFontIndirect(&logicalFont);
    ReleaseDC(dc);

    m_MinesRemainingLabel.SetFont(&m_LargerFont);
    m_ElapsedTimeLabel.SetFont(&m_LargerFont);
    m_GameStatusLabel.SetFont(&m_LargerFont);

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

    try
    {
        m_MineFieldWindow.reset(new CMineFieldWindow);
        m_MineFieldWindow->Create(0, 0, 10, 10, GetSafeHwnd(), s_nMineFieldWindowId);
    }
    catch (const std::runtime_error& e)
    {
        AfxMessageBox(CString(e.what()));
        return FALSE;
    }

    StartNewGame(s_BeginnerGameParameters);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMineSweeperGuiDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMineSweeperGuiDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMineSweeperGuiDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMineSweeperGuiDlg::OnGameNew()
{
    CNewGameDialog dialog(this);
    if (dialog.DoModal() == IDOK)
    {
        switch (dialog.GetSkillLevel())
        {
        case CNewGameDialog::SkillLevel::Beginner:
            StartNewGame(s_BeginnerGameParameters);
            break;
        case CNewGameDialog::SkillLevel::Intermediate:
            StartNewGame(s_IntermediateGameParameters);
            break;
        case CNewGameDialog::SkillLevel::Expert:
            StartNewGame(s_ExpertGameParameters);
            break;
        case CNewGameDialog::SkillLevel::Custom:
            StartNewGame(dialog.GetNewGameParameters());
            break;
        }
    }
}

void CMineSweeperGuiDlg::OnGameRestart()
{
    StartNewGame(m_CurrentParameters);
}

void CMineSweeperGuiDlg::OnGameExit()
{
    PostQuitMessage(0);
}

void CMineSweeperGuiDlg::OnBnClickedRestartgame()
{
    OnGameRestart();
}

void CMineSweeperGuiDlg::OnTimer(UINT_PTR nIDEvent)
{
    if (m_MineSweeper)
    {
        if (m_MineSweeper->GetMineFields()[0].IsGameFirstMove())
            m_ElapsedTimeLabel.SetWindowText(_T("00:00"));
        else if(m_MineSweeper->GetMineFields()[0].IsGamePlaying())
        {
            std::chrono::system_clock::duration elapsedTime = m_MineSweeper->GetMineFields()[0].GetElapsedTime();
            auto minutes = std::chrono::duration_cast<std::chrono::minutes>(elapsedTime);
            auto seconds = std::chrono::duration_cast<std::chrono::seconds>(elapsedTime);
            if (minutes.count() >= 60)
                m_ElapsedTimeLabel.SetWindowText(_T("60:00"));
            else
            {
                CString sText;
                sText.Format(_T("%02u:%02u"), minutes.count(), seconds.count() % 60);
                m_ElapsedTimeLabel.SetWindowText(sText);
            }
        }
    }

    CDialogEx::OnTimer(nIDEvent);
}

BOOL CMineSweeperGuiDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
    NMHDR* notify = reinterpret_cast<NMHDR*>(lParam);

    if (notify->idFrom == s_nMineFieldWindowId)
    {
        switch (notify->code)
        {
        case CMineFieldWindow::s_nMineCountChanged:
            UpdateRemainingMineCount();
            break;
        case CMineFieldWindow::s_nGameStarted:
            m_GameStatusLabel.SetWindowText(L"Game Started!");
            break;
        case CMineFieldWindow::s_nGameWon:
            m_GameStatusLabel.SetWindowText(L"Game Won!");
            break;
        case CMineFieldWindow::s_nGameLost:
            m_GameStatusLabel.SetWindowText(L"Game Lost :-(");
            break;
        }
    }

    return CDialogEx::OnNotify(wParam, lParam, pResult);
}

void CMineSweeperGuiDlg::StartNewGame(const NewGameParameters& newGameParameters)
{
    KillTimer(0);
    SetTimer(0, 250, nullptr);

    ResizeWindowAndControls(newGameParameters);

    m_CurrentParameters = newGameParameters;

    m_MineSweeper.reset(new MineSweeper::MineSweeper(
        MineSweeper::MineSweeperFactory::CreateFromMetrics(
            newGameParameters.m_nColumnCount, newGameParameters.m_nLineCount, newGameParameters.m_nMineCount)));

    m_MineFieldWindow->StartNewGame(m_MineSweeper.get());

    UpdateRemainingMineCount();
    m_GameStatusLabel.SetWindowText(L"Click the board to start.");
}

void CMineSweeperGuiDlg::UpdateRemainingMineCount()
{
    if (m_MineSweeper && m_MineSweeper->GetMineFields().size() == 1)
    {
        CString sText;
        sText.Format(_T("%d"), m_MineSweeper->GetMineFields()[0].GetMinesRemainingCount());
        m_MinesRemainingLabel.SetWindowText(sText);
    }
}

void CMineSweeperGuiDlg::ResizeWindowAndControls(const NewGameParameters & gameParameters)
{
    if (m_CurrentParameters == gameParameters) // Don't bother if nothing has changed.
        return;

    // We need these rectangles to work out where to put them
    RECT minesRemainingLabelRect;
    m_MinesRemainingLabel.GetClientRect(&minesRemainingLabelRect);
    RECT elapsedTimeLabelRect;
    m_ElapsedTimeLabel.GetClientRect(&elapsedTimeLabelRect);
    RECT gameStatusRect;
    m_GameStatusLabel.GetClientRect(&gameStatusRect);
    RECT restartGameRect;
    m_RestartGameButton.GetClientRect(&restartGameRect);

    // How big is the game board?
    int nMineFieldWidth = gameParameters.m_nColumnCount * CMineFieldWindow::s_nSquareSize + 4; // 4 extra pixels for the border
    int nMineFieldHeight = gameParameters.m_nLineCount * CMineFieldWindow::s_nSquareSize + 4; // 4 extra pixels for the border

    // Move and size the main window
    RECT windowRect;
    windowRect.left = windowRect.top = 0;
    windowRect.right = s_nPadding + nMineFieldWidth + s_nPadding;
    windowRect.bottom = s_nPadding + minesRemainingLabelRect.bottom + s_nPadding +
        nMineFieldHeight + s_nPadding + minesRemainingLabelRect.bottom + s_nPadding;

    LONG_PTR nStyle = GetWindowLongPtr(GetSafeHwnd(), GWL_STYLE);
    LONG_PTR nExStyle = GetWindowLongPtr(GetSafeHwnd(), GWL_EXSTYLE);
    AdjustWindowRectEx(&windowRect, static_cast<DWORD>(nStyle), TRUE, static_cast<DWORD>(nExStyle));

    windowRect.right -= windowRect.left;
    windowRect.bottom -= windowRect.top;

    int x = (GetSystemMetrics(SM_CXSCREEN) / 2) - (windowRect.right / 2);
    int y = (GetSystemMetrics(SM_CYSCREEN) / 2) - (windowRect.bottom / 2);

    SetWindowPos(nullptr, x, y, windowRect.right, windowRect.bottom, SWP_NOZORDER);

    RECT clientRect;
    GetClientRect(&clientRect);

    y = s_nPadding;

    // Place the mines remaining label
    m_MinesRemainingLabel.SetWindowPos(nullptr, s_nPadding, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);

    // Place the restart button
    m_RestartGameButton.SetWindowPos(nullptr, (clientRect.right / 2) - (restartGameRect.right / 2), y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);

    // Place the elapsed time label
    LONG rightHandEdgeOfGameBoard = (s_nPadding + nMineFieldWidth) - 4;
    m_ElapsedTimeLabel.SetWindowPos(nullptr, rightHandEdgeOfGameBoard - elapsedTimeLabelRect.right, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);

    y += minesRemainingLabelRect.bottom + s_nPadding;

    // Place the game board
    m_MineFieldWindow->SetWindowPos(
        nullptr, s_nPadding, y,
        nMineFieldWidth, nMineFieldHeight, SWP_NOZORDER);

    y += nMineFieldHeight + s_nPadding;

    // Place the game status label
    m_GameStatusLabel.SetWindowPos(
        nullptr, s_nPadding, y,
        clientRect.right - (2 * s_nPadding), minesRemainingLabelRect.bottom, SWP_NOZORDER);
}
