#ifndef MOUSECONTROLLER_H
#define MOUSECONTROLLER_H

#include <QObject>
#include <QGraphicsSceneMouseEvent>

class MouseController : public QObject
{
    Q_OBJECT
public:
    MouseController();
    ~MouseController();
    void mouseInput(QGraphicsSceneMouseEvent* me);
    int getX();
    int getY();
    QPoint* getPoint();
    void printPos();

signals:
    void mouseMoved(QPoint);

private:
    int _xPos;
    int _yPos;
};

#endif // MOUSECONTROLLER_H
