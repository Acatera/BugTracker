#ifndef ISSUEDETAILSUI_H
#define ISSUEDETAILSUI_H

#include "issue.h"

#include <QDialog>

namespace Ui {
class IssueDetailsUI;
}

class IssueDetailsUI : public QDialog
{
    Q_OBJECT

public:
    explicit IssueDetailsUI(QWidget *parent = 0);
    ~IssueDetailsUI();
    Issue getIssue();
    void updateIssue(Issue &issue);

    void loadIssue(Issue &issue);

private:
    Ui::IssueDetailsUI *ui;
    void keyPressEvent(QKeyEvent *evt);
};

#endif // ISSUEDETAILSUI_H
