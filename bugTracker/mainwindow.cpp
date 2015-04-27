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
    ui->treeWidget->clear();
    ui->treeWidget_2->clear();

    for (int i = 0; i < dataStore.issues.size(); i++){
        Issue &rIssue = dataStore.issues[i];
        QVariant qv = qVariantFromValue((void *) &dataStore.issues[i]);

        QString timeStamp;
        if (rIssue.isClosed()) {
            if (QDateTime::currentDateTime().daysTo(rIssue.closedAt) < -1) {
                timeStamp = rIssue.closedAt.toString("yyyy-MM-dd");
            } else {
                timeStamp = "Today " + rIssue.closedAt.toString("hh:mm");
            }
        }

        QTreeWidgetItem *twi = new QTreeWidgetItem();

        twi->setData(0, Qt::UserRole, qv);

        if (rIssue.isClosed()) {
            twi->setText(0, rIssue.title);
            twi->setText(1, timeStamp);
            ui->treeWidget_2->addTopLevelItem(twi);
        } else {
            twi->setText(0, timeStamp + rIssue.title);
            twi->setText(1, rIssue.priority);
            ui->treeWidget->addTopLevelItem(twi);
        }
    }
}

void MainWindow::displayIssueDetails(Issue *ptrIssue)
{
    ui->issueDetails->setPlainText(ptrIssue->title);
    ui->issueDetails->appendPlainText("--------------------");
    ui->issueDetails->appendPlainText("Type:" + ptrIssue->type + "; Status:" + ptrIssue->status);
    ui->issueDetails->appendPlainText("Created at:" + ptrIssue->createdAt.toString("yyyy-MM-dd hh:mm:ss"));
    ui->issueDetails->appendPlainText("Updated at:" + ptrIssue->updatedAt.toString("yyyy-MM-dd hh:mm:ss"));
    if (ptrIssue->closedAt.toMSecsSinceEpoch() != 0) {
        ui->issueDetails->appendPlainText("Closed at:" + ptrIssue->closedAt.toString("yyyy-MM-dd hh:mm:ss"));
    }
    ui->issueDetails->appendPlainText("--------------------");

    for (int i = 0; i < ptrIssue->description.count(); i++){
        ui->issueDetails->appendPlainText(ptrIssue->description.at(i));
    }
}

void MainWindow::on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    (void) previous;
    if (current == nullptr)
        return;

    Issue *ptrIssue = (Issue *) current->data(Qt::UserRole).value<void *>();

    displayIssueDetails(ptrIssue);
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

void MainWindow::editIssue(Issue *issue)
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

void MainWindow::keyPressEvent(QKeyEvent *evt)
{
    if ((evt->key() == Qt::Key_Escape)) {
        QApplication::quit();
        return;
    }

    if ((evt->key() == Qt::Key_F1)) {
        QMessageBox::about(this, "Hotkeys...",
                           "F1 - shows this message\nEsc - exits the application\nLeft - sets focus to the left pane when right pane is focused\nRight - sets focus to the right pane when left pane is focused\nShift + Left - Marks selected issue as 'Pending'\nShift + Right - Marks selected issue as 'Done'\n");
        return;
    }

    if ((evt->key() == Qt::Key_Return)) {
        if (ui->treeWidget->hasFocus() || ui->treeWidget_2->hasFocus()) {
            Issue *ptrIssue = getSelectedIssue();
            if (ptrIssue != nullptr) {
                editIssue(ptrIssue);
            }
        }
    }

    /* Horrible hack. Will fix when I know a bit more about stuffs */
    bool isShiftPressed = evt->modifiers() & Qt::ShiftModifier;
    if (ui->treeWidget->hasFocus() && (evt->key() == Qt::Key_Right)) {
        if (isShiftPressed) {
            Issue *ptrIssue = getSelectedIssue();

            if (ptrIssue == nullptr)
                return;

            setIssueDone(ptrIssue);
            displayIssues();
        } else {
            ui->treeWidget_2->setFocus();
            if (ui->treeWidget_2->selectedItems().size() == 0 && ui->treeWidget->topLevelItemCount() > 0) {
                ui->treeWidget_2->setCurrentItem(0);
            }
        }
    } else if (ui->treeWidget_2->hasFocus() && (evt->key() == Qt::Key_Left)) {
        if (isShiftPressed) {
            Issue *ptrIssue = getSelectedIssue();

            if (ptrIssue == nullptr)
                return;

            setIssuePending(ptrIssue);
            displayIssues();
        } else {
            ui->treeWidget->setFocus();
            if (ui->treeWidget->selectedItems().size() == 0 && ui->treeWidget->topLevelItemCount() > 0) {
                ui->treeWidget->setCurrentItem(0);
            }
        }
    }
}

