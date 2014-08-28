#pragma once
/*
作者 白彦聪
日期	20140822
说明	底层串口通信类，负责控制串口发送接收，使用select监听事件。
*/
#include     <stdio.h>      /*标准输入输出定义*/
#include     <stdlib.h>     /*标准函数库定义*/
#include     <unistd.h>     /*Unix 标准函数定义*/
#include     <sys/types.h>  
#include     <sys/stat.h>   
#include     <fcntl.h>      /*文件控制定义*/
#include     <termios.h>    /*PPSIX 终端控制定义*/
#include     <errno.h>      /*错误号定义*/
#include	<pthread.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#define FALSE -1
#define TRUE 0
#define  TIMEOUT 2    //等待时间2秒
#define  BUFFERSIZE 512		//缓存大小

//回调函数，返回从设备接收到的数据。调用者类指针，终端ID，数据内容，长度
typedef int (*pf) (void *,char * ,char *,int );
class CComDev
{
public:
	CComDev();
	~CComDev();

	//设置参数	
	/*
	*@object   类型  void * 调用者类指针
	*@nSpeed   类型  int 速率
	*@param  Dev     类型  char  设备路径
	*@param  nBits 类型  int 数据位   取值 为 7 或者8
	*@param  nStop 类型  int 停止位   取值为 1 或者2
	*@param  nEvent  类型  int  效验类型 取值为N,E,O,,S
	*/
	void SetParam(void* object,
		char *Dev,
		int nSpeed, 
		int nBits, 
		char nEvent, 
		int nStop,
		pf myFun);

	// 开启通信服务
	int Start();

	//关闭服务
	int Stop();

	//发送数据
	int SendData(char *buff, int size);
	
	// 等待设备接收通知
	int WaitDevNotif();


protected:
	
	//开启串口文件
	int OpenDev(char *Dev);

	//设置参数配置
	int set_opt(int nSpeed, int nBits, char nEvent, int nStop);

	//关闭文件
	int CloseDev();

	//获取数据
	int RecvData();

	// 字符类型转换为十六进制显示
	int CharToHex(char *Dest, int nDestSize,
		char *Source, int nSourceLength);

	//回调函数，返回从设备接收到的数据
	pf m_RecvDevDataFun;


	int m_nFd;		//串口文件句柄

	pthread_t m_RecvPt;		//接收线程句柄

	char m_cDev[20];
	int m_nSpeed;
	int m_nBits;
	int m_nEvent;
	int m_nStop;

	char m_buff[BUFFERSIZE];
	long m_nBuffLength ;

	void *m_object;
};

