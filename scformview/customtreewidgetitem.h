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
#ifndef CUSTOMTREEWIDGETITEM_H
#define CUSTOMTREEWIDGETITEM_H
#include "scformview_global.h"
#include <QTreeWidgetItem>
#include "textblock.h"
#include <QObject>



/** \class CustomTreeWidgetItem
  *
  * \brief This tree-view diplays the state machine.
  * \ingroup FormView

  * It is essentially a QTreeWidgetItem, but it also holds a pointer to the state or transition item (from the data-model)
  * for each node in the tree.
  *
  */

class SCFORMVIEWSHARED_EXPORT CustomTreeWidgetItem :  public QObject, public QTreeWidgetItem
{
    Q_OBJECT

public:
    CustomTreeWidgetItem(CustomTreeWidgetItem * parent=NULL);
    void setData(QObject* d);
    QObject * data();
public slots:
    void treeAddTextBlock(SCTextBlock*);
    void handleAttributeChanged(StateName*);
    void handleAttributeChanged(TransitionStringAttribute*);

private:
    QObject * _stateOrTransitionItem;


};

#endif // CUSTOMTREEWIDGETITEM_H
