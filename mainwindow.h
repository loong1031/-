#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mythread.h"
#include<QThread>
#include<QString>
typedef struct
{
    char **buf;
    int num;
    int percentage;
    float currcnt_time;
    float total_time;
    int i;
    char control_plan[128];
    int control_plan_i;

}DATA;
extern DATA data1;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void dealSignal();
public slots:
    void on_pushButton_clicked();

private slots:
    void on_pushButton_2_clicked();
signals:
    void startThread();
private:
    Ui::MainWindow *ui;
    myThread *myT;
    QThread *thread;
};

#endif // MAINWINDOW_H
