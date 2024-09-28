#include "Video.h"

int  VideoInit() //初始化
{
    //判断是否存在管道文件， 不存在才创建
    if (access("/fifo", F_OK)) //默认管道文件创建在根目录 F_OK 判断是否存在
    {
        //如果条件成立，不存在
        mkfifo("/fifo", 777);//创建管道文件
    }

    fd_fifo = open("/fifo", O_RDWR);
    if (fd_fifo == -1)
    {
        printf("打开管道文件失败！\n");
        return -1;
    }

    return 0;
}

int  VideoFree() //资源回收
{
    system("killall -9 mplayer");
    show_bmp("/mnt/udisk/ui/main_ui/MainUi.bmp",0,0,0);//显示主界面，没改{没改}
    posx = -1;
    posy = -1;


    return 0;
}

int  SendCmd(char *cmd)//发送命令,不能用来杀死进程或者启动进程
{
    write(fd_fifo, cmd, strlen(cmd));
    return 0;
}

int StartVideo() //启动视频
{   
    doubleLinklist *video_list=NULL;  
    char buf[300] ;
    int pause_flag = 1;//static 关键字的目的是为了保持变量的状态，即使函数退出，该变量的值也会被保留
    getPathsByIO("/mdata/video",&video_list,".avi");
    printf("--------------------获取的视频文件路径-----------------------\n");
    display(video_list);
    printf("--------------------获取的视频文件路径-----------------------\n");
    doubleLinklist *temp=video_list;
    show_bmp("/AlbumProject/image/UI/video_stop.bmp",0,0,0);//显示视频界面{没改}

    VideoInit(); //初始化
    if(video_list==NULL){
        return 0;
    }
    memset(buf,0,sizeof(buf)); 
    sprintf(buf,"mplayer -slave -quiet -input file=/fifo -geometry 0:0 -zoom -x 800 -y 400  %s &",temp->data);
    system(buf);
    while(1)
    {
        xy_ret(&posx, &posy);
        if (posx > 560 && posx < 680 && posy > 400 && posy < 480) //声音+按钮
        {
            printf("+按钮！\n");
            SendCmd("volume +10\n");
        }

        if (posx > 100 && posx < 230 && posy > 400 && posy < 480) //声音-按钮
        {
            printf("-按钮！\n");
            SendCmd("volume -10\n");
        }

        if (posx > 0 && posx < 100 && posy > 400 && posy < 480) //快退按钮
        {
            printf("快退按钮！\n");
            SendCmd("seek -10\n");
        }

        

        if (posx > 680 && posx < 800 && posy > 400 && posy < 480) //快进按钮
        {
            printf("快进按钮！\n");
            SendCmd("seek +10\n");
        }

        if (posx > 230 && posx < 330 && posy > 400 && posy < 480) //上一个视频
        {   
            printf("上一个视频按钮！\n");
            if(temp != NULL && temp->prev != NULL)
            {
                int result_killall = system("killall -9 mplayer");
                if (result_killall != 0)
                {
                    printf("Error: Failed to kill mplayer processes.\n");
                    // 处理错误逻辑
                }
                temp=temp->prev;
                memset(buf,0,sizeof(buf));//清空buf 
                sprintf(buf,"mplayer -slave -quiet -input file=/fifo -geometry 0:0 -zoom -x 800 -y 400  %s &",temp->data);
                show_bmp("/AlbumProject/image/UI/video_stop.bmp",0,0,0);
                system(buf);
                pause_flag = 1;
            }
            
        }

        if (posx > 450 && posx < 560 && posy > 400 && posy < 480) //下一个视频
        {
            printf("下一个视频按钮！\n");
            if(temp != NULL && temp->next != NULL)
            {
                int result_killall = system("killall -9 mplayer");
                if (result_killall != 0)
                {
                    printf("Error: Failed to kill mplayer processes.\n");
                    // 处理错误逻辑
                }
                temp=temp->next;
                memset(buf,0,sizeof(buf));//清空buf 
                sprintf(buf,"mplayer -slave -quiet -input file=/fifo -geometry 0:0 -zoom -x 800 -y 400  %s &",temp->data);
                show_bmp("/AlbumProject/image/UI/video_stop.bmp",0,0,0);
                system(buf);
                pause_flag = 1;
            }
            else
            {
                printf("没有下一个视频！");
            }
            
        }
        if (posx > 330 && posx < 450 && posy > 400 && posy < 480) //暂停/播放按钮
           {
            printf("暂停|播放按钮！\n");
            if(pause_flag == 1)//pause_flag是开关，点击播放下一次暂停
            {
                SendCmd("pause\n");
                pause_flag=0;
                show_bmp("/AlbumProject/image/UI/video_play.bmp",0,0,0);//播放图片的路径  暂停 
            }
            else
            {
                SendCmd("pause\n");
                pause_flag=1;
                show_bmp("/AlbumProject/image/UI/video_stop.bmp",0,0,0);//图片的路径  播放
            }
             
        }

        if (posx > 700 && posx < 800 && posy > 0 && posy < 100) //退出按钮
        {
            printf("退出按钮！\n");
            break;
        }
    }

    VideoFree(); 
    system("sudo sync && sudo echo 3 | sudo tee /proc/sys/vm/drop_caches");
    return 0;
}
