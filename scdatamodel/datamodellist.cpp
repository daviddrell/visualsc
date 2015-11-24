#include "datamodellist.h"

DataModelList * DataModelList::_instance=NULL;

DataModelList* DataModelList::singleton()
{
    if ( _instance == NULL)
        _instance = new DataModelList();
    return _instance;
}

DataModelList::DataModelList()
{
}

DataModelList::~DataModelList()
{
}

SCDataModel* DataModelList::root()
{
    return _root;
}

void DataModelList::setRoot(SCDataModel *m)
{
    _root = m;
    if( ! _models.contains(m))
    {
        _models.append(m);
    }
}

void DataModelList::addModel(SCDataModel *m)
{
    if( ! _models.contains(m))
    {
        _models.append(m);
    }
}

void DataModelList::save(QString scxmlFilePath, QString msg)
{
    foreach(SCDataModel* dm, _models)
    {
        dm->save(scxmlFilePath, msg);
    }
}
