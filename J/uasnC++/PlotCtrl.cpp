// PlotCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "WSNTopoGen.h"
#include "PlotCtrl.h"
#include "MemDC.h"
#include <math.h>
#include <wtypes.h>

#include "WSNTopoGenDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPlotCtrl

CPlotCtrl::CPlotCtrl()
{
	// ������ʼ��
	// ��λʱ��Ĵ�ͷ�����ܺ�
	m_dListenCost = 0.14;
	// �㷨��������
	m_dAdjustTime = 2;
	// ��λ�����ٶ�
	m_dFlowVelocity = 1;
	// �ܺ�ģ��ϵ��
	m_dEnergyConsumModelCoef = 0.00002;
	// �ڵ��ʼ����
	m_iNodeEnergy = 10;
	// ִ�д���
	m_iDoTimes = 100;
	// ͨ�Ű뾶
	m_dRadius = 70;

	m_arrbHeaderFlags = NULL;
	m_arriBelongIndx = NULL;
	m_arrbCoverFlags = NULL;
	m_arrbMST = NULL;
	m_arriParent = NULL;
	m_arrdRestEnergy = NULL;
	m_arriXCoors = NULL;
	m_arriYCoors = NULL;

	// �ڵ����
	m_iNodeCnt = 0;

	m_hGenTopoThread = INVALID_HANDLE_VALUE;
}

CPlotCtrl::~CPlotCtrl()
{
}


BEGIN_MESSAGE_MAP(CPlotCtrl, CStatic)
//{{AFX_MSG_MAP(CPlotCtrl)
ON_WM_PAINT()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlotCtrl message handlers

void CPlotCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rect;
	GetClientRect(&rect);
	CMemDC memDC(&dc, &rect);
	
	// �������
	DOUBLE dXRatioBase = rect.Width() / 100.;
	DOUBLE dYRatioBase = rect.Height() / 100.;

	// ���ͷ������꣬���ڻ��ƴ�ͷ����ͷ������
	vector<CPoint> vecHeaderPts;

	CString strDistance;
	CString strHeaderIndx;

	COLORREF crTextColor = ::GetTextColor(memDC.m_hDC);

	// ���ƽڵ�
	for (INT iNodeIndx=0, iHeaderIndx=1; iNodeIndx<m_iNodeCnt; iNodeIndx++)
	{
		CBrush objBrush(RGB(0, 0, 0));
		CBrush *oldObjBrush = memDC.SelectObject(&objBrush);
		// ���Ƶ�
		memDC.Ellipse((INT)(m_arriXCoors[iNodeIndx] * dXRatioBase) - 3, 
			(INT)(m_arriYCoors[iNodeIndx] * dYRatioBase) - 3, 
			(INT)(m_arriXCoors[iNodeIndx] * dXRatioBase) + 3, 
			(INT)(m_arriYCoors[iNodeIndx] * dYRatioBase) + 3);
		memDC.SelectObject(oldObjBrush);

		// ���ƴ�ͷ
		if (m_arrbHeaderFlags[iNodeIndx] == TRUE)
		{
			CBrush objBrushHeader(RGB(255, 0, 0));
			CBrush *oldObjBrushHeader = memDC.SelectObject(&objBrushHeader);
			memDC.Ellipse((INT)(m_arriXCoors[iNodeIndx] * dXRatioBase) - 3, 
				(INT)(m_arriYCoors[iNodeIndx] * dYRatioBase) - 3, 
				(INT)(m_arriXCoors[iNodeIndx] * dXRatioBase) + 3, 
				(INT)(m_arriYCoors[iNodeIndx] * dYRatioBase) + 3);
			memDC.SelectObject(oldObjBrushHeader);

			vecHeaderPts.push_back(CPoint((INT)(m_arriXCoors[iNodeIndx] * dXRatioBase), 
				(INT)(m_arriYCoors[iNodeIndx] * dYRatioBase)));

			// ����belong��ͷ����ͨ�ڵ�����
			for (INT iBelongIndx=0; iBelongIndx<m_iNodeCnt; iBelongIndx++)
			{
				if (m_arriBelongIndx[iBelongIndx] == iNodeIndx)
				{
					CPen objPenLine(PS_SOLID, 1, RGB(0, 0, 0));
					CPen *oldObjPenLine = memDC.SelectObject(&objPenLine);
					memDC.MoveTo((INT)(m_arriXCoors[iBelongIndx] * dXRatioBase), 
						(INT)(m_arriYCoors[iBelongIndx] * dYRatioBase));
					memDC.LineTo(vecHeaderPts[iHeaderIndx - 1].x, vecHeaderPts[iHeaderIndx - 1].y);
					memDC.SelectObject(oldObjPenLine);

					//���ƾ���
// 					CBrush objBrushDistance(RGB(0, 0, 0)); 
// 					CBrush *oldObjBrushDistance = memDC.SelectObject(&objBrushDistance);
					// �������
					strDistance.Format(_T("%0.2f"), 
						GetDistance(fabs(m_arriXCoors[iNodeIndx] - m_arriXCoors[iBelongIndx]), 
						fabs(m_arriYCoors[iNodeIndx] - m_arriYCoors[iBelongIndx])));
					memDC.TextOut((INT)(m_arriXCoors[iBelongIndx] * dXRatioBase + m_arriXCoors[iNodeIndx] * dXRatioBase) / 2, 
						(INT)(m_arriYCoors[iBelongIndx] * dYRatioBase + m_arriYCoors[iNodeIndx] * dYRatioBase) / 2, 
						strDistance);  
//					memDC.SelectObject(oldObjBrushDistance);
				}
			}
			iHeaderIndx++;
		}
	}

	// ���ƴ�ͷ�ʹ�ͷ֮�����
	INT iHeaderIndxSrc, iHeaderIndxDes;
	for (iHeaderIndxSrc=0; iHeaderIndxSrc<iHeaderIndx-2; iHeaderIndxSrc++)
	{
		for (iHeaderIndxDes=iHeaderIndxSrc+1; iHeaderIndxDes<iHeaderIndx-1; iHeaderIndxDes++)
		{
			CPen objPenHdr2Hdr(PS_SOLID, 1, RGB(255, 0, 0));
			CPen *oldObjPenHdr2Hdr = memDC.SelectObject(&objPenHdr2Hdr);
			memDC.MoveTo(vecHeaderPts[iHeaderIndxSrc].x, vecHeaderPts[iHeaderIndxSrc].y);
			memDC.LineTo(vecHeaderPts[iHeaderIndxDes].x, vecHeaderPts[iHeaderIndxDes].y);
			memDC.SelectObject(oldObjPenHdr2Hdr);

			// ���ƴ�ͷ֮��ľ���
// 			CBrush objBrushHdr2HdrDistance(RGB(0, 0, 0));
// 			CBrush *oldObjBrushHdr2HdrDistance = memDC.SelectObject(&objBrushHdr2HdrDistance);
			// �������
			::SetTextColor(memDC.m_hDC, crTextColor);
			strDistance.Format(_T("%0.2f"), 
				GetDistance(fabs(m_arriXCoors[iHeaderIndxSrc] - m_arriXCoors[iHeaderIndxDes]), 
				fabs(m_arriYCoors[iHeaderIndxSrc] - m_arriYCoors[iHeaderIndxDes])));
			memDC.TextOut((vecHeaderPts[iHeaderIndxSrc].x + vecHeaderPts[iHeaderIndxDes].x) / 2, 
				(vecHeaderPts[iHeaderIndxSrc].y + vecHeaderPts[iHeaderIndxDes].y) / 2, 
				strDistance);  
//			memDC.SelectObject(oldObjBrushHdr2HdrDistance);
		}
	}
	
	::SetTextColor(memDC.m_hDC, RGB(0, 0, 255));
	for (INT iHeaderI=0; iHeaderI<iHeaderIndx-1; iHeaderI++)
	{
		// ���ƴ�ͷ�·��ı��
// 		CBrush objBrushIndx(RGB(0, 0, 255));
// 		CBrush *oldObjBrushIndx = memDC.SelectObject(&objBrushIndx);
		strHeaderIndx.Format (_T("%d"), iHeaderI + 1);
		memDC.TextOut((INT)(vecHeaderPts[iHeaderI].x), 
			(INT)(vecHeaderPts[iHeaderI].y), 
			strHeaderIndx);
//		memDC.SelectObject(oldObjBrushIndx);
	}

	::SetTextColor(memDC.m_hDC, crTextColor);
}

