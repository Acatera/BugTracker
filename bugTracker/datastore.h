#ifndef DATASTORE_H
#define DATASTORE_H

#include "issue.h"

#include <QJsonObject>



class DataStore
{
public:
    DataStore();
    ~DataStore();
    bool read();
    bool write();
    QList<Issue> issues;
private:
    QJsonObject json;

};

#endif // DATASTORE_H
