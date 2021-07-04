// NewGameDialog.cpp : implementation file
//

#include "stdafx.h"
#include "MineSweeperGui.h"
#include "NewGameDialog.h"
#include "afxdialogex.h"

#include "../MineSweeperLib/MineField.h"


// CNewGameDialog dialog

IMPLEMENT_DYNAMIC(CNewGameDialog, CDialogEx)

CNewGameDialog::CNewGameDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_NEWGAME, pParent)
    , m_SkillLevel(SkillLevel::Beginner)
{
    m_NewGameParameters.m_nColumnCount = 50;
    m_NewGameParameters.m_nLineCount = 30;
    m_NewGameParameters.m_nMineCount = 400;
}

CNewGameDialog::~CNewGameDialog()
{
}

void CNewGameDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);

    if (!pDX->m_bSaveAndValidate || (pDX->m_bSaveAndValidate && m_SkillLevel == SkillLevel::Custom))
    {
        DDX_Text(pDX, IDC_WIDTH, m_NewGameParameters.m_nColumnCount);
        DDV_MinMaxInt(pDX, m_NewGameParameters.m_nColumnCount,
            MineSweeper::MineField::s_nMinColumnCount, MineSweeper::MineField::s_nMaxColumnCount);

        DDX_Text(pDX, IDC_HEIGHT, m_NewGameParameters.m_nLineCount);
        DDV_MinMaxInt(pDX, m_NewGameParameters.m_nLineCount,
            MineSweeper::MineField::s_nMinLineCount, MineSweeper::MineField::s_nMaxLineCount);

        std::int32_t nMinMineCount = MineSweeper::MineField::GetMinMineCountForSize(m_NewGameParameters.m_nColumnCount, m_NewGameParameters.m_nLineCount);
        std::int32_t nMaxMineCount = MineSweeper::MineField::GetMaxMineCountForSize(m_NewGameParameters.m_nColumnCount, m_NewGameParameters.m_nLineCount);

        DDX_Text(pDX, IDC_MINECOUNT, m_NewGameParameters.m_nMineCount);
        DDV_MinMaxInt(pDX, m_NewGameParameters.m_nMineCount, nMinMineCount, nMaxMineCount);
    }
}

void CNewGameDialog::EnableDisable()
{
    BOOL bEnable = (m_SkillLevel == SkillLevel::Custom);

    GetDlgItem(IDC_WIDTH_LABEL)->EnableWindow(bEnable);
    GetDlgItem(IDC_HEIGHT_LABEL)->EnableWindow(bEnable);
    GetDlgItem(IDC_COUNT_LABEL)->EnableWindow(bEnable);

    GetDlgItem(IDC_WIDTH)->EnableWindow(bEnable);
    GetDlgItem(IDC_HEIGHT)->EnableWindow(bEnable);
    GetDlgItem(IDC_MINECOUNT)->EnableWindow(bEnable);
}

BOOL CNewGameDialog::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    CheckDlgButton(IDC_BEGINNER, BST_CHECKED);
    OnBnClickedBeginner();

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}


BEGIN_MESSAGE_MAP(CNewGameDialog, CDialogEx)
    ON_BN_CLICKED(IDC_BEGINNER, &CNewGameDialog::OnBnClickedBeginner)
    ON_BN_CLICKED(IDC_INTERMEDIATE, &CNewGameDialog::OnBnClickedIntermediate)
    ON_BN_CLICKED(IDC_EXPERT, &CNewGameDialog::OnBnClickedExpert)
    ON_BN_CLICKED(IDC_CUSTOM, &CNewGameDialog::OnBnClickedCustom)
END_MESSAGE_MAP()


// CNewGameDialog message handlers

void CNewGameDialog::OnBnClickedBeginner()
{
    m_SkillLevel = SkillLevel::Beginner;
    EnableDisable();
}

void CNewGameDialog::OnBnClickedIntermediate()
{
    m_SkillLevel = SkillLevel::Intermediate;
    EnableDisable();
}

void CNewGameDialog::OnBnClickedExpert()
{
    m_SkillLevel = SkillLevel::Expert;
    EnableDisable();
}

void CNewGameDialog::OnBnClickedCustom()
{
    m_SkillLevel = SkillLevel::Custom;
    EnableDisable();
}
