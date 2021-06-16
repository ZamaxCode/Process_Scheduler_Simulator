#include "finishprocess.h"
#include "ui_finishprocess.h"

FinishProcess::FinishProcess(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FinishProcess)
{
    ui->setupUi(this);
}

FinishProcess::~FinishProcess()
{
    delete ui;
}

void FinishProcess::setData(const int &id, const QString &operation, const QString &result)
{
    ui->idLB->setNum(id);
    ui->opLB->setText(operation);
    ui->resLB->setText(result);
}
