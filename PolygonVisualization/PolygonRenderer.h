#if !defined _POYGONRENDERER_H
#define _POYGONRENDERER_H

class PolygonModel;
class PolygonRenderer;

class IRenderStyleObserver
{
public:
	IRenderStyleObserver(){};
	virtual ~IRenderStyleObserver(){};
	virtual void OnRenderStyleChanged(const PolygonRenderer* p_renderer) = 0;
};

class PolygonRenderer
{
public:
	PolygonRenderer();
	virtual ~PolygonRenderer();
	void Draw(CDC& dc, const CRect& cli_rect, const PolygonModel& model);
	void SetObserver(IRenderStyleObserver* p_observer_)
	{
		p_observer = p_observer_;
	}


private:
	IRenderStyleObserver* p_observer;
	COLORREF polygon_color, points_color, bcg_color;
private:
	void DrawPolygon(CDC& dc, const PolygonModel& model);
	void DrawPoints(CDC& dc, const PolygonModel& model);
	void DrawTestPoint(CDC& dc, const PolygonModel& model);
	void DrawBackground(CDC& dc, const CRect& cli_rect);
};

#endif