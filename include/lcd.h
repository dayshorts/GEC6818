#ifndef __LCD_H__
#define __LCD_H__

/**
 * @brief DrawPixel 画点函数
 * @param h 像素点所在行
 * @param w 像素点所在列
 * @param color 像素点需要显示的颜色
*/
void DrawPixel(int h,int w,int color);

//画棋盘线
void Drawboard_line(int color);

//画背景
void DrawBackgournd(int color);


//显示棋盘
void Darw_board(int first,char board[ROW][COL]);

//获取手指点击屏幕的坐标之后进行开始游戏的操作
void Start_game(char board[ROW][COL],char displayBoard[ROW][COL],int Y,int X);

//初始化界面
int Initial();

//重新开始游戏与退出游戏程序
int Restart();

//显示游戏图片
int DisplayBMPPicture(int posx,int posy,const char *pic_path);

#endif