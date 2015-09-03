#ifndef SCTRANSITIONBRANCH_H
#define SCTRANSITIONBRANCH_H

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
class SCForkedTransition;

/**
 * @brief The SCTransitionBranch class
 *
 * each SCtransition will be made of one or mode transition branches
 * the focal point is the forked elbow, which will be a common root for all branches
 *
 *
 */



class SCDATAMODELSHARED_EXPORT SCTransitionBranch : public SCItem
{

    Q_OBJECT

public:
    SCTransitionBranch(SCForkedTransition* group, SCState* source);
    ~SCTransitionBranch();

    void initialize();
    TransitionAttributes attributes;

    void addParentAttributes();
    void removeParentAttributes();
    void deleteSafely();

    SCState* parentAsSCState();
    SCState* targetState();

    virtual IAttributeContainer * getAttributes(); //reimplemented from base SCItem

    QString getEventName();
    void setEventName(QString name);
    SCTextBlock* getEventTextBlock();
    TransitionStringAttribute* getTransStringAttr(QString key);

    void    writeSCVXML(QXmlStreamWriter & sw);
    void setAttributeValue(QString key, QString val);

public slots:
    void handleTargetStateNameChanged(StateName*);


signals:
    void clicked(SCTransitionBranch*);

private:
    SCForkedTransition* _group;

    SCTextBlock* _eventTextBlock;
};

#endif // SCTRANSITIONBRANCH_H