void MainWindow::on_listWidget_doubleClicked(const QModelIndex &index)
{
    (void) index;
    Issue *ptrIssue = getSelectedIssue();
    if (ptrIssue == nullptr)
        return;
    editIssue(ptrIssue);
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
        Issue *ptrIssue = getSelectedIssue();
        if (ptrIssue != nullptr) {
            int idx = -1;
            for (int i = 0; i < dataStore.issues.size(); i++) {
                if (&dataStore.issues[i] == ptrIssue) {
                    idx = i;
                    break;
                }
            }
            if (idx >= 0) {
                dataStore.issues.removeAt(idx);
                displayIssues();
            }
        }
    }
}

void MainWindow::setIssueDone(Issue *ptrIssue)
{
    ptrIssue->status = "done";
    ptrIssue->setClosed(true);
    ptrIssue->updatedAt = QDateTime::currentDateTime();
    ptrIssue->closedAt = QDateTime::currentDateTime();
}

void MainWindow::setIssuePending(Issue *ptrIssue)
{
    ptrIssue->status = "pending";
    ptrIssue->setClosed(false);
    ptrIssue->updatedAt = QDateTime::currentDateTime();
    ptrIssue->closedAt = QDateTime::fromMSecsSinceEpoch(0);
}

void MainWindow::on_btnUpdateStatus_clicked()
{
    Issue *ptrIssue = getSelectedIssue();
    if (ptrIssue == nullptr)
        return;

    setIssueDone(ptrIssue);
    displayIssues();
}

Issue* MainWindow::getSelectedIssue()
{
    QTreeWidgetItem *ptrCurrent;
    if (ui->treeWidget->hasFocus()) {
        ptrCurrent = ui->treeWidget->currentItem();
    } else if (ui->treeWidget_2->hasFocus()) {
        ptrCurrent = ui->treeWidget_2->currentItem();
    }

    if (ptrCurrent != nullptr) {
        Issue *pIssue = (Issue *) ptrCurrent->data(0, Qt::UserRole).value<void *>();
        return pIssue;
    }
    return nullptr;
}

void MainWindow::on_pushButton_3_clicked()
{
    Issue *ptrIssue = getSelectedIssue();
    if (ptrIssue == nullptr)
        return;

    setIssuePending(ptrIssue);
    displayIssues();
}

void MainWindow::on_listWidget_2_doubleClicked(const QModelIndex &index)
{
    (void) index;
    Issue *ptrIssue = getSelectedIssue();
    if (ptrIssue == nullptr)
        return;

    editIssue(ptrIssue);
}

void MainWindow::on_listWidget_2_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    (void) previous;
    if (current == nullptr)
        return;

    Issue *ptrIssue = (Issue *) current->data(Qt::UserRole).value<void *>();

    displayIssueDetails(ptrIssue);
}

void MainWindow::on_treeWidget_doubleClicked(const QModelIndex &index)
{
    (void) index;
    Issue *ptrIssue = getSelectedIssue();
    if (ptrIssue == nullptr)
        return;
    editIssue(ptrIssue);
}

void MainWindow::on_treeWidget_2_doubleClicked(const QModelIndex &index)
{
    (void) index;
    Issue *ptrIssue = getSelectedIssue();
    if (ptrIssue == nullptr)
        return;
    editIssue(ptrIssue);
}

void MainWindow::on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    (void) previous;
    if (current == nullptr)
        return;

    Issue *ptrIssue = (Issue *) current->data(0, Qt::UserRole).value<void *>();

    displayIssueDetails(ptrIssue);
}

void MainWindow::on_treeWidget_2_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    (void) previous;
    if (current == nullptr)
        return;

    Issue *ptrIssue = (Issue *) current->data(0, Qt::UserRole).value<void *>();

    displayIssueDetails(ptrIssue);
}
