#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QPushButton>
#include<iostream>


using namespace std;
DATA data1;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    myT = new myThread;

    thread = new QThread(this);
    myT->moveToThread(thread);
    connect(myT,&myThread::mySignal,this,&MainWindow::dealSignal);

    connect(this,&MainWindow::startThread,myT,&myThread::myTimeout);
}
void MainWindow::dealSignal()
{
    cout<<"jjj"<<endl;

    //ui->progress->setValue(data1.percentage);

    //cout<<(data1.buf[0])<<endl;

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    //cout<<"next mp3"<<endl;
    thread->start();
    myT->setFlag(false);
    emit startThread();
}

void MainWindow::on_pushButton_2_clicked()
{
    myT->setFlag(true);
    thread->quit();
    thread->wait();
    cout<<data1.percentage<<endl;
        cout<<data1.currcnt_time<<endl;
        cout<<data1.total_time<<endl;
}
