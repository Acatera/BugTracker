#include "issue.h"
#include <QJsonObject>
#include <QDateTime>
#include <QtCore>

Issue::Issue()
{
    createdAt = QDateTime::currentDateTime();
}

Issue::~Issue()
{
}

void Issue::read(const QJsonObject &json)
{
  title = json["title"].toString();
  type = json["type"].toString();
  status = json["status"].toString();
  description = json["description"].toString().split("\n");

  createdAt = QDateTime::fromString(json["createdAt"].toString(), "yyyy-MM-dd hh:mm:ss");
  updatedAt = QDateTime::fromString(json["updatedAt"].toString(), "yyyy-MM-dd hh:mm:ss");
  closedAt = QDateTime::fromString(json["closedAt"].toString(), "yyyy-MM-dd hh:mm:ss");
}

void Issue::write(QJsonObject &json) const
{
  json["title"] = title;
  json["type"] = type;
  json["status"] = status;
  json["description"] = description.join("\n");

  json["createdAt"] = createdAt.toString("yyyy-MM-dd hh:mm:ss");
  json["updatedAt"] = updatedAt.toString("yyyy-MM-dd hh:mm:ss");
  json["closedAt"] = closedAt.toString("yyyy-MM-dd hh:mm:ss");
}
