#if !defined(AFX_PLOTCTRL_H__8DA375E1_0320_4710_A985_B7C07C12A801__INCLUDED_)
#define AFX_PLOTCTRL_H__8DA375E1_0320_4710_A985_B7C07C12A801__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PlotCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPlotCtrl window

#include <vector>
using std::vector;

#define HCN_SAFE_DELETEA(pBase)		if (pBase) { delete [] pBase; pBase = NULL; }

// ��ƽ����
#define HCN_WTG_SQUARE(x)			((x) * (x))

// DOUBLE�����ֵ��ȡ�Կ��ļ�
#define DOUBLE_INFINITY_VALUE_HIGH  0x7ff00000

// �߽����������ߣ��ٷֱ�
#define HCN_MARGIN_X				3
#define HCN_MARGIN_Y				3

// ָ�������ڵ�ָ��
#define afxpMainWnd					((CWSNTopoGenDlg *)AfxGetMainWnd())

class CPlotCtrl : public CStatic
{
// Construction
public:
	CPlotCtrl();

// Attributes
public:

// Operations
public:
	// ��ʼ������
	void InitParams(
		DOUBLE dListenCost, 
		DOUBLE dAdjustTime, 
		DOUBLE dFlowVelocity, 
		DOUBLE dEnergyConsumModelCoef, 
		INT iNodeEnergy, 
		INT iDoTimes, 
		DOUBLE dRadius
		);
	// ��������
	void GenerateTopo(INT iNodeCnt);

protected:
	// ��������
	void _GenerateTopo();
	
	// ���������̺߳���
	static DWORD WINAPI _GenTopoThread(LPVOID lParam);

	// �õ���֮��ľ���
	DOUBLE GetDistance(DOUBLE dX, DOUBLE dY);

	// �ж�MST�����Ƿ����ͷ����һ�£�һ�·���TRUE
	BOOL CompareMst();

	// ��ʼ�����꣬��ͷ��ǣ�������Ϣ��mst��Ϣ
	void InitMst(INT *XCoors, INT *YCoors);
	// �����Ϣ
	void Reset();

	void HeaderBelong(INT *XCoors, INT *YCoors);
	void TransmitConsume(INT *XCoors, INT *YCoors);
	void ListenConsume();

	// �Ƿ�ȫ���ڵ��Ѹ��ǣ����ȫ�����ǣ�����TRUE
	BOOL CheckConnectAll();
	// �ҵ���ǿ׳�㣬�����±�
	INT FindMostRobust();
	// �ж��Ƿ��нڵ�����Ϊ�յ�ֵ������У�����FALSE
	BOOL CheckEmptyNode();
	
	// ��λʱ��Ĵ�ͷ�����ܺ�
	DOUBLE m_dListenCost;
	// �㷨��������
	DOUBLE m_dAdjustTime;
	// ��λ�����ٶ�
	DOUBLE m_dFlowVelocity;
	// �ܺ�ģ��ϵ��
	DOUBLE m_dEnergyConsumModelCoef;
	// ��ʼ����
	INT m_iNodeEnergy;
	// �㷨ִ�д���
	INT m_iDoTimes;
	
	// ��ͷ���,���Ǵ�ͷ���ò�ͬ�Ĳ�ɫ��ɫ��ʾ
	BOOL *m_arrbHeaderFlags;
	// ������ͷ���ú���ɫ�ĵ��ʾ
	INT *m_arriBelongIndx;
	// ���Ǳ��
	BOOL *m_arrbCoverFlags;
	
	// �Ƿ�����ڹǸ�����
	BOOL *m_arrbMST;
	// �Ǹ����и��ڵ�
	INT *m_arriParent;
	
	// ͨ�Ű뾶
	DOUBLE m_dRadius;
	// ʣ������
	DOUBLE *m_arrdRestEnergy;
	
	// ��¼�������
	INT *m_arriXCoors;
	INT *m_arriYCoors;

	// �ڵ����
	INT m_iNodeCnt;

	HANDLE m_hGenTopoThread;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlotCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPlotCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPlotCtrl)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLOTCTRL_H__8DA375E1_0320_4710_A985_B7C07C12A801__INCLUDED_)
