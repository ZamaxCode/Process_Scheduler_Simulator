#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pagesSW->setCurrentIndex(0);
    initializeFramesArray();
    printFrameTable();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//Funciones

void MainWindow::setProcess(const int &totalProcess)
{
    srand(time(NULL));
    for(int i(0); i<totalProcess; ++i)
    {
        Proceso p;
        int op = rand() % 5;
        switch (op) {
            case 0:
                p.setOperation('+');
            break;
            case 1:
                p.setOperation('-');
            break;
            case 2:
                p.setOperation('*');
            break;
            case 3:
                p.setOperation('/');
            break;
            case 4:
                p.setOperation('%');
            break;
        }

        p.setDigit1(rand() % 100);
        if(op==3 || op==4)
            p.setDigit2(rand() % 99+1);
        else
            p.setDigit2(rand() % 99+1);
        p.setId(contActualProcess);
        p.setResult(0);
        p.setTimeMax(rand() % 10+6);
        p.setTt(0);
        p.setLlegada(0);
        p.setFinal(0);
        p.setRetorno(0);
        p.setRespuesta(0);
        p.setEspera(0);
        p.setServicio(0);
        p.setFirst(false);
        p.setBlocked(false);
        p.setError(false);
        p.setExec(false);
        p.setWorking(false);
        p.setContBlocked(5);
        p.setMemory(rand() % 20+5);

        ui->bcpTW->insertRow(pendientList.size());
        pendientList.push_back(p);
        ++contActualProcess;
    }
}

void MainWindow::startProcess()
{
    pendientToReady();

    ui->globalContLB->setText("Contador: "+QString::number(globalCont));
    ui->quantumLB->setText("Quantum: "+QString::number(quantum));

    while(!processList.empty() || !blockedList.empty() || !suspendedList.empty())
    {
        //Se muestra quien es el siguinte proceso en entrar
        if(!pendientList.empty())
            ui->nextProcessLB->setText("Siguiente en entrar: ID - "+QString::number(pendientList.first().getId())+", Mem - "+QString::number(pendientList.first().getMemory()));
        else
            ui->nextProcessLB->setText("Siguiente en entrar: ninguno");

        //Se limpia seccion de pendientes
        QLayoutItem* child;
        while((child = ui->pendientProcessGL->takeAt(0))!=0)
        {
            delete child->widget();
        }
        //Se reimprime seccion de pendientes
        for(int j(0); j<processList.size(); ++j)
        {

            PendientProcess* pproc = new PendientProcess();
            pproc->setData(processList.at(j).getId(), processList.at(j).getTimeMax(), processList.at(j).getTt());
            ui->pendientProcessGL->addWidget(pproc);

            if(processList.at(j).getFirst()==false)
            {
                processList[j].setLlegada(globalCont);
                processList[j].setFirst(true);
            }
        }

        //Se limpia seccion de bloqueados
        while((child = ui->blockedGL->takeAt(0))!=0)
        {
            delete child->widget();
        }
        //Se reimprime seccion de blockeados
        int col=0;
        for(int j(0); j<blockedList.size(); ++j)
        {
            BlockedProcess* bproc = new BlockedProcess();
            bproc->setBlockedProcess(blockedList.at(j).getId(), blockedList.at(j).getContBlocked());
            ui->blockedGL->addWidget(bproc, 0, col,Qt::AlignLeft);
            ++col;
        }
        ui->contOfBatchesLB->setText("Procesos Pendientes: "+QString::number(contOfProcess));


        //Decision de si se ejecuta con normalidad o si se hace el procesos nulo
        if(!processList.empty())
        {
            //Se elimina el primer proceso de pendientes para pasarlo a ejecutados
            child=ui->pendientProcessGL->takeAt(0);
            delete child->widget();

            //Se calcula el tiempo de respuesta
            if(processList.at(0).getExec()==false)
            {
                processList[0].setRespuesta(globalCont-processList.at(0).getLlegada());
                processList[0].setExec(true);
            }

            //Se coloca el estado en trabajando
            processList[0].setWorking(true);

            //Se cambia el estado del proceso a ejecutar en la tabla de frames
            changeFrameState(processList.first(),"Ejecucion");

            //Se inicializa el tiempo restante del proceso
            int tr=processList.at(0).getTimeMax()-processList.at(0).getTt();

            //Se imprime en la seccion de ejecucion
            ui->nameLB->setText("ID: "+QString::number(processList.at(0).getId()));
            ui->opLB->setText("Operacion: "+QString::number(processList.at(0).getDigit1())+processList.at(0).getOperation()+QString::number(processList.at(0).getDigit2()));
            ui->tmLB->setText("Tiempo Maximo: "+QString::number(processList.at(0).getTimeMax()));
            ui->ttLB->setText("Tiempo Transcurrido: "+QString::number(processList.at(0).getTt()));
            ui->trLB->setText("Tiempo Restante: "+QString::number(tr));
            ui->quantumContLB->setText("Quantum Cont: "+QString::number(0));

            //Se realiza la ejecucion del proceso
            int k;
            for(k=0; k<quantum && processList.at(0).getTt()<processList.at(0).getTimeMax(); ++k)
            {
                delay(1000);

                processList[0].setTt(processList.at(0).getTt()+1);
                ui->ttLB->setText("Tiempo Transcurrido: "+QString::number(processList.at(0).getTt()));
                ui->trLB->setText("Tiempo Restante: "+QString::number(--tr));

                ++globalCont;

                ui->globalContLB->setText("Contador: "+QString::number(globalCont));
                ui->quantumContLB->setText("Quantum Cont: "+QString::number(k+1));

                //Se recorre la lista de bloqueados para disminuir su contador
                for(int n(0); n<blockedList.size(); ++n)
                {
                    blockedList[n].setContBlocked(blockedList.at(n).getContBlocked()-1);

                    //Si el proceso ya termino su tiempo en bloqueado, se regresa a listos
                    if(blockedList.at(n).getContBlocked()==0)
                    {
                        //Se regresa el estado a listo en la tabla de frames
                        changeFrameState(blockedList.at(n), "Listo");

                        //Reseteamos valores y regresamos a la lista de listos
                        blockedList[n].setContBlocked(5);
                        blockedList[n].setBlocked(false);
                        processList.push_back(blockedList.at(n));
                        blockedList.removeAt(n);
                        --n;
                        //Se reimprime en la seccion de listos
                        PendientProcess* pproc = new PendientProcess();
                        pproc->setData(processList.back().getId(), processList.back().getTimeMax(), processList.back().getTt());
                        ui->pendientProcessGL->addWidget(pproc);
                    }
                }

                //Se limpia la seccion de bloquedos
                while((child = ui->blockedGL->takeAt(0))!=0)
                {
                    delete child->widget();
                }

                //Se reimprime la seccion de bloqueados para mostrar los cambios en los contadores
                int column=0;
                for(int n(0); n<blockedList.size(); ++n)
                {
                    BlockedProcess* bproc = new BlockedProcess();
                    bproc->setBlockedProcess(blockedList.at(n).getId(), blockedList.at(n).getContBlocked());
                    ui->blockedGL->addWidget(bproc, 0, column, Qt::AlignLeft);
                    ++column;
                }

                //Validacion para poder realizar interrupciones y errores
                if((interFlag||errorFlag) && processList.first().getTt()<processList.first().getTimeMax())
                    break;
                else
                {
                    interFlag=false;
                    errorFlag=false;
                }

                //Pausa y Continuar
                if(pauseFlag)
                {
                    while(true)
                    {
                        delay(100);
                        if(continueFlag)
                            break;
                    }
                    pauseFlag=false;
                    continueFlag=false;
                }

                //Tabla de bcp y continuar
                if(bcpFlag)
                {
                    setBCP();
                    ui->pagesSW->setCurrentIndex(3);
                    while(true)
                    {
                        delay(100);
                        if(continueFlag)
                        {
                            ui->pagesSW->setCurrentIndex(1);
                            break;
                        }
                    }
                    bcpFlag=false;
                    continueFlag=false;
                }

                //Generar nuevo proceso
                if(newFlag)
                {
                    setProcess(1);
                    ++contOfProcess;

                    //Si la lista de pendientes es igual a 1, significa que el proceso quiza pueda entrar
                    if(pendientList.size()==1)
                    {
                        pendientToReady();

                        //Si la lista de pendientes vuelve a estar vacia, entonces se imprime el nuevo proceso
                        if(pendientList.empty())
                        {
                            PendientProcess* pproc = new PendientProcess();
                            pproc->setData(processList.back().getId(), processList.back().getTimeMax(), processList.back().getTt());
                            ui->pendientProcessGL->addWidget(pproc);
                            processList[processList.size()-1].setLlegada(globalCont);
                            processList[processList.size()-1].setFirst(true);
                        }
                    }
                    //Se actualiza el contador de procesos pendientes y se apaga la bandera de nuevo proceso
                    ui->contOfBatchesLB->setText("Procesos Pendientes: "+QString::number(contOfProcess));
                    newFlag=false;

                    //Se actualiza la etiqueta de nextProcess
                    if(!pendientList.empty())
                        ui->nextProcessLB->setText("Siguiente en entrar: ID - "+QString::number(pendientList.first().getId())+", Mem - "+QString::number(pendientList.first().getMemory()));
                    else
                        ui->nextProcessLB->setText("Siguiente en entrar: ninguno");
                }

                if(suspendFlag)
                {
                    if(!blockedList.empty())
                    {
                        changeFrameState(blockedList.first(),"Libre");
                        blockedList.first().setContBlocked(5);
                        blockedList.first().PosMemory.clear();
                        suspendedList.push_back(blockedList.first());
                        blockedList.removeFirst();

                        writeSuspendedFile();

                        ui->contSuspendedLB->setText("Procesos Suspendidos: " + QString::number(suspendedList.size()));
                        ui->nextSuspendedLB->setText("Siguiente Suspendido en entrar: " + QString::number(suspendedList.first().getId())+", Mem - "+QString::number(suspendedList.first().getMemory()));

                        child = ui->blockedGL->takeAt(0);
                        delete child->widget();

                        int frames = suspendedList.back().getMemory()/4;
                        if(suspendedList.back().getMemory()%4 > 0)
                            ++frames;

                        contAvailableFrames += frames;

                        pendientToReady();

                        ui->contOfBatchesLB->setText("Procesos Pendientes: "+QString::number(contOfProcess));

                        if(!pendientList.empty())
                            ui->nextProcessLB->setText("Siguiente en entrar: ID - "+QString::number(pendientList.first().getId())+", Mem - "+QString::number(pendientList.first().getMemory()));
                        else
                            ui->nextProcessLB->setText("Siguiente en entrar: ninguno");

                    }
                    suspendFlag=false;
                }

                if(returnFlag)
                {
                    if(!suspendedList.empty())
                    {
                        int frames = suspendedList.first().getMemory()/4;
                        if(suspendedList.first().getMemory()%4 > 0)
                            ++frames;

                        if(frames<=contAvailableFrames)
                        {
                            blockedList.push_back(suspendedList.first());
                            suspendedList.removeFirst();
                            contAvailableFrames-=frames;

                            insertFrame(blockedList.back());

                            changeFrameState(blockedList.back(), "Bloqueado");

                            printFrameTable();

                            writeSuspendedFile();

                            ui->contSuspendedLB->setText("Procesos Suspendidos: " + QString::number(suspendedList.size()));
                            if(!suspendedList.empty())
                                ui->nextSuspendedLB->setText("Siguiente Suspendido en entrar: " + QString::number(suspendedList.first().getId())+", Mem - "+QString::number(suspendedList.first().getMemory()));
                            else
                                ui->nextSuspendedLB->setText("Siguiente Suspendido en entrar: ninguno");
                        }
                    }
                    returnFlag=false;
                }
            }
            //Se apaga el estado de trabajando en el proceso
            processList[0].setWorking(false);

            //Se revisa si el proceso termino
            if(processList.first().getTt()==processList.first().getTimeMax() || k<quantum)
            {
                //Caso donde no hubo interrupcion
                if(!interFlag)
                {
                    FinishProcess* fproc = new FinishProcess();
                    //Caso donde no hubo error
                    if(!errorFlag)
                    {
                        processList[0].setResult(resolveProcess(processList.first().getDigit1(), processList.first().getDigit2(), processList.first().getOperation()));
                        fproc->setData(processList.first().getId(), QString::number(processList.first().getDigit1())+processList.first().getOperation()+QString::number(processList.first().getDigit2()), QString::number(processList.first().getResult()));
                    }
                    //Caso donde si hubo error
                    else
                    {
                        fproc->setData(processList.first().getId(), QString::number(processList.first().getDigit1())+processList.first().getOperation()+QString::number(processList.first().getDigit2()), "Error");
                        errorFlag=false;
                        processList[0].setError(true);
                    }
                    //Se calculan los tiempos del proceso
                    processList[0].setFinal(globalCont);
                    processList[0].setRetorno(processList.first().getFinal()-processList.first().getLlegada());
                    processList[0].setServicio(processList.first().getTt());
                    processList[0].setEspera(processList.first().getRetorno()-processList.first().getServicio());

                    //Se pasa el proceso de listos a terminados
                    finishedList.push_back(processList.first());
                    processList.pop_front();

                    //Se aumenta el contador de frames puesto que un proceso va a salir
                    int frames = finishedList.back().getMemory()/4;
                    if(finishedList.back().getMemory()%4 > 0)
                        ++frames;
                    contAvailableFrames+=frames;

                    //Se cambia el estado de los frames del proceso a libre
                    changeFrameState(finishedList.back(), "Libre");

                    //Se revisa si es que pueden entrar nuevos procesos
                    pendientToReady();

                    //Se imprime el proceso en la seccion de terminados
                    ui->finishProcessGL->addWidget(fproc);
                }
                //Caso donde si hubo interrupcion
                else
                {
                    processList[0].setBlocked(true);
                    blockedList.push_back(processList.first());
                    processList.pop_front();
                    interFlag=false;

                    //Se coloca el estado de bloquedo en la tabla de frames
                    changeFrameState(blockedList.back(), "Bloqueado");
                }
            }
            //Si el proceso no termino, entonces se vuelve a insertar en la cola de listos
            else
            {
                PendientProcess* pproc = new PendientProcess();
                pproc->setData(processList.first().getId(), processList.first().getTimeMax(), processList.first().getTt());
                ui->pendientProcessGL->addWidget(pproc);
                processList.move(0,processList.size()-1);

                //Se regresa el estado a listo en la tabla de frames
                changeFrameState(processList.back(), "Listo");
            }
        }
        //Proceso nulo
        else
        {
            //Imprimimos etiquetas vacias
            ui->nameLB->setText("ID: -");
            ui->opLB->setText("Operacion: -");
            ui->tmLB->setText("Tiempo Maximo: -");
            ui->ttLB->setText("Tiempo Transcurrido: -");
            ui->trLB->setText("Tiempo Restante: -");
            ui->quantumContLB->setText("Quantum Cont: -");

            //Hacemos delay y aumentamos contador global
            delay(1000);
            ++globalCont;
            ui->globalContLB->setText("Contador: "+QString::number(globalCont));

            //Se recorre la lista de bloqueados para disminuir su contador
            for(int n(0); n<blockedList.size(); ++n)
            {
                blockedList[n].setContBlocked(blockedList.at(n).getContBlocked()-1);

                //Si el proceso ya termino su tiempo en bloqueado, se regresa a listos
                if(blockedList.at(n).getContBlocked()==0)
                {
                    //Se regresa el estado a listo en la tabla de frames
                    changeFrameState(blockedList.at(n), "Listo");

                    //Reseteamos valores y regresamos a la lista de listos
                    blockedList[n].setContBlocked(5);
                    blockedList[n].setBlocked(false);
                    processList.push_back(blockedList.at(n));
                    blockedList.removeAt(n);
                    --n;
                    //Se reimprime en la seccion de listos
                    PendientProcess* pproc = new PendientProcess();
                    pproc->setData(processList.back().getId(), processList.back().getTimeMax(), processList.back().getTt());
                    ui->pendientProcessGL->addWidget(pproc);
                }
            }

            //Se limpia la seccion de bloquedos
            while((child = ui->blockedGL->takeAt(0))!=0)
            {
                delete child->widget();
            }

            //Se reimprime la seccion de bloqueados para mostrar los cambios en los contadores
            int column=0;
            for(int n(0); n<blockedList.size(); ++n)
            {
                BlockedProcess* bproc = new BlockedProcess();
                bproc->setBlockedProcess(blockedList.at(n).getId(), blockedList.at(n).getContBlocked());
                ui->blockedGL->addWidget(bproc, 0, column, Qt::AlignLeft);
                ++column;
            }

            //Pausa y Continuar
            if(pauseFlag)
            {
                while(true)
                {
                    delay(100);
                    if(continueFlag)
                        break;
                }
                pauseFlag=false;
                continueFlag=false;
            }

            //Tabla de bcp y continuar
            if(bcpFlag)
            {
                setBCP();
                ui->pagesSW->setCurrentIndex(3);
                while(true)
                {
                    delay(100);
                    if(continueFlag)
                    {
                        ui->pagesSW->setCurrentIndex(1);
                        break;
                    }
                }
                bcpFlag=false;
                continueFlag=false;
            }

            //Generar nuevo proceso
            if(newFlag)
            {
                setProcess(1);
                ++contOfProcess;

                //Si la lista de pendientes es igual a 1, significa que el proceso quiza pueda entrar
                if(pendientList.size()==1)
                {
                    pendientToReady();

                    //Si la lista de pendientes vuelve a estar vacia, entonces se imprime el nuevo proceso
                    if(pendientList.empty())
                    {
                        PendientProcess* pproc = new PendientProcess();
                        pproc->setData(processList.back().getId(), processList.back().getTimeMax(), processList.back().getTt());
                        ui->pendientProcessGL->addWidget(pproc);
                        processList[processList.size()-1].setLlegada(globalCont);
                        processList[processList.size()-1].setFirst(true);
                    }
                }
                //Se actualiza el contador de procesos pendientes y se apaga la bandera de nuevo proceso
                ui->contOfBatchesLB->setText("Procesos Pendientes: "+QString::number(contOfProcess));
                newFlag=false;

                //Se actualiza la etiqueta de nextProcess
                if(!pendientList.empty())
                    ui->nextProcessLB->setText("Siguiente en entrar: ID - "+QString::number(pendientList.first().getId())+", Mem - "+QString::number(pendientList.first().getMemory()));
                else
                    ui->nextProcessLB->setText("Siguiente en entrar: ninguno");
            }

            if(suspendFlag)
            {
                if(!blockedList.empty())
                {
                    changeFrameState(blockedList.first(),"Libre");
                    blockedList.first().setContBlocked(5);
                    blockedList.first().PosMemory.clear();
                    suspendedList.push_back(blockedList.first());
                    blockedList.removeFirst();

                    writeSuspendedFile();

                    ui->contSuspendedLB->setText("Procesos Suspendidos: " + QString::number(suspendedList.size()));
                    ui->nextSuspendedLB->setText("Siguiente Suspendido en entrar: " + QString::number(suspendedList.first().getId())+", Mem - "+QString::number(suspendedList.first().getMemory()));

                    child = ui->blockedGL->takeAt(0);
                    delete child->widget();

                    int frames = suspendedList.back().getMemory()/4;
                    if(suspendedList.back().getMemory()%4 > 0)
                        ++frames;

                    contAvailableFrames += frames;

                    pendientToReady();

                    ui->contOfBatchesLB->setText("Procesos Pendientes: "+QString::number(contOfProcess));

                    if(!pendientList.empty())
                        ui->nextProcessLB->setText("Siguiente en entrar: ID - "+QString::number(pendientList.first().getId())+", Mem - "+QString::number(pendientList.first().getMemory()));
                    else
                        ui->nextProcessLB->setText("Siguiente en entrar: ninguno");

                }
                suspendFlag=false;
            }

            if(returnFlag)
            {
                if(!suspendedList.empty())
                {
                    int frames = suspendedList.first().getMemory()/4;
                    if(suspendedList.first().getMemory()%4 > 0)
                        ++frames;

                    if(frames<=contAvailableFrames)
                    {
                        blockedList.push_back(suspendedList.first());
                        suspendedList.removeFirst();
                        contAvailableFrames-=frames;

                        insertFrame(blockedList.back());

                        changeFrameState(blockedList.back(), "Bloqueado");

                        printFrameTable();

                        writeSuspendedFile();

                        ui->contSuspendedLB->setText("Procesos Suspendidos: " + QString::number(suspendedList.size()));
                        if(!suspendedList.empty())
                            ui->nextSuspendedLB->setText("Siguiente Suspendido en entrar: " + QString::number(suspendedList.first().getId())+", Mem - "+QString::number(suspendedList.first().getMemory()));
                        else
                            ui->nextSuspendedLB->setText("Siguiente Suspendido en entrar: ninguno");
                    }
                }
                returnFlag=false;
            }
        }
    }

    //Se ponen las etiquetas en blanco
    ui->nameLB->setText("ID: -");
    ui->opLB->setText("Operacion: -");
    ui->tmLB->setText("Tiempo Maximo: -");
    ui->ttLB->setText("Tiempo Transcurrido: -");
    ui->trLB->setText("Tiempo Restante: -");
    ui->quantumContLB->setText("Quantum Cont: -");
    //Se muestra la alerta de que ya termino la simulacion
    QMessageBox msg;
    msg.setText("Simulacion Terminada!");
    msg.exec();
    ui->finishPB->setEnabled(true);
}

