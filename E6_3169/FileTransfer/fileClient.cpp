#include "stdio.h"
#include "winsock2.h"
#pragma comment(lib,"ws2_32.lib")
#define SERVERIP    "192.168.0.102"
#define SERVERPORT  6666

int main(void)
{
   int ret;
   char sendBuf[512],recvBuf[512];
   WSADATA data;
   SOCKET sockClient;

   	//输入需要传输的文件名
   printf("请输入要传输的文件名：\n");  
   scanf("%s", sendBuf);
   //0.打开文件
	FILE *fp=fopen(sendBuf,"rb");
	if(fp == NULL){  
        printf("open file %s failed\n", sendBuf);  
        return -1;  
    } 
   //1.初始化winsock
   ret=WSAStartup(MAKEWORD(2,2),&data);
   if (SOCKET_ERROR==ret){
     printf("WSAStartup 错误");
	 return -1;
   }
   //2.创建套接字:(af)指定地址族,对于TCP/IP协议的套接字,它只能是AF_INET(也可以写成PF_INET)、SOCKE_STREAM指定产生流式套接字
   sockClient=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
   if (INVALID_SOCKET==sockClient){
     printf("socket 错误");
	 return -2;
   }
   //3.设置服务器地址
   struct sockaddr_in addrServer;
   int len;
   addrServer.sin_addr.S_un.S_addr=inet_addr(SERVERIP);
   addrServer.sin_family=AF_INET;
   addrServer.sin_port=htons(SERVERPORT);
   len=sizeof(sockaddr);
  //4.连接服务器
   printf("客户端初始化完成，正在连接服务器...\n\n");
   ret=connect(sockClient,(sockaddr*)&addrServer,len);
   if (SOCKET_ERROR==ret){
     printf("connect 错误");
	 return -3;
   }

   printf("连接服务器成功，下面开始通信...\n\n");
   //5.接受数据
   memset(recvBuf,0,sizeof(recvBuf));
   ret=recv(sockClient,recvBuf,sizeof(recvBuf),0);
   if (SOCKET_ERROR==ret){
     printf("recv 错误");
	 return -4;
   }

   printf("从服务器收到以下信息:\n%s\n\n",recvBuf);
   //6.将文件拆分一次次放入缓冲区，向服务器一次次发送temp
   int num=0;
   while(!feof(fp)){//检测流上的文件结束符，如果文件结束，则返回非0值，否则返回0，文件结束符只能被clearerr()清除
	   /*size_t fread ( void *buffer, size_t size, size_t count, FILE *stream) ;
	     buffer		   用于接收数据的内存地址
		 size		   要读的每个数据项的字节数，单位是字节
		 count		   要读count个数据项，每个数据项size个字节.
		 stream		   输入流*/
	   num=fread(sendBuf,1,512,fp);
	   ret=send(sockClient,sendBuf,num,0);
	   if (SOCKET_ERROR==ret){
		   printf("send 错误");
		   return -5;
	   }
   }
   
   printf("client file transfer success!\n"); 
   //7.关闭文件
   fclose(fp);
   //8.shutdown()函数用于任何类型的套接口禁止接收、禁止发送或禁止收发。
   // 并不关闭套接口，且套接口所占有的资源将被一直保持到closesocket()调用。
   // 选择SE_SEND,则表示不允许再调用发送函数
   ret=shutdown(sockClient,SD_SEND);
   if(ret == SOCKET_ERROR){  
	   printf("server shutdown failed %ld\n", WSAGetLastError());  
	   closesocket(sockClient);  
	   WSACleanup();  
	   return 1;  
   } 
   //9.关闭套接字
   closesocket(sockClient);
   //10.卸载winsock
   WSACleanup();
   system("pause");

   return 0;
}