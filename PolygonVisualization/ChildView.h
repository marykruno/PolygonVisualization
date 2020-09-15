

#pragma once

#include "PolygonModel.h"
#include "PolygonRenderer.h"
#include "SceneInMemory.h"


// CChildView window

class CChildView : public CWnd, public IViewObserver, public IRenderStyleObserver
{
	// Construction
public:
	CChildView();
	virtual ~CChildView();
	virtual void OnModelChanged(const PolygonModel* p_model);
	virtual void OnRenderStyleChanged(const PolygonRenderer* p_renderer);


	// Attributes
public:

	// Operations
public:

	// Overrides
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	// Implementation

	// Generated message map functions
protected:
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT, int, int);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnOptionsBuildingPolygonMode();
	afx_msg void OnOptionsClearall();
	DECLARE_MESSAGE_MAP()

private:
	PolygonModel* p_model;
	PolygonRenderer renderer;
	SceneInMemory mem_scn;
private:
	void ImmediateViewUpdate();
	void SyncMenuStateWithModelState();

};

