#pragma once
#include <vector>

using namespace std;

class PolygonModel;
class PointIterator;


struct Point2D
{
    int x, y;
	Point2D() : x(0), y(0) {}
    Point2D(int x_, int y_) : x(x_), y(y_) {}
	Point2D& operator+=(const Point2D& other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	Point2D& operator-=(const Point2D& other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}
};

Point2D operator+(Point2D first, const Point2D& second);

Point2D operator-(Point2D first, const Point2D& second);



enum class ePolygonState {
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
	virtual void onModelChanged(const PolygonModel* p_model) = 0;
};

class PolygonModel
{
public:

	PolygonModel();
	~PolygonModel(){};
	void setObserver(IViewObserver* observer)
	{
		my_observer = observer;
	}
    ePolygonState getCurrentState() const
	{
		return state;
	}
    void setCurrentState(ePolygonState state_)
	{
        if (state_ == ePolygonState::POLYGON_COMPLETED && (pts.size() < 3))
			return;
		if (state != state_)
		{
			state = state_;
			notifyObserver();
		}
	}
	
	size_t getNumPoints() const
	{
		return pts.size();
	}

	void addPoint(const Point2D pnt)
	{
		
		pts.push_back(pnt);
		notifyObserver();
			
	}

	bool getTestPoint(Point2D& fill) const
	{
		fill = tst_info.test_point;
		return tst_info.is_initialized;
	}

	void setTestPoint(const Point2D pnt)
	{
        if (state == ePolygonState::POLYGON_COMPLETED)
		{
			tst_info.test_point = pnt;
			tst_info.is_initialized = true;
			notifyObserver();
		}
		
	}
	void clearAll()
	{
		pts.clear();
		tst_info.is_initialized = false;
		notifyObserver();
	}
	bool getTestPointsAttributes(Point2D& proj_pnt, double& dist, bool& is_inside) const;
public:
	class PointsIterator
	{
	private:
		const vector<Point2D>& ar_pts;
		size_t ind;
	public:
		PointsIterator(const vector<Point2D>& ar_pts_) : ar_pts(ar_pts_), ind(0) {}
	    ~PointsIterator(){}
		Point2D getCurValue() const
		{
			Point2D ret;
			if (ind < ar_pts.size())
				ret = ar_pts[ind];

			return ret;
		}
		bool hasMoreElements() const
		{
			return ind < ar_pts.size();
		}
		void moveNext()
		{
			if (ind < ar_pts.size())
				++ind;
		}

	};
	PointsIterator getPointsIterator() const;
	double getOrientedArea() const;
private:
    ePolygonState state;
	vector<Point2D> pts;
	TestPointInfo tst_info;
	IViewObserver* my_observer;
private:
	void notifyObserver() const
    {
        if (my_observer != nullptr)
			my_observer->onModelChanged(this);
	}

};

