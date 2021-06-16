#ifndef FINISHPROCESS_H
#define FINISHPROCESS_H

#include <QWidget>

namespace Ui {
class FinishProcess;
}

class FinishProcess : public QWidget
{
    Q_OBJECT

public:
    explicit FinishProcess(QWidget *parent = nullptr);
    ~FinishProcess();
    void setData(const int &id, const QString &operation, const QString &result);

private:
    Ui::FinishProcess *ui;
};

#endif // FINISHPROCESS_H
