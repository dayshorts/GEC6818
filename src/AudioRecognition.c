#include"Project.h"

char* linkKeDaAudioRecognition(){
    
    struct sockaddr_in server_addr; 
    server_addr.sin_family = PF_INET; //家族协议
    server_addr.sin_port = htons(55558); //端口
    server_addr.sin_addr.s_addr = inet_addr(SERVERIP); //ip地址

    char buf[100] ={0};//存储音频文件的发送片段
    char recv_buf[100] ={0};//存储音频文件的发送片段
    int n;//记录读取的字节数
    int ret;//接收到的字节数
    
    

    //创建套接字
    int sock_fd=socket(PF_INET,SOCK_STREAM,0);
    if(sock_fd==-1){
        printf("socket error\n");
        return "socket error";
    }

    //发起连接
    connect(sock_fd,(struct sockaddr *)&server_addr,sizeof(struct sockaddr_in));
    
    int fd = open("./1.wav", O_RDONLY);
    if(fd == -1){

        printf("open audio file error\n");
        return "open audio file error";

    }
    //获取音频文件的总大小

    int file_size=lseek(fd,0,SEEK_END);
    lseek(fd,0,SEEK_SET);
    printf("file_size = %d\n",file_size);
    if(file_size < 0){
        return "error";
    }
    send(sock_fd,&file_size,sizeof(int),0);//先将文件大小发送给服务器

    //再将整个文件发送给服务器
    while (1){
        n = read(fd,buf,100);
        if(n == 0)
        {
            printf("读取文件完毕！\n");
            break;
        }
        send(sock_fd,buf,n,0);
    }
    // memset(buf,0,sizeof(buf));
    ret=recv(sock_fd,recv_buf,100,0);
    printf("接收的信息： %d\n",recv_buf);

    if(ret < 0){
        return "error";
    }
    close(sock_fd);
    close(fd);
    
    
    return recv_buf;
}

//语音识别的控制函数（主要的）
void*  audioRecognitionControl(){
     // 检查文件是否存在
    if (access("./1.wav", F_OK) != -1) {
        // 文件存在，删除文件
        if (remove("./1.wav") == 0) {
            printf("文件 %s 已删除\n", "./1.wav");
        } else {
            printf("无法删除文件 %s\n", "./1.wav");
        }
    } else {
        printf("文件 %s 不存在\n", "./1.wav");
    }



    printf("语音识别\n");
    char * recv_buf={0};//接收获取的值

        printf("开始录音\n");
        show_bmp("/mnt/udisk/ui/main_ui/MainUiStartSpeech.bmp",0,0,0);//打开录音功能
        system("arecord -d3 -c1 -r16000 -twav -fS16_LE 1.wav");

    recv_buf= linkKeDaAudioRecognition();

    printf("---------------------语音识别---------------------------\n");
    printf("语音识别后，返回的值：%s\n",recv_buf);
    printf("---------------------语音识别---------------------------\n");

    if(strstr(recv_buf,"error") != NULL){
        printf("语音识别失败！\n");
        //展示一张语音识别失败的图片
        show_bmp("/mnt/udisk/ui/main_ui/MainUiErrorSpeech.bmp",0,0,0);//展示失败的图片
        //回到主界面 及展示主界面的图片
        pthread_exit(NULL);
        return "-1"; // 不做任何操作
    }
    if(recv_buf == 0){
        printf("语音识别失败！\n");
        //展示一张语音识别失败的图片
        show_bmp("/mnt/udisk/ui/main_ui/MainUiErrorSpeech.bmp",0,0,0);//展示失败的图片
        //回到主界面 及展示主界面的图片
        pthread_exit(NULL);
        return "-1"; // 不做任何操作
    }
    if(strstr(recv_buf,"没有识别结果！")!=NULL){
         printf("语音识别失败！\n");
        //展示一张语音识别失败的图片
        show_bmp("/mnt/udisk/ui/main_ui/MainUiErrorSpeech.bmp",0,0,0);//展示失败的图片
        //回到主界面 及展示主界面的图片
        pthread_exit(NULL);
        return "-1"; // 不做任何操作
    }
    if(strstr(recv_buf,"相册") != NULL){
        printf("打开相册\n");
        Album_UI();
        pthread_exit(NULL);
            
    }
    if(strstr(recv_buf,"音乐") != NULL){
        printf("打开音乐\n");
        // system("pkill arecord");
        playMusic();
        system("pkill madplay");
        pthread_exit(NULL);
    }
    if(strstr(recv_buf,"视频") != NULL){
        printf("打开视频\n");
        // system("pkill arecord");
        StartVideo();
        system("pkill mplayer");
        pthread_exit(NULL);
    }
    if(strstr(recv_buf,"游戏") != NULL){
        printf("打开游戏\n");
        StartGame();
        pthread_exit(NULL);

    }
    // pthread_exit(NULL);
    return 0;
}