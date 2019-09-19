#include "mythread.h"
#include<QThread>
myThread::myThread(QObject *parent) : QObject(parent)
{
    isStop = false;
}

void myThread::myTimeout()
{
    while(isStop == false)
    {
        QThread::usleep(500*100);
        emit mySignal();
        if(isStop)
            break;
    }
}

void myThread::setFlag(bool flag)
{
    isStop = flag;
}
