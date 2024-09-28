#include "Project.h"

int init(){

   lcd = open("/dev/fb0", O_RDWR);
   ts_fd=open("/dev/input/event0", O_RDONLY);
   printf("lcd----(%d)------ts_fd-------%d\n",lcd,ts_fd);
   if(lcd==-1|| ts_fd==-1){
        printf("open  file error.\n");
        return -1;
   }else{
        printf("open  file success\n");
   }
   
   mmap_start=(int *)mmap(NULL,LCD_HEIGHT*LCD_WIDTH*4,PROT_READ|PROT_WRITE,MAP_SHARED,lcd,0);
   if (mmap_start==MAP_FAILED)
   {
    printf("内存映射失败\n");
    return -2;
   }
   
    image_paths=NULL;
    rb=NULL;
    current=NULL;//当前展示节点
    num_images=getPathsByIO("/AlbumProject/image",&image_paths,".bmp");
    display(image_paths);

   return 0;

}
int show_bmp(char * bmp_path ,int where_x ,int where_y,int flag){
//第一步：打开lcd的设备文件
   
    int picture=open(bmp_path,O_RDONLY);
    
     if(picture == -1)
    {
        printf("open picture error!\n");
        return -1;
    }
    else
    {
        printf("open picture success!\n");
    }
  
    int bmp_w,bmp_h;
    lseek(picture,18,SEEK_SET);//跳到19前面
    read(picture,&bmp_w,4);
    read(picture,&bmp_h,4);
    //判断图片的大小
    if(bmp_h>480||bmp_w>800){
        printf("图片尺寸大于屏幕尺寸，没办法完整显示出来，抱歉。麻烦你换张图片\n");
        return -2;
    }

    int num;//存放每行需要补的字节数
    if(bmp_w*3%4==0){
        num=0;
    }else{
        num=4-(bmp_w*3%4);
    }

    int max=bmp_w*bmp_h*3+num*bmp_h;
    char picBuf[max];   //char 类型的
    lseek(picture,54,SEEK_SET);

    int read_ret=read(picture,picBuf,sizeof(picBuf));

    if(read_ret == -1){
        printf("write lcd error!\n");
        return -3;
    }
    else
    {
        printf("write lcd success!\n");
    }

    if(flag != 10)
    {
        int n,y,x;
    
        for(y=0,n=0;y<480;y++){
             for(x=0;x<800;x++){
                if(y>=where_y&&y<where_y+bmp_h&&x>=where_x&&x<where_x+bmp_w){
                    //continue;
                  *(mmap_start+800*(479-y)+x) = (picBuf[n]<<0) + (picBuf[n+1]<<8) + (picBuf[n+2]<<16);
                   n+=3;
                }else{
                   
                }
               
            }
            if(y>=where_y&&y<where_y+bmp_h){
                n+=num;
            }
        } 
    }//改造像素点
    else
    {
        int n, y, x;
        for (y = 0, n = 0; y < bmp_h; y += 2) { // 每次增加2，跳过奇数行
            for (x = 0; x < bmp_w; x += 2, n += 6) { // 每次增加2，跳过奇数列
              *(mmap_start + 800 * (479 - y / 2) + x / 2) = (picBuf[n] << 0) + (picBuf[n + 1] << 8) + (picBuf[n + 2] << 16);
            }
        n += num+bmp_w*3; // 跳过不需要的字节
        }
    }
    
    
    //第三步：关闭lcd的设备文件

    close(picture);
    return 0;

}

void showGif(char * gif_Mpath,int length , int where_x,int where_y){
    char image_path[200];
    int i;
     for(i=1;i<=length;i++){
        //拼接图片的路径
        sprintf(image_path,"%s%d.bmp",gif_Mpath,i);
        printf("%s\n",image_path);
        show_bmp(image_path,where_x,where_y,0);
        usleep(50000);
     }

}

//打印链表
/**
 * 接收参数： 头节点指针
 * 逻辑：使用assert() 判断 phead 是否为空，不为空则遍历链表，直到当前的节点指针==头节点指针
*/
void display(doubleLinklist *phead) {
       if(phead!=NULL){
         doubleLinklist* cur=phead;
       do{
          printf("%s\n",cur->data);
          cur=cur->next;
       }while(cur!=phead);
       printf("\n");
       }
      
}

//创建返回链表的头结点  
doubleLinklist * createNode(char * data){  //创建节点 

       doubleLinklist * newNode = (doubleLinklist *)malloc(sizeof(doubleLinklist));
       if(newNode == NULL){
           printf("malloc fail\n");
           exit(1);
       }
       strcpy(newNode->data, data);
       newNode->next = newNode->prev=NULL;
       return newNode;


}
doubleLinklist * init_doubleLinklist(char * data){//创建链表
      
      doubleLinklist* phead = (doubleLinklist*)malloc(sizeof(doubleLinklist));
      phead->next=phead;
      phead->prev=phead;
      strcpy(phead->data,data);
      return phead;   

}
//插入节点 从尾部插入
void doubleLinklist_insert(doubleLinklist * phead, doubleLinklist * p){//插入节点
          //判断是否为空
          if(phead==NULL){
              phead=p;
              phead->prev=phead;
              phead->next=phead;
                          
          }else{
             
                 p->prev = phead->prev;//最后一个节点
                 p->next = phead;
                if(phead->prev != NULL) {
                      phead->prev->next = p;
                  } else {
                      phead->prev = p;
                }
                  phead->prev = p;
              
                 
          }
         
}
//删除指定位置的节点

