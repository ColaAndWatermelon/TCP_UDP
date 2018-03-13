#include "stdafx.h"

#include <stdio.h>  
#include <WINSOCK2.H>  

#pragma comment(lib,"WS2_32.lib")  
#define BUF_SIZE    64  

int main(void)
{
	WSADATA wsd;
	SOCKET  s;

	// ��ʼ���׽��ֶ�̬��  
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		printf("WSAStartup failed !\n");
		return 1;
	}

	// �����׽���  
	s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s == INVALID_SOCKET)
	{
		printf("socket() failed, Error Code:%d\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	char        recvbuf[BUF_SIZE];  // ��������  
	char        sendbuf[BUF_SIZE];
	SOCKADDR_IN servAddr;       // �������׽��ֵ�ַ  
	SOCKET      sockClient = socket(AF_INET, SOCK_DGRAM, 0);
	int         nRet;



	// ���÷�������ַ  
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.S_un.S_addr = inet_addr("120.35.80.104");
	servAddr.sin_port = htons(5000);

	while (1)
	{
		// ���������������  
		int nServAddLen = sizeof(servAddr);
		printf("Send a message:\n");
		fgets(sendbuf, BUF_SIZE, stdin);
		if (sendto(sockClient, sendbuf, BUF_SIZE, 0, (sockaddr *)&servAddr, nServAddLen) == SOCKET_ERROR)
		{
			printf("recvfrom() failed:%d\n", WSAGetLastError());
			closesocket(s);
			WSACleanup();
			return 1;
		}
		memset(sendbuf, 0, sizeof(sendbuf));

		nRet = recvfrom(sockClient,recvbuf, BUF_SIZE, 0, (sockaddr *)&servAddr, &nServAddLen);
		if (SOCKET_ERROR == nRet)
		{
			printf("recvfrom failed !\n");
			closesocket(s);
			WSACleanup();
			return -1;
		}
		// ��ӡ���Է���˷�����������  
		printf("Recv From Server:\n%s", recvbuf);
		memset(recvbuf, 0, sizeof(recvbuf));
	}
	closesocket(s);
	WSACleanup();
	return 0;
}