DOUBLE CPlotCtrl::GetDistance(DOUBLE dX, DOUBLE dY)
{
	return sqrt(HCN_WTG_SQUARE(dX) + HCN_WTG_SQUARE(dY));
}

// �ж��Ƿ��нڵ�����Ϊ�յ�ֵ������У�����FALSE
BOOL CPlotCtrl::CheckEmptyNode()
{
	BOOL bRet = TRUE;
	for (INT i=0; i<m_iNodeCnt; i++)
	{
		// �ж��Ƿ��нڵ�����Ϊ�յ�ֵ
		if (m_arrdRestEnergy[i] <= 0)
		{
			bRet = FALSE;
		}
	}
	return bRet;
}

// �ҵ���ǿ׳�㣬�����±�
INT CPlotCtrl::FindMostRobust()
{
	DOUBLE dMax=0;
	INT iPos;
	for (INT i=5; i<m_iNodeCnt; i++)
	{
		// �ҵ���ǿ׳���Ϊ��ʼ��
		if (m_arrdRestEnergy[i] > dMax)
		{
			iPos = i;
			dMax = m_arrdRestEnergy[i];
		}
	}
	return iPos;
}

// �Ƿ�ȫ���ڵ��Ѹ��ǣ����ȫ�����ǣ�����TRUE
BOOL CPlotCtrl::CheckConnectAll()
{
	BOOL bMask = TRUE;
	for (INT i=0; i<m_iNodeCnt; i++)
	{
		bMask = bMask * m_arrbCoverFlags[i];
	}
	return bMask;
}

void CPlotCtrl::HeaderBelong(INT *XCoors, INT *YCoors)
{
	for (INT i=0; i<m_iNodeCnt; i++)
	{
		DOUBLE dDistance = 0;
		DOUBLE dMin = DOUBLE_INFINITY_VALUE_HIGH;
		INT iIndxMin;
		if (m_arrbHeaderFlags[i] == TRUE)
		{
			m_arriBelongIndx[i] = i;
		}
		else
		{
			for (INT j=0; j<m_iNodeCnt; j++)
			{
				if (m_arrbHeaderFlags[j] == TRUE)
				{
					dDistance = GetDistance(fabs(XCoors[j] - XCoors[i]), fabs(YCoors[j] - YCoors[i]));
					if (dDistance < dMin)
					{
						dMin = dDistance;
						iIndxMin = j;
					}
				}
				else
				{
					continue;
				}
			}
			m_arriBelongIndx[i] = iIndxMin;
		}
	}
}

void CPlotCtrl::Reset()
{
	for (INT i=0; i<m_iNodeCnt; i++)
	{
		m_arrbHeaderFlags[i] = FALSE;
		m_arriBelongIndx[i]	 = m_iNodeCnt;
		m_arrbCoverFlags[i]  = FALSE;
		m_arrbMST[i]         = FALSE;
		m_arriParent[i]      = 0; //���ڵ㲻����Ϊ0����Ϊ��۽ڵ㲻���Ϊ��ͷ����belong�����Բ������ر���
	}
}

void CPlotCtrl::ListenConsume()
{
	for (INT i=5; i<m_iNodeCnt; i++)
	{
		if (m_arrbHeaderFlags[i] == TRUE)
		{
            m_arrdRestEnergy[i] = m_arrdRestEnergy[i] - m_dAdjustTime * m_dListenCost;
		}
	}
}

