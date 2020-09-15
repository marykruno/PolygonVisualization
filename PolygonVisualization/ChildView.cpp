// ChildView.cpp : implementation of the CChildView class
//
#include "stdafx.h"
#include "PolygonVisualization.h"
#include "ChildView.h"
#include "PolygonModel.h"
#include "PolygonRenderer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
	p_model = new PolygonModel();
	p_model->SetObserver(this);
	renderer.SetObserver(this);
}

CChildView::~CChildView()
{
	delete p_model;
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_OPTIONS_BUILDINGPOLYGON, &CChildView::OnOptionsBuildingPolygonMode)
	ON_COMMAND(ID_OPTIONS_CLEARALL, &CChildView::OnOptionsClearall)
END_MESSAGE_MAP()



// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1), NULL);

	return TRUE;
}

int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	int ret = CWnd::OnCreate(lpCreateStruct);
	mem_scn.CreateCompatibleDC(*GetDC());
	SyncMenuStateWithModelState();
	return ret;
}

void CChildView::OnSize(UINT nType, int cx, int cy)
{
	mem_scn.SetDirty(true);
	CWnd::OnSize(nType, cx, cy);

}

void CChildView::ImmediateViewUpdate()
{
	CRect cli_rect;
	GetClientRect(&cli_rect);
	InvalidateRect(&cli_rect);
	UpdateWindow();
}

void CChildView::OnModelChanged(const PolygonModel* p_model)
{
	mem_scn.SetDirty(true);
	ImmediateViewUpdate();

}

void CChildView::OnRenderStyleChanged(const PolygonRenderer* p_renderer)
{
	mem_scn.SetDirty(true);
	ImmediateViewUpdate();
}


void CChildView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	SyncMenuStateWithModelState();
	if (p_model->GetCurrentState() == BUILDING_POLYGON)
	    p_model->AddPoint(Point2D(point.x, point.y));
	else
	{
		p_model->SetTestPoint(Point2D(point.x, point.y));
	}
}

void CChildView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect cli_rect;
	GetClientRect(&cli_rect);
	if (mem_scn.IsDirty())
	{
		mem_scn.PrepareBitmap(cli_rect, dc);
		renderer.Draw(mem_scn.GetMemoryDC(), cli_rect, *p_model);
		mem_scn.SetDirty(false);
	}
	mem_scn.DrawScene(dc, dc.m_ps.rcPaint);
	// TODO: Add your message handler code here

	// Do not call CWnd::OnPaint() for painting messages
}


void CChildView::OnOptionsBuildingPolygonMode()
{
	State state = p_model->GetCurrentState();
	// try to invert state
	if (state == BUILDING_POLYGON)
		state = POLYGON_COMPLETED;
	else
		state = BUILDING_POLYGON;

	p_model->SetCurrentState(state);
	SyncMenuStateWithModelState();
}
void CChildView::SyncMenuStateWithModelState()
{
	State state = p_model->GetCurrentState();
	CMenu* p_main_menu = GetParentFrame()->GetMenu();
	CString caption;
	//GetParentFrame()->GetWindowTextW(caption);
	if (state == BUILDING_POLYGON)
	{
		p_main_menu->CheckMenuItem(ID_OPTIONS_BUILDINGPOLYGON, MF_CHECKED | MF_BYCOMMAND);
		caption = _T("PolygonVisualization: building polygon mode is active");
	}
	else
	{
		p_main_menu->CheckMenuItem(ID_OPTIONS_BUILDINGPOLYGON, MF_UNCHECKED | MF_BYCOMMAND);
		caption = _T("PolygonVisualization: polygon is completed");
		caption += _T(", oriented area of polygon =");
		CString str;
		str.Format(_T("%f"), p_model->GetOrientedArea());
		caption += str;
		double dist;
		Point2D fill, proj_pnt;
		bool is_inside;
		if ((p_model->GetTestPoint(fill)) && (p_model->GetTestPointsAttributes(proj_pnt, dist, is_inside)))
		{
			caption += _T(", distance from test point to polygon =");
			CString str_1;
			str_1.Format(_T("%f"), dist);
			caption += str_1;
		}

		
	}
	GetParentFrame()->SetWindowTextW(caption);
}

void CChildView::OnOptionsClearall()
{
	p_model->SetCurrentState(BUILDING_POLYGON);
	p_model->ClearAll();
	SyncMenuStateWithModelState();
}
