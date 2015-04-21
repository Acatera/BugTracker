#ifndef ISSUE_H
#define ISSUE_H
#include <QString>
#include <QStringList>
#include <QMetaType>
#include <QDateTime>

class Issue
{
  public:
    Issue();
    ~Issue();
    QString title;
    QStringList description;
    QString type;
    QString status;
    QDateTime createdAt;
    QDateTime updatedAt;
    QDateTime closedAt;
    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;
};

Q_DECLARE_METATYPE(Issue)
#endif // ISSUE_H
