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
#ifndef SCSTATE_H
#define SCSTATE_H
#include <QMetaType>
#include <QObject>
#include <QList>
#include "stateattributes.h"
#include "sctransition.h"
class QString;
class QXmlStreamWriter;
class QPoint;
#include "textblock.h"
#include "scdatamodel_global.h"
#include "scitem.h"

/**

  \class   SCState
  \ingroup DataModel
  \brief   The class represents a state in a state-chart Data-Model

  The state contains children elements which are sub-states (also instances of SCState) and
  state-transitions (SCTransition class). The state also contains a public attribute container as
  well as convenience methods to accessing the attributes. Since te attribute container is public,
  attributes can be accessed directly.

  The children states and transitions are contained as QObject children in the base QObject class
  (see addState or addTransition methods).

  */


class SCDATAMODELSHARED_EXPORT  SCState : public SCItem
{
    Q_OBJECT
public:
    SCState(QObject *parent);
    SCState(const SCState& st);
    SCState(bool topState=false);
    ~SCState();

    virtual IAttributeContainer * getAttributes(); // reimplemented from SCItem base

    enum TransitionTransitDirection { kTransitIn, kTransitOut, kDestination };
    void addTransitionReference(SCTransition*, TransitionTransitDirection );

    void setStateName(QString n);
    void addTransistion(SCTransition*);
    void addState(SCState *);
   // void makeTargetConnections(QList<SCTransition*> & transitionList);

    void setAttributeValue(QString key, QString value);
    void addAttribute(QString key, QString value);
    QString getAttributeValue(QString key);

    int  getStateCount();

    void setSize (QPointF& size);
    void setSize (QPoint& size);
    void setPosition (QPointF& size);

    /**
      * \fn getAllTransitions
      * \brief Get all transitions including decendant state's transitions
      *
      */
     void getAllTransitions( QList<SCTransition*> & list);

     /**
       * \fn getTransitions
       * \brief Get transitions of this state only
       *
       */

     void getTransitions(QList<SCTransition*> & transitionList);

    /**
      * \fn getStates
      * \brief Get direct children states, no decendants
      *
      */
    void getStates(QList<SCState *> & stateList);

    /**
      * \fn getAllStates
      * \brief Get all states, including decendants
      *
      */
    void getAllStates(QList<SCState *> & stateList);

    /**
      * \fn getParentState
      * \brief Returns the QObject parent, cast as a state, or NULL if parent is not a state or no parent
      *
      */
    SCState * getParentState();



    /**
      * \fn getLevel
      * \brief Top state machine is level 0. Each sub-state increments level.
      *
      */
    int getLevel();

    /**
      * \fn setLevel
      * \brief Top state machine is level 0. Each sub-state increments level.
      *  The level should only be set by the state chart constructor.
      */

    void setLevel(int level);



    /**
      * \fn getStateByName
      * \brief find descendant state by name
      *
      */
    SCState* getStateByName(QString name);

    /**
      * \fn writeSCVXML
      * \brief Write out the SCXML for the entire tree from this point downward
      * \param xmlWriter is the writer that writes the xml
      *
      */
    void writeSCVXML(QXmlStreamWriter & xmlWriter);

    /**
      * \fn getIDTextBlock
      * \brief Returns a pointer to the ID TextBlock
      * \param xmlWriter is the writer that writes the xml
      *
      */

    SCTextBlock* getIDTextBlock();

    StateAttributes         attributes;

signals:

     void changed();



public slots:


private slots:

     void handleNameChanged(IAttribute* name);     
     void handleTextBlockChanged();


private:


    void initCommon();

    //private data

    SCTextBlock * _IdTextBlock;
    QList<SCTransition*> _transitingTransitionsOut;
    QList<SCTransition*> _transitingTransitionsIn;
    QList<SCTransition*> _transitionsTerminatingHere;


    int _level ; ///< distance from top state in terms of parent-child relationships

};

Q_DECLARE_METATYPE (SCState*)


#endif // SCSTATE_H
