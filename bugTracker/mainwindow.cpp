#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <issue.h>
#include "datastore.h"
#include "issuedetailsui.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    on_actionLoadJSon_triggered();
}

MainWindow::~MainWindow()
{
    on_actionSaveJSon_triggered();
    delete ui;
}

void MainWindow::displayIssues()
{
    ui->listWidget->clear();
    foreach (const Issue issue, dataStore.issues) {
        QVariant qv;
        qv.setValue(issue);

        QListWidgetItem *issueItem = new QListWidgetItem();
        issueItem->setText(issue.title);
        issueItem->setData(Qt::UserRole, qv);
        ui->listWidget->addItem(issueItem);
    }
}

void MainWindow::on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (current == 0)
        return;

    Issue issue = current->data(Qt::UserRole).value<Issue>();

    ui->issueDetails->setPlainText(issue.title);
    ui->issueDetails->appendPlainText("--------------------");
    ui->issueDetails->appendPlainText("Type:" + issue.type);
    ui->issueDetails->appendPlainText("--------------------");

    for (int i = 0; i < issue.description.count(); i++){
        ui->issueDetails->appendPlainText(issue.description.at(i));
    }
}

void MainWindow::on_actionLoadJSon_triggered() {
    if (!dataStore.read()) {
        QMessageBox::information(this, "Error", tr("Error while reading json file"), QMessageBox::Ok);
    } else {
        displayIssues();
    }
}

void MainWindow::on_actionSaveJSon_triggered() {
    if (!dataStore.write()) {
        QMessageBox::information(this, "Error", tr("Error while writing json file"), QMessageBox::Ok);
    }
}

void MainWindow::editIssue(Issue &issue)
{
    idUI = new IssueDetailsUI(this);
    idUI->loadIssue(issue);
    if (idUI->exec() == QDialog::Accepted) {
        idUI->updateIssue(issue);
        displayIssues();
    }
    delete idUI;
}

void MainWindow::newIssue()
{
    idUI = new IssueDetailsUI(this);
    if (idUI->exec() == QDialog::Accepted) {
        dataStore.issues.append(idUI->getIssue());
        displayIssues();
    }
    delete idUI;
}

void MainWindow::on_listWidget_doubleClicked(const QModelIndex &index)
{
    int idx = ui->listWidget->row(ui->listWidget->currentItem());
    Issue &issue = dataStore.issues[idx];
    editIssue(issue);
}

void MainWindow::on_pushButton_clicked()
{
    newIssue();
}

void MainWindow::on_pushButton_2_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirm issue deletion...", "Are you sure you want to delete the selected issue?",
                 QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        int idx = ui->listWidget->row(ui->listWidget->currentItem());
        dataStore.issues.removeAt(idx);
        displayIssues();
    }
}
