#include"Album.h"
int get_xy()
{
    struct input_event buf;
    int x1, y1;       // 保存第一次的坐标
    int x2, y2;       // 保存最后一次的坐标
    int tmp_x, tmp_y; // 保存临时坐标
    x1 = y1 = x2 = y2 = -1;
    while (1)
    {
        read(ts_fd, &buf, sizeof(buf)); // 读取触摸屏数据
        if (buf.type == EV_ABS)         // 触摸屏事件
        {
            if (buf.code == ABS_X) // X轴
            {
                tmp_x = buf.value*800/1024;
            }
            else if (buf.code == ABS_Y) // Y轴
            {
                tmp_y = buf.value*480/600;
            }
        }
        // 判断手指松开
        if (buf.type == EV_KEY && buf.code == BTN_TOUCH)
        {
            if (buf.value == 1) // 按下
            {
                x1 = tmp_x;
                y1 = tmp_y;
            }
            else if (buf.value == 0) // 松开,保存最后一次坐标
            {
                x2 = tmp_x;
                y2 = tmp_y;
                break;
            }
            


        }
    }

    int var_x = abs(x2 - x1); // x轴变化量
    int var_y = abs(y2 - y1); // y轴变化量
    // 判断是滑动还是点击
    if ((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1) >=200*200) // 滑动
    {
        if (x1 > x2 && var_x > var_y) // 左
        {
            printf("left\n");
            return 1;

        }
        else if (x1 < x2 && var_x > var_y) // 右
        {
            printf("right\n");
            return 2;

        }
        else if(y1 < y2 && var_y > var_x)//下
        {
            printf("down\n");
            return 6;
        }
         else if(y1 > y2 && var_y > var_x)//上
        {
            printf("up\n");
            return 7;
        }
      
    }
    if(x1>=0&&x1<200&&y1>=280&&y1<480){
        printf("点击左下角\n");
        return 3;
    }
    if(x1>=600&&x1<800&&y1>=280&&y1<480){
        printf("点击右下角\n");
        return 4;
    }
    if(x1>=300&&x1<500&&y1>=0&&y1<200)//上
     {
            printf("中间上\n");
            return 8;
    }
    if(x1>=300&&x1<500&&y1>=380&&y1<480)//上
     {
            printf("中间下\n");
            return 9;
    }
    //退出  (右上角（100*100）退出)
    if (x1>=700&&x1<800&&y1>=0&&y1<100)
    {
        printf("bye\n");
        return 5;
    }
    if (x1>=0&&x1<100&&y1>=0&&y1<100)
    {
        return 10;
    }
}
/**
 * 参数： 第一个：操作展示图片的链表的指针的指针
 *        第二个：手势标记符 flag 代表的意思   left:1 right:2 exit:5 左下角：3 右下角：4 上划：7 下划：6 删除：8 恢复：9
 *        第三个：操作图片的链表的指针的指针   用于恢复，删除图片的功能
 *       
*/
int get_images(doubleLinklist ** phead, int flag ,doubleLinklist **operation)
{
    
    doubleLinklist *show;   
    if((flag>=1&&flag<=7)||flag==8||flag==11){
        memset(mmap_start,0,LCD_WIDTH*LCD_HEIGHT*4);
        if (flag == 2||flag==3||flag==6)// 如果flag=2，滑向前一张图片(右划) 点击左下角 、下划
        {
                   
                    show=current->prev;
                    current=show; 
                    current_image_index=getIndex((*phead),current);
                    
                         
        }
        else if (flag == 1||flag==4||flag==7)// 如果flag=1，滑向后一张图片 左划
        {
              
                show=current->next;
                current=show;
                current_image_index=getIndex((*phead),current);
              
        }
        else if(flag==8){
           doubleLinklist *temp = current;
            if (current == NULL)
            {
                printf("当前没有图片可删除\n");
                current_image_index = -1;
                printf("----------相册图片-----%d\n", current_image_index);
                display(*phead);
                printf("----------相册图片-----%d\n", current_image_index);
                return 5;
            }
            printf("删除-----%d\n", current_image_index);
            display(*phead);
            doubleLinklist_delete(phead, current_image_index);
            printf("删除-----hou %d\n", current_image_index);
            display(*phead);
            printf("删除的路径--------存进rb --------%s\n",current->data);
            printf("删除的路径--------存进rb ------temp --------%s\n",temp->data);
            if (*operation == NULL)
            {
                *operation = init_doubleLinklist(temp->data);
            }
            else
            {   doubleLinklist * new = createNode(temp->data);
                doubleLinklist_insert(*operation, new);
            }

            // 更新show指针
           
            current = *phead;
            show=current;
            current_image_index=getIndex(*phead,current);
           
        } 
        else if(flag==5){
            printf("谢谢使用，退出相册\n");
            return 5;
        }
        // char *current_image_path = image_paths[current_image_index].data;
         if(show!=NULL){
            memset(mmap_start,0,LCD_WIDTH*LCD_HEIGHT*4);
            char *current_image_path =show->data;
            show_bmp(current_image_path,0,0,0);
            printf("current_index-------%d\n",current_image_index);
            return 0;
        }
    }
    else if(flag==10){
            memset(mmap_start,0,LCD_WIDTH*LCD_HEIGHT*4);
            printf("into 10\n");
            printf("Current Image Path: %s\n", current->data);
            char *current_image_path =current->data;
            show_bmp(current_image_path,0,0,10);
            return 0;
    }else if(flag > 11){
        return 0;
    }
    
    return 0;     
   
}
//第一个参数是回收站的文件路径链表指针的指针，第二个参数是相册的文件路径链表的指针的指针
int get_Rimages(doubleLinklist ** phead, int flag ,doubleLinklist **operation)
{
    
    doubleLinklist *show;   
    if((flag>=1&&flag<=7)||flag==9){
        
        if (flag == 2||flag==3||flag==6)// 如果flag=2，滑向前一张图片(右划) 点击左下角 、下划
        {
                   
                    show=current->prev;
                    current=show; 
                    current_image_index=getIndex((*phead),current);
                    
              
            
        }
        else if (flag == 1||flag==4||flag==7)// 如果flag=1，滑向后一张图片 左划
        {
              
                show=current->next;
                current=show;
                current_image_index=getIndex((*phead),current);
              
        }
        else if(flag==9){
            current_image_index = getIndex(*phead,current);
           if((*phead)==NULL){
               printf("回收站为空\n");
            // show=current;
               return 5;
           }else{
               printf("------------恢复前RB----------------\n");
               display(*phead);
               printf("------------恢复前RB----------------\n");
               printf("------------恢复前image----------------\n");
               display(*operation);
               printf("------------恢复前image----------------\n");
               restoreFromRecycleBin(operation,phead ,current,current_image_index);
               if((*phead)==NULL){
                  show=NULL;
                  current=NULL;
                  return 5;
               }
               printf("------------恢复后RB----------------\n");
               display(*phead);
               printf("------------恢复后RB----------------\n");
               printf("------------恢复后image----------------\n");
               display(*operation);
               printf("------------恢复后image----------------\n");
               show=(*phead);
               current=show;
               current_image_index=getIndex((*phead),current);
           }  
        }
        else if(flag==5){
            printf("谢谢使用，退出回收箱\n");
            return 5;
        }
        if(show!=NULL){
            memset(mmap_start,0,LCD_WIDTH*LCD_HEIGHT*4);
            char *current_image_path =show->data;
            show_bmp(current_image_path,0,0,0);
            printf("current_index-------%d\n",current_image_index);
            return 0;
        }
   
       
    }
    else if(flag==10){
            memset(mmap_start,0,LCD_WIDTH*LCD_HEIGHT*4);
            printf("into 10\n");
            printf("Current Image Path: %s\n", current->data);
            char *current_image_path =current->data;
            show_bmp(current_image_path,0,0,10);
            return 0;
    }else if(flag > 11){

            return 0;
    }
   return 0;
}
void restoreFromRecycleBin(doubleLinklist **phead, doubleLinklist **rb ,doubleLinklist *del, int index) {
    if (*rb != NULL) {
      doubleLinklist * temp = createNode(del->data);
      doubleLinklist_delete(rb,index);
      if((*phead)==NULL){
        (*phead)=init_doubleLinklist(temp->data);
      }else{
         doubleLinklist_insert(*phead, temp);
      }
     
    }
}
void  albumControl(){  
        int flag=0;//手势标识符
        int flagT=0; //退出相册的标识符 
     if(image_paths->data!=NULL){
                        memset(mmap_start,0,LCD_WIDTH*LCD_HEIGHT*4);
                        show_bmp(image_paths->data,0,0,0);
                        current=image_paths;
                        current_image_index=getIndex(image_paths,current);
                        while (1)
                        {
                           flag=get_xy();                      //得到操作符
                            printf("%d---flag\n",flag);
                           flagT= get_images(&(image_paths),flag,&rb);      //展示图片
                            printf("flagT--------%d\n",flagT);
                            if(flagT==5){ 
                                    printf("-----------------回收站\n");
                                    display(rb);
                                    printf("-----------------回收站\n");
                                    printf("-----------------相册\n");
                                    display(image_paths);
                                    printf("-----------------相册\n");
                                    printf("flagT--------%d\n",flagT);
                                break;
                            }
                                
                        } 
                    }
       if(current_image_index==-1){
                        image_paths=NULL;
        }
                    
}
void recycleRinControl(){
        int flag=0;//手势标识符
        int flagT=0; //退出回收站的标识符 
         if(rb->data!=NULL){
                       
                         memset(mmap_start,0,LCD_WIDTH*LCD_HEIGHT*4);
                         show_bmp(rb->data,0,0,0);
                         current=rb;
                         current_image_index=getIndex(rb,current);
                         printf("-----------------(当前)回收站\n");
                         display(current);
                         printf("-----------------(当前)回收站\n");
                         
                         while (1)
                             {
                                 flag=get_xy();                      //得到操作符
                                printf("%d---flag\n",flag);
                                 flagT=get_Rimages(&(rb),flag,&image_paths);      //展示图片
                                printf("flagT--------%d\n",flagT);
                                if(flagT==5)
                                {  
                                    printf("-----------------回收站\n");
                                    display(rb);
                                    printf("-----------------回收站\n");
                                    printf("-----------------相册\n");
                                    display(image_paths);
                                    printf("-----------------相册\n");
                                    printf("flagT--------%d\n",flagT);
                                    break;
                                }
                               
                     }
         }else{
               show_bmp("/mnt/udisk/laji.bmp",0,0,0);
               sleep(1);
         }
}

