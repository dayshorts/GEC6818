#include"Project.h"


/**
 * @brief DrawPixel 画点函数
 * @param h 像素点所在行
 * @param w 像素点所在列
 * @param color 像素点需要显示的颜色
*/
void DrawPixel(int h,int w,int color)
{
    // 约束一下坐标范围
    if(h>=0&&h<480&&w>=0&&w<800)
        *(mmap_start+h*800+w) = color;
}

//画棋盘线
void Drawboard_line(int color)
{
    int h = 0;
    int w = 1;
    //画竖线
    for (h = 1; h < 480; h++)
    {
        if (h % 40 == 0)
        {
            for (w = 0; w < 680; w++)
            {
                DrawPixel(h,w,color);
            }
        }
    }
    //画横线
    for (w = 0; w < 700; w++)
    {
        if (w % 40 == 0)
        {
            for (h = 0; h <= 440; h++)
            {
                DrawPixel(h,w,color);
            }
        }
    }
}

//画背景
void DrawBackgournd(int color)
{
    int h;
    for( h = 0;h < 480;h++)
    {
        int w;
        for(w = 0;w < 800;w++)
        {
            DrawPixel(h,w,color);
        }
    }
}


//显示棋盘
void Darw_board(int first,char board[ROW][COL])
{
    //最开始不显示结束游戏的图标
    if (first)
    {
       
        DisplayBMPPicture(680,5,"/mnt/udisk/over.bmp");
    }
    int h;
    for(h = 0;h < ROW;h++)
    { 
        int w;
        for(w = 0; w < COL; w++)
        {
            //根据棋盘元素在对应位置打印相应的图片
            switch (board[h][w])
            {
            case '\0':            
                
                DisplayBMPPicture(w*40,h*40,"/mnt/udisk/0.bmp");

                break;
            case '1':
                DisplayBMPPicture(w*40,h*40,"/mnt/udisk/1.bmp");
                break;
            case '2':
                DisplayBMPPicture(w*40,h*40,"/mnt/udisk/2.bmp");
                break;
            case '3':
                DisplayBMPPicture(w*40,h*40,"/mnt/udisk/3.bmp");
                break;
            case '4':
                DisplayBMPPicture(w*40,h*40,"/mnt/udisk/4.bmp");
                break;
            case '5':
                DisplayBMPPicture(w*40,h*40,"/mnt/udisk/5.bmp");
                break;
            case '6':
                DisplayBMPPicture(w*40,h*40,"/mnt/udisk/6.bmp");
                break;
            case '7':
                DisplayBMPPicture(w*40,h*40,"/mnt/udisk/7.bmp");
                break;
            case '8':
                DisplayBMPPicture(w*40,h*40,"/mnt/udisk/8.bmp");
                break;
            case '*':
                DisplayBMPPicture(w*40,h*40,"/mnt/udisk/mines.bmp");
                break;
            default:
               
                DisplayBMPPicture(w*40,h*40,"/mnt/udisk/initial.bmp");
                break;
            }
        }
    }
    //画棋盘线
    Drawboard_line(0x000000);
}

//获取手指点击屏幕的坐标之后进行开始游戏的操作
void Start_game(char board[ROW][COL],char displayBoard[ROW][COL],int Y,int X)
{
    int first = 1;
    while(1)
    {
        //获取手指点击坐标
        xy_ret(&Y,&X);
        Y=Y/40;
        X=X/40;
        if (Y < 17 && X < 11)
        {
            //为了避免第一次就寄掉所以在第一次输入坐标之后再对内部棋盘进行落雷并插入数字
            if (first)//第一次选完点
            {
                DownMines(board,X,Y);//落雷

                InsertData(board,X,Y);//在内部棋盘插入数字

                first = 0;//之后输入坐标不再随机落雷与插数字
            }
            if (board[X][Y] == '*') //游戏以踩雷结束
            {
                
                
                DisplayBMPPicture(60,0,"/mnt/udisk/FAIL.bmp");
                sleep(1);
                DrawBackgournd(0xffffff);
                Darw_board(1,board);//打印出内部棋盘
                break;
            }
            else
            {
                revealCell(board,displayBoard,X,Y);//扩散
            }
            //显示展示棋盘
            Darw_board(1,displayBoard);
        }
        //主动结束游戏
        if (Y > 17 && X <2)
        {
            
            DisplayBMPPicture(60,0,"/mnt/udisk/FAIL.bmp");
            sleep(1);
            DrawBackgournd(0xffffff);
            Darw_board(1,board);//打印出内部棋盘
            break;
        }
        
        if (Judge_func(displayBoard))//雷被全部排完，成功通关
        {
            sleep(1);  
            DisplayBMPPicture(60,0,"/mnt/udisk/VICTORY.bmp");//展示成功通关图片
            sleep(1);
            DrawBackgournd(0xffffff);
            Darw_board(1,board);//打印出内部棋盘
            break;
        }
    }
    //出循环代表本局游戏结束，显示再来一局以及退出游戏程序的图片
    DisplayBMPPicture(680,80,"/mnt/udisk/ag.bmp");
    DisplayBMPPicture(680,230,"/mnt/udisk/exit.bmp");
    
    
}

//初始化界面
int Initial()
{
    int Y = -1;
    int X = -1;
    //展示界面图片
    DisplayBMPPicture(0,0,"/mnt/udisk/start2.bmp");
    sleep(3);
    DisplayBMPPicture(0,0,"/mnt/udisk/start.bmp");
    while(1)
    {   //获取点击坐标
        xy_ret(&Y,&X);
        Y=Y/40;
        X=X/40;
        //限制进入游戏区域
        if (Y < 13 && Y > 8 && X < 8 && X > 4)
        {
            return 1;
        }
    }
    return 0;
}

//重新开始游戏与退出游戏程序
int Restart()
{
    int X = -1;
    int Y = -1;
    while(1)
    {   //获取点击坐标
        xy_ret(&Y,&X);
        Y=Y/40;
        X=X/40;
        //限制重新开始的区域
        if (Y > 17 && X > 2 && X < 5)
        {

            return 1;
        }
        else if (Y >17 && X >5 && X <8)
        {
            return 0;
        }
    }
}
int DisplayBMPPicture(int posx,int posy,const char *pic_path)
{
    // 第一步：打开图片
    int pic_fd = open(pic_path,O_RDONLY);
    if(pic_fd == -1)
    {
        perror("Open bmp file failed");
        return 1;
    }

    // 第二步读取数据，判断文件类型
    unsigned char data[4]={0};
    read(pic_fd,data,2);//（文件描述符，指向缓冲区的指针，要读取的字节数）

    // 判断是否为BMP文件
    if(data[0]!=0x42||data[1]!=0x4d)
    {
        puts("this picture not BMP file");
        close(pic_fd);
        return 1;
    }

    // 读取图片的偏移量
    lseek(pic_fd,0x0a,SEEK_SET);//文件描述符 pic_fd 的读写位置移动到文件头的偏移量 0x0a,这是BMP文件头中指示图像数据开始位置的字段
    read(pic_fd,data,4);
    int offset = data[3]<<24|data[2]<<16|data[1]<<8|data[0];

    lseek(pic_fd,0x12,SEEK_SET); //这是BMP文件头中图像宽度字段的位置
    read(pic_fd,data,4);//读4个字节进data，表示宽度
    int width = data[3]<<24|data[2]<<16|data[1]<<8|data[0];//求宽度，bmp用的小端字节序，如0x1234，存储就是4321，所以需要倒过来

    read(pic_fd,data,4);//通过指向缓冲区的指针，指向下四个字节，高度
    int height = data[3]<<24|data[2]<<16|data[1]<<8|data[0];

    lseek(pic_fd,0x1C,SEEK_SET);//颜色深度
    read(pic_fd,data,2);
    int depth = data[1]<<8|data[0];

    // 如果图片的宽度不为4的倍数，那么图片显示将会异常。字节对齐引起
    int rand_num = 0; // 填充字节数
    if((width*(depth / 8))%4)//每个像素8位，256色，除8得到每个像素字节数乘宽看看余4吗
    {
        rand_num = 4 - ((width*(depth / 8))%4);
    }

    int real_width_bytes = (width*depth/8)+rand_num;

    // 像素数组的数据

    // 开辟了一个动态可变数组
    //无符号字符类型。unsigned char 的取值范围是 0 到 255（8 位），方便存储图像
    unsigned char *color=calloc(sizeof(char),real_width_bytes*abs(height));//calloc 分配的内存大小为 real_width_bytes * abs(height) 字节，并将其初始化为 0
    lseek(pic_fd,offset,SEEK_SET);
    read(pic_fd,color,real_width_bytes*abs(height));//读色彩进color

    // 处理数据
    // 注意： BMP图片的像素数组对于三原色的存储是反着来的，也就是像素数组的第一个字节保存的是第一个像素点的Blue值

    unsigned char *color_p = color;
    // 显示整幅图像
    int h;
    for(h = 0;h < abs(height);h++)
    {
        int w;
        for(w = 0;w < width;w++)
        {
            unsigned char a,r,g,b;//小端
            b =  *color_p++;
            g =  *color_p++;
            r =  *color_p++;
            a = (depth == 24)?0:*color_p++;
            //int color_var = color[2] << 16|color[1]<<8|color[0];
            int color_var = a<<24|r<<16|g<<8|b;
            DrawPixel(((height < 0)?h:height-1-h)+posy,w+posx,color_var);
            //判断图像高度是不是小于0，小于0从下往上，否则从上往下
        }
        
        //跳过填充的字节数
        color_p+= rand_num;
    }

    // 收尾工作
    free(color);
    close(pic_fd);
    return 0;
}
