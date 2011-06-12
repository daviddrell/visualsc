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


#include <selectableboxgraphic.h>

class SCState;
class IAttribute;



/**
  * \class StateBoxGraphic
  * This is short example/demonstration of creating a custom Qt QGraphicsItem.
  *  Example usage of a custom QGraphicsItem in a QGraphicsScene with examples of mouse hover events.
  *
  *  My goal was to create a simpe box, in the shape of a UML State/Class box, with a title
  *  area seprated from a main arear below by line.
  *
  *  This sample class inherits from QGraphicsItem, and must reimplement boundingRect() and paint()
  *  from the base class.
  *
  *  To learn about handling mouse events in a QGraphicsScene, I wanted my box to turn red when the mouse moves inside, and black
  *  when the mouse moves out.
  */

class StateBoxGraphic :  public SelectableBoxGraphic
{
    Q_OBJECT

public:
    StateBoxGraphic(QGraphicsObject*parent, SCState * state);
    ~StateBoxGraphic();

    QGraphicsTextItem _title;    ///< sample text to go in the title area.

    void setGridSpace(int space);
    void setTitle(QString t);
    void setSize(QPoint size);
    QString getPositionAsString();
    QString getSizeAsString();
    void    getSize(QPoint& size);
   void setHighlighted(bool);

private:


    virtual void graphicHasChanged (); ///< implemented to receive updates from the SelectableBoxGraphic


    // private slots

private slots:


    void handleAttributeChanged(IAttribute *attr);



private:
    //private data

    SCState * _stateModel;
};

#endif // STATEBOX_H
