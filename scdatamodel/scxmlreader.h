#ifndef SCXMLREADER_H
#define SCXMLREADER_H

#include <QString>
#include <QStringList>
#include <QXmlStreamReader>
#include <QThread>

class StateAttributes;
class TransitionAttributes;
class TransitionPathAttributes;

class StateData;

#include "scdatamodel_global.h"

class  SCDATAMODELSHARED_EXPORT  SCXMLReader : public QThread
{
    Q_OBJECT

public:
    SCXMLReader( );
    void readFile(QString infile);
    virtual void run();

signals:
     void done(bool result, QStringList message);
     void makeANewState(StateAttributes *);
     void enterStateElement();
     void leaveStateElement();

     void enterTransistionElement();
     void leaveTransistionElement();
     void makeANewTransistion(TransitionAttributes *);

     void enterTransitionPathElement();
     void leaveTransitionPathElement();
     void makeANewTransistionPath(TransitionPathAttributes *);

     void leaveElement();
     void enterElement();

private:

    QXmlStreamReader _reader;
    QString _file;
    QStringList _resultMessages;

    void readElement();
    void readState();
    void readTransistion();
    void readTransistionPath();
    void readFinal();
    void readOnEntry();
    void readOnExit();
};

#endif // SCXMLREADER_H
