#if !defined _POLYGONMODEL_H
#define _POLYGONMODEL_H
#include <vector>

using namespace std;

class PolygonModel;
class PointIterator;
struct Point2D
{
	int x, y;
	Point2D() : x(0), y(0) {}
	Point2D(int x_, int y_) : x(x_), y(y_) {}
};


enum State {
	BUILDING_POLYGON = 0,
	POLYGON_COMPLETED = 1
};

struct TestPointInfo
{
	Point2D test_point;
	bool is_initialized;
	TestPointInfo() : is_initialized(false){}
};

class IViewObserver
{
public:
	IViewObserver() {};
	virtual ~IViewObserver() {};
	virtual void OnModelChanged(const PolygonModel* p_model) = 0;
};

class PolygonModel
{
public:

	PolygonModel();
	~PolygonModel(){};
	void SetObserver(IViewObserver* observer)
	{
		my_observer = observer;
	}
	State GetCurrentState() const
	{
		return state;
	}
	void SetCurrentState(State state_)
	{
		if (state_ == POLYGON_COMPLETED && (pts.size() < 3))
			return;
		if (state != state_)
		{
			state = state_;
			NotifyObserver();
		}
	}
	
	size_t GetNumPoints() const
	{
		return pts.size();
	}

	void AddPoint(const Point2D pnt)
	{
		
		pts.push_back(pnt);
		NotifyObserver();
			
	}

	bool GetTestPoint(Point2D& fill) const
	{
		fill = tst_info.test_point;
		return tst_info.is_initialized;
	}

	void SetTestPoint(const Point2D pnt)
	{
		if (state == POLYGON_COMPLETED)
		{
			tst_info.test_point = pnt;
			tst_info.is_initialized = true;
			NotifyObserver();
		}
		
	}
	void ClearAll()
	{
		pts.clear();
		tst_info.is_initialized = false;
		NotifyObserver();
	}
	bool GetTestPointsAttributes(Point2D& proj_pnt, double& dist, bool& is_inside) const;
public:
	class PointsIterator
	{
	private:
		const vector<Point2D>& ar_pts;
		size_t ind;
	public:
		PointsIterator(const vector<Point2D>& ar_pts_) : ar_pts(ar_pts_), ind(0) {}
	    ~PointsIterator(){}
		Point2D GetCurValue() const
		{
			Point2D ret;
			if (ind < ar_pts.size())
				ret = ar_pts[ind];

			return ret;
		}
		bool HasMoreElements() const
		{
			return ind < ar_pts.size();
		}
		void MoveNext()
		{
			if (ind < ar_pts.size())
				++ind;
		}

	};
	PointsIterator GetPointsIterator() const;
	double GetOrientedArea() const;
private:
	State state;
	vector<Point2D> pts;
	TestPointInfo tst_info;
	IViewObserver* my_observer;
private:
	void NotifyObserver() const
	{
		if (my_observer != NULL)
			my_observer->OnModelChanged(this);
	}

};


#endif