void Album_UI(){
    // image_paths=NULL;
    // rb=NULL;
    // current=NULL;//当前展示节点
    // num_images=getPathsByIO("/AlbumProject/image",&image_paths,".bmp");
    printf("num_images:%d\n",num_images);
    display(image_paths); 
     //1.开机提示
    printf("Welcome to the e-Album!\n");
    printf("-------------------------\n");

    //2.项目功能
    int choice;
    do{
        show_bmp("/AlbumProject/Yefanyin/PUI.bmp",0,0,0);//显示菜单（UI背景图片）
        //提示选择要进行的项目
        choice = ChoiceSelect();
        printf("choice=%d\n",choice);
        if(choice==0){
                printf("bye,have a good day!\n");
                char gif_Mpath[35]="/AlbumProject/image/gif_bmp/";
                int length=15;//gif的帧数
                showGif(gif_Mpath,length,0,0);      //退出展示GIF  
                posx=posy=0;            
                break;
            }
        switch(choice)
        {               
            //(1)电子相册
            case 1:{
                    albumControl();
                    break;
               } 
            //(2)回收站
            case 2:
                // 展示回收站的图片
                {   
                    
                   recycleRinControl();  
                   break;
                }
                
            //输入错误
            default:{
                printf("Please select an action\n");
                break;
            }
                
        }
    }while(1);

    printf("-----退出相册-----\n");
  
}
