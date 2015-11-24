#ifndef DATAMODELLIST_H
#define DATAMODELLIST_H
#include "scdatamodel.h"

/**
 * @brief The DataModelList class
 * This contains a list of state machine data models. There is one model that
 * is the root of the statemachine.
 *
 * Each data model is also a scxml document. The documents can be linked. One is the root and
 * contains children. Any datamodel can have a state that is defined in another datamodel by link.
 */
class SCDATAMODELSHARED_EXPORT DataModelList
{
public:
    static DataModelList* singleton();
    SCDataModel *  root();
    void addModel(SCDataModel*m);
    void setRoot(SCDataModel*m);
    void save(QString scxmlFilePath, QString msg);

private:
    DataModelList();
    ~DataModelList();
    static DataModelList * _instance;
    SCDataModel*           _root;
    QList<SCDataModel*>    _models;
};

#endif // DATAMODELLIST_H
