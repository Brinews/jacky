#include "stdafx.h"
#include "WSNTopoGen.h"
#include "uasn_simula.h"
#include "MemDC.h"
#include <math.h>
#include <wtypes.h>

#include "WSNTopoGenDlg.h"

CUasnSimula::CUasnSimula()
{
	m_iNodeCnt = 0;

	m_arriXCoors = NULL;
	m_arriYCoors = NULL;
	m_arrdPower = NULL;
	m_arrdRSend = NULL;
	m_arrdRRecv = NULL;
	m_arrdPn = NULL;
	m_arriTopMatrix = NULL;
	m_arriMstTop = NULL;
	m_arrdDist = NULL;

	m_hUasnSimulaThread = INVALID_HANDLE_VALUE;

}

/***
 * �ڴ�����
 */
void CUasnSimula::Clear()
{
	INT i;

	HCN_SAFE_DELETEA(m_arriXCoors);
	HCN_SAFE_DELETEA(m_arriYCoors);
	HCN_SAFE_DELETEA(m_arrdPower);

	HCN_SAFE_DELETEA(m_arrdRSend);
	HCN_SAFE_DELETEA(m_arrdRRecv);

	if (NULL != m_arriTopMatrix)
	{
		for (i = 0; i < m_iNodeCnt; i++)
			if (NULL != m_arriTopMatrix[i])
			{
				delete []m_arriTopMatrix[i];
				m_arriTopMatrix[i] = NULL;
			}

		delete []m_arriTopMatrix;

		m_arriTopMatrix = NULL;
	}

	if (NULL != m_arriMstTop)
	{
		for (i = 0; i < m_iNodeCnt; i++)
			if (NULL != m_arriMstTop[i])
			{
				delete []m_arriMstTop[i];
				m_arriMstTop[i] = NULL;
			}

		delete []m_arriMstTop;

		m_arriMstTop = NULL;
	}

	if (NULL != m_arrdDist)
	{
		for (i = 0; i < m_iNodeCnt; i++)
			if (NULL != m_arrdDist[i])
			{
				delete []m_arrdDist[i];
				m_arrdDist[i] = NULL;
			}
		delete []m_arrdDist;

		m_arrdDist = NULL;
	}

	if (NULL != m_arrdPn)
	{
		for (i = 0; i < m_iNodeCnt; i++)
			if (NULL != m_arrdPn[i])
			{
				delete []m_arrdPn[i];
				m_arrdPn[i] = NULL;
			}
		delete []m_arrdPn;

		m_arrdPn = NULL;
	}

}

CUasnSimula::~CUasnSimula()
{
	Clear();
	m_hUasnSimulaThread = INVALID_HANDLE_VALUE;
}

BEGIN_MESSAGE_MAP(CUasnSimula, CStatic)
//{{AFX_MSG_MAP(CUasnSimula)
ON_WM_PAINT()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/***
 * �������ͼ����
 */
