#include "stdafx.h"
#include "PolygonModel.h"
#include "SQDist.h"
#include <math.h>
#include "IsPointInsidePolygon.h"

double CalculateArea( const vector<Point2D>& pts)
{
	double ret = 0;
	if (pts.size() < 3)
		return ret;

	for (size_t i = 0; i < pts.size() - 1; ++i)
	{
		ret += std::abs((pts[i].x*pts[i + 1].y - pts[i].y*pts[i + 1].x));
	}
	ret += std::abs((pts[pts.size() - 1].x*pts[0].y - pts[pts.size() - 1].y*pts[0].x));
	ret = ret / 2.0;
	return ret;
}

PolygonModel::PolygonModel()
{
	my_observer = NULL;
	state = BUILDING_POLYGON;
}

PolygonModel::PointsIterator PolygonModel::GetPointsIterator() const
{
	return PolygonModel::PointsIterator(pts);
}

double PolygonModel::GetOrientedArea() const
{
	return CalculateArea(pts);
}

bool PolygonModel::GetTestPointsAttributes(Point2D& proj_pnt, double& dist, bool& is_inside) const
{
	if (pts.size() == 0)
		return false;

		double temp = SQDistFromPointToEdge<Point2D>(tst_info.test_point, pts[pts.size()-1], pts[0], proj_pnt);
		for (size_t i = 0; i < pts.size() - 1; ++i)
		{
			Point2D temp_proj_pnt;
			double temp_temp = SQDistFromPointToEdge<Point2D>(tst_info.test_point, pts[i], pts[i + 1], temp_proj_pnt);
			if (SQDistFromPointToEdge<Point2D>(tst_info.test_point, pts[i], pts[i + 1], temp_proj_pnt) < temp)
			{
				temp = temp_temp;
				proj_pnt = temp_proj_pnt;
			}
		}
		dist = temp;
		is_inside = IsPointInPolygon<Point2D, Traits<Point2D>>(pts, tst_info.test_point);
		return true;

}