void MainWindow::delay(const int &mSeconds)
{
    QTime dieTime=QTime::currentTime().addMSecs(mSeconds);
    while(QTime::currentTime()<dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

int MainWindow::resolveProcess(const int &d1, const int &d2, const char op)
{
    int res = 0;
    switch (op) {
        case '+':
            res = d1+d2;
            break;

        case '-':
            res = d1-d2;
            break;

        case '*':
            res = d1*d2;
            break;

        case '/':
            res = d1/d2;
            break;

        case '%':
            res = d1%d2;
            break;
    }
    return res;
}

void MainWindow::printResults()
{
    for(int i(0);i<finishedList.size();++i)
    {
        ResultProcess* rproc = new ResultProcess();
        rproc->setResultProcess(finishedList.at(i));
        ui->resultsGL->addWidget(rproc, i/5, i%5,Qt::AlignCenter);
    }
}

void MainWindow::setBCP()
{
    totalProcessList += pendientList;
    totalProcessList += processList;
    totalProcessList += blockedList;
    totalProcessList += finishedList;
    for(int b(0);b<totalProcessList.size();++b)
    {
        ui->bcpTW->setItem(b,0,new QTableWidgetItem(QString::number(totalProcessList.at(b).getId())));

        if(totalProcessList.at(b).getFirst())
        {
            ui->bcpTW->setItem(b,2,new QTableWidgetItem(QString::number(totalProcessList.at(b).getDigit1())+totalProcessList.at(b).getOperation()+QString::number(totalProcessList.at(b).getDigit2())));
            ui->bcpTW->setItem(b,4,new QTableWidgetItem(QString::number(totalProcessList.at(b).getLlegada())));

            if(totalProcessList.at(b).getFinal()>0)
            {
                ui->bcpTW->setItem(b,1,new QTableWidgetItem("Finalizado"));

                if(totalProcessList.at(b).getError())
                    ui->bcpTW->setItem(b,3,new QTableWidgetItem("Error"));
                else
                    ui->bcpTW->setItem(b,3,new QTableWidgetItem(QString::number(totalProcessList.at(b).getResult())));

                ui->bcpTW->setItem(b,5,new QTableWidgetItem(QString::number(totalProcessList.at(b).getFinal())));
                ui->bcpTW->setItem(b,6,new QTableWidgetItem(QString::number(totalProcessList.at(b).getRetorno())));
                ui->bcpTW->setItem(b,7,new QTableWidgetItem(QString::number(totalProcessList.at(b).getEspera())));
                ui->bcpTW->setItem(b,9,new QTableWidgetItem("0"));
                ui->bcpTW->setItem(b,11,new QTableWidgetItem("-"));

            }
            else
            {
                ui->bcpTW->setItem(b,3,new QTableWidgetItem("-"));
                ui->bcpTW->setItem(b,5,new QTableWidgetItem("-"));
                ui->bcpTW->setItem(b,6,new QTableWidgetItem("-"));
                ui->bcpTW->setItem(b,7,new QTableWidgetItem(QString::number(globalCont-totalProcessList.at(b).getLlegada()-totalProcessList.at(b).getTt())));
                ui->bcpTW->setItem(b,9,new QTableWidgetItem(QString::number(totalProcessList.at(b).getTimeMax()-totalProcessList.at(b).getTt())));

                if(totalProcessList.at(b).getBlocked())
                {
                    ui->bcpTW->setItem(b,1,new QTableWidgetItem("Bloqueado"));
                    ui->bcpTW->setItem(b,11,new QTableWidgetItem(QString::number(totalProcessList.at(b).getContBlocked())));
                }
                else
                {
                    ui->bcpTW->setItem(b,1,new QTableWidgetItem("Listo"));
                    ui->bcpTW->setItem(b,11,new QTableWidgetItem("-"));
                }
            }
            ui->bcpTW->setItem(b,8,new QTableWidgetItem(QString::number(totalProcessList.at(b).getTt())));

            if(totalProcessList.at(b).getExec())
            {
                ui->bcpTW->setItem(b,10,new QTableWidgetItem(QString::number(totalProcessList.at(b).getRespuesta())));
                if(totalProcessList.at(b).getWorking())
                    ui->bcpTW->setItem(b,1,new QTableWidgetItem("Ejecucion"));
            }
            else
                ui->bcpTW->setItem(b,10,new QTableWidgetItem("-"));
        }

        else
        {
            ui->bcpTW->setItem(b,1,new QTableWidgetItem("Pendiente"));
            ui->bcpTW->setItem(b,2,new QTableWidgetItem("-"));
            ui->bcpTW->setItem(b,3,new QTableWidgetItem("-"));
            ui->bcpTW->setItem(b,4,new QTableWidgetItem("-"));
            ui->bcpTW->setItem(b,5,new QTableWidgetItem("-"));
            ui->bcpTW->setItem(b,6,new QTableWidgetItem("-"));
            ui->bcpTW->setItem(b,7,new QTableWidgetItem("-"));
            ui->bcpTW->setItem(b,8,new QTableWidgetItem("-"));
            ui->bcpTW->setItem(b,9,new QTableWidgetItem("-"));
            ui->bcpTW->setItem(b,10,new QTableWidgetItem("-"));
            ui->bcpTW->setItem(b,11,new QTableWidgetItem("-"));
        }
    }
    ui->bcpTW->resizeColumnsToContents();
    totalProcessList.clear();
}

void MainWindow::initializeFramesArray()
{
    //Llena el arreglo de frames con el estado de Libre;
    Frame f;
    for(int i(0); i<42; ++i)
    {
        f.setState("Libre");
        framesArray.push_back(f);
    }
}

void MainWindow::printFrameTable()
{
    //Imprimimos los frames de los procesos
    int i;
    for(i=0; i<framesArray.size(); ++i)
    {
        //Se revisa si el estado es igual a Libre para poner valores nulos
        if(framesArray.at(i).getState()=="Libre")
        {
            ui->frameTable->setItem(i,0,new QTableWidgetItem("-"));
            ui->frameTable->item(i,0)->setBackground(Qt::white);
            ui->frameTable->setItem(i,1,new QTableWidgetItem("-"));
            ui->frameTable->item(i,1)->setBackground(Qt::white);
            ui->frameTable->setItem(i,2,new QTableWidgetItem("Libre"));
            ui->frameTable->item(i,2)->setBackground(Qt::white);

        }
        //Si el estado no es libre, se ingresan los valores del proceso
        else
        {
            ui->frameTable->setItem(i,0,new QTableWidgetItem(QString::number(framesArray.at(i).getId())));
            ui->frameTable->setItem(i,1,new QTableWidgetItem(QString::number(framesArray.at(i).getNumPages())+"/4"));
            ui->frameTable->setItem(i,2,new QTableWidgetItem(framesArray.at(i).getState()));
            //Se cambian los colores dependiendo el estado
            if(framesArray.at(i).getState()=="Listo")
            {
                ui->frameTable->item(i,0)->setBackground(Qt::yellow);
                ui->frameTable->item(i,1)->setBackground(Qt::yellow);
                ui->frameTable->item(i,2)->setBackground(Qt::yellow);
            }
            else
                if(framesArray.at(i).getState()=="Ejecucion")
                {
                    ui->frameTable->item(i,0)->setBackground(Qt::red);
                    ui->frameTable->item(i,1)->setBackground(Qt::red);
                    ui->frameTable->item(i,2)->setBackground(Qt::red);
                }
                else
                    if(framesArray.at(i).getState()=="Bloqueado")
                    {
                        ui->frameTable->item(i,0)->setBackground(Qt::magenta);
                        ui->frameTable->item(i,1)->setBackground(Qt::magenta);
                        ui->frameTable->item(i,2)->setBackground(Qt::magenta);
                    }

        }
    }

    //imprimimos los frames del SO
    ui->frameTable->setItem(i,0,new QTableWidgetItem("SO"));
    ui->frameTable->item(i,0)->setBackground(Qt::green);
    ui->frameTable->setItem(i,1,new QTableWidgetItem("SO"));
    ui->frameTable->item(i,1)->setBackground(Qt::green);
    ui->frameTable->setItem(i,2,new QTableWidgetItem("SO"));
    ui->frameTable->item(i,2)->setBackground(Qt::green);
    ++i;
    ui->frameTable->setItem(i,0,new QTableWidgetItem("SO"));
    ui->frameTable->item(i,0)->setBackground(Qt::green);
    ui->frameTable->setItem(i,1,new QTableWidgetItem("SO"));
    ui->frameTable->item(i,1)->setBackground(Qt::green);
    ui->frameTable->setItem(i,2,new QTableWidgetItem("SO"));
    ui->frameTable->item(i,2)->setBackground(Qt::green);
    ++i;
    ui->frameTable->setItem(i,0,new QTableWidgetItem("SO"));
    ui->frameTable->item(i,0)->setBackground(Qt::green);
    ui->frameTable->setItem(i,1,new QTableWidgetItem("SO"));
    ui->frameTable->item(i,1)->setBackground(Qt::green);
    ui->frameTable->setItem(i,2,new QTableWidgetItem("SO"));
    ui->frameTable->item(i,2)->setBackground(Qt::green);

    //Se ajusta la tabla
    ui->frameTable->resizeColumnsToContents();
    ui->frameTable->resizeRowsToContents();
}

void MainWindow::pendientToReady()
{
    //Mientras la lista de pendientes no esté vacia, va a revisar si pueden entrar nuevos procesos
    while(!pendientList.empty())
    {
        //Se calculan los frames que ocupa el proceso
        int contFrames=pendientList.first().getMemory()/4;
        if(pendientList.first().getMemory()%4 > 0)
            ++contFrames;

        //si la cantidad de frames del proceso es menor o igual a la cantidad de procesos disponibles, entonces entra
        if(contFrames<=contAvailableFrames)
        {
            processList.push_back(pendientList.first());
            pendientList.removeFirst();
            --contOfProcess;
            contAvailableFrames-=contFrames;
            //Se insertan los frames del proceso a la lista de frames
            insertFrame(processList.back());

            PendientProcess* pproc = new PendientProcess();
            pproc->setData(processList.back().getId(), processList.back().getTimeMax(), processList.back().getTt());
            ui->pendientProcessGL->addWidget(pproc);
        }
        //Si no hay espacio, se rompe el ciclo
        else
            break;
    }
    //Se imprimen los frames en la tabla
    printFrameTable();
}

void MainWindow::insertFrame(Proceso &p)
{
    //se pasa el valor de memoria del proceso a una variable llamada pages
    int pages=p.getMemory();
    //Se recorre la lista de frames para encontrar frames disponibles
    for(int i(0); i<framesArray.size(); ++i)
    {
        //Si el estado del frame es libre, entonces entra el frame del proceso
        if(framesArray.at(i).getState()=="Libre")
        {
            framesArray[i].setId(p.getId());
            framesArray[i].setState("Listo");
            //Si las paginas son mayores o iguales a 4, se coloca el valor de 4 paginas y se disminuye en 4 la variable
            if(pages>=4)
            {
                framesArray[i].setNumPages(4);
                //Se guarda la posicion del frame en el arreglo memoria del proceso
                p.PosMemory.push_back(i);
                pages-=4;

            }
            //De no ser el caso, se revisa si es mayor a 0 y coloca ese valor de paginas restantes al ultimo frame y se disminuye la variable
            else if(pages>0)
            {
                framesArray[i].setNumPages(pages);
                //Se guarda la posicion del frame en el arreglo memoria del proceso
                p.PosMemory.push_back(i);
                pages-=pages;
            }
        }
        //si las paginas ya son iguales a 0, termina el ciclo
        if(pages==0)
            break;
    }
}

void MainWindow::changeFrameState(const Proceso &p, const QString state)
{
    //Se recorre el alrreglo de posiciones de memoria del proceso
    for(int i(0); i<p.PosMemory.size(); ++i)
    {
        //Se pasa la posicion a una variable
        int pos = p.PosMemory.at(i);
        //Se cambia el estado del frame correspondiente
        framesArray[pos].setState(state);
    }
    //Se reimprimen los frames en la tabla
    printFrameTable();
}

void MainWindow::writeSuspendedFile()
{
    QFile suspendedFile;
    suspendedFile.setFileName("D:/Escritorio/Proyecto ZamaxSO/Suspendidos/Styles/suspended.txt");
    suspendedFile.open(QIODevice::ReadWrite | QIODevice::Text);

    suspendedFile.resize(0);

    QByteArray dataFile = "Id / Memoria / Digito 1 / Operacion / Digito 2 / Tiempo Max / T. Transcurrido\n";

    for (int i(0); i<suspendedList.size(); ++i)
        dataFile += QString::number(suspendedList.at(i).getId()) +
                " / " + QString::number(suspendedList.at(i).getMemory()) +
                " / " + QString::number(suspendedList.at(i).getDigit1()) +
                " / " + suspendedList.at(i).getOperation() +
                " / " + QString::number(suspendedList.at(i).getDigit2()) +
                " / " + QString::number(suspendedList.at(i).getTt()) +
                " / " + QString::number(suspendedList.at(i).getTimeMax()) + "\n";

    suspendedFile.write(dataFile);
    suspendedFile.close();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(!ui->startPB->isEnabled())
    {
        if(event->key()==Qt::Key_N && pauseFlag==false && bcpFlag==false)
            newFlag=true;
        else if(event->key()==Qt::Key_I && pauseFlag==false && bcpFlag==false)
            interFlag=true;
        else if(event->key()==Qt::Key_E && pauseFlag==false && bcpFlag==false)
            errorFlag=true;
        else if(event->key()==Qt::Key_P && bcpFlag==false)
            pauseFlag=true;
        else if(event->key()==Qt::Key_A && bcpFlag==false)
            pauseFlag=true;
        else if(event->key()==Qt::Key_B && pauseFlag==false)
            bcpFlag=true;
        else if(event->key()==Qt::Key_C && (pauseFlag==true || bcpFlag==true))
            continueFlag=true;
        else if(event->key()==Qt::Key_S && (pauseFlag==false || bcpFlag==false))
            suspendFlag=true;
        else if(event->key()==Qt::Key_R && (pauseFlag==false || bcpFlag==false))
            returnFlag=true;
    }
}

//Slots

void MainWindow::on_numOfProcessPB_clicked()
{
    contOfProcess=ui->numOfProcessLE->text().toInt();
    quantum=ui->insertQuantumLE->text().toInt();
    setProcess(contOfProcess);
    ui->pagesSW->setCurrentIndex(1);
}

void MainWindow::on_numOfProcessLE_textChanged(const QString &arg1)
{
    if(arg1.toInt()>0 && ui->insertQuantumLE->text().toInt()>0)
        ui->numOfProcessPB->setEnabled(true);
    else
        ui->numOfProcessPB->setEnabled(false);
}

void MainWindow::on_insertQuantumLE_textChanged(const QString &arg1)
{
    if(arg1.toInt()>0 && ui->numOfProcessLE->text().toInt()>0)
        ui->numOfProcessPB->setEnabled(true);
    else
        ui->numOfProcessPB->setEnabled(false);
}

void MainWindow::on_startPB_clicked()
{
    ui->startPB->setEnabled(false);
    startProcess();
}

void MainWindow::on_finishPB_clicked()
{
    printResults();
    ui->pagesSW->setCurrentIndex(2);
}
