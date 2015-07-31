#include "customtablewidgetitem.h"
#include <QDebug>

CustomTableWidgetItem::CustomTableWidgetItem(QString text):
    QTableWidgetItem(text)
{

}

CustomTableWidgetItem::~CustomTableWidgetItem()
{

}

void CustomTableWidgetItem::handleAttributeChanged(SizeAttribute* sz)
{
    setText(sz->asString());
}

void CustomTableWidgetItem::handleAttributeChanged(PositionAttribute* po)
{
    setText(po->asString());
}

void CustomTableWidgetItem::handleAttributeChanged(StateName * sn)
{
    qDebug() << "CustomTableWidgetItem::handleAttributeChanged";
    setText(sn->asString());
}

void CustomTableWidgetItem::handleAttributeChanged(TransitionStringAttribute* tsa)
{
    setText(tsa->asString());
}

/*
void CustomTableWidgetItem::setText(const QString &text)
{
    if(text != this->text())
    {
        QTableWidgetItem::setText(text);
        emit textChanged(text);
    }
}
*/
