#include "pendientprocess.h"
#include "ui_pendientprocess.h"

PendientProcess::PendientProcess(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PendientProcess)
{
    ui->setupUi(this);
}

PendientProcess::~PendientProcess()
{
    delete ui;
}

void PendientProcess::setData(const int &id, const int &time, const int &tt)
{
    ui->idLB->setNum(id);
    ui->timeLB->setNum(time);
    ui->ttLB->setNum(tt);
}

