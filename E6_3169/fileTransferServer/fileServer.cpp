#include "stdio.h"
#include "winsock2.h"
#pragma comment(lib,"ws2_32.lib")

#define SERVERIP "192.168.0.102"
#define SERVERPORT 6666
int main(){
	int ret;
	char sendBuf[512],recvBuf[512],file_name[512];
	//���windows socket��ʼ����Ϣ
	WSADATA data;
	SOCKET sockListen,sockTongXun;


	printf("����洢�ļ����ļ���:\n");
	scanf("%s",file_name);

	//1.create file
	FILE *fp=fopen(file_name,"wb");
	if (fp==NULL){
		printf("creat file %s failed\n",file_name);
		return -1;
	}
	//2.��ʼ��winsock
	ret=WSAStartup(MAKEWORD(2,2),&data);
	if (SOCKET_ERROR==ret){
		printf("client WSAStartup failed: %d\n",ret);
		return -1;
	}
	//3.�����׽���(�׽�������,��ʽ�׽���,TCPЭ��)
	sockListen=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if (SOCKET_ERROR==sockListen){
		printf("socket ����");
		return -2;
	}
	//4.���÷�������ַ
	struct sockaddr_in addrServer;
	int len;
	addrServer.sin_addr.S_un.S_addr=inet_addr(SERVERIP);
	addrServer.sin_family=AF_INET;
	addrServer.sin_port=htons(SERVERPORT);
	len=sizeof(sockaddr);
	//5.���׽���
	ret=bind(sockListen,(sockaddr*)&addrServer,len);
	if (SOCKET_ERROR==ret){
		printf("bind ����");
		return -3;
	}

	//6.��ʼ����
	ret=listen(sockListen,1);
	if (SOCKET_ERROR==ret){
		printf("listen ����");
		return -4;
	}
	printf("�����������ɹ������ڼ���...\n\n");
	//7.���û��clientҪ���ӣ�server�ڵ���ʱ����������clientҪ���ӣ��ʹ����Ҫ���ӵĶ�����ȡ��Ȼ��accept
	//����֪����clientҪ����
	struct sockaddr_in addrClient;
	len=sizeof(sockaddr);
	sockTongXun=accept(sockListen,(sockaddr*)&addrClient,&len);
	if (INVALID_SOCKET==ret){
		printf("accept ����");
		return -5;
	}

	printf("���յ�һ���ͻ������ӣ����濪ʼͨ��...\n\n");
	//8.��client������Ϣ
	memset(sendBuf,0,sizeof(sendBuf));
	strcpy(sendBuf,"Welcome to tcpServer�����Ѿ����ӵ���TCP��������");
	ret=send(sockTongXun,sendBuf,sizeof(sendBuf)+1,0);
	if (SOCKET_ERROR==ret){
		printf("send ����");
		return -6;
	}
	printf("��ͻ��˳ɹ�����������Ϣ:\n%s\n\n",sendBuf);

	memset(recvBuf,0,sizeof(recvBuf));
	
	//9.���ܴӿͻ��˷�������Ϣ
	int num = 0;
	while (1){
		num = recv(sockTongXun, recvBuf, 512, 0);
		if (SOCKET_ERROR==num){
			printf("recv ����");
			return -7;
		}
		if(num == 0) 
			break;
		fwrite(recvBuf, 1, num, fp);
	}
	printf("transmission done\n");
	//10.�ر��ļ�
	fclose(fp);
	//11.shutdown ����sock
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
	//12.�ر��׽���
	closesocket(sockListen);
	closesocket(sockTongXun);
	//13.ж��winsock
	WSACleanup();
	system("pause");

	return 0;
}