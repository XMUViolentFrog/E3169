#include "stdio.h"
#include "winsock2.h"
#pragma comment(lib,"ws2_32.lib")

#define SERVERIP "192.168.0.102"
#define SERVERPORT 6666
int main(){
	int ret;
	char sendBuf[512],recvBuf[512],file_name[512];
	//存放windows socket初始化信息
	WSADATA data;
	SOCKET sockListen,sockTongXun;


	printf("输入存储文件的文件名:\n");
	scanf("%s",file_name);

	//1.create file
	FILE *fp=fopen(file_name,"wb");
	if (fp==NULL){
		printf("creat file %s failed\n",file_name);
		return -1;
	}
	//2.初始化winsock
	ret=WSAStartup(MAKEWORD(2,2),&data);
	if (SOCKET_ERROR==ret){
		printf("client WSAStartup failed: %d\n",ret);
		return -1;
	}
	//3.创建套接字(套接字类型,流式套接字,TCP协议)
	sockListen=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if (SOCKET_ERROR==sockListen){
		printf("socket 错误");
		return -2;
	}
	//4.设置服务器地址
	struct sockaddr_in addrServer;
	int len;
	addrServer.sin_addr.S_un.S_addr=inet_addr(SERVERIP);
	addrServer.sin_family=AF_INET;
	addrServer.sin_port=htons(SERVERPORT);
	len=sizeof(sockaddr);
	//5.绑定套接字
	ret=bind(sockListen,(sockaddr*)&addrServer,len);
	if (SOCKET_ERROR==ret){
		printf("bind 错误");
		return -3;
	}

	//6.开始监听
	ret=listen(sockListen,1);
	if (SOCKET_ERROR==ret){
		printf("listen 错误");
		return -4;
	}
	printf("服务器启动成功，正在监听...\n\n");
	//7.如果没有client要连接，server在调用时就阻塞，有client要连接，就从这个要连接的队列里取出然后accept
	//监听知道有client要连接
	struct sockaddr_in addrClient;
	len=sizeof(sockaddr);
	sockTongXun=accept(sockListen,(sockaddr*)&addrClient,&len);
	if (INVALID_SOCKET==ret){
		printf("accept 错误");
		return -5;
	}

	printf("接收到一个客户端连接，下面开始通信...\n\n");
	//8.向client发送消息
	memset(sendBuf,0,sizeof(sendBuf));
	strcpy(sendBuf,"Welcome to tcpServer，你已经连接到了TCP服务器！");
	ret=send(sockTongXun,sendBuf,sizeof(sendBuf)+1,0);
	if (SOCKET_ERROR==ret){
		printf("send 错误");
		return -6;
	}
	printf("向客户端成功发送以下信息:\n%s\n\n",sendBuf);

	memset(recvBuf,0,sizeof(recvBuf));
	
	//9.接受从客户端发来的消息
	int num = 0;
	while (1){
		num = recv(sockTongXun, recvBuf, 512, 0);
		if (SOCKET_ERROR==num){
			printf("recv 错误");
			return -7;
		}
		if(num == 0) 
			break;
		fwrite(recvBuf, 1, num, fp);
	}
	printf("transmission done\n");
	//10.关闭文件
	fclose(fp);
	//11.shutdown 两个sock
	/*ret=shutdown(sockListen,SD_SEND);
	if(ret == SOCKET_ERROR){  
	printf("server shutdown failed %ld\n", WSAGetLastError());  
	closesocket(sockListen);  
	WSACleanup();  
	return 1;  
	} 
	ret=shutdown(sockTongXun,SD_SEND);
	if(ret == SOCKET_ERROR){  
	printf("server shutdown failed %ld\n", WSAGetLastError());  
	closesocket(sockTongXun);  
	WSACleanup();  
	return 1;  
	} */
	//12.关闭套接字
	closesocket(sockListen);
	closesocket(sockTongXun);
	//13.卸载winsock
	WSACleanup();
	system("pause");

	return 0;
}