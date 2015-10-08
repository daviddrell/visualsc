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

void CustomTableWidgetItem::handleAttributeChanged(StateString* ss)
{
    setText(ss->asString());
}

void CustomTableWidgetItem::handleAttributeChanged(GenericAttribute* ga)
{
//    qDebug() << "customtablewidgetitem::handleattribtuechagned ga";
    setText(ga->asString());
}

void CustomTableWidgetItem::handleAttributeChanged(FontFamilyAttribute * fa)
{
    setText(fa->asString());
}

void CustomTableWidgetItem::handleAttributeChanged(FontSizeAttribute * fs)
{
    setText(fs->asString());
}

void CustomTableWidgetItem::handleAttributeChanged(FontBoldAttribute * fba)
{
    setText(fba->asString());
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
