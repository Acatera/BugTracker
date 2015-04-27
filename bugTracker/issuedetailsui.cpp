#include "issuedetailsui.h"
#include "ui_issuedetailsui.h"

IssueDetailsUI::IssueDetailsUI(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IssueDetailsUI)
{
    ui->setupUi(this);
}

IssueDetailsUI::~IssueDetailsUI()
{
    delete ui;
}

Issue IssueDetailsUI::getIssue()
{
    Issue issue;
    issue.title = ui->lineEdit->text();
    issue.description.append(ui->plainTextEdit->toPlainText().split("\n"));
    issue.type = ui->comboBox->currentText();
    issue.priority = ui->comboBox_2->currentText();

    return issue;
}

void IssueDetailsUI::updateIssue(Issue *issue)
{
    issue->title = ui->lineEdit->text();
    issue->description.clear();
    issue->description.append(ui->plainTextEdit->toPlainText().split("\n"));
    issue->priority = ui->comboBox_2->currentText();
    issue->type = ui->comboBox->currentText();
}

void IssueDetailsUI::loadIssue(Issue *issue)
{
    ui->lineEdit->setText(issue->title);
    ui->plainTextEdit->setPlainText(issue->description.join("\n"));
    ui->comboBox_2->setCurrentText(issue->priority);
    ui->comboBox->setCurrentText(issue->type);
}

void IssueDetailsUI::keyPressEvent(QKeyEvent *evt)
{
    if (evt->key() == Qt::Key_Enter || evt->key() == Qt::Key_Return)
        return;
    QDialog::keyPressEvent(evt);
}
