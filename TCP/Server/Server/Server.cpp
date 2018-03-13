#include "stdafx.h"
#include <stdio.h>
#include <winsock2.h>  //WINSOCK.H(�����WINSOCK API��ͷ�ļ�,WIN2K����֧��WINSOCK2,���� 
//������WINSOCK2.H); 

#pragma comment(lib, "WS2_32.lib")//Ws2_32.lib(WINSOCK API���ӿ��ļ�).

#define BUF_SIZE 1024

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET  socketL;  // socket for listening
	SOCKET  socketC;  // socket for Communication
	SOCKADDR_IN serverAddr;
	int nRet = -1;
	char recvbuf[BUF_SIZE];
	char sendbuf[BUF_SIZE];

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("WSAStartup failed!\n");
		return 1;
	}

	socketL = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(5678);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(socketL, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
	listen(socketL, 5);
	while (1)
	{
		socketC = accept(socketL, NULL, NULL);
		if (socketC != INVALID_SOCKET)
		{
			nRet = recv(socketC, recvbuf, sizeof(recvbuf), 0);
		
			if (nRet > 0)
			{
				if (strcmp(recvbuf, "stop\n") == 0)
				{
					printf("�Ի�����\n");
					break;
				}
				else
					printf("Recv From Client:\n%s ", recvbuf);
				memset(recvbuf, 0, sizeof(recvbuf));
			}

			printf("Send a message:\n");
			fgets(sendbuf,BUF_SIZE, stdin);
			send(socketC, sendbuf, sizeof(sendbuf), 0);

			if (strcmp(sendbuf, "stop\n") == 0)
			{
				printf("�Ի�����\n");
				break;
			}

			memset(sendbuf, 0, sizeof(sendbuf));
		}
	}

	closesocket(socketC);
	closesocket(socketL);
	WSACleanup();

	printf("�밴�س����ر�");
	getchar();
	return 0;
}
