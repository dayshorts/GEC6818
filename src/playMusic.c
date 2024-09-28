#include "Music.h"
#include "Video.h"

int playMusic(){
    doubleLinklist *music_list=NULL;   
    int flag=0,pause=0;   //pause=> 1:可继续 0：可暂停 
    char buf[300];
    int  vol=-18;
    char musicUI[200];//唱片转动的调用指令
    VideoInit();
    memset(musicUI,0,sizeof(musicUI)); 
    sprintf(musicUI,"mplayer -slave -quiet -input file=/fifo -geometry 0:0 -zoom -x 800 -y 400  %s &","/mdata/music_video/music_play_stop.avi");    
    
    show_bmp("/mnt/udisk/MusicGif/1.bmp",0,80,0);  
    show_bmp("/mnt/udisk/ui/music_ui/musicUI_stop.bmp",0,0,0);
    getPathsByIO("/mdata/music",&music_list,".mp3");
    printf("--------------------获取的音频文件路径-----------------------\n");
    display(music_list);
    printf("--------------------获取的音频文件路径-----------------------\n");
    doubleLinklist * temp=music_list;
    if(music_list==NULL){
        return 0;
    }
    printf("--------------------播放音频-----------------------\n");
    memset(buf,0,sizeof(buf));  
    sprintf(buf,"madplay -a %d  %s </dev/null &",vol,temp->data);
    printf("%s\n",buf);
    printf("--------------------播放音频-----------------------%s\n",buf);
    
    
    
    while(1){
        xy_ret( &posx , &posy );

        if(flag == 0 && ( posy >= 300 && posy < 480 && posx >= 360 && posx < 460)){
            printf("播放\n");
            show_bmp("/mnt/udisk/ui/music_ui/musicUI_play.bmp",0,0,0);
            printf("%s\n",buf);
            system(musicUI);
            system(buf);
            flag=1;
            posx=posy=0;//清空坐标
        }
       
      
        if(posy >= 300 && posy < 480 && posx >= 460 && posx < 580){  //下一首
            printf("下一首\n");
            system("killall -9 mplayer");
            system("killall -9 madplay");
            temp=temp->next;
            memset(buf,0,sizeof(buf));//清空buf 
            sprintf(buf,"madplay -a %d  %s </dev/null &",vol,temp->data);
            show_bmp("/mnt/udisk/ui/music_ui/musicUI_play.bmp",0,0,0);
            printf("%s\n",buf);
            system(musicUI);
            system(buf);  
            posx=posy=0;//清空坐标  
            pause=0;     
        }
        if(posy >=300 && posy < 480 && posx >= 230 && posx < 360){  //上一首
            printf("上一首\n");
            system("killall -9 mplayer");
            system("killall -9 madplay");
            temp=temp->prev;
            memset(buf,0,sizeof(buf));//清空buf 
            sprintf(buf,"madplay -a %d  %s </dev/null &",vol,temp->data);
            printf("%s\n",buf);
            show_bmp("/mnt/udisk/ui/music_ui/musicUI_play.bmp",0,0,0);
            system(musicUI);
            system(buf);   
            posx=posy=0;//清空坐标 
            pause=0;       
        }
        if(posy >= 300 && posy < 480 && posx >= 580 && posx < 700){  //增加音量
            printf("增加音量\n");
            if(vol>=-48&&vol<=8){
                vol+=10;
                if(vol>0){
                  sprintf(buf,"madplay -a %d  %s </dev/null &",vol,temp->data);
                }else{
                  sprintf(buf,"madplay -a %d  %s </dev/null &",vol,temp->data); 
                }
                system("killall -9 madplay");
                system("killall -9 mplayer");
                show_bmp("/mnt/udisk/ui/music_ui/musicUI_play.bmp",0,0,0);
                system(musicUI);
                system(buf);
                pause=0;
            }
        }
        if(posy >= 300 && posy < 480 && posx >= 120 && posx < 230){  //减少音量
            printf("减少音量\n");
            if(vol<=18&&vol>=-38){
                vol-=10;
                if(vol>0){
                  sprintf(buf,"madplay -a %d  %s </dev/null &",vol,temp->data);  
                  printf("%s\n",buf); 
                }else{
                  sprintf(buf,"madplay -a %d  %s </dev/null &",vol,temp->data); 
                  printf("%s\n",buf);
                }
                system("killall -9 madplay");
                system("killall -9 mplayer");
                show_bmp("/mnt/udisk/ui/music_ui/musicUI_play.bmp",0,0,0);
                system(musicUI);
                system(buf);
                pause=0;
            }
        }

   //播放|暂停
         if(posy >= 300 && posy < 480 && posx >= 360 && posx<460){
            
            if(pause==1){
                
                show_bmp("/mnt/udisk/ui/music_ui/musicUI_play.bmp",0,0,0);
                printf("----------发送继续播放的指令---------\n");
                SendCmd("pause\n");
                system("killall -SIGCONT madplay &");

            }else{  

                  show_bmp("/mnt/udisk/ui/music_ui/musicUI_stop.bmp",0,0,0);   
                  SendCmd("pause\n");
                  printf("----------发送暂停播放的指令---------\n");
                  system("killall -SIGSTOP madplay &");

            }
            pause=!pause;
            posx=posy=0;//清空坐标
        }

     
        if(posx >= 700 && posx < 800 && posy >=0 && posy <100){  //点击右上角退出音乐
            system("killall -9 madplay");
            printf("退出音乐界面\n");
            break;
        }           
   } 
   system("killall -9 madplay");
   VideoFree();
   system("sudo sync && sudo echo 3 | sudo tee /proc/sys/vm/drop_caches");//保留一下
   return 0;
}