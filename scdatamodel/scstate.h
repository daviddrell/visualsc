#ifndef SCSTATE_H
#define SCSTATE_H

#include <QObject>
#include <QList>
#include "stateattributes.h"
#include "sctransition.h"
class QString;

class QXmlStreamWriter;
class QPoint;

#include "scdatamodel_global.h"


class SCDATAMODELSHARED_EXPORT  SCState : public QObject
{
    Q_OBJECT
public:
    explicit SCState(QObject *parent = 0);
    ~SCState();

    void setStateName(QString name);
    void setSize(QPoint size);
    void setPosition(QPoint pos);
    void addTransistion(SCTransition*);
    void addState(SCState *);
    void makeTargetConnections(QList<SCTransition*> & transitionList);
    void setAttributes(StateAttributes & sa);
    bool hasBeenSized();

    /**
      * \fn getAttributes
      * \brief Get this state's attributes
      *
      */
    void getAttributes(StateAttributes& attr);

    /**
      * \fn getAllTransitions
      * \brief Get all transitions including decendant state's transitions
      *
      */
     void getAllTransitions( QList<SCTransition*> & list);

    /**
      * \fn getStates
      * \brief Get direct children states, no decendants
      *
      */
    void getStates(QList<SCState *> & stateList);


    /**
      * \fn writeSCVXML
      * \brief Write out the SCXML for the entire tree from this point downward
      * \param xmlWriter is the writer that writes the xml
      *
      */
    void writeSCVXML(QXmlStreamWriter & xmlWriter);


signals:

     void changed();



public slots:



private:

    StateAttributes         _stateAttributes;


};

#endif // SCSTATE_H
