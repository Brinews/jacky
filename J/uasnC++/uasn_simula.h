#ifndef _UASN_SIMULA_H
#define _UASN_SIMULA_H


#include <vector>

using std::vector;

// ��ƽ����
#define HCN_WTG_SQUARE(x)			((x) * (x))
#define HCN_SAFE_DELETEA(pBase)		if (pBase) { delete [] pBase; pBase = NULL; }
// DOUBLE�����ֵ��ȡ�Կ��ļ�
#define DOUBLE_INFINITY_VALUE_HIGH  0x7ff00000

// �߽����������ߣ��ٷֱ�
#define HCN_MARGIN_X				3
#define HCN_MARGIN_Y				3

// ָ�������ڵ�ָ��
#define afxpMainWnd					((CWSNTopoGenDlg *)AfxGetMainWnd())

class CUasnSimula : public CStatic
{
public:
	CUasnSimula();

	void InitParams(INT mcnt, DOUBLE a, DOUBLE b, DOUBLE c);
	
	//��ʼ�ڵ�
	void UasnSimula(INT CNT);

	//��������
	void UasnMstTop(INT cnt);

	//���˽���
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
	//��ʼ�ڵ�
	void _UasnSimula();
	void _UasnMstTop();
	void _UasnEvolutionTop();

	//�̺߳���
	static DWORD WINAPI _UasnSimulaThread(LPVOID lParam);
	static DWORD WINAPI _UasnMstThread(LPVOID lParam);
	static DWORD WINAPI _UasnEvolutionThread(LPVOID lParam);

	//�õ���֮��ľ���
	DOUBLE GetDistance(DOUBLE dX, DOUBLE dY);

	DOUBLE GetMin(DOUBLE r1, DOUBLE r2)
	{
		if (r1 < r2) return r1;

		return r2;
	}

	void Clear();

	INT m_iStep;

	//�������
	INT *m_arriXCoors;
	INT *m_arriYCoors;

	//����,���հ뾶
	DOUBLE *m_arrdRSend;
	DOUBLE *m_arrdRRecv;

	//�ڵ㷢�͹���
	DOUBLE *m_arrdPower;

	//���͸���
	DOUBLE **m_arrdPn;
	
	//�ڵ����
	INT m_iNodeCnt;

	//��������ͼ����
	INT **m_arriTopMatrix;
	INT **m_arriMstTop;

	//����Ȩֵ
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