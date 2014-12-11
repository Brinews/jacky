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
	// 变量初始化
	// 单位时间的簇头侦听能耗
	m_dListenCost = 0.14;
	// 算法调整周期
	m_dAdjustTime = 2;
	// 单位流量速度
	m_dFlowVelocity = 1;
	// 能耗模型系数
	m_dEnergyConsumModelCoef = 0.00002;
	// 节点初始能量
	m_iNodeEnergy = 10;
	// 执行次数
	m_iDoTimes = 100;
	// 通信半径
	m_dRadius = 70;

	m_arrbHeaderFlags = NULL;
	m_arriBelongIndx = NULL;
	m_arrbCoverFlags = NULL;
	m_arrbMST = NULL;
	m_arriParent = NULL;
	m_arrdRestEnergy = NULL;
	m_arriXCoors = NULL;
	m_arriYCoors = NULL;

	// 节点个数
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
	
	// 计算比例
	DOUBLE dXRatioBase = rect.Width() / 100.;
	DOUBLE dYRatioBase = rect.Height() / 100.;

	// 存放头结点坐标，用于绘制簇头到簇头的线条
	vector<CPoint> vecHeaderPts;

	CString strDistance;
	CString strHeaderIndx;

	COLORREF crTextColor = ::GetTextColor(memDC.m_hDC);

	// 绘制节点
	for (INT iNodeIndx=0, iHeaderIndx=1; iNodeIndx<m_iNodeCnt; iNodeIndx++)
	{
		CBrush objBrush(RGB(0, 0, 0));
		CBrush *oldObjBrush = memDC.SelectObject(&objBrush);
		// 绘制点
		memDC.Ellipse((INT)(m_arriXCoors[iNodeIndx] * dXRatioBase) - 3, 
			(INT)(m_arriYCoors[iNodeIndx] * dYRatioBase) - 3, 
			(INT)(m_arriXCoors[iNodeIndx] * dXRatioBase) + 3, 
			(INT)(m_arriYCoors[iNodeIndx] * dYRatioBase) + 3);
		memDC.SelectObject(oldObjBrush);

		// 绘制簇头
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

			// 绘制belong簇头的普通节点连线
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

					//绘制距离
// 					CBrush objBrushDistance(RGB(0, 0, 0)); 
// 					CBrush *oldObjBrushDistance = memDC.SelectObject(&objBrushDistance);
					// 计算距离
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

	// 绘制簇头和簇头之间的线
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

			// 绘制簇头之间的距离
// 			CBrush objBrushHdr2HdrDistance(RGB(0, 0, 0));
// 			CBrush *oldObjBrushHdr2HdrDistance = memDC.SelectObject(&objBrushHdr2HdrDistance);
			// 计算距离
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
		// 绘制簇头下方的编号
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

// 判断是否有节点能量为空的值，如果有，返回FALSE
BOOL CPlotCtrl::CheckEmptyNode()
{
	BOOL bRet = TRUE;
	for (INT i=0; i<m_iNodeCnt; i++)
	{
		// 判断是否有节点能量为空的值
		if (m_arrdRestEnergy[i] <= 0)
		{
			bRet = FALSE;
		}
	}
	return bRet;
}

// 找到最强壮点，返回下标
INT CPlotCtrl::FindMostRobust()
{
	DOUBLE dMax=0;
	INT iPos;
	for (INT i=5; i<m_iNodeCnt; i++)
	{
		// 找到最强壮点成为起始点
		if (m_arrdRestEnergy[i] > dMax)
		{
			iPos = i;
			dMax = m_arrdRestEnergy[i];
		}
	}
	return iPos;
}

// 是否全部节点已覆盖，如果全部覆盖，返回TRUE
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
		m_arriParent[i]      = 0; //父节点不可能为0，因为汇聚节点不会成为簇头。如belong，所以不进行特别处理
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
	// 汇聚点所属簇头，该簇头可先减去耗费
	INT iIndxHeader = m_arriBelongIndx[0];
	// 距离的平方，符合能耗模型
	// 能耗指数
	DOUBLE dCostIndex = HCN_WTG_SQUARE(HCN_WTG_SQUARE(fabs(XCoors[iIndxHeader] - XCoors[0])) + HCN_WTG_SQUARE(fabs(YCoors[iIndxHeader] - YCoors[0])));
	// 4个源点的流量
	m_arrdRestEnergy[iIndxHeader] = m_arrdRestEnergy[iIndxHeader] - 4 * m_dFlowVelocity * m_dEnergyConsumModelCoef * m_dAdjustTime * dCostIndex;
	
	// 对每个源点计算产生流量沿路所耗能量
	for (INT i=1; i<5; i++)
	{
		INT iTmp = m_arriBelongIndx[i];
		// 下跳节点号
		INT dIndxNext = m_arriBelongIndx[i];
		// 持续寻路直至到达iIndxHeader
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
	// MST发起者编号，即汇聚节点所属簇头
	INT iIndxStarter = m_arriBelongIndx[0];
	// 发起者首先加入MST
    m_arrbMST[iIndxStarter] = TRUE;

	// 尚有没加入MST的簇头
	while (!CompareMst())
	{
		INT i, j;
		DOUBLE dDistance = 0;
		DOUBLE dDisMin = m_dRadius;
		INT iIndxChild;
		INT iIndxParent;
		for (i=0; i<m_iNodeCnt; i++)
		{
			// 是否已在MST内
			if (m_arrbMST[i] == TRUE)
			{
				// 第二层，选择未加入的簇头节点
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
		// 找到子节点，给它标注父点
		m_arriParent[iIndxChild] = iIndxParent;
		// 加入MST
		m_arrbMST[iIndxChild] = TRUE;
	}
}

// 判断MST集合是否与簇头集合一致，一致返回TRUE
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
	// 总持续时间
	INT iTotalDuration = 0;
	// 平均持续时间
	DOUBLE dAverageDuration = 0;
	// 总簇头数
	INT iTotalHeaderCnt = 0;
	// 平均簇头数
	DOUBLE dAverageHeadCnt = 0;
	// 强壮节点下标
	INT iMostRobustIndx;
	// 新簇头节点号
	INT iNewNodeIndx;
	// 程序持续周期数
	INT iTimeDuration = 0;
	
	srand((UINT)time(NULL));

	for (INT iDoI=1; iDoI<=m_iDoTimes; iDoI++)
	{
		((CProgressCtrl *)afxpMainWnd->GetDlgItem(IDC_PROGRESS))->SetPos(iDoI);
		// 横纵坐标
		INT *iXCoors = new INT[m_iNodeCnt];
		INT *iYCoors = new INT[m_iNodeCnt];
		
		for (INT iInitEnergyI=0; iInitEnergyI<m_iNodeCnt; iInitEnergyI++)
		{
			m_arrdRestEnergy[iInitEnergyI] = m_iNodeEnergy;
		}
		// 1. 生成节点 (第一个节点为汇聚点 后续四个节点为源点 其余为普通节点)
		
		// 真正随机节点位置的确定
		for (INT j=0; j<m_iNodeCnt; j++)
		{
			iXCoors[j] = (rand() % (100 - HCN_MARGIN_X * 2)) + HCN_MARGIN_X;
			iYCoors[j] = (rand() % (100 - HCN_MARGIN_Y * 2)) + HCN_MARGIN_Y;
		}
		iTimeDuration = 0;
		
		while (CheckEmptyNode()) 
		{
			iTimeDuration++;
			// 新一轮调整首先清空五种标记 
			Reset();
			// 2 形成拓扑
			// 2.1 寻找簇头
			// 找到发起者
			iMostRobustIndx = FindMostRobust();    
			m_arrbHeaderFlags[iMostRobustIndx] = TRUE;
			for (INT n=0; n<m_iNodeCnt; n++)
			{ 
				if (GetDistance(fabs(iXCoors[iMostRobustIndx] - iXCoors[n]), fabs(iYCoors[iMostRobustIndx] - iYCoors[n])) <= m_dRadius)
				{
					m_arrbCoverFlags[n] = TRUE;
				}
			}
			
			// 是否全部节点已覆盖，若未则继续寻找其他簇头
			while (!CheckConnectAll())
			{
				DOUBLE dMaxWeight = 0;
				DOUBLE dTmp = 0;

				// 选择出手的簇头
				for (INT iHeaderI=5; iHeaderI<m_iNodeCnt; iHeaderI++)
				{
					if (m_arrbHeaderFlags[iHeaderI] == TRUE)
					{
						// 选定出手簇头后选新簇头
						for (INT iChildI=5; iChildI<m_iNodeCnt; iChildI++)
						{
							// 如果和出手簇头标号一样，或者是簇头，跳过
							if (iChildI == iHeaderI || m_arrbHeaderFlags[iChildI] == TRUE)
							{
								continue;
							}
							DOUBLE dDistance = GetDistance(fabs(iXCoors[iChildI] - iXCoors[iHeaderI]), fabs(iYCoors[iChildI] - iYCoors[iHeaderI]));
							// 在当前簇头覆盖域内
							if (dDistance <= m_dRadius)
							{
								// 计算权重
								// 改变权值处
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
				// 找到新簇头
				m_arrbHeaderFlags[iNewNodeIndx] = TRUE;
				
				for (INT iCheckCoverI=0; iCheckCoverI<m_iNodeCnt; iCheckCoverI++)
				{
					if (GetDistance(fabs(iXCoors[iNewNodeIndx] - iXCoors[iCheckCoverI]), fabs(iYCoors[iNewNodeIndx] - iYCoors[iCheckCoverI])) <= m_dRadius)
					{
						m_arrbCoverFlags[iCheckCoverI] = TRUE;
					}
				} // end for (iCheckCoverI)
			}
			// 2.1 寻找簇头 end
			
			// 2.2 每个节点归属簇头
			
			HeaderBelong(iXCoors, iYCoors);
			
			// 3 计算能量损耗
			// 簇头侦听能耗
			ListenConsume();
			// 构建本地最小生成树，并确定上下级信息
			InitMst(iXCoors, iYCoors);
			// 兴趣数据传输能耗
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
		
		// 复制到成员数组，用于绘制
		memcpy(m_arriXCoors, iXCoors, sizeof(INT) * m_iNodeCnt);
		memcpy(m_arriYCoors, iYCoors, sizeof(INT) * m_iNodeCnt);

		HCN_SAFE_DELETEA(iXCoors);
		HCN_SAFE_DELETEA(iYCoors);
	}

	// 计算平均持续时间
	dAverageDuration = (DOUBLE)iTotalDuration / m_iDoTimes;
	// 计算平均簇头个数
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