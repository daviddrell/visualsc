#ifndef SCDATAMODEL_H
#define SCDATAMODEL_H

#include "scdatamodel_global.h"
#include "sctransition.h"
#include "scstate.h"
#include <QList>
#include "scxmlreader.h"
#include <QXmlStreamWriter>

class SCDATAMODELSHARED_EXPORT SCDataModel : public QObject
{
      Q_OBJECT


public:
    SCDataModel(QObject * parent);

    void open(QString scxmlFile);

    bool save(QString scxmlFile, QString & message);

    void getStates(QList<SCState *>& list);

signals:
    void openCompleted(bool sucess, QStringList message);
    void newStateSignal(SCState * newState);

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


};

#endif // SCDATAMODEL_H
