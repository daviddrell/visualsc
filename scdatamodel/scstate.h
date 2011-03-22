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
class QStandardItem;

#include "scdatamodel_global.h"



class SCDATAMODELSHARED_EXPORT  SCState : public QObject
{
    Q_OBJECT
public:
    explicit SCState(QObject *parent = 0);
    explicit SCState(const SCState& st);
    ~SCState();

    void setStateName(QString n);
    void addTransistion(SCTransition*);
    void addState(SCState *);
   // void makeTargetConnections(QList<SCTransition*> & transitionList);

    void setAttributeValue(QString key, QString value);
    QString getAttributeValue(QString key);

    int  getStateCount();

    void setParentItem(QStandardItem * item);
    void setItem(QStandardItem * item);

    QStandardItem * getParentItem();
    QStandardItem * getItem();

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
      * \fn writeSCVXML
      * \brief Write out the SCXML for the entire tree from this point downward
      * \param xmlWriter is the writer that writes the xml
      *
      */
    void writeSCVXML(QXmlStreamWriter & xmlWriter);


    StateAttributes         attributes;

signals:

     void changed();



public slots:


 private slots:


private:

    int                     _stateCount;
    QStandardItem *         _parentDMItem;
    QStandardItem *         _thisDMItem;

    void initCommon();

};

Q_DECLARE_METATYPE (SCState*)


#endif // SCSTATE_H
