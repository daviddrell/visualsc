#ifndef CUSTOMGRAPHICSSCENE_H
#define CUSTOMGRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QPen>

class CustomGraphicsScene : public QGraphicsScene
{

    Q_OBJECT

public:
    CustomGraphicsScene(QObject *parent);
    ~CustomGraphicsScene();

    bool gridEnabled();
    void setGridEnabled(bool enable);

public slots:
//    void handleScaleChanged(qreal);
    void handleGridToggled(bool);

protected:

    QPen _pen;
    void drawBackground(QPainter *painter, const QRectF &rect);
    bool _gridEnable;

};

#endif // CUSTOMGRAPHICSSCENE_H
