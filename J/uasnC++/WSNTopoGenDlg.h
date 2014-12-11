// WSNTopoGenDlg.h : header file
//

#if !defined(AFX_WSNTOPOGENDLG_H__27D41E75_DF4A_43CF_9488_BCCC68F5FDB7__INCLUDED_)
#define AFX_WSNTOPOGENDLG_H__27D41E75_DF4A_43CF_9488_BCCC68F5FDB7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//#include "PlotCtrl.h"
#include "uasn_simula.h"

/////////////////////////////////////////////////////////////////////////////
// CWSNTopoGenDlg dialog

class CWSNTopoGenDlg : public CDialog
{
// Construction
public:
	CWSNTopoGenDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CWSNTopoGenDlg)
	enum { IDD = IDD_WSNTOPOGEN_DIALOG };
	//CPlotCtrl	m_ctrlPlotCtrl;
	CUasnSimula m_ctrlUasnSimula;
	INT		m_iNodeCnt;
	DOUBLE	m_dListenCost;
	DOUBLE	m_dAdjustTime;
	DOUBLE	m_dFlowVelocity;
	DOUBLE	m_dEnergyConsumModelCoef;
	DOUBLE	m_iNodeEnergy;
	INT		m_iDoTimes;
	DOUBLE	m_dRadius;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWSNTopoGenDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CWSNTopoGenDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnGenerate();
	afx_msg void OnBtnLmst();
	afx_msg void OnBtnUasn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WSNTOPOGENDLG_H__27D41E75_DF4A_43CF_9488_BCCC68F5FDB7__INCLUDED_)
