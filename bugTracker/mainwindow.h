#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "datastore.h"
#include "issuedetailsui.h"

#include <QMainWindow>
#include <QListWidgetItem>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void displayIssueDetails(Issue *ptrIssue);
    void setIssueDone(Issue *ptrIssue);
    void setIssuePending(Issue *ptrIssue);
private slots:
    void on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
    void on_listWidget_2_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
    void on_listWidget_doubleClicked(const QModelIndex &index);
    void on_listWidget_2_doubleClicked(const QModelIndex &index);
    void on_actionLoadJSon_triggered();
    void on_actionSaveJSon_triggered();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_btnUpdateStatus_clicked();
    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
    IssueDetailsUI *idUI;
    DataStore dataStore;
    Issue* getSelectedIssue();

    void displayIssues();
    void editIssue(Issue *issue);
    void newIssue();
    void keyPressEvent(QKeyEvent *evt);
};

#endif // MAINWINDOW_H
