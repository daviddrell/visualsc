#include "keycontroller.h"
#include <QGraphicsScene>
#include <QKeyEvent>
#include <scdatamodel.h>

KeyController::KeyController()
{

}

void KeyController::keyInput(QKeyEvent *keyPress)
{
    _lastKey = keyPress->key();
    emit keyPressed(_lastKey);
}

int KeyController::getLastKeyEvent()
{
    return _lastKey;
}

KeyController::~KeyController()
{

}



