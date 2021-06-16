#ifndef FRAME_H
#define FRAME_H
#include <QString>

class Frame
{
public:
    Frame();

    int getId() const;
    void setId(int newId);

    int getNumPages() const;
    void setNumPages(int newNumPages);

    const QString &getState() const;
    void setState(const QString &newState);

private:
    int id;
    int numMem;
    QString state;

};

#endif // FRAME_H
