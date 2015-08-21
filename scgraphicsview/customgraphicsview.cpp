/**
  this is a wrapper class to gain control over the mouse events of the QGraphicsView class
  */



#include "customgraphicsview.h"

CustomGraphicsView::CustomGraphicsView(QWidget *parent):
    QGraphicsView(parent)
{
    setDragMode(QGraphicsView::ScrollHandDrag);
}

CustomGraphicsView::CustomGraphicsView(QGraphicsScene *scene, QWidget *parent):
    QGraphicsView(scene,parent)
{
    setDragMode(QGraphicsView::ScrollHandDrag);
}

CustomGraphicsView::~CustomGraphicsView()
{

}


void CustomGraphicsView::enterEvent(QEvent *event)
{
    QGraphicsView::enterEvent(event);
    viewport()->setCursor(Qt::ArrowCursor);
}

void CustomGraphicsView::mousePressEvent(QMouseEvent *event)
{
    QGraphicsView::mousePressEvent(event);
    viewport()->setCursor(Qt::ClosedHandCursor);
}

void CustomGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent(event);
    viewport()->setCursor(Qt::ArrowCursor);
}