void CPlotCtrl::TransmitConsume(INT XCoors[], INT YCoors[])
{
	// ��۵�������ͷ���ô�ͷ���ȼ�ȥ�ķ�
	INT iIndxHeader = m_arriBelongIndx[0];
	// �����ƽ���������ܺ�ģ��
	// �ܺ�ָ��
	DOUBLE dCostIndex = HCN_WTG_SQUARE(HCN_WTG_SQUARE(fabs(XCoors[iIndxHeader] - XCoors[0])) + HCN_WTG_SQUARE(fabs(YCoors[iIndxHeader] - YCoors[0])));
	// 4��Դ�������
	m_arrdRestEnergy[iIndxHeader] = m_arrdRestEnergy[iIndxHeader] - 4 * m_dFlowVelocity * m_dEnergyConsumModelCoef * m_dAdjustTime * dCostIndex;
	
	// ��ÿ��Դ��������������·��������
	for (INT i=1; i<5; i++)
	{
		INT iTmp = m_arriBelongIndx[i];
		// �����ڵ��
		INT dIndxNext = m_arriBelongIndx[i];
		// ����Ѱ·ֱ������iIndxHeader
		while (dIndxNext != iIndxHeader)
		{
			iTmp = m_arriParent[dIndxNext];
			DOUBLE y = HCN_WTG_SQUARE(fabs(XCoors[dIndxNext] - XCoors[iTmp])) + HCN_WTG_SQUARE(fabs(YCoors[dIndxNext] - YCoors[iTmp]));
			DOUBLE x = m_dFlowVelocity * m_dEnergyConsumModelCoef * m_dAdjustTime * y;
			m_arrdRestEnergy[dIndxNext] = m_arrdRestEnergy[dIndxNext] - x;
			dIndxNext = iTmp;
		}
	}
}

void CPlotCtrl::InitMst(INT *XCoors, INT *YCoors)
{
	// MST�����߱�ţ�����۽ڵ�������ͷ
	INT iIndxStarter = m_arriBelongIndx[0];
	// ���������ȼ���MST
    m_arrbMST[iIndxStarter] = TRUE;

	// ����û����MST�Ĵ�ͷ
	while (!CompareMst())
	{
		INT i, j;
		DOUBLE dDistance = 0;
		DOUBLE dDisMin = m_dRadius;
		INT iIndxChild;
		INT iIndxParent;
		for (i=0; i<m_iNodeCnt; i++)
		{
			// �Ƿ�����MST��
			if (m_arrbMST[i] == TRUE)
			{
				// �ڶ��㣬ѡ��δ����Ĵ�ͷ�ڵ�
				for (j=0; j<m_iNodeCnt; j++)
				{
					if (m_arrbHeaderFlags[j] == TRUE && m_arrbMST[j] != TRUE)
					{
						dDistance = GetDistance(fabs(XCoors[j] - XCoors[i]), fabs(YCoors[j] - YCoors[i]));
						if (dDistance <= dDisMin)
						{
							dDisMin = dDistance;
							iIndxChild = j;
							iIndxParent = i;
						}
					}
					else
					{
						continue;
					}
				}
			}
		}
		// �ҵ��ӽڵ㣬������ע����
		m_arriParent[iIndxChild] = iIndxParent;
		// ����MST
		m_arrbMST[iIndxChild] = TRUE;
	}
}

// �ж�MST�����Ƿ����ͷ����һ�£�һ�·���TRUE
BOOL CPlotCtrl::CompareMst()
{
	for (INT i=0; i<m_iNodeCnt; i++)
	{
		if (m_arrbHeaderFlags[i] == TRUE && m_arrbMST[i] == FALSE)
		{
			return FALSE;
		}
	}
    return TRUE;
}

