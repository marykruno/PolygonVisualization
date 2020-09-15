
// PolygonVisualization.h : main header file for the PolygonVisualization application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CPolygonVisualizationApp:
// See PolygonVisualization.cpp for the implementation of this class
//

class CPolygonVisualizationApp : public CWinApp
{
public:
	CPolygonVisualizationApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CPolygonVisualizationApp theApp;
