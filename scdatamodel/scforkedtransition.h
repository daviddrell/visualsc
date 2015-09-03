#ifndef SCFORKEDTRANSITION_H
#define SCFORKEDTRANSITION_H

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
#include "sctransitionbranch.h"

/**
 * @brief The SCForkedTransition class
 *
 * a forked transition will hold 1 or more SCTransitionBranches, Have NO parent
 *
 * the forked transition will have 1 path from forked elbow to target state
 *
 */

class SCDATAMODELSHARED_EXPORT SCForkedTransition : public QObject
{
    Q_OBJECT
public:
    SCForkedTransition();
    ~SCForkedTransition();
    TransitionAttributes attributes;

    void setTargetState(SCState* target);
    SCState* targetState();
    SCTransitionBranch* addSourceState(SCState* source);
    void writeSCXML(QXmlStreamWriter&sw);
    void initialize();
    SCTextBlock* getEventTextBlock();

    QList<SCTransitionBranch*>* getSourceBranches();

    QString getEventName();
private:
    QList<QString> DEFAULT_ATTRIBUTES;
    QHash<QString, int> DO_NOT_DISPLAY;
    SCState* _targetState;

    SCTextBlock* _eventTextBlock;
    QList<SCTransitionBranch*> _sourceBranches;


};

#endif // SCFORKEDTRANSITION_H
