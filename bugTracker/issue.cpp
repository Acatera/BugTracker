#include "issue.h"
#include <QJsonObject>
#include <QDateTime>
#include <QtCore>

Issue::Issue()
{
    createdAt = QDateTime::currentDateTime();
    closed = false;
}

Issue::~Issue()
{
}

bool Issue::isClosed()
{
    return closed;
}

void Issue::setClosed(bool mClosed)
{
    closed = mClosed;
}

void Issue::read(const QJsonObject &json)
{
  title = json["title"].toString();
  description = json["description"].toString().split("\n");
  type = json["type"].toString();
  status = json["stage"].toString();
  closed = json["isClosed"].toBool();
  priority = json["priority"].toString();

  createdAt = QDateTime::fromString(json["createdAt"].toString(), "yyyy-MM-dd hh:mm:ss");
  updatedAt = QDateTime::fromString(json["updatedAt"].toString(), "yyyy-MM-dd hh:mm:ss");
  closedAt = QDateTime::fromString(json["closedAt"].toString(), "yyyy-MM-dd hh:mm:ss");
}

void Issue::write(QJsonObject &json) const
{
  json["title"] = title;
  json["description"] = description.join("\n");
  json["type"] = type;
  json["stage"] = status;
  json["priority"] = priority;
  json["isClosed"] = closed;

  json["createdAt"] = createdAt.toString("yyyy-MM-dd hh:mm:ss");
  json["updatedAt"] = updatedAt.toString("yyyy-MM-dd hh:mm:ss");
  json["closedAt"] = closedAt.toString("yyyy-MM-dd hh:mm:ss");
}
