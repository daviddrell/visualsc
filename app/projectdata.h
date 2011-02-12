#ifndef PROJECTDATA_H
#define PROJECTDATA_H

#include <QString>
#include <QList>
#include <QObject>

class ProjectData
{
public:
    ProjectData(QObject *parent=0);
    ~ProjectData();

    QString scxmlFileName; ///< SCXML file associated with the project
    QString scvxmlFileName;///< State-Chart-Visualdata-XML associated with the project


};

#endif // PROJECTDATA_H
