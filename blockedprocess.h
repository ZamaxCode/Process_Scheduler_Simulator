#ifndef BLOCKEDPROCESS_H
#define BLOCKEDPROCESS_H

#include <QWidget>

namespace Ui {
class BlockedProcess;
}

class BlockedProcess : public QWidget
{
    Q_OBJECT

public:
    explicit BlockedProcess(QWidget *parent = nullptr);
    ~BlockedProcess();
    void setBlockedProcess(const int id, const int cont);

private:
    Ui::BlockedProcess *ui;
};

#endif // BLOCKEDPROCESS_H
