#include "settings.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

Settings::Settings()
{
    read();
}

Settings::~Settings()
{
    write();
}

void Settings::read()
{
    QFile loadFile("settings.json");

    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
        return;
    }

    QByteArray saveData = loadFile.readAll();

    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

    currentProjectIndex = loadDoc.object()["currentProject"].toInt();

    QJsonArray jsonArray = loadDoc.object()["projects"].toArray();

    for (int i=0; i < jsonArray.size(); i++)
    {
        QJsonObject issueObject = jsonArray[i].toObject();
        QString project = issueObject["path"].toString();

        projects.append(project);
    }
}

void Settings::write()
{
    QFile saveFile("settings.json");

    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return;
    }

    QJsonObject json;

    json["currentProject"] = currentProjectIndex;

    QJsonArray issueArray;
    foreach (const QString project, projects) {
        QJsonObject issueObject;
        issueObject["path"] = project;
        issueArray.append(issueObject);
    }

    json["projects"] = issueArray;

    QJsonDocument saveDoc(json);
    saveFile.write(saveDoc.toJson());
}

