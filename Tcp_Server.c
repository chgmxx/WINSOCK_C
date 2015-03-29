#include "stdafx.h"
#include <Winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#pragma comment(lib, "ws2_32.lib");
#define DEFAULT_PORT 5050
int _tmain(int argc, char* argv[])
{
	int iPort = DEFAULT_PORT;
	WSADATA wsaData;
	SOCKET sListen, sAccept;
	int iLen;
	int iSend;
	char buf[] = "I am a server";
	struct sockaddr_in ser, cli;
	if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("Failed to load Winsock. \n");
		return -1;
	}
	sListen = socket(AF_INET, SOCK_STREAM, 0);
	if(sListen == INVALID_SOCKET)
	{
		printf("socket() Failed: %d \n",WSAGetLastError());
		return -1;
	}
	//以下建立服务器端地址
	ser.sin_family = AF_INET;
	ser.sin_port = htons(iPort);
	ser.sin_addr.s_addr = htonl(INADDR_ANY);
	if(bind(sListen, (LPSOCKADDR) &ser, sizeof(ser)) == SOCKET_ERROR)
	{
		printf("bind() Failed: %d \n", WSAGetLastError());
		return -1;
	}
	//进入监听状态
	if(listen(sListen, 5) == SOCKET_ERROR)
	{
		printf("listen() Failed: %d \n",WSAGetLastError());
		return -1;
	}
	//初始化客户地址长度参数
	iLen = sizeof(cli);
	while(true)
	{
		sAccept = accept(sListen, (struct sockaddr*)&cli, &iLen);
		if(sAccept == INVALID_SOCKET)
		{
			printf("accept() Failed:%d \n", WSAGetLastError());
			return -1;
		}
		printf("Accepted client IP: [s%], port[%d]\n",
		inet_ntoa(cli.sin_addr),ntohs(cli.sin_port));
		//给连接的客户发送信息
		iSend = send(sAccept, buf, sizeof(buf), 0);
		if(iSend == SOCKET_ERROR)
		{
			printf("send() Failed: %d \n",WSAGetLastError());
			break;
		}
		else if(iSend == 0)
		{
			break;
		}
		else
		{
			printf("send() bye: %d \n",iSend);
		}
		closesocket(sAccept);
	}
	closesocket(sListen);
	WSACleanup();
	return 0;
}
