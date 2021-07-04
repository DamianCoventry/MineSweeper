#pragma once
#include "afxwin.h"
#include "NewGameParameters.h"

// CNewGameDialog dialog

class CNewGameDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CNewGameDialog)

public:
	CNewGameDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNewGameDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NEWGAME };
#endif

public:
    enum class SkillLevel { Beginner, Intermediate, Expert, Custom };
    SkillLevel GetSkillLevel() const { return m_SkillLevel; }
    const NewGameParameters GetNewGameParameters() const { return m_NewGameParameters; }

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();

private:
    void EnableDisable();

private:
    SkillLevel m_SkillLevel;
    NewGameParameters m_NewGameParameters;

private:
    DECLARE_MESSAGE_MAP()
    afx_msg void OnBnClickedBeginner();
    afx_msg void OnBnClickedIntermediate();
    afx_msg void OnBnClickedExpert();
    afx_msg void OnBnClickedCustom();
};
