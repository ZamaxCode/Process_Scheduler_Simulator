#ifndef RESULTPROCESS_H
#define RESULTPROCESS_H

#include <QWidget>
#include "proceso.h"

namespace Ui {
class ResultProcess;
}

class ResultProcess : public QWidget
{
    Q_OBJECT

public:
    explicit ResultProcess(QWidget *parent = nullptr);
    ~ResultProcess();
    void setResultProcess(Proceso P);

private:
    Ui::ResultProcess *ui;
};

#endif // RESULTPROCESS_H
