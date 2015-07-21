#ifndef FORMVIEWOBJECT_H
#define FORMVIEWOBJECT_H

#include <QTreeWidget>
#include <QTreeWidgetItem>

/**
 * @brief The FormViewObject class
 *
 * this is the generic object that is used by the scform view
 *
 * it will represent either a state or transition and link all of its relevant objects together.
 *
 * Attributes
 * TreeWidgetObject
 * TextBlock
 *
 */

class FormViewObject
{
public:
    FormViewObject();
    ~FormViewObject();

private:
   // _treeWidget QTreeWidget;


};

#endif // FORMVIEWOBJECT_H
