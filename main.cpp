#include "mainwindow.h"
#include <QApplication>

#include<stdio.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>
#include<dirent.h>
void *deal_fun(void *arg)
{
    int fd =  *(int *)arg;
    while(1)
    {
        char buf[128]="";
        read(fd, buf,sizeof(buf));
        //printf("buf=%s\n", buf);

        char cmd[128]="";
        sscanf(buf,"%[^=]",cmd);
        if(strcmp(cmd,"ANS_PERCENT_POSITION") == 0)//百分比
        {
            int percent_pos = 0;
            sscanf(buf,"%*[^=]=%d", &percent_pos);
            data1.percentage = percent_pos;
            printf("\r当前的百分比为:%%%d \t", percent_pos);
        }
        else if(strcmp(cmd,"ANS_TIME_POSITION") == 0)//当前时间
        {
            float time_pos = 0;
            sscanf(buf,"%*[^=]=%f", &time_pos);
            data1.currcnt_time = time_pos;
            printf("当前的时间为:%02d:%02d \t", (int)(time_pos+0.5)/60, (int)(time_pos+0.5)%60);
        }
        else if(strcmp(cmd,"ANS_LENGTH") == 0)//获取总时间
        {
            float time_sum = 0;
            sscanf(buf,"%*[^=]=%f", &time_sum);
            data1.total_time= time_sum;
            printf("总时间为:%02d:%02d", (int)(time_sum+0.5)/60, (int)(time_sum+0.5)%60);

        }
        fflush(stdout);
    }
}

void *deal_fun2(void *arg)
{
    int fifo_fd = *(int *)arg ;
    //不停的给fifo_cmd发送获取当前时间以及进度
    while(1)
    {
        usleep(500*1000);//0.5秒发指令
        write(fifo_fd,"get_percent_pos\n", strlen("get_percent_pos\n"));
        usleep(500*1000);//0.5秒发指令
        write(fifo_fd,"get_time_pos\n", strlen("get_time_pos\n"));
        usleep(100*100);//0.5秒发指令
        write(fifo_fd,"get_time_length\n", strlen("get_time_length\n"));
    }
    close(fifo_fd);
}
void *deal_fun3(void *arg)
{

    data1.buf = new char*[1024];
    DIR *dir = NULL;
    struct dirent *namelist=NULL;
    dir = opendir("./song");
    if(dir == NULL)
        perror("");
    data1.num = 0;
    while(1)
    {
        namelist = readdir(dir);
        if(namelist == NULL)
            break;
        if(strcmp(namelist->d_name,".")==0 || strcmp(namelist->d_name,"..")==0)
            continue;
        data1.buf[data1.num++] = namelist->d_name;
    }

}

int main(int argc, char *argv[])
{

    //创建一个无名管道 获取mplayer回应
    int fd[2];
    pipe(fd);

    pid_t pid = fork();
    if(pid == 0)//子进程
    {
        //创建一个管道
        mkfifo("fifo_cmd",0666);
        //将1设备重定向到fd[1]
        dup2(fd[1], 1);

        //使用execlp启动mplayer
        execlp("mplayer","mplayer","-idle","-slave","-quiet",\
               "-input","file=./fifo_cmd", "./01.mp3",NULL);
    }
    else//父进程
    {

        //创建一个接受mplayer回应的线程
        pthread_t mplayer_ack;
        pthread_create(&mplayer_ack,NULL,deal_fun , &fd[0]);
        pthread_detach(mplayer_ack);



        //创建一个管道
        mkfifo("fifo_cmd",0666);
        int fifo_fd = open("fifo_cmd",O_WRONLY);
        //创建一个线程 给mplayer发送指令
        pthread_t send_mplayer;
        pthread_create(&send_mplayer,NULL,deal_fun2 , &fifo_fd);
        pthread_detach(send_mplayer);

        //创建一个线程
        pthread_t Song_list;
        pthread_create(&Song_list,NULL,deal_fun3 ,NULL);
        pthread_detach(Song_list);
        //ui设计
        QApplication a(argc, argv);
        MainWindow w;
        w.show();

        return a.exec();
    }

}