void CPlotCtrl::_GenerateTopo()
{
	// �ܳ���ʱ��
	INT iTotalDuration = 0;
	// ƽ������ʱ��
	DOUBLE dAverageDuration = 0;
	// �ܴ�ͷ��
	INT iTotalHeaderCnt = 0;
	// ƽ����ͷ��
	DOUBLE dAverageHeadCnt = 0;
	// ǿ׳�ڵ��±�
	INT iMostRobustIndx;
	// �´�ͷ�ڵ��
	INT iNewNodeIndx;
	// �������������
	INT iTimeDuration = 0;
	
	srand((UINT)time(NULL));

	for (INT iDoI=1; iDoI<=m_iDoTimes; iDoI++)
	{
		((CProgressCtrl *)afxpMainWnd->GetDlgItem(IDC_PROGRESS))->SetPos(iDoI);
		// ��������
		INT *iXCoors = new INT[m_iNodeCnt];
		INT *iYCoors = new INT[m_iNodeCnt];
		
		for (INT iInitEnergyI=0; iInitEnergyI<m_iNodeCnt; iInitEnergyI++)
		{
			m_arrdRestEnergy[iInitEnergyI] = m_iNodeEnergy;
		}
		// 1. ���ɽڵ� (��һ���ڵ�Ϊ��۵� �����ĸ��ڵ�ΪԴ�� ����Ϊ��ͨ�ڵ�)
		
		// ��������ڵ�λ�õ�ȷ��
		for (INT j=0; j<m_iNodeCnt; j++)
		{
			iXCoors[j] = (rand() % (100 - HCN_MARGIN_X * 2)) + HCN_MARGIN_X;
			iYCoors[j] = (rand() % (100 - HCN_MARGIN_Y * 2)) + HCN_MARGIN_Y;
		}
		iTimeDuration = 0;
		
		while (CheckEmptyNode()) 
		{
			iTimeDuration++;
			// ��һ�ֵ�������������ֱ�� 
			Reset();
			// 2 �γ�����
			// 2.1 Ѱ�Ҵ�ͷ
			// �ҵ�������
			iMostRobustIndx = FindMostRobust();    
			m_arrbHeaderFlags[iMostRobustIndx] = TRUE;
			for (INT n=0; n<m_iNodeCnt; n++)
			{ 
				if (GetDistance(fabs(iXCoors[iMostRobustIndx] - iXCoors[n]), fabs(iYCoors[iMostRobustIndx] - iYCoors[n])) <= m_dRadius)
				{
					m_arrbCoverFlags[n] = TRUE;
				}
			}
			
			// �Ƿ�ȫ���ڵ��Ѹ��ǣ���δ�����Ѱ��������ͷ
			while (!CheckConnectAll())
			{
				DOUBLE dMaxWeight = 0;
				DOUBLE dTmp = 0;

				// ѡ����ֵĴ�ͷ
				for (INT iHeaderI=5; iHeaderI<m_iNodeCnt; iHeaderI++)
				{
					if (m_arrbHeaderFlags[iHeaderI] == TRUE)
					{
						// ѡ�����ִ�ͷ��ѡ�´�ͷ
						for (INT iChildI=5; iChildI<m_iNodeCnt; iChildI++)
						{
							// ����ͳ��ִ�ͷ���һ���������Ǵ�ͷ������
							if (iChildI == iHeaderI || m_arrbHeaderFlags[iChildI] == TRUE)
							{
								continue;
							}
							DOUBLE dDistance = GetDistance(fabs(iXCoors[iChildI] - iXCoors[iHeaderI]), fabs(iYCoors[iChildI] - iYCoors[iHeaderI]));
							// �ڵ�ǰ��ͷ��������
							if (dDistance <= m_dRadius)
							{
								// ����Ȩ��
								// �ı�Ȩֵ��
								dTmp = dDistance * pow(m_arrdRestEnergy[iChildI], 0.2);
								if (dTmp > dMaxWeight)
								{
									iNewNodeIndx = iChildI;
									dMaxWeight = dTmp;
								}
							}
						} // end for (iChildI)
					}
				} // end for (iHeaderI)
				// �ҵ��´�ͷ
				m_arrbHeaderFlags[iNewNodeIndx] = TRUE;
				
				for (INT iCheckCoverI=0; iCheckCoverI<m_iNodeCnt; iCheckCoverI++)
				{
					if (GetDistance(fabs(iXCoors[iNewNodeIndx] - iXCoors[iCheckCoverI]), fabs(iYCoors[iNewNodeIndx] - iYCoors[iCheckCoverI])) <= m_dRadius)
					{
						m_arrbCoverFlags[iCheckCoverI] = TRUE;
					}
				} // end for (iCheckCoverI)
			}
			// 2.1 Ѱ�Ҵ�ͷ end
			
			// 2.2 ÿ���ڵ������ͷ
			
			HeaderBelong(iXCoors, iYCoors);
			
			// 3 �����������
			// ��ͷ�����ܺ�
			ListenConsume();
			// ����������С����������ȷ�����¼���Ϣ
			InitMst(iXCoors, iYCoors);
			// ��Ȥ���ݴ����ܺ�
			TransmitConsume(iXCoors, iYCoors);
		} // while

		iTotalDuration = iTotalDuration + (INT)(iTimeDuration * m_dAdjustTime);

		for (INT i=0; i<m_iNodeCnt; i++)
		{
			if (m_arrbHeaderFlags[i] == TRUE)
			{
				iTotalHeaderCnt++;
			}
		}
		
		// ���Ƶ���Ա���飬���ڻ���
		memcpy(m_arriXCoors, iXCoors, sizeof(INT) * m_iNodeCnt);
		memcpy(m_arriYCoors, iYCoors, sizeof(INT) * m_iNodeCnt);

		HCN_SAFE_DELETEA(iXCoors);
		HCN_SAFE_DELETEA(iYCoors);
	}

	// ����ƽ������ʱ��
	dAverageDuration = (DOUBLE)iTotalDuration / m_iDoTimes;
	// ����ƽ����ͷ����
	dAverageHeadCnt = (DOUBLE)iTotalHeaderCnt / m_iDoTimes;

	CString strInfo;
	strInfo.Format(_T("%.2f"), dAverageDuration);
	afxpMainWnd->GetDlgItem(IDC_EDIT_AVG_DURATION)->SetWindowText(strInfo);
	strInfo.Format(_T("%.2f"), dAverageHeadCnt);
	afxpMainWnd->GetDlgItem(IDC_EDIT_AVG_HDR_CNT)->SetWindowText(strInfo);
	
	Invalidate();
}

