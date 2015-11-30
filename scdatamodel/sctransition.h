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
#include "scitem.h"
#include "textblock.h"
#include "transitionattributes.h"
#include <QUuid>


/**

  \class   SCTransition
  \ingroup DataModel
  \brief   The class represents a transition in a state-chart Data-Model

  The transition does not contain children elements. The transiton does contain a public attribute container as
  well as convenience methods to accessing the attributes. Since te attribute container is public,
  attributes can be accessed directly.

  The transition holds a reference to the target state.

  */


class SCDATAMODELSHARED_EXPORT  SCTransition : public SCItem
{
    Q_OBJECT

public:
    SCTransition(QObject * parent);

    ~SCTransition();

    SCState* parentSCState();


    bool isConnectToFinished();
    void deleteSafely();
    void setUid(QString);
    QString getUid();
    QString getUidFirstName();
    void setEventName(QString text);
    QString getEventName();
    //void setTextPos(QPointF);
 //   void setTextSize(qreal w, qreal h);
    void setText(QString eventText);

    TransitionStringAttribute* getTransStringAttr(QString key);
    TransitionPathAttribute* getPathAttr();

    virtual IAttributeContainer * getAttributes(); //reimplemented from base SCItem
    void    setAttributeValue(QString key, QString value);
    void setPathAttr(QString pathString);
    QString getAttributeValue(QString key);
    void addAttribute(QString key, QString value);
    bool removeAttribute(QString key);

    //void addTextBlock(QString key, QString value);

    TransitionAttributes attributes;

    void    writeSCVXML(QXmlStreamWriter & sw);
    SCState *targetState();


    bool doNotPrint(QString);
    int doNotPrintSize();

    //QList<SCTextBlock*> getTextBlocks();
    //SCTextBlock* getTextBlock(QString textBlockName);
    SCTextBlock* getEventTextBlock();

     //public data members


    void setTargetState(SCState*);

//    void setFont(QString);
//    void setFontSize(int);
    void setFont(QFont* font);


signals:

    void eventChangedInFormView(SCTransition*, QString);
    void eventChangedInDataModel(SCTransition*, QString);
    void clicked(SCTransition*);
    void selected();
    void unselected();
    void transitionAddTextBlock(SCTextBlock*);          // connected in scformview::loadtree for sctransitions. connected to the parentitem's treeAddTextBlock(SCTextBlock) in customtreewidgetitem
    void changedTarget(SCTransition*, SCState*);

private slots:
    //void handleLineSelected();
   //void handleLineUnSelected();

    void handleTargetStateNameChanged(IAttribute*);
    void detachFromSource(QObject*);
    void detachFromSink(QObject*);
    void handleTextBlockChanged();

private:
    QList<QString> DEFAULT_ATTRIBUTES_LIST;
    QHash<QString,int> DO_NOT_DISPLAY_HASH;
    SCState *       _targetState;

    //QList<SCTextBlock*> _textBlocks;

    SCTextBlock* _eventTextBlock;


};


#endif // SCTRANSISTION_H
