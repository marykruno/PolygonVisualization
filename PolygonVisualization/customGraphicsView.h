#pragma once
#include <QGraphicsView>
#include <QMouseEvent>
#include "PolygonModel.h"
#include "PolygonRenderer.h"

class customGraphicsView : public QGraphicsView, public IViewObserver, public IRenderStyleObserver
{
    Q_OBJECT
public:
    explicit customGraphicsView(QWidget *parent = 0);
    ~customGraphicsView();
    virtual void onModelChanged(const PolygonModel* p_model) override;
    virtual void onRenderStyleChanged(const PolygonRenderer* p_renderer) override;

signals:
    void updateStatus(QString& message);
public slots:
    void on_optionsPolygonModeChanged();
    void on_optionsClearAll();
protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;
private:
    PolygonModel* p_model;
    PolygonRenderer renderer;
    QGraphicsScene* scene;
private:
    void immediateViewUpdate();
    void syncMenuStateWithModelState();
};
