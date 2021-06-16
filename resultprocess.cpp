#include "resultprocess.h"
#include "ui_resultprocess.h"

ResultProcess::ResultProcess(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ResultProcess)
{
    ui->setupUi(this);
}

ResultProcess::~ResultProcess()
{
    delete ui;
}

void ResultProcess::setResultProcess(Proceso P)
{
    ui->idLB->setText("ID: "+QString::number(P.getId()));
    ui->tmLB->setText("Tiempo Max: "+QString::number(P.getTimeMax()));
    ui->opLB->setText("Operacion: "+QString::number(P.getDigit1())+P.getOperation()+QString::number(P.getDigit2()));

    if(P.getError())
        ui->resLB->setText("Resultado: Error");
    else
        ui->resLB->setText("Resultado: "+QString::number(P.getResult()));

    ui->llegadaLB->setText("T. Llegada: "+QString::number(P.getLlegada()));
    ui->finalLB->setText("T. Finalizacion: "+QString::number(P.getFinal()));
    ui->retornoLB->setText("T. Retorno: "+QString::number(P.getRetorno()));
    ui->respuestaLB->setText("T. Respuesta: "+QString::number(P.getRespuesta()));
    ui->esperaLB->setText("T. Espera: "+QString::number(P.getEspera()));
    ui->servicioLB->setText("T. Servicio:"+QString::number(P.getServicio()));
}
