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
    QString priority;
    QDateTime createdAt;
    QDateTime updatedAt;
    QDateTime closedAt;

    bool isClosed();
    void setClosed(bool mClosed);

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;
private:
    bool closed;
};

Q_DECLARE_METATYPE(Issue)
#endif // ISSUE_H
