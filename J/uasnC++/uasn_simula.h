#ifndef _UASN_SIMULA_H
#define _UASN_SIMULA_H


#include <vector>

using std::vector;

// 求平方宏
#define HCN_WTG_SQUARE(x)			((x) * (x))
#define HCN_SAFE_DELETEA(pBase)		if (pBase) { delete [] pBase; pBase = NULL; }
// DOUBLE的最大值，取自库文件
#define DOUBLE_INFINITY_VALUE_HIGH  0x7ff00000

// 边界留白区域宽高，百分比
#define HCN_MARGIN_X				3
#define HCN_MARGIN_Y				3

// 指向主窗口的指针
#define afxpMainWnd					((CWSNTopoGenDlg *)AfxGetMainWnd())

class CUasnSimula : public CStatic
{
public:
	CUasnSimula();

	void InitParams(INT mcnt, DOUBLE a, DOUBLE b, DOUBLE c);
	
	//初始节点
	void UasnSimula(INT CNT);

	//拓扑生成
	void UasnMstTop(INT cnt);

	//拓扑进化
	void UasnEvolutionTop(INT cnt);

	void UasnSetStep(INT i)
	{
		m_iStep = i;
	}

	INT UasnGetStep()
	{
		return m_iStep;
	}

	virtual ~CUasnSimula();

protected:
	//初始节点
	void _UasnSimula();
	void _UasnMstTop();
	void _UasnEvolutionTop();

	//线程函数
	static DWORD WINAPI _UasnSimulaThread(LPVOID lParam);
	static DWORD WINAPI _UasnMstThread(LPVOID lParam);
	static DWORD WINAPI _UasnEvolutionThread(LPVOID lParam);

	//得到点之间的距离
	DOUBLE GetDistance(DOUBLE dX, DOUBLE dY);

	DOUBLE GetMin(DOUBLE r1, DOUBLE r2)
	{
		if (r1 < r2) return r1;

		return r2;
	}

	void Clear();

	INT m_iStep;

	//结果坐标
	INT *m_arriXCoors;
	INT *m_arriYCoors;

	//发送,接收半径
	DOUBLE *m_arrdRSend;
	DOUBLE *m_arrdRRecv;

	//节点发送功率
	DOUBLE *m_arrdPower;

	//发送概率
	DOUBLE **m_arrdPn;
	
	//节点个数
	INT m_iNodeCnt;

	//拓扑连接图矩阵
	INT **m_arriTopMatrix;
	INT **m_arriMstTop;

	//距离权值
	DOUBLE **m_arrdDist;

	DOUBLE m_dPa, m_dPb, m_dPc;


	HANDLE m_hUasnSimulaThread;
	HANDLE m_hUasnMstThread;
	HANDLE m_hUasnEvolutionThread;

protected:
	//{{AFX_MSG(CUasnSimula)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP();
};
#endif