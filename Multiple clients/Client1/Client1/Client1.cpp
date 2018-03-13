#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <iostream>
#pragma comment(lib, "ws2_32.lib")
using namespace std;

#define PORT 4000
#define IP_ADDRESS "127.0.0.1"
int main(void)
{
	WSADATA     WSA;
	SOCKET      clientScoket;
	struct      sockaddr_in  serveAddr;
	int         AddrLen = 0;
	HANDLE      hThread = NULL;
	int         RET = 0;
	char        SendBuffer[MAX_PATH];
	//init windows socket
	if (WSAStartup(MAKEWORD(2, 2), &WSA) != 0)
	{
		cout << "init failed" << endl;
		return -1;
	}
	//creat socket
	clientScoket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientScoket == INVALID_SOCKET)
	{
		cout << "creat failed" << GetLastError() << endl;
		return -1;
	}
	serveAddr.sin_family = AF_INET;
	serveAddr.sin_addr.s_addr = inet_addr(IP_ADDRESS);
	serveAddr.sin_port = htons(PORT);
	memset(serveAddr.sin_zero, 0x00, 8);

	RET = connect(clientScoket, (struct sockaddr*)&serveAddr, sizeof(serveAddr));
	if (RET != 0)
	{
		cout << "connect failed";
		return -1;
	}
	else { cout << "Á´½Ó³É¹¥" << endl; }
	while (true)
	{
		cin.getline(SendBuffer, sizeof(SendBuffer));
		RET = send(clientScoket, SendBuffer, (int)strlen(SendBuffer), 0);
		if (RET == SOCKET_ERROR)
		{
			cout << "send to error" << endl;
			break;
		}
	}
	closesocket(clientScoket);
	WSACleanup();
	return 0;
}