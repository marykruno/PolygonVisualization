#include "PolygonRenderer.h"
#include "PolygonModel.h"
#include <QGraphicsEllipseItem>


PolygonRenderer::PolygonRenderer()
{
    polygon_color = QColor(136, 136, 136);
    points_color = QColor(0, 0, 0);
    bcg_color = QColor(255, 255, 255);
    p_observer = nullptr;
}
PolygonRenderer::~PolygonRenderer()
{

}

void PolygonRenderer::drawPolygon(QGraphicsScene& scene, const PolygonModel& model)
{
	if (model.getNumPoints() < 3)
		return;
	PolygonModel::PointsIterator iter = model.getPointsIterator();
    QPolygonF polygon;
    polygon.reserve(model.getNumPoints());
    int i = 0;
	while (iter.hasMoreElements())
	{
		Point2D cur_pnt = iter.getCurValue();
        polygon.append(QPointF(cur_pnt.x, cur_pnt.y));

		++i;
		iter.moveNext();
	}
    QBrush polygon_brush(polygon_color);
    scene.addPolygon(polygon, QPen(Qt::PenStyle::SolidLine), polygon_brush);
}
void PolygonRenderer::drawPoints(QGraphicsScene& scene, const PolygonModel& model)
{
	if (model.getNumPoints() == 0)
		return;
    QBrush points_brush(points_color, Qt::SolidPattern);

    QPen points_pen(points_brush, 2, Qt::PenStyle::SolidLine);

	PolygonModel::PointsIterator iter = model.getPointsIterator();
	Point2D prev_pnt = iter.getCurValue();

    scene.addEllipse(prev_pnt.x-points_rad, prev_pnt.y-points_rad, points_rad*2.0, points_rad*2.0, points_pen,
              points_brush);

    Point2D first_pnt = prev_pnt;

    iter.moveNext();
	while (iter.hasMoreElements())
	{
		Point2D cur_pnt = iter.getCurValue();

        scene.addEllipse(cur_pnt.x-points_rad, cur_pnt.y-points_rad, points_rad*2.0, points_rad*2.0, points_pen,
                         points_brush);

        scene.addLine(prev_pnt.x,
                    prev_pnt.y,
                    cur_pnt.x,
                    cur_pnt.y,
                    points_pen);

		prev_pnt = cur_pnt;
		iter.moveNext();
	}
    if (model.getCurrentState() == ePolygonState::POLYGON_COMPLETED)
	{
        scene.addLine(prev_pnt.x,
                    prev_pnt.y,
                    first_pnt.x,
                    first_pnt.y,
                    points_pen);
	}
}

void PolygonRenderer::drawBackground(QGraphicsScene& scene)
{
    QBrush bcg_brush(bcg_color);
    scene.setBackgroundBrush(bcg_brush);
}

void PolygonRenderer::drawTestPoint(QGraphicsScene& scene, const PolygonModel& model)
{
	Point2D fill;

	if (model.getTestPoint(fill))
	{

		bool is_inside;
		double dist;
		Point2D proj_pnt;

        if (model.getTestPointsAttributes(proj_pnt, dist, is_inside))
		{
            QColor projection_point_color = is_inside ? QColor(255, 0, 0) : QColor(255, 255, 255);
            QBrush brush(projection_point_color, Qt::SolidPattern);
            QPen pen(QColor(255, 0, 0), 2, Qt::PenStyle::SolidLine);

              scene.addEllipse(fill.x - points_rad, fill.y - points_rad, points_rad*2.0, points_rad*2.0, pen, brush);

              //draw proj_pnt mark
              scene.addLine(proj_pnt.x - points_rad, proj_pnt.y - points_rad,
                        proj_pnt.x + points_rad, proj_pnt.y + points_rad,
                        pen);
              scene.addLine(proj_pnt.x + points_rad, proj_pnt.y - points_rad,
                        proj_pnt.x - points_rad, proj_pnt.y + points_rad,
                        pen);

            QBrush projection_brush(QColor(0, 0, 255), Qt::SolidPattern);
            QPen projection_pen(projection_brush, 1, Qt::PenStyle::DashLine);
            //draw projection line
            scene.addLine(fill.x, fill.y,
                        proj_pnt.x, proj_pnt.y,
                        projection_pen);

		}
	}
}

void PolygonRenderer::draw(QGraphicsScene& scene, const PolygonModel& model)
{
    drawBackground(scene);
    if (model.getCurrentState() == ePolygonState::POLYGON_COMPLETED)
	{
        drawPolygon(scene, model);
        drawTestPoint(scene, model);
	}
    drawPoints(scene, model);

}
