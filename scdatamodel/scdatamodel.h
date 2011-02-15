#ifndef SCDATAMODEL_H
#define SCDATAMODEL_H

#include "scdatamodel_global.h"
#include "sctransition.h"
#include "scstate.h"
#include <QList>
#include "scxmlreader.h"
#include <QXmlStreamWriter>

class QStandardItemModel;
class QStandardItem;

class SCDATAMODELSHARED_EXPORT SCDataModel : public QObject
{
      Q_OBJECT


public:
    SCDataModel(QObject * parent);

    // direct interface
    void open(QString scxmlFile);

    bool save(QString scxmlFile, QString & message);

    void getStates(QList<SCState *>& list);
    void getAllStates(QList<SCState *>& list);

    // Q Data Model Interface
    QStandardItemModel * getStandardModel();

signals:
    void openCompleted(bool sucess, QStringList message);
    void newStateSignal(SCState * newState);
    void newTransitionSignal(SCTransition * newTransition);

private slots:
    void handleReaderDone(bool sucess, QStringList message);
    void handleMakeANewState(StateAttributes*);
    void handleMakeANewTransition(TransitionAttributes*);
    void handleLeaveTransitionElement();
    void handleMakeANewTransitionPath(TransitionPathAttributes * tp);
    void handleTransitUp();
    void handleTransitDown();

private:
    SCXMLReader     _reader;
    QXmlStreamWriter *_writer;
    int              _level;
    int              _topLevel;
    SCState         *_currentState;
    SCTransition    *_currentTransition;
    SCState         *_topState;
    QStandardItemModel *_qtDM; // standard Qt Data Model implementation


//private methods

    QStandardItem * makeAStateItem(SCState *st);

    QStandardItem * makeATransitionItem(SCTransition *tr);


};

#endif // SCDATAMODEL_H
