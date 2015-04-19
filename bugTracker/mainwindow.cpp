#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <issue.h>
#include "datastore.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //  dataStore = new DataStore;
}

MainWindow::~MainWindow()
{
    delete ui;
    //  delete dataStore;
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
    Issue issue = current->data(Qt::UserRole).value<Issue>();

    ui->issueDetails->setPlainText(issue.title);
    ui->issueDetails->appendPlainText("--------------------");
    ui->issueDetails->appendPlainText("Type:" + issue.type);
    ui->issueDetails->appendPlainText("--------------------");

    for (int i = 0; i < issue.description.count(); i++){
        ui->issueDetails->appendPlainText(issue.description.at(i));
    }
}

void MainWindow::on_pushButton_clicked()
{
    if (!dataStore.write()) {
        QMessageBox::information(this, "Error", tr("Error while writing json file"), QMessageBox::Ok);
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    if (!dataStore.read()) {
        QMessageBox::information(this, "Error", tr("Error while reading json file"), QMessageBox::Ok);
    } else {
        displayIssues();
    }
}