void CPlotCtrl::InitParams(DOUBLE dListenCost, 
						   DOUBLE dAdjustTime, 
						   DOUBLE dFlowVelocity, 
						   DOUBLE dEnergyConsumModelCoef, 
						   INT iNodeEnergy, 
						   INT iDoTimes, 
						   DOUBLE dRadius)
{
	m_dListenCost               = dListenCost;
	m_dAdjustTime               = dAdjustTime;
	m_dFlowVelocity             = dFlowVelocity;
	m_dEnergyConsumModelCoef    = dEnergyConsumModelCoef;
	m_iNodeEnergy               = iNodeEnergy;
	m_iDoTimes                  = iDoTimes;
	m_dRadius                   = dRadius;
}

DWORD WINAPI CPlotCtrl::_GenTopoThread(LPVOID lParam)
{
	CPlotCtrl *pThis = (CPlotCtrl *)lParam;
	afxpMainWnd->GetDlgItem(IDC_EDIT_AVG_DURATION)->SetWindowText(_T(""));
	afxpMainWnd->GetDlgItem(IDC_EDIT_AVG_HDR_CNT)->SetWindowText(_T(""));
	((CProgressCtrl *)afxpMainWnd->GetDlgItem(IDC_PROGRESS))->SetRange32(0, pThis->m_iDoTimes);
	pThis->_GenerateTopo();
	return 0L;
}

void CPlotCtrl::GenerateTopo(INT iNodeCnt)
{
	if (m_hGenTopoThread != INVALID_HANDLE_VALUE)
	{
		if (WaitForSingleObject(m_hGenTopoThread, 1000) == WAIT_TIMEOUT)
		{
			return;
		}
		else
		{
			m_hGenTopoThread = INVALID_HANDLE_VALUE;
		}
	}
	if (m_hGenTopoThread == INVALID_HANDLE_VALUE)
	{
		HCN_SAFE_DELETEA(m_arrbHeaderFlags);
		HCN_SAFE_DELETEA(m_arriBelongIndx);
		HCN_SAFE_DELETEA(m_arrbCoverFlags);
		HCN_SAFE_DELETEA(m_arrbMST);
		HCN_SAFE_DELETEA(m_arriParent);
		HCN_SAFE_DELETEA(m_arrdRestEnergy);
		HCN_SAFE_DELETEA(m_arriXCoors);
		HCN_SAFE_DELETEA(m_arriYCoors);
		
		m_iNodeCnt = iNodeCnt;
		
		m_arrbHeaderFlags = new BOOL[m_iNodeCnt];
		m_arriBelongIndx  = new INT[m_iNodeCnt];
		m_arrbCoverFlags  = new BOOL[m_iNodeCnt];
		m_arrbMST         = new BOOL[m_iNodeCnt];
		m_arriParent      = new INT[m_iNodeCnt];
		m_arrdRestEnergy  = new DOUBLE[m_iNodeCnt];
		m_arriXCoors      = new INT[m_iNodeCnt];
		m_arriYCoors      = new INT[m_iNodeCnt];

		m_hGenTopoThread = CreateThread(NULL, 0, _GenTopoThread, this, 0, NULL);
	}
}