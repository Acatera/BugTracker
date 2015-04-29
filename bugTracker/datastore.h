#ifndef DATASTORE_H
#define DATASTORE_H

#include "issue.h"

#include <QJsonObject>



class DataStore
{
public:
    DataStore();
    ~DataStore();
    bool read(QString filePath);
    bool write(QString filePath);
    QList<Issue> issues;
    QString projectName;
private:
    QJsonObject json;

};

#endif // DATASTORE_H
