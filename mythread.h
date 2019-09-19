#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QObject>

class myThread : public QObject
{
    Q_OBJECT
public:
    explicit myThread(QObject *parent = nullptr);

    void myTimeout();
    void setFlag(bool flag = true);

signals:
    void mySignal();
private:
    bool isStop;
public slots:
};

#endif // MYTHREAD_H
