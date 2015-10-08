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

#include "customtreewidgetitem.h"
#include "fvitem.h"
#include <QTreeWidgetItem>

#include <QDebug>
CustomTreeWidgetItem::CustomTreeWidgetItem(CustomTreeWidgetItem * parent) :
        QTreeWidgetItem(parent)
{

}

void CustomTreeWidgetItem::setData(QObject* d)
{
    _stateOrTransitionItem = d;
}


/**
 * @brief CustomTreeWidgetItem::treeAddTextBlock
 * @param textBlock
 *
 * connected in scformview.cpp
 *
 * when a SCTransition emits insertNew
 *
 */
void CustomTreeWidgetItem::treeAddTextBlock(SCTextBlock* textBlock)
{
    CustomTreeWidgetItem * item=0;

    item = new CustomTreeWidgetItem(this);
    item->setData(textBlock);

    QString text = textBlock->getText();
    item->setText(0, text);

    item->setIcon(0,QIcon(":/SCFormView/textblock.bmp"));
}

QObject * CustomTreeWidgetItem::data()
{
    return  _stateOrTransitionItem ;
}

FVItem* CustomTreeWidgetItem::dataAsFvItem()
{
    return dynamic_cast<FVItem*>(_stateOrTransitionItem);
}

void CustomTreeWidgetItem::handleAttributeChanged(StateName* sn)
{
    qDebug() << "CustomTreeWidgetItem::handleATtribtueCHanged";
    this->setText(0, sn->asString());
}

void CustomTreeWidgetItem::handleAttributeChanged(TransitionStringAttribute* tsa)
{
    this->setText(0, tsa->asString());
}

