#include "customGraphicsView.h"

customGraphicsView::customGraphicsView(QWidget *parent) :
    QGraphicsView(parent) {
    scene = new QGraphicsScene();
    scene->setSceneRect(0, 0, 1024, 768);
    setScene(scene);
    scene->setParent(this);
    p_model = new PolygonModel();
    p_model->setObserver(this);
    renderer.setObserver(this);

    syncMenuStateWithModelState();
}

customGraphicsView::~customGraphicsView() {
    delete p_model;
}


void customGraphicsView::onModelChanged(const PolygonModel* p_model_) {
    if (p_model_ == p_model) {
        immediateViewUpdate();
    }
}

void customGraphicsView::onRenderStyleChanged(const PolygonRenderer* p_renderer_) {
    if (p_renderer_ == &renderer) {
        immediateViewUpdate();
    }
}

void customGraphicsView::paintEvent(QPaintEvent *event) {
    syncMenuStateWithModelState();
    renderer.draw(*scene, *p_model);
    QGraphicsView::paintEvent(event);
}

void customGraphicsView::mouseDoubleClickEvent(QMouseEvent *event) {
    syncMenuStateWithModelState();
    QPointF pt = mapToScene(event->pos());
    Point2D point2d = Point2D(pt.x(), pt.y());

    if (p_model->getCurrentState() == ePolygonState::BUILDING_POLYGON)
        p_model->addPoint(point2d);
    else {
        p_model->setTestPoint(point2d);
    }
    QGraphicsView::mouseDoubleClickEvent(event);
}

void customGraphicsView::syncMenuStateWithModelState() {
    ePolygonState state = p_model->getCurrentState();
    QString status;
    if (state == ePolygonState::BUILDING_POLYGON)
    {
        status = "PolygonVisualization: building polygon mode is active";
    }
    else
    {
        status = "PolygonVisualization: polygon is completed, oriented area of polygon = ";
        QString str;
        str.setNum(p_model->getOrientedArea(), 'f', 3);
        status += str;
        double dist;
        Point2D fill, proj_pnt;
        bool is_inside;
        if ((p_model->getTestPoint(fill)) && (p_model->getTestPointsAttributes(proj_pnt, dist, is_inside)))
        {
            status += ", distance from test point to polygon = ";
            QString str1;
            str1.setNum(dist, 'f', 3);
            status += str1;
        }
    }
    emit updateStatus(status);
}

void customGraphicsView::on_optionsPolygonModeChanged() {
    ePolygonState state = p_model->getCurrentState();
    // try to invert state
    if (state == ePolygonState::BUILDING_POLYGON)
        state = ePolygonState::POLYGON_COMPLETED;
    else
        state = ePolygonState::BUILDING_POLYGON;

    p_model->setCurrentState(state);
    syncMenuStateWithModelState();
}

void customGraphicsView::on_optionsClearAll() {
    p_model->setCurrentState(ePolygonState::BUILDING_POLYGON);
    p_model->clearAll();
    syncMenuStateWithModelState();
}

void customGraphicsView::immediateViewUpdate() {
    scene->clear();
}
