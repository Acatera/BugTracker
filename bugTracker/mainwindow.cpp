#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <issue.h>
#include "datastore.h"
#include "issuedetailsui.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->treeWidget->setColumnWidth(1, 20);
    ui->treeWidget_2->setColumnWidth(1, 20);
    loadProjects();
}

MainWindow::~MainWindow()
{
    on_actionSaveJSon_triggered();
    settings.currentProjectIndex = ui->treeWidget_3->currentIndex().row();
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
//    if (!dataStore.read("save.json")) {
//        QMessageBox::information(this, "Error", tr("Error while reading json file"), QMessageBox::Ok);
//    } else {
//        displayIssues();
//    }
}

void MainWindow::on_actionSaveJSon_triggered() {
    if (ui->treeWidget_3->topLevelItemCount() > 0) {
        QString savePath = ui->treeWidget_3->currentItem()->data(0, Qt::UserRole).value<QString>();
        if (!dataStore.write(savePath)) {
            QMessageBox::information(this, "Error", tr("Error while writing json file"), QMessageBox::Ok);

        }
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

void MainWindow::loadProjects()
{
    for (int i = 0; i < settings.projects.size(); i++) {
        dataStore.read(settings.projects.at(i));
        qDebug() << "Loaded " << settings.projects.at(i);
        QTreeWidgetItem *twi = new QTreeWidgetItem();
        twi->setData(0, Qt::UserRole, settings.projects.at(i));
        twi->setText(0, dataStore.projectName);
        twi->setFlags(twi->flags() | Qt::ItemIsEditable);

        ui->treeWidget_3->addTopLevelItem(twi);
        if (i == settings.currentProjectIndex) {
            ui->treeWidget_3->setCurrentItem(twi);
        }
    }

    if (settings.projects.size() > 0) {
        dataStore.read(settings.projects.at(settings.currentProjectIndex));
        displayIssues();
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

void MainWindow::on_treeWidget_3_customContextMenuRequested(const QPoint &pos)
{
    QMenu *menu = new QMenu(this);
    menu->addAction(ui->actionSave_project);
    menu->addAction(ui->actionOpen_project);
    menu->addAction(ui->actionRemove_project);
    menu->popup(ui->treeWidget_3->viewport()->mapToGlobal(pos));
}

void MainWindow::on_actionNew_project_triggered()
{

}

void MainWindow::on_actionOpen_project_triggered()
{
    //Add existing project;
    QFileDialog *fd = new QFileDialog(this);
    fd->setAcceptMode(QFileDialog::AcceptOpen);
    fd->setFileMode(QFileDialog::ExistingFile);

    QStringList filters;
    filters << "Project files (*.json)"
            << "Any files (*)";
    fd->setNameFilters(filters);

    if (fd->exec()){
        editingState = 1;
        if (ui->treeWidget_3->topLevelItemCount() > 0) {
            QString savePath = ui->treeWidget_3->currentItem()->data(0, Qt::UserRole).value<QString>();
            dataStore.write(savePath);
            qDebug() << "Saved old project to " << savePath << " with title " << dataStore.projectName;
        }

        QString path = fd->selectedFiles().first();
        if (!dataStore.read(path)) {
            QMessageBox::information(this, "Error", tr("Error while reading json file"), QMessageBox::Ok);
        } else {
            settings.projects.append(path);

            QTreeWidgetItem *twi = new QTreeWidgetItem();
            twi->setData(0, Qt::UserRole, path);
            twi->setText(0, dataStore.projectName);
            twi->setFlags(twi->flags() | Qt::ItemIsEditable);

            ui->treeWidget_3->addTopLevelItem(twi);
            ui->treeWidget_3->setCurrentItem(twi);
            displayIssues();
        }
    }
}

void MainWindow::on_actionSave_project_triggered()
{
    //Add new project;
    QFileDialog *fd = new QFileDialog(this);
    fd->setAcceptMode(QFileDialog::AcceptSave);

    QStringList filters;
    filters << "Project files (*.json)"
            << "Any files (*)";
    fd->setNameFilters(filters);

    if (fd->exec()){
        editingState = 1;
        if (ui->treeWidget_3->topLevelItemCount() > 0) {
            QString savePath = ui->treeWidget_3->currentItem()->data(0, Qt::UserRole).value<QString>();
            dataStore.write(savePath);
            qDebug() << "Saved old project to " << savePath << " with title " << dataStore.projectName;
        }
        QString path = fd->selectedFiles().first();
        dataStore.issues.clear();
        if (!dataStore.write(path)) {
            QMessageBox::information(this, "Error", tr("Error while saving json file"), QMessageBox::Ok);
        } else {
            settings.projects.append(path);

            QTreeWidgetItem *twi = new QTreeWidgetItem();
            twi->setData(0, Qt::UserRole, path);
            twi->setText(0, path);
            twi->setFlags(twi->flags() | Qt::ItemIsEditable);

            ui->treeWidget_3->addTopLevelItem(twi);
            ui->treeWidget_3->setCurrentItem(twi);
            displayIssues();
        }
    }
}

void MainWindow::on_treeWidget_3_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    if (editingState != 1) {
    if (previous != nullptr)
    {
            QString savePath = previous->data(0, Qt::UserRole).value<QString>();
            dataStore.write(savePath);
            qDebug() << "Saved to " << savePath << " with title " << dataStore.projectName;
        }

        if (current != nullptr) {
            QString loadPath = current->data(0, Qt::UserRole).value<QString>();
            dataStore.read(loadPath);
            qDebug() << "Loaded from " << loadPath << " with title " << dataStore.projectName;
        }
        displayIssues();
    }
    editingState = 0;
}

void MainWindow::on_treeWidget_3_doubleClicked(const QModelIndex &index)
{
    ui->treeWidget_3->edit(index);
}

void MainWindow::on_treeWidget_3_itemChanged(QTreeWidgetItem *item, int column)
{
    (void) item;
    (void) column;
    dataStore.projectName = ui->treeWidget_3->currentItem()->text(0);
    qDebug() << dataStore.projectName;
}

void MainWindow::on_actionRemove_project_triggered()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirm project removal...", "Are you sure you want to delete the selected project?\nThe project will only be excluded from the list.",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        QString project = ui->treeWidget_3->currentItem()->data(0, Qt::UserRole).value<QString>();
        settings.projects.removeAt(settings.projects.indexOf(project));
        delete ui->treeWidget_3->currentItem();
    }
}
