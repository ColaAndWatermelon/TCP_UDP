#include "stdafx.h"
#include <stdio.h>  
#include <WINSOCK2.H>  

#pragma comment(lib,"WS2_32.lib")  
#define BUF_SIZE    64  

int main(void)
{
	WSADATA wsd;
	SOCKET  s;
	int     nRet;


	// 初始化套接字动态库  
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		printf("WSAStartup failed !\n");
		return 1;
	}

	// 创建套接字  
	s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s == INVALID_SOCKET)
	{
		printf("socket() failed ,Error Code:%d\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	SOCKET      socketSrv = socket(AF_INET, SOCK_DGRAM, 0);
	SOCKADDR_IN addrSrv;
	SOCKADDR_IN addrClient;
	char        sendbuf[BUF_SIZE];
	char        recvbuf[BUF_SIZE];
	int         len = sizeof(SOCKADDR);

	// 设置服务器地址  

	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(5000);

	// 绑定套接字  
	
	nRet = bind(socketSrv, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
	if (SOCKET_ERROR == nRet)
	{
		printf("bind failed !\n");
		closesocket(s);
		WSACleanup();
		return -1;
	}

	while (1)
	{
		// 从客户端接收数据  
		nRet = recvfrom(socketSrv, recvbuf, BUF_SIZE, 0, (SOCKADDR*)&addrClient, &len);
		if (SOCKET_ERROR == nRet)
		{
			printf("recvfrom failed !\n");
			closesocket(s);
			WSACleanup();
			return -1;
		}
		// 打印来自客户端发送来的数据  
		printf("Recv From Client:\n%s", recvbuf);
		memset(recvbuf, 0, sizeof(recvbuf));
		printf("Send a message:\n");
		fgets(sendbuf, BUF_SIZE, stdin);
		// 向客户端发送数据  
		sendto(socketSrv, sendbuf, sizeof(sendbuf), 0, (SOCKADDR*)&addrClient, len);
		memset(sendbuf, 0, sizeof(sendbuf));
	}
	closesocket(s);
	WSACleanup();
	return 0;
}
