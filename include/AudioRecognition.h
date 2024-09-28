#ifndef _AudioRecognition_H_
#define _AudioRecognition_H_ //防止头文件冲突


#define SERVERIP "192.168.18.11"

//变量声明


//函数声明

/**
 * char * linkKeDaAudioRecognition(struct sockaddr_in server_addr)
 * 发送音频文件给科大讯飞 (connect,send,recv) 返回值识别到的信息
 * 
 *   第一个参数： 装服务器的ip地址、端口、家族协议
 *   返回值： 科大讯飞返回的信息
 *
 **/
char* linkKeDaAudioRecognition();

/**
 * void audioRecognitionControl()
 * 语音识别的主要实现函数，用来调用各个模块的函数
 * 
**/
void * audioRecognitionControl();



#endif