int doubleLinklist_delete(doubleLinklist **head, int index){
  int i = 0;
    doubleLinklist* temp = (*head);
    
    if (*head == NULL) {
        return  -1;
    }
    
    //①.如果删除到只剩下首元结点了,则直接将*head置空;
    if(temp->next == *head){
         printf("删除的是头结点,只有一个节点\n");
         (*head) = NULL;
        // free(*head);
         
        return 0;
    }
    
    //1.找到要删除的结点
    while (i <index) {
        temp = temp->next;
        i++;
    }
    //  temp->prev     temp             temp->next (新头) 
    if(i==0){
        printf("删除的是头结点\n");
        (*head) = temp->next;
        temp->prev->next = temp->next;
        temp->next->prev = temp->prev;
        free(temp);
    }else{
        //3.修改被删除结点的前驱结点的后继指针 图1️⃣
    temp->prev->next = temp->next;
    //4.修改被删除结点的后继结点的前驱指针 图2️⃣
    temp->next->prev = temp->prev;
    //5. 删除结点temp
    free(temp);
    }   

}

int getIndex(doubleLinklist *p, doubleLinklist *currentNode) {
    int index = 0;
    doubleLinklist *temp = p;
    do {
        if (temp == currentNode) {
            return index;
        }
        temp = temp->next;
        index++;
    } while (temp != p); // 当循环回到起始节点时结束
    return -1; // 如果未找到当前节点，返回-1
}

int getPathsByIO(char * dir_path , doubleLinklist** phead ,char * type){
    struct dirent *dirInformation;
    char  * dir_name;
    char path[300];
    struct stat st;
    char *pos;
    int i=0;
    DIR *dir =opendir(dir_path);
    if(dir==NULL){
        printf("open dir error\n");
        // return -1;
    }
    // dirInformation=readdir(dir)
    // printf("%s",dirInformation->d_name);
    while((dirInformation=readdir(dir))!=NULL){
        
        dir_name=dirInformation->d_name;
        if(strcmp(dir_name,".")==0 || strcmp(dir_name,"..")==0){
            continue;
        }

            printf("file-----%s\n",dir_name);
            pos=strstr(dir_name,type);
            if(pos!=NULL){
              
                sprintf(path,"%s/%s",dir_path,dir_name);
                printf("%s\n",path);
                if(*phead==NULL){
                    printf("%s-------------\n",path);
                    (*phead)=init_doubleLinklist(path);
                    display(*phead);
                }else{
                    doubleLinklist *temp=createNode(path);
                    doubleLinklist_insert(*phead,temp);
                   
                }               
                i++;
                  
            }       
    }
    
    closedir(dir);
    return i;
}

//返回点击坐标的函数
int xy_ret(int *th_x, int *th_y)
{
    while (1)
    {
        read(ts_fd, &buf, sizeof(buf));

        if (buf.type == EV_ABS && buf.code == ABS_X)
            *th_x = buf.value * 800 / 1024;

        if (buf.type == EV_ABS && buf.code == ABS_Y)
            *th_y = buf.value * 480 / 600;

        if (buf.type == EV_KEY && buf.code == BTN_TOUCH && buf.value == 0)
        {
            break;
        }
    }

    return 0;
}


//在菜单中点击可选择选项的函数
int ChoiceSelect(void)
{

    //获取坐标
    xy_ret(&posx, &posy);
    printf("posx = %d, posy = %d\n", posx, posy); //test

    //返回choice
    if(posx >= 700 && posx < 800 && posy >= 0 && posy < 100)    return 0;
    else if(posx >= 100 && posx < 420 && posy >= 100 && posy < 480)    return 1;
    else if(posx >= 420 && posx < 700 && posy >= 100 && posy < 480)    return 2;
    else    return 4;

}


//显示UI界面
int UI_menu(void)
{
    //1.开机提示
    printf("Welcome to my project!\n");
    printf("-------------------------\n");
    char gif_Mpath[35]="/AlbumProject/image/gif2_bmp/";
    int length=9;//gif的帧数
    showGif(gif_Mpath,length,0,0);

    do{
        show_bmp("/mnt/udisk/ui/main_ui/MainUi.bmp",0,0,0);//显示菜单（UI背景图片）
       
        xy_ret(&posx,&posy);   

        printf("------------点击主界面---------------\n"); 
        printf("------------posx----%d------posy-----%d---------\n",posx,posy);     

        if(posx>150&&posx<400&&posy>50&&posy<230){
            Album_UI(); //相册
            posx=posy=0;
        }
        if(posx>400&&posx<650&&posy>50&&posy<230){
            playMusic();// 音乐
            posx=posy=0;  
        }
        if(posx>100&&posx<700&&posy>400&&posy<480){
            pthread_t audioRnCl; //语音识别线程标识符
            int ret_1 = pthread_create(&audioRnCl,NULL,audioRecognitionControl,NULL);//创造一个线程
            if(ret_1 != 0 )
            {
                perror("pthread_create");
                return -1;
            }
            pthread_join(audioRnCl,NULL);//等待线程结束
            show_bmp("/mnt/udisk/ui/main_ui/MainUi.bmp",0,0,0);//显示菜单（UI背景图片）
            posx=posy=0;       
        }
        if(posx>150&&posx<400&&posy>230&&posy<400){
              StartVideo();// 视频
              posx=posy=0;
        }
        if(posx>400&&posx<650&&posy>230&&posy<400){
              StartGame();//游戏
              posx=posy=0;
              
        }
        if(posx>700&&posx<800&&posy>0&&posy<100){
           break;//退出
        }


    }while(1);

    return 0;
}
//释放函数
void closeAll(){
    close(ts_fd);
    close(lcd);
    munmap(mmap_start,LCD_WIDTH*LCD_HEIGHT*4);
}
