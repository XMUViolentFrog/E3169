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

   	//������Ҫ������ļ���
   printf("������Ҫ������ļ�����\n");  
   scanf("%s", sendBuf);
   //0.���ļ�
	FILE *fp=fopen(sendBuf,"rb");
	if(fp == NULL){  
        printf("open file %s failed\n", sendBuf);  
        return -1;  
    } 
   //1.��ʼ��winsock
   ret=WSAStartup(MAKEWORD(2,2),&data);
   if (SOCKET_ERROR==ret){
     printf("WSAStartup ����");
	 return -1;
   }
   //2.�����׽���:(af)ָ����ַ��,����TCP/IPЭ����׽���,��ֻ����AF_INET(Ҳ����д��PF_INET)��SOCKE_STREAMָ��������ʽ�׽���
   sockClient=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
   if (INVALID_SOCKET==sockClient){
     printf("socket ����");
	 return -2;
   }
   //3.���÷�������ַ
   struct sockaddr_in addrServer;
   int len;
   addrServer.sin_addr.S_un.S_addr=inet_addr(SERVERIP);
   addrServer.sin_family=AF_INET;
   addrServer.sin_port=htons(SERVERPORT);
   len=sizeof(sockaddr);
  //4.���ӷ�����
   printf("�ͻ��˳�ʼ����ɣ��������ӷ�����...\n\n");
   ret=connect(sockClient,(sockaddr*)&addrServer,len);
   if (SOCKET_ERROR==ret){
     printf("connect ����");
	 return -3;
   }

   printf("���ӷ������ɹ������濪ʼͨ��...\n\n");
   //5.��������
   memset(recvBuf,0,sizeof(recvBuf));
   ret=recv(sockClient,recvBuf,sizeof(recvBuf),0);
   if (SOCKET_ERROR==ret){
     printf("recv ����");
	 return -4;
   }

   printf("�ӷ������յ�������Ϣ:\n%s\n\n",recvBuf);
   //6.���ļ����һ�δη��뻺�������������һ�δη���temp
   int num=0;
   while(!feof(fp)){//������ϵ��ļ�������������ļ��������򷵻ط�0ֵ�����򷵻�0���ļ�������ֻ�ܱ�clearerr()���
	   /*size_t fread ( void *buffer, size_t size, size_t count, FILE *stream) ;
	     buffer		   ���ڽ������ݵ��ڴ��ַ
		 size		   Ҫ����ÿ����������ֽ�������λ���ֽ�
		 count		   Ҫ��count�������ÿ��������size���ֽ�.
		 stream		   ������*/
	   num=fread(sendBuf,1,512,fp);
	   ret=send(sockClient,sendBuf,num,0);
	   if (SOCKET_ERROR==ret){
		   printf("send ����");
		   return -5;
	   }
   }
   
   printf("client file transfer success!\n"); 
   //7.�ر��ļ�
   fclose(fp);
   //8.shutdown()���������κ����͵��׽ӿڽ�ֹ���ա���ֹ���ͻ��ֹ�շ���
   // �����ر��׽ӿڣ����׽ӿ���ռ�е���Դ����һֱ���ֵ�closesocket()���á�
   // ѡ��SE_SEND,���ʾ�������ٵ��÷��ͺ���
   ret=shutdown(sockClient,SD_SEND);
   if(ret == SOCKET_ERROR){  
	   printf("server shutdown failed %ld\n", WSAGetLastError());  
	   closesocket(sockClient);  
	   WSACleanup();  
	   return 1;  
   } 
   //9.�ر��׽���
   closesocket(sockClient);
   //10.ж��winsock
   WSACleanup();
   system("pause");

   return 0;
}