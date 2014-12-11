// WSNTopoGenDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WSNTopoGen.h"
#include "WSNTopoGenDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWSNTopoGenDlg dialog

CWSNTopoGenDlg::CWSNTopoGenDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWSNTopoGenDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWSNTopoGenDlg)
	m_iNodeCnt = 100;
	m_dListenCost = 0.14;
	m_dAdjustTime = 2.0;
	m_dFlowVelocity = 0.2;
	m_dEnergyConsumModelCoef = 0.3;
	m_iNodeEnergy = 0.5;
	m_iDoTimes = 1000;
	m_dRadius = 70.0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWSNTopoGenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWSNTopoGenDlg)
	//DDX_Control(pDX, IDC_STATIC_PLOT, m_ctrlPlotCtrl);
	DDX_Control(pDX, IDC_STATIC_PLOT, m_ctrlUasnSimula);
	DDX_Text(pDX, IDC_EDIT_NODE_CNT, m_iNodeCnt);
	DDV_MinMaxInt(pDX, m_iNodeCnt, 80, 130);
	DDX_Text(pDX, IDC_EDIT_LISTEN_COST, m_dListenCost);
	DDV_MinMaxDouble(pDX, m_dListenCost, 1.e-002, 100.);
	DDX_Text(pDX, IDC_EDIT_ADJUST_TIME, m_dAdjustTime);
	DDV_MinMaxDouble(pDX, m_dAdjustTime, 2., 100.);
	DDX_Text(pDX, IDC_EDIT_FLOW_VELOCITY, m_dFlowVelocity);
	DDV_MinMaxDouble(pDX, m_dFlowVelocity, 0., 1.);
	DDX_Text(pDX, IDC_EDIT_ENERGY_CONSUM_MODEL_COEF, m_dEnergyConsumModelCoef);
	DDV_MinMaxDouble(pDX, m_dEnergyConsumModelCoef, 0., 1.);
	DDX_Text(pDX, IDC_EDIT_NODE_ENERGY, m_iNodeEnergy);
	DDV_MinMaxDouble(pDX, m_iNodeEnergy, 0., 1.);
	DDX_Text(pDX, IDC_EDIT_DO_TIME, m_iDoTimes);
	DDV_MinMaxInt(pDX, m_iDoTimes, 1, 60000);
	DDX_Text(pDX, IDC_EDIT_RADIUS, m_dRadius);
	DDV_MinMaxDouble(pDX, m_dRadius, 20., 1000.);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CWSNTopoGenDlg, CDialog)
	//{{AFX_MSG_MAP(CWSNTopoGenDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_GENERATE, OnBtnGenerate)
	ON_BN_CLICKED(IDC_BTN_LMST, OnBtnLmst)
	ON_BN_CLICKED(IDC_BTN_UASN, OnBtnUasn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWSNTopoGenDlg message handlers

BOOL CWSNTopoGenDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CWSNTopoGenDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CWSNTopoGenDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CWSNTopoGenDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CWSNTopoGenDlg::OnBtnGenerate() 
{
	// TODO: Add your control notification handler code here
	if (!UpdateData(TRUE))
	{
		return;
	}
	/**
	m_ctrlPlotCtrl.InitParams(m_dListenCost, 
		m_dAdjustTime, 
		m_dFlowVelocity, 
		m_dEnergyConsumModelCoef, 
		m_iNodeEnergy, 
		m_iDoTimes, 
		m_dRadius);
	m_ctrlPlotCtrl.GenerateTopo(m_iNodeCnt);
	**/
	if (m_dFlowVelocity + m_dEnergyConsumModelCoef + 
		m_iNodeEnergy != 1)
	{
		MessageBox("【 a + b + c != 1 】");
		return;
	}

	m_ctrlUasnSimula.InitParams(m_iNodeCnt, m_dFlowVelocity, 
		m_dEnergyConsumModelCoef, m_iNodeEnergy);
	m_ctrlUasnSimula.UasnSimula(m_iNodeCnt);
	m_ctrlUasnSimula.UasnSetStep(1);
}

//初始拓扑生成
void CWSNTopoGenDlg::OnBtnLmst() 
{
	// TODO: Add your control notification handler code here

	if (m_ctrlUasnSimula.UasnGetStep() != 1) 
	{
		MessageBox("先执行①");
		return;
	}

	m_ctrlUasnSimula.UasnMstTop(m_iNodeCnt);
	m_ctrlUasnSimula.UasnSetStep(2);
}

//合作进化拓扑控制
void CWSNTopoGenDlg::OnBtnUasn() 
{
	// TODO: Add your control notification handler code here

	if (m_ctrlUasnSimula.UasnGetStep() != 2)
	{
		MessageBox("先执行①和②");
		return;
	}

	m_ctrlUasnSimula.UasnEvolutionTop(m_iNodeCnt);
}
