// Thread.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <iostream>
#pragma comment(lib, "ws2_32.lib")
using namespace std;

#define PORT 4000
#define IP_ADDRESS "127.0.0.1"

//创建线程
DWORD WINAPI ClientThread(LPVOID ipParameter)
{
	SOCKET ClientScoket = (SOCKET)ipParameter;
	int RET = 0;
	char RecvBuffer[MAX_PATH];
	//初始化 recvBuffer 
	while (true) {
		memset(RecvBuffer, 0x00, sizeof(RecvBuffer));
		RET = recv(ClientScoket, RecvBuffer, MAX_PATH, 0);
		if (RET == 0 || RET == SOCKET_ERROR)
		{
			cout << "failed,exit" << endl;
			break;
		}
		cout << "接受的消息为" << RecvBuffer << endl;
	}

	return 0;
}
int main(void)
{
	WSADATA     WSA;
	SOCKET      severScoket, clientScoket;
	struct      sockaddr_in  LocalAddr, clientAddr;
	int         AddrLen = 0;
	HANDLE      hThread = NULL;
	int         RET = 0;
	//init windows socket
	if (WSAStartup(MAKEWORD(2, 2), &WSA) != 0)
	{
		cout << "init failed" << endl;
		return -1;
	}
	//creat socket
	severScoket = socket(AF_INET, SOCK_STREAM, 0);
	if (severScoket == INVALID_SOCKET)
	{
		cout << "creat failed" << GetLastError() << endl;
		return -1;
	}
	LocalAddr.sin_family = AF_INET;
	LocalAddr.sin_addr.s_addr = inet_addr(IP_ADDRESS);
	LocalAddr.sin_port = htons(PORT);
	memset(LocalAddr.sin_zero, 0x00, 8);
	//bind socket
	RET = bind(severScoket, (struct sockaddr*)&LocalAddr, sizeof(LocalAddr));
	if (RET != 0)
	{
		cout << "bind failed";
		return -1;
	}
	RET = listen(severScoket, 5);
	if (RET != 0)
	{
		cout << "listen failed";
		return -1;
	}
	cout << "服务器已启动" << endl;
	while (true)
	{
		AddrLen = sizeof(clientAddr);
		clientScoket = accept(severScoket, (struct sockaddr*)&clientAddr, &AddrLen);
		if (clientScoket == INVALID_SOCKET)
		{
			cout << "accept failed";
			break;
		}
		cout << "客户端连接" << inet_ntoa(clientAddr.sin_addr) << "." << clientAddr.sin_port << endl;
		hThread = CreateThread(NULL, 0, ClientThread, (LPVOID)clientScoket, 0, NULL);
		if (hThread == NULL)
		{
			cout << "creat thread failed" << endl;
			break;
		}
		CloseHandle(hThread);
	}
	closesocket(severScoket);
	closesocket(clientScoket);
	WSACleanup();
	return 0;
}