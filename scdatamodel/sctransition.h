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

#ifndef SCTRANSISTION_H
#define SCTRANSISTION_H

#include "transitionattributes.h"
#include <QObject>
#include <QList>
#include <QPointF>
class SCState;

#include "scdatamodel_global.h"
class QXmlStreamWriter;


class SCDATAMODELSHARED_EXPORT  SCTransition : public QObject
{
    Q_OBJECT

public:
    SCTransition(QObject * parent);

    ~SCTransition();

    void    setAttributeValue(QString key, QString value);
    QString getAttributeValue(QString key);
    void    writeSCVXML(QXmlStreamWriter & sw);

     //public data members

    TransitionAttributes attributes;

signals:
    void selected();
    void unselected();

private slots:
    void handleLineSelected();
    void handleLineUnSelected();

private:

    SCState *       _targetState;

};


#endif // SCTRANSISTION_H
