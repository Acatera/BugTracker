#ifndef ISSUE_H
#define ISSUE_H
#include <QString>
#include <QStringList>
#include <QMetaType>

class Issue
{
  public:
    Issue();
    ~Issue();
    QString title;
    QStringList description;
    QString type;
    QString status;
    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;
};

Q_DECLARE_METATYPE(Issue)
#endif // ISSUE_H
