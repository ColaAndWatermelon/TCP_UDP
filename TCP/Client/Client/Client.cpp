#include "stdafx.h"
#include <stdio.h>
#include <winsock2.h>
#pragma comment(lib, "WS2_32.lib")
#define BUF_SIZE 1024
int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKADDR_IN serverAddr;
	SOCKET socketC;
	char sendbuf[BUF_SIZE];
	char recvbuf[BUF_SIZE];
	int nRet = -1;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("WSAStartup failed!\n");
		return 1;
	}
	
	
	

	while (1)
	{
		socketC = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_port = htons(5678);
		serverAddr.sin_addr.s_addr = inet_addr("120.35.80.104");
		connect(socketC, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
		printf("Send a message:\n");
		fgets(sendbuf, BUF_SIZE, stdin);
		send(socketC, sendbuf, sizeof(sendbuf), 0);
		if (strcmp(sendbuf, "stop\n") == 0)
		{
			printf("对话结束\n");
			break;
		}
		memset(sendbuf, 0, sizeof(sendbuf));
		if (socketC != INVALID_SOCKET)
		{
			nRet = recv(socketC, recvbuf, sizeof(recvbuf), 0);
			if (nRet > 0)
			{			
					if (strcmp(recvbuf, "stop\n") == 0)
					{
						printf("对话结束\n");
						break;
					}
					else
				        printf("Recv from server:\n%s ", recvbuf);
				memset(recvbuf, 0, sizeof(recvbuf));
			}
		}
	}



	closesocket(socketC);

	WSACleanup();

	printf("请按回车键关闭");
	getchar();
	return 0;
}
