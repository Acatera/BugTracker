#include "datastore.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QMessageBox>
#include <QString>

DataStore::DataStore()
{
}

DataStore::~DataStore()
{
}

bool DataStore::read()
{
    issues.clear();
    QFile loadFile(QStringLiteral("save.json"));

    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    QByteArray saveData = loadFile.readAll();

    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

    QJsonArray jsonArray = loadDoc.object()["issues"].toArray();

    for (int i=0; i < jsonArray.size(); i++)
    {
        QJsonObject issueObject = jsonArray[i].toObject();
        Issue issue;
        issue.read(issueObject);

        //      QWidget *qw = new QWidget;
        //      QMessageBox::information(qw, QString("Bla"), issueObject.value("title").toString(), QMessageBox::Save);

        issues.append(issue);
    }

    return true;
}

bool DataStore::write()
{
    QFile saveFile(QStringLiteral("save.json"));

    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    QJsonArray issueArray;
    foreach (const Issue issue, issues) {
        QJsonObject issueObject;
        issue.write(issueObject);
//        QWidget *qw = new QWidget;
//        QMessageBox::information(qw, QString("Bla"), issueObject.value("title").toString(), QMessageBox::Save);
        issueArray.append(issueObject);
    }

    json["issues"] = issueArray;

    QJsonDocument saveDoc(json);
    saveFile.write(saveDoc.toJson());

    return true;
}

