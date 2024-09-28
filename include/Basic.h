#ifndef _Basic_H_
#define _Basic_H_ //防止头文件冲突

#include"stdio.h"
#include"sys/types.h"
#include"fcntl.h"
#include"unistd.h"
#include"linux/input.h"
#include"stdlib.h"
#include"sys/mman.h"
#include"string.h"
#include "dirent.h"
#include "sys/stat.h"
#include "assert.h"
#include "stdbool.h"
//网络通讯使用的头文件
#include "netinet/in.h"//提供宏定义 INADDR_ANY
#include "pthread.h"
#include "sys/socket.h"
#include "arpa/inet.h" //提供专属结构体的头文件和 htons
#include <time.h>
#include <sys/ioctl.h>
#include <linux/fb.h>

#define LCD_HEIGHT 480
#define LCD_WIDTH 800

//游戏方框s
#define ROW 11
#define COL 17

typedef struct DoubleLinklist {
    char  data[200];
    struct DoubleLinklist *prev;
    struct DoubleLinklist *next;
    // int deleted; // 标志节点是否已删除
} doubleLinklist;

//链表的初始化函数
doubleLinklist * init_doubleLinklist(char * data);//创建链表
doubleLinklist * createNode(char * data);//创建节点 

void display(doubleLinklist *phead);//打印链表
void doubleLinklist_insert(doubleLinklist * phead, doubleLinklist * p);//插入节点
int doubleLinklist_delete(doubleLinklist **head, int index);//删除指定节点


int lcd; //lcd 的文件描述符
int ts_fd; //touch 的文件描述符
int * mmap_start; //lcd 映射内存
int posx, posy;    //存放点击坐标
struct input_event buf; //触摸屏的结构体

//相册的变量声明
doubleLinklist * image_paths;
doubleLinklist * rb;
doubleLinklist * current;//当前展示节点
int num_images;

//语音识别的变量声明
//  pthread_t audioRnCl; //语音识别线程

//函数声明
int init(); //初始化函数，负责
int show_bmp(char * bmp_path ,int where_x ,int where_y,int flag);//显示图片
int getPathsByIO(char * dir_path , doubleLinklist** phead ,char * type);//获取路径
int xy_ret(int *th_x, int *th_y);//返回点击坐标的函数
int ChoiceSelect(void);//在菜单中点击可选择选项的函数
int UI_menu(void);//UI界面
void closeAll(); //释放内存
void showGif(char * gif_Mpath,int length , int where_x,int where_y);//展示gif

#endif