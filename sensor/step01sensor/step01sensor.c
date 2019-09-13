#define _CRT_SECURE_NO_WARNG
#include <WinSock2.h>
#include <process.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#pragma comment(lib,"ws2_32.lib")
#include "md5.h"  
#include "pbc.h"
#include "pbc_fp.h"
#include "pbc_fieldquadratic.h"
#include "pbc_curve.h"
#define RECV_OVER 1
#define RECV_YET 0
#pragma comment(lib,"pbc.lib")

char userName[16] = { 0 };
unsigned char ident[1000] = { 0 };
char file[6000];
int iStatus = RECV_YET;
int receiving = 0;
unsigned char* pos;
int px1, px2, py1, py2;
int block_num;
int seq_num;
int b1;
element_t w0;
int private;
int other;


char file[6000] = { 0 };
int state = 1;


unsigned __stdcall ThreadRecv(void* param)
{
	//printf("rec");
	unsigned char buf[2048] = { 0 };
	while (1)
	{
		int ret = recv(*(SOCKET*)param, buf, sizeof(buf), 0);
		if (ret == SOCKET_ERROR)
		{
			Sleep(500);
			continue;
		}

		if (strlen(buf) != 0)
		{
			pos = buf;//pos = reading
			if (buf[0] == 1) {
				printf("I am receiving the packet of type 1\n");
				get_first();
				if (type1_counter == 2) {
					char message[6] = { 0 };
					message[0] = 'r';
					message[1] = 'e';
					message[2] = 'a';
					message[3] = 'd';
					message[4] = userName[0];
					
					send(*(SOCKET*)param, message, sizeof(message), 0);
					printf("\n --------------I am ready for next step--------------------\n");
				}
			}
			if (buf[0] == 2) {
				get_file_image();
			}
			if (buf[0] == 3 && userName[0] == '1') {
				get_one_image();

			}
			if (buf[0] == 4 && userName[0] == '3') {
				get_some_frame();
			}
			if (state == 2) {
				//Sleep(2000);
				printf("\n--------------------------------\n");
				printf("first state finished:\n code I received:\n %s", file);
				// test correctness packet
				int o = 0;
				int x = 0;
				while (o < 6) {
					if (packet_lost[o] == 1) {
						state = 1;
						char message[6] = { 0 };
						message[0] = 1;
						message[1] = o;
						message[2] = 0;
						message[3] = 0;
						message[4] = userName[0];
						send(*(SOCKET*)param, message, sizeof(message), 0);
						Sleep(1000);
						x++;
					}
					
					o++;
				}
				if (x > 0) {
					continue;
				}
				printf("no pakcet lost happen\n");
				//test frame
				o = 0;
				int flag_1 = 0;
				char message2[23] = { 0 };
				
				while (o < 21) {
					if (frame_lost[o] == 1) {
						message2[o + 2] = 1;
						printf(" %d's fram is incorrect ", o);
						flag_1 = 1;
					}
					
					o++;
				}
				printf("\n");
				if (flag_1) {
					
					message2[0] = 2;
					message2[1] = userName[0];
					printf("%s\n", message2);
					send(*(SOCKET*)param, message2, sizeof(message2), 0);

					Sleep(1000);
				}
				if (flag_1) {
					continue;
				}
				//
				
				//all correct
				char message[6] = { 0 };
				message[0] = 3;
				message[1] = 0;
				message[2] = 0;
				message[3] = 0;
				message[4] = userName[0];
				send(*(SOCKET*)param, message, sizeof(message), 0);

				printf("\n------------the file -----------------\n %s", file);
				printf("\n----------------end-------------------------------\n");
				Sleep(1000000000000);
			}
			
		}
	}
}

int ConnectServer()
{
	WSADATA wsaData = { 0 };
	SOCKET ClientSocket = INVALID_SOCKET;
	SOCKADDR_IN ServerAddr = { 0 };
	USHORT uPort = 18000;
						 
	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		printf("WSAStartup failed with error code: %d\n", WSAGetLastError());
		return -1;
	}
	
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		printf("wVersion was not 2.2\n");
		return -1;
	}
	
	ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ClientSocket == INVALID_SOCKET)
	{
		printf("socket failed with error code: %d\n", WSAGetLastError());
		return -1;
	}
	
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(uPort);
	ServerAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	printf("connecting......\n");
	
	if (SOCKET_ERROR == connect(ClientSocket, (SOCKADDR*)& ServerAddr, sizeof(ServerAddr)))
	{
		printf("connect failed with error code: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		return -1;
	}
	printf("Connecting server successfully IP:127.0.0.1 Port:%d\n",
		htons(ServerAddr.sin_port));
	printf("Please input your UserName: ");
	gets_s(userName, sizeof(userName));
	send(ClientSocket, userName, sizeof(userName), 0);
	printf("\n\n");
	_beginthreadex(NULL, 0, ThreadRecv, &ClientSocket, 0, NULL); 
	
	for (int k = 0; k < 1000; k++)
		Sleep(10000000);
	closesocket(ClientSocket);
	WSACleanup();
	return 0;
}

int main()
{
	type1_counter = 0;
	ConnectServer();
	return 0;
}

