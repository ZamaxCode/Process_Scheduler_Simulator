#include "blockedprocess.h"
#include "ui_blockedprocess.h"

BlockedProcess::BlockedProcess(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BlockedProcess)
{
    ui->setupUi(this);
}

BlockedProcess::~BlockedProcess()
{
    delete ui;
}

void BlockedProcess::setBlockedProcess(const int id, const int cont)
{
    ui->idLB->setText("ID: "+QString::number(id));
    ui->contLB->setText("Cont: "+QString::number(cont));
}
