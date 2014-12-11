// WSNTopoGen.h : main header file for the WSNTOPOGEN application
//

#if !defined(AFX_WSNTOPOGEN_H__98604C45_C728_4FA9_A4CF_B71038FDF971__INCLUDED_)
#define AFX_WSNTOPOGEN_H__98604C45_C728_4FA9_A4CF_B71038FDF971__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CWSNTopoGenApp:
// See WSNTopoGen.cpp for the implementation of this class
//

class CWSNTopoGenApp : public CWinApp
{
public:
	CWSNTopoGenApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWSNTopoGenApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CWSNTopoGenApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WSNTOPOGEN_H__98604C45_C728_4FA9_A4CF_B71038FDF971__INCLUDED_)
