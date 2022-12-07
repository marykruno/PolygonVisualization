#include "PolygonModel.h"
#include "SQDist.h"
#include <math.h>
#include "IsPointInsidePolygon.h"

Point2D operator+(Point2D first, const Point2D& second) {
    first += second;
    return first;
}
Point2D operator-(Point2D first, const Point2D& second) {
    first -= second;
    return first;
}

double CalculateArea(const vector<Point2D>& pts)
{
    double ret = 0.0;
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
    my_observer = nullptr;
    state = ePolygonState::BUILDING_POLYGON;
}

PolygonModel::PointsIterator PolygonModel::getPointsIterator() const
{
	return PolygonModel::PointsIterator(pts);
}

double PolygonModel::getOrientedArea() const
{
	return CalculateArea(pts);
}

bool PolygonModel::getTestPointsAttributes(Point2D& proj_pnt, double& dist, bool& is_inside) const
{
	if (pts.size() == 0)
		return false;
    
    double min_dist = SQDistFromPointToEdge<Point2D, Accessor<Point2D> >(tst_info.test_point, pts[pts.size()-1], pts[0], proj_pnt);
    
    for (size_t i = 0; i < pts.size() - 1; ++i)
    {
        Point2D cur_proj_pnt;
        double cur_dist = SQDistFromPointToEdge<Point2D, Accessor<Point2D>>(tst_info.test_point, pts[i], pts[i + 1], cur_proj_pnt);
        if (cur_dist < min_dist)
        {
            min_dist = cur_dist;
            proj_pnt = cur_proj_pnt;
        }
    }
    dist = min_dist;
    is_inside = isPointInPolygon<Point2D, Traits<Point2D>>(pts, tst_info.test_point);
    return true;

}