void CUasnSimula::OnPaint()
{
	INT i, j;

	CPaintDC dc(this);
	CRect rect;
	GetClientRect(&rect);
	CMemDC memDC(&dc, &rect);

	// �������
	DOUBLE dXRatioBase = rect.Width() / 100.;
	DOUBLE dYRatioBase = rect.Height() / 100.;

	CString strDistance;
	CString strHeaderIndx;

	COLORREF crTextColor = ::GetTextColor(memDC.m_hDC);

	//���ƽڵ�
	for (INT iNodeIndx = 0; iNodeIndx < m_iNodeCnt; iNodeIndx++)
	{
		CBrush objBrush(RGB(0, 0, 0));
		CBrush *oldObjBrush = memDC.SelectObject(&objBrush);
		// ���Ƶ�
		memDC.Ellipse((INT)(m_arriXCoors[iNodeIndx] * dXRatioBase) - 3, 
			(INT)(m_arriYCoors[iNodeIndx] * dYRatioBase) - 3, 
			(INT)(m_arriXCoors[iNodeIndx] * dXRatioBase) + 3, 
			(INT)(m_arriYCoors[iNodeIndx] * dYRatioBase) + 3);
		memDC.SelectObject(oldObjBrush);

		// ���Ƶ�ʣ�๦��
		strHeaderIndx.Format(_T("(%.1f)"), m_arrdPower[iNodeIndx]);
		memDC.TextOut((INT)(m_arriXCoors[iNodeIndx] * dXRatioBase),
			(INT)(m_arriYCoors[iNodeIndx] * dYRatioBase),
			strHeaderIndx);
		/*
		strHeaderIndx.Format(_T("(%.1f,%.1lf,%.1lf,%.1lf)"),
			m_arrdRSend[iNodeIndx], m_arrdRRecv[iNodeIndx],
			m_arrdPower[iNodeIndx], m_arrdPn[iNodeIndx]);
		memDC.TextOut((INT)(m_arriXCoors[iNodeIndx] * dXRatioBase),
			(INT)(m_arriYCoors[iNodeIndx] * dYRatioBase),
			strHeaderIndx);
		*/
	}

	//���ƽڵ�֮�������
	for (i = 0; i < m_iNodeCnt; i++)
	{
		for (j = 0; j < m_iNodeCnt; j++)
		{
			if (m_arriTopMatrix[i][j] == 1)
			{
				CPen objPenHdr2Hdr(PS_SOLID, 1, RGB(255, 0, 0));
				CPen *oldObjPenHdr2Hdr = memDC.SelectObject(&objPenHdr2Hdr);
				
				memDC.MoveTo((INT)m_arriXCoors[i]* dXRatioBase,
					(INT)m_arriYCoors[i]* dYRatioBase);
				memDC.LineTo((INT)m_arriXCoors[j]* dXRatioBase, 
					(INT)m_arriYCoors[j]* dYRatioBase);

				memDC.SelectObject(oldObjPenHdr2Hdr);
			}
		}
	}

}

/***
 * �������
 */
DOUBLE CUasnSimula::GetDistance(DOUBLE dX, DOUBLE dY)
{
	return sqrt(HCN_WTG_SQUARE(dX) + HCN_WTG_SQUARE(dY));
}

void CUasnSimula::InitParams(INT mcnt, DOUBLE a, DOUBLE b, DOUBLE c)
{
	INT i;

	m_dPa = a;
	m_dPb = b;
	m_dPc = c;

	/***
	 * ��ʼ����ر���
	 */
	if (mcnt != m_iNodeCnt || NULL == m_arriXCoors)
	{

		if (NULL != m_arriXCoors)
		{
			Clear();
		}

		m_iNodeCnt = mcnt;

		m_arriXCoors = new INT[m_iNodeCnt];
		m_arriYCoors = new INT[m_iNodeCnt];
		m_arrdPower = new DOUBLE[m_iNodeCnt];
		
		m_arrdRSend = new DOUBLE[m_iNodeCnt];
		m_arrdRRecv = new DOUBLE[m_iNodeCnt];
		//
		m_arrdPn = new DOUBLE*[m_iNodeCnt];
		for (i = 0; i < m_iNodeCnt; i++)
			m_arrdPn[i] = new DOUBLE[m_iNodeCnt];
		//
		m_arriTopMatrix = new INT*[m_iNodeCnt];

		for (i = 0; i < m_iNodeCnt; i++)
			m_arriTopMatrix[i] = new INT[m_iNodeCnt];
		//
		m_arriMstTop = new INT*[m_iNodeCnt];

		for (i = 0; i < m_iNodeCnt; i++)
			m_arriMstTop[i] = new INT[m_iNodeCnt];
		//
		m_arrdDist = new DOUBLE*[m_iNodeCnt];

		for (i = 0; i < m_iNodeCnt; i++)
			m_arrdDist[i] = new DOUBLE[m_iNodeCnt];
	}
}

