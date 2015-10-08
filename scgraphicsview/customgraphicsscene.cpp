#include "customgraphicsscene.h"
#include <QDebug>
#include <QPainter>

#define GRID_STEP 25
#define LINE_WIDTH  1.61803398875

CustomGraphicsScene::CustomGraphicsScene(QObject* parent) :
    QGraphicsScene(parent)
{
    _pen.setColor(QColor(0,0,0,35));
    _pen.setWidthF(LINE_WIDTH);
    _gridEnable = false;
}

CustomGraphicsScene::~CustomGraphicsScene()
{

}

/**
 * @brief CustomGraphicsScene::drawBackground
 * @param painter
 * @param rect
 *
 * grid background handled here
 *
 * Based on the view rectangle (a smaller rectangle of what is on screen and inside of the large scene rectangle)
 * and based on the grid size, determine the number of lines needed to fill up the current view
 *
 */
void CustomGraphicsScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    // draw the grid background
    if(_gridEnable)
    {
        // the distance between each line, horizontally or vertically, will be set by gridSize
        int gridSize = GRID_STEP;

        // determine the left and top position for our current view as a multiple of the gridSize
        qreal left = int(rect.left()) - (int(rect.left()) % gridSize);
        qreal top = int(rect.top()) - (int(rect.top()) % gridSize);

        QVarLengthArray<QLineF, 100> lines;


        for (qreal x = left; x < rect.width() + rect.left(); x += gridSize){
            lines.append(QLineF(x, rect.top(), x, rect.bottom()));
        }

        for (qreal y = top; y < rect.bottom(); y += gridSize)
            lines.append(QLineF(rect.left(), y, rect.right(), y));

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

/**
 * @brief CustomGraphicsScene::handleGridToggled
 * @param en
 *
 * connected to the QAction grid button when it is toggled
 *
 */
void CustomGraphicsScene::handleGridToggled(bool en)
{
    _gridEnable = en;
    this->update();
}

bool CustomGraphicsScene::gridEnabled()
{
    return _gridEnable;
}

void CustomGraphicsScene::setGridEnabled(bool enable)
{
    _gridEnable = enable;
}
