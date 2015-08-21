#ifndef CUSTOMGRAPHICSVIEW_H
#define CUSTOMGRAPHICSVIEW_H

#include <QGraphicsView>

class CustomGraphicsView : public QGraphicsView
{
public:
    CustomGraphicsView(QWidget *parent = 0);
    CustomGraphicsView(QGraphicsScene *scene, QWidget *parent = 0);
    ~CustomGraphicsView();

protected:
    void enterEvent(QEvent *event);

    void mousePressEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *event);
};

#endif // CUSTOMGRAPHICSVIEW_H
