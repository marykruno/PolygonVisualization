#if !defined _ISPOINTINSIDEPOLYGON_H
#define _ISPOINTINSIDEPOLYGON_H

using namespace std;

template <class T> class Traits
{
public:
	static bool IsYCoordinatesEqual(const T& a, const T& b)
	{
		return a.y == b.y;
	}
};

template <class T, class Traits> static bool DoesRayIntersectEdge(const T& a, const T& b, const T& pt)
{
	if (Traits::IsYCoordinatesEqual(a, b) || ((a.y - pt.y)*(b.y - pt.y) > 0) || (b.x >= pt.x && Traits::IsYCoordinatesEqual(b, pt)))
		return false;
	else
	{
		auto temp = (b.y - a.y);
		if ((temp > 0 && a.x*temp + (b.x - a.x)*(pt.y - a.y) >= temp*pt.x) || (temp < 0 && a.x*temp + (b.x - a.x)*(pt.y - a.y) <= temp*pt.x))
			return true;
		else
			return false;
	}

};
template <class T, class Tr = Traits<T> > bool IsPointInPolygon(const vector<T>& ar_pts, const T& pt)
{
	int count = 0;
	for (size_t i = 0; i < ar_pts.size() - 1; ++i)
	{
		if (DoesRayIntersectEdge<T, Tr>(ar_pts[i], ar_pts[i + 1], pt))
			++count;

	}

	if (DoesRayIntersectEdge<T, Tr>(ar_pts[ar_pts.size() - 1], ar_pts[0], pt))
		++count;
	return (count % 2) == 1;

};




#endif