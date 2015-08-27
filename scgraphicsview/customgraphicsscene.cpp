#include "customgraphicsscene.h"
#include <QDebug>
#include <QPainter>

#define GRID_STEP 15
#define LINE_WIDTH 1.61803398875

CustomGraphicsScene::CustomGraphicsScene(QObject* parent) :
    QGraphicsScene(parent)
{
    _pen.setColor(QColor(0,0,0,35));
    _pen.setWidthF(LINE_WIDTH);
    _enabled = false;
}

CustomGraphicsScene::~CustomGraphicsScene()
{

}

void CustomGraphicsScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    if(_enabled)
    {
        //    QRectF sRect = this->sceneRect();
        //    int gridSize = (int)ceil(GRID_STEP *_scale);

            int gridSize = GRID_STEP;

            qDebug() << "gridSize: " << gridSize;
        //    int gridSize = (int)ceil(rect.width()/_scale/(0.0+GRID_STEP));

            qreal left = int(rect.left()) - (int(rect.left()) % gridSize);
            qreal top = int(rect.top()) - (int(rect.top()) % gridSize);

            QVarLengthArray<QLineF, 100> lines;
        //    QVarLengthArray<QLine, 100> lines;

        //    qDebug () << "left: " << left <<"\tright: " << rect.right() << "\ttop: "<<rect.top()<< "\tbottom: "<< rect.bottom();
            for (qreal x = left; x < rect.width() + rect.left(); x += gridSize){
        //        qDebug() << "appending line at: "<< x;
                lines.append(QLineF(x, rect.top(), x, rect.bottom()));
        }
            qDebug() << "top: " << top<< "\tbottom: " << rect.bottom();
            for (qreal y = top; y < rect.bottom(); y += gridSize)
                lines.append(QLineF(rect.left(), y, rect.right(), y));

            qDebug() << "Number of lines: " << lines.size();
            qDebug() << "rect:  " << rect;
            painter->setPen(_pen);
            painter->setRenderHint(QPainter::Antialiasing, true);
            painter->setRenderHint(QPainter::SmoothPixmapTransform, true);
            painter->drawLines(lines.data(), lines.size());
    }
    else
    {
        // do nothing
    }
}

void CustomGraphicsScene::handleGridToggled(bool en)
{
    _enabled = en;
    this->update();
}
