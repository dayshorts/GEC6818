#ifndef _Album_H_
#define _Album_H_ //防止头文件冲突

#include"Basic.h"


//变量声明
int current_image_index; // 用于跟踪当前显示的图片索引

//函数声明
void restoreFromRecycleBin(doubleLinklist **phead, doubleLinklist **rb ,doubleLinklist *del, int index);
int get_xy();//获取坐标，判断手势操作，返回值left:1 right:2 exit:5 左下角：3 右下角：4 上划：7 下划：6 删除：8 恢复：9
int get_Rimages(doubleLinklist ** phead, int flag ,doubleLinklist **operation);
int get_images(doubleLinklist ** phead, int flag ,doubleLinklist **operation);//获取点前节点的索引
void albumControl();//相册功能函数
void recycleRinControl();//回收站功能函数
void Album_UI();//相册的界面

#endif