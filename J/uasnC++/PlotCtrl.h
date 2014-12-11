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

// 求平方宏
#define HCN_WTG_SQUARE(x)			((x) * (x))

// DOUBLE的最大值，取自库文件
#define DOUBLE_INFINITY_VALUE_HIGH  0x7ff00000

// 边界留白区域宽高，百分比
#define HCN_MARGIN_X				3
#define HCN_MARGIN_Y				3

// 指向主窗口的指针
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
	// 初始化参数
	void InitParams(
		DOUBLE dListenCost, 
		DOUBLE dAdjustTime, 
		DOUBLE dFlowVelocity, 
		DOUBLE dEnergyConsumModelCoef, 
		INT iNodeEnergy, 
		INT iDoTimes, 
		DOUBLE dRadius
		);
	// 生成拓扑
	void GenerateTopo(INT iNodeCnt);

protected:
	// 生成拓扑
	void _GenerateTopo();
	
	// 生成拓扑线程函数
	static DWORD WINAPI _GenTopoThread(LPVOID lParam);

	// 得到点之间的距离
	DOUBLE GetDistance(DOUBLE dX, DOUBLE dY);

	// 判断MST集合是否与簇头集合一致，一致返回TRUE
	BOOL CompareMst();

	// 初始化坐标，簇头标记，所属信息，mst信息
	void InitMst(INT *XCoors, INT *YCoors);
	// 清空信息
	void Reset();

	void HeaderBelong(INT *XCoors, INT *YCoors);
	void TransmitConsume(INT *XCoors, INT *YCoors);
	void ListenConsume();

	// 是否全部节点已覆盖，如果全部覆盖，返回TRUE
	BOOL CheckConnectAll();
	// 找到最强壮点，返回下标
	INT FindMostRobust();
	// 判断是否有节点能量为空的值，如果有，返回FALSE
	BOOL CheckEmptyNode();
	
	// 单位时间的簇头侦听能耗
	DOUBLE m_dListenCost;
	// 算法调整周期
	DOUBLE m_dAdjustTime;
	// 单位流量速度
	DOUBLE m_dFlowVelocity;
	// 能耗模型系数
	DOUBLE m_dEnergyConsumModelCoef;
	// 初始能量
	INT m_iNodeEnergy;
	// 算法执行次数
	INT m_iDoTimes;
	
	// 簇头标记,凡是簇头，用不同的彩色颜色表示
	BOOL *m_arrbHeaderFlags;
	// 所属簇头，用黑颜色的点表示
	INT *m_arriBelongIndx;
	// 覆盖标记
	BOOL *m_arrbCoverFlags;
	
	// 是否存在于骨干网络
	BOOL *m_arrbMST;
	// 骨干网中父节点
	INT *m_arriParent;
	
	// 通信半径
	DOUBLE m_dRadius;
	// 剩余能量
	DOUBLE *m_arrdRestEnergy;
	
	// 记录结果坐标
	INT *m_arriXCoors;
	INT *m_arriYCoors;

	// 节点个数
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
