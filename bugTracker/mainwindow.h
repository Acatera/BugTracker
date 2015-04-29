#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "datastore.h"
#include "issuedetailsui.h"
#include "settings.h"

#include <QMainWindow>
#include <QListWidgetItem>
#include <QTreeWidget>

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
    void on_treeWidget_doubleClicked(const QModelIndex &index);
    void on_treeWidget_2_doubleClicked(const QModelIndex &index);
    void on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
    void on_treeWidget_2_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
    void on_treeWidget_3_customContextMenuRequested(const QPoint &pos);
    void on_actionNew_project_triggered();
    void on_actionOpen_project_triggered();
    void on_actionSave_project_triggered();
    void on_treeWidget_3_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
    void on_treeWidget_3_doubleClicked(const QModelIndex &index);
    void on_treeWidget_3_itemChanged(QTreeWidgetItem *item, int column);

    void on_actionRemove_project_triggered();

private:
    Ui::MainWindow *ui;
    IssueDetailsUI *idUI;
    DataStore dataStore;
    Settings settings;

    Issue* getSelectedIssue();
    int editingState;
    void displayIssues();
    void editIssue(Issue *issue);
    void newIssue();
    void keyPressEvent(QKeyEvent *evt);
    void loadProjects();
};

#endif // MAINWINDOW_H