void CUasnSimula::_UasnSimula()
{
	INT i, j;

RETRY:

	//������ɽڵ�
	srand((UINT)time(NULL));

	for (i = 0; i < m_iNodeCnt; i++)
	{
		m_arriXCoors[i] = (rand()%(100 - HCN_MARGIN_X*2)) + HCN_MARGIN_X;
		m_arriYCoors[i] = (rand()%(100 - HCN_MARGIN_Y*2)) + HCN_MARGIN_Y;

		/***
		 * ���ָ�����ͷ�Χ
		 */
		m_arrdRSend[i] = (rand()%15) + 12;
		m_arrdRRecv[i] = m_arrdRSend[i];
		m_arrdPower[i] = 10.1;
	}

	//�������Ȩֵ
	for (i = 0; i < m_iNodeCnt; i++)
	{
		for (j = 0; j < m_iNodeCnt; j++)
		{
			m_arrdPn[i][j] = (rand()%20 + 80) / 100.0;

			m_arriTopMatrix[i][j] = 0;
			
			if (i == j) m_arrdDist[i][j] = 0.0;
			else
			{
				m_arrdDist[i][j] = 
					GetDistance(m_arriXCoors[i] - m_arriXCoors[j],
								m_arriYCoors[i] - m_arriYCoors[j]);

				//�ڵ�֮��ɴ�
				if (m_arrdDist[i][j] < 
					GetMin(m_arrdRSend[i], m_arrdRSend[j]))
					m_arriTopMatrix[i][j] = 1;

			}

			m_arriMstTop[i][j] = m_arriTopMatrix[i][j];
		}

		//����ÿ���ڵ���ͨ��
		INT dims = 0;
		for (j = 0; j < m_iNodeCnt; j++)
		{
			dims += m_arriTopMatrix[i][j];
		}

		//��֤�����ڹ����ڵ�,�趨�˽ڵ���ͨά�����ֵ30
		if (0 == dims || dims > 30) goto RETRY;
	}

	//���½���
	((CProgressCtrl *)afxpMainWnd->GetDlgItem(IDC_PROGRESS))->SetRange32(100, 100);

	Invalidate();
}

DWORD WINAPI CUasnSimula::_UasnSimulaThread(LPVOID lParam)
{
	CUasnSimula *pThis = (CUasnSimula *)lParam;
	//afxpMainWnd->GetDlgItem(IDC_EDIT_AVG_DURATION)->SetWindowText(_T(""));
	//afxpMainWnd->GetDlgItem(IDC_EDIT_AVG_HDR_CNT)->SetWindowText(_T(""));
	((CProgressCtrl *)afxpMainWnd->GetDlgItem(IDC_PROGRESS))->SetRange32(0, 100);
	pThis->_UasnSimula();
	
	return 0L;
}

/***
 * ������ʼ���߳�
 */
void CUasnSimula::UasnSimula(INT iNodeCnt)
{
	if (m_hUasnSimulaThread != INVALID_HANDLE_VALUE)
	{
		if (WaitForSingleObject(m_hUasnSimulaThread, 1000) == WAIT_TIMEOUT)
		{
			return;
		}
		else
		{
			m_hUasnSimulaThread = INVALID_HANDLE_VALUE;
		}
	}

	if (m_hUasnSimulaThread == INVALID_HANDLE_VALUE)
	{
		m_iNodeCnt = iNodeCnt;

		m_hUasnSimulaThread = CreateThread(NULL, 0, _UasnSimulaThread, this, 0, NULL);
	}
}

/****
 * ����MST��������ͼ
 */
void CUasnSimula::_UasnMstTop()
{
	INT i, j, start = 0, next = 0;
	DOUBLE *minDist = new DOUBLE[m_iNodeCnt];
	INT *closeSet = new INT[m_iNodeCnt];

	for (i = 0; i < m_iNodeCnt; i++)
		for (j = 0; j < m_iNodeCnt; j++)
			m_arriTopMatrix[i][j] = 0;

	//����Prime�㷨
	for (i = 0; i < m_iNodeCnt; i++)
	{
		closeSet[i] = -1;
		minDist[i] = DOUBLE_INFINITY_VALUE_HIGH;

		if (start != i && m_arriMstTop[start][i] == 1)
		{
			closeSet[i] = start;
			minDist[i] = m_arrdDist[start][i];
		}
	}
	closeSet[start] = -2;

	for (i = 1; i < m_iNodeCnt; i++)
	{
		//get min closeSet
		DOUBLE min = DOUBLE_INFINITY_VALUE_HIGH;

		/***
		 * ȡ����С�����(�����������Ȩֵ)
		 */
		for (j = 0; j < m_iNodeCnt; j++)
		{
			if (min > minDist[j])
			{
				min = minDist[j];
				next = j;
				start = closeSet[j];
			}
		}

		/**
		 * �ҵ���һ���ڵ����MST����
		 */
		if (min != DOUBLE_INFINITY_VALUE_HIGH)
		{
			m_arriTopMatrix[start][next] = 1;
			m_arrdPower[next] = min;
			minDist[next] = DOUBLE_INFINITY_VALUE_HIGH;
			closeSet[next] = -2;
		}

		/***
		 * ���¾����б������ҳ���һ�������
		 */
		for (j = 0; j < m_iNodeCnt; j++)
		{
			if (closeSet[j] != -2 && 
				m_arriMstTop[next][j] == 1 &&
				m_arrdDist[next][j] < minDist[j])
			{
				closeSet[j] = next;
				minDist[j] = m_arrdDist[next][j];
			}
		}

		start = next;
	}

	delete []minDist;
	delete []closeSet;

	Invalidate();
}

