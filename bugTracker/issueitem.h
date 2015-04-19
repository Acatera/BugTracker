#include <QListWidgetItem>
#include "issue.h"

class IssueItem: public QListWidgetItem
{
public:
    IssueItem();
    ~IssueItem();
    Issue issue;
};
