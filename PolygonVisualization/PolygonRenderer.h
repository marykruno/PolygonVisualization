#pragma once
#include <QGraphicsScene>

class PolygonModel;
class PolygonRenderer;

class IRenderStyleObserver
{
public:
	IRenderStyleObserver(){};
	virtual ~IRenderStyleObserver(){};
	virtual void onRenderStyleChanged(const PolygonRenderer* p_renderer) = 0;
};

class PolygonRenderer
{
public:
	PolygonRenderer();
	virtual ~PolygonRenderer();
    void draw(QGraphicsScene& scene, const PolygonModel& model);
	void setObserver(IRenderStyleObserver* p_observer_)
	{
		p_observer = p_observer_;
	}


private:
	IRenderStyleObserver* p_observer;
    QColor polygon_color, points_color, bcg_color;
    const double points_rad = 6.0;
private:
    void drawPolygon(QGraphicsScene& scene, const PolygonModel& model);
    void drawPoints(QGraphicsScene& scene, const PolygonModel& model);
    void drawTestPoint(QGraphicsScene& scene, const PolygonModel& model);
    void drawBackground(QGraphicsScene& scene);
};
