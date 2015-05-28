/*
    visualsc - Visual State Chart Editing for SCXML State Engines
    Copyright (C) 2011  David W. Drell (david@davidwdrell.net) and
    Contributing Authors:


    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef STATEBOX_H
#define STATEBOX_H


#include "selectableboxgraphic.h"
#include "selectabletextblock.h"

class SCState;
class IAttribute;



/**
  * \class StateBoxGraphic
    \ingroup GraphicsView
    \brief This visually prepresents the state.

  */

class StateBoxGraphic :  public SelectableBoxGraphic
{
    Q_OBJECT

public:
    StateBoxGraphic(QGraphicsObject*parent, SCState * state);
    ~StateBoxGraphic();

    SelectableTextBlock TextItem;    ///<  text to go in the title area.

    void setGridSpace(int space);
    //void setTitle(QString t);
    void setSize(QPoint size);
    QString getPositionAsString();
    QString getSizeAsString();
    QPointF getSize();
    void setHighlighted(bool);


public slots:
    void handleTransitionLineStartMoved(QPointF newPos);

private:


    virtual void graphicHasChanged (); ///< implemented to receive updates from the SelectableBoxGraphic
    virtual void paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget); ///< must be re-implemented here to pain the box on the paint-event


    // private slots

private slots:

    void handleAttributeChanged(void *attr);


private:

    //private data

    SCState * _stateModel;

    QPointF _diagLineStart;
    QPointF _diagLineEnd;
    bool    _diagLineDrawIt;
    QPointF _intersection;

};

#endif // STATEBOX_H
