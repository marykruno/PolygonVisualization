#include "stdafx.h"
#include "PolygonRenderer.h"
#include "PolygonModel.h"


PolygonRenderer::PolygonRenderer()
{
	polygon_color = RGB(136, 136, 136);
	points_color = RGB(0, 0, 0);
	bcg_color = RGB(255, 255, 255);
	p_observer = NULL;
}
PolygonRenderer::~PolygonRenderer()
{

}

void PolygonRenderer::DrawPolygon(CDC& dc, const PolygonModel& model)
{
	if (model.GetNumPoints() < 3)
		return;
	PolygonModel::PointsIterator iter = model.GetPointsIterator();

	CRgn polygon;
	CBrush polygon_brush(polygon_color);
	CPoint* array = new CPoint[model.GetNumPoints()];
	int i = 0;
	while (iter.HasMoreElements())
	{
		Point2D cur_pnt = iter.GetCurValue();
		array[i].x = cur_pnt.x;
		array[i].y = cur_pnt.y;

		++i;
		iter.MoveNext();
	}
	polygon.CreatePolygonRgn(&array[0], model.GetNumPoints(), ALTERNATE);
	dc.FillRgn(&polygon, &polygon_brush);
	delete[] array;
}
void PolygonRenderer::DrawPoints(CDC& dc, const PolygonModel& model)
{
	if (model.GetNumPoints() == 0)
		return;
	CRgn marker;
	marker.CreateEllipticRgn(-6, -6, 6, 6);
	CBrush points_brush(points_color);


	CPen pn(PS_SOLID, 2, polygon_color);
	CPen* oldPen = dc.SelectObject(&pn);

	PolygonModel::PointsIterator iter = model.GetPointsIterator();
	Point2D prev_pnt = iter.GetCurValue();
	Point2D first_pnt = prev_pnt;
	marker.OffsetRgn(prev_pnt.x, prev_pnt.y);
	dc.FillRgn(&marker, &points_brush);
	dc.MoveTo(prev_pnt.x, prev_pnt.y);
	iter.MoveNext();
	while (iter.HasMoreElements())
	{
		Point2D cur_pnt = iter.GetCurValue();
		marker.OffsetRgn(cur_pnt.x - prev_pnt.x, cur_pnt.y - prev_pnt.y);
		dc.FillRgn(&marker, &points_brush);
		dc.LineTo(cur_pnt.x, cur_pnt.y);
		prev_pnt = cur_pnt;
		iter.MoveNext();
	}
	if (model.GetCurrentState() == POLYGON_COMPLETED)
	{
		dc.LineTo(first_pnt.x, first_pnt.y);
	}
	dc.SelectObject(oldPen);
}

void PolygonRenderer::DrawBackground(CDC& dc, const CRect& cli_rect)
{
	CBrush bcg_brush(bcg_color);
	CBrush* oldBrush = dc.SelectObject(&bcg_brush);
	//stuff
	dc.FillRect(&cli_rect, &bcg_brush);

	dc.SelectObject(oldBrush);
}

void PolygonRenderer::DrawTestPoint(CDC& dc, const PolygonModel& model)
{
	Point2D fill;

	if (model.GetTestPoint(fill))
	{

		bool is_inside;
		double dist;
		Point2D proj_pnt;
		CPen pn(PS_SOLID, 2, RGB(255, 0, 0));
		CPen* oldPen = dc.SelectObject(&pn);
		if (model.GetTestPointsAttributes(proj_pnt, dist, is_inside))
		{
			if (is_inside)
			{
				CRgn marker;
				marker.CreateEllipticRgn(-6, -6, 6, 6);
				CBrush points_brush(RGB(255, 0, 0));
				marker.OffsetRgn(fill.x, fill.y);
				dc.FillRgn(&marker, &points_brush);
			}
			else
			{				
				dc.Ellipse(fill.x - 6, fill.y - 6, fill.x + 6, fill.y + 6);
			}
			//draw proj_pnt

			dc.MoveTo(proj_pnt.x - 6, proj_pnt.y - 6);
			dc.LineTo(proj_pnt.x + 6, proj_pnt.y + 6);
			dc.MoveTo(proj_pnt.x + 6, proj_pnt.y - 6);
			dc.LineTo(proj_pnt.x - 6, proj_pnt.y + 6);

			//
			CPen pn_1(PS_DASH, 1, RGB(0, 0, 255));
			CPen* oldPen_1 = dc.SelectObject(&pn_1);
			dc.MoveTo(fill.x, fill.y);
			dc.LineTo(proj_pnt.x, proj_pnt.y);
			dc.SelectObject(oldPen_1);
			
		}
		dc.SelectObject(oldPen);
	
	}
}

void PolygonRenderer::Draw(CDC& dc, const CRect& cli_rect, const PolygonModel& model)
{
	DrawBackground(dc, cli_rect);
	if (model.GetCurrentState() == POLYGON_COMPLETED)
	{
		DrawPolygon(dc, model);
		DrawTestPoint(dc, model);
	}
	DrawPoints(dc, model);

}