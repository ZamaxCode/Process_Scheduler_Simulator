#ifndef PENDIENTPROCESS_H
#define PENDIENTPROCESS_H

#include <QWidget>
#include <QString>

namespace Ui {
class PendientProcess;
}

class PendientProcess : public QWidget
{
    Q_OBJECT

public:
    explicit PendientProcess(QWidget *parent = nullptr);
    ~PendientProcess();
    void setData(const int &id, const int &time, const int &tt);

private:
    Ui::PendientProcess *ui;
};

#endif // PENDIENTPROCESS_H
