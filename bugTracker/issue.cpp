#include "issue.h"
#include <QJsonObject>

Issue::Issue()
{
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
}

void Issue::write(QJsonObject &json) const
{
  json["title"] = title;
  json["type"] = type;
  json["status"] = status;
  json["description"] = description.join("\n");
}
