#ifndef KEYCONTROLLER_H
#define KEYCONTROLLER_H

#include <QGraphicsScene>
#include <QKeyEvent>
#include <QObject>

class KeyController : public QObject
{

Q_OBJECT

public:
    KeyController();
    ~KeyController();
    void keyInput(QKeyEvent *keyPress);
    int getLastKeyEvent();

    signals:
    void keyPressed(int);

private:
    int _lastKey;
};

#endif // KEYCONTROLLER_H