DWORD WINAPI CUasnSimula::_UasnMstThread(LPVOID lParam)
{
	CUasnSimula *pThis = (CUasnSimula *)lParam;
	
	pThis->_UasnMstTop();
	
	return 0L;
}

/***
 * ����MST�����߳�
 */
void CUasnSimula::UasnMstTop(INT iNodeCnt)
{
	if (m_hUasnMstThread != INVALID_HANDLE_VALUE)
	{
		if (WaitForSingleObject(m_hUasnMstThread, 1000) == WAIT_TIMEOUT)
		{
			return;
		}
		else
		{
			m_hUasnMstThread = INVALID_HANDLE_VALUE;
		}
	}

	if (m_hUasnMstThread == INVALID_HANDLE_VALUE)
	{
		m_hUasnMstThread = CreateThread(NULL, 0, _UasnMstThread, this, 0, NULL);
	}
}

//���ݽ����㷨��������ͼ
void CUasnSimula::_UasnEvolutionTop()
{
	//�������ֵ��������Ȩֵ����
	INT i, j;
	static INT K = 1.0;
	INT A, S;
	INT p;

	for (i = 0; i < m_iNodeCnt; i++)
	{
		DOUBLE avgP = 0.0;
		DOUBLE aphla = 0.0;

		/***
		 * ��ÿ���ڵ㣬���ݸ��ʵ�����ʽ���и���ʣ������
		 */
		for (j = 0; j < m_iNodeCnt; j++)
		{
			if (m_arriMstTop[i][j] == 1)
			{
				//��S
				if (avgP == 0.0) 
				{
					INT cnt = 0;
					for (p = 0; p < m_iNodeCnt; p++)
					{
						if (m_arriMstTop[i][p] == 1)
						{
							avgP += m_arrdPn[i][p];
							cnt++;
						}
					}

					/* ������Χ�ڵ�������������ƽ��ֵ���� */
					avgP = avgP / cnt;
				}

				S = 0;
				if (m_arrdPn[i][j] - avgP >= 0)
					S = 1;

				//��A
				if (aphla == 0.0)
				{
					DOUBLE pc = 1.0;
					for (p = 0; p < m_iNodeCnt; p++)
					{
						if (m_arriMstTop[i][p] == 1)
						{
							pc *= m_arrdPn[i][j];
						}
					}

					/* ����aphlaֵ */
					aphla = pc;
				}

				A = 0;
				if (aphla > m_dPb) A = 1;

				//���¸���ֵ
				m_arrdPn[i][j] = K*m_dPa + A*m_dPb + S*m_dPc;

				//����Ȩֵ
				while (m_arrdPn[i][j] > 1)
				{
					m_arrdPn[i][j] = m_arrdPn[i][j] - 1;
				}

				m_arrdDist[i][j] = m_arrdPn[i][j] * m_arrdDist[i][j];
			}
		}
	}

	_UasnMstTop();

	K = 0;
}


DWORD WINAPI CUasnSimula::_UasnEvolutionThread(LPVOID lParam)
{
	CUasnSimula *pThis = (CUasnSimula *)lParam;
	
	pThis->_UasnEvolutionTop();
	
	return 0L;
}

/***
 * ���������̺߳���
 */
void CUasnSimula::UasnEvolutionTop(INT iNodeCnt)
{
	if (m_hUasnEvolutionThread != INVALID_HANDLE_VALUE)
	{
		if (WaitForSingleObject(m_hUasnEvolutionThread, 1000) == WAIT_TIMEOUT)
		{
			return;
		}
		else
		{
			m_hUasnEvolutionThread = INVALID_HANDLE_VALUE;
		}
	}

	if (m_hUasnEvolutionThread == INVALID_HANDLE_VALUE)
	{
		m_hUasnEvolutionThread = CreateThread(NULL, 0, _UasnEvolutionThread, this, 0, NULL);
	}
}
