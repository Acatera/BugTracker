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
  description = json["description"].toString().split("\n");
}

void Issue::write(QJsonObject &json) const
{
  json["title"] = title;
  json["type"] = type;
  json["description"] = description.join("\n");
}
