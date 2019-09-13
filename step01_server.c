#include <WinSock2.h>
#include <process.h>
#include <stdio.h>
#include <stdlib.h>
#include "pbc.h"
#include "pbc_fp.h"
#include "pbc_fieldquadratic.h"
#include "pbc_curve.h"
#include "md5.h"  
#include "server_p.c"
//#include "windows.h"
#include "time.h"
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"pbc.lib")


SOCKET g_ServerSocket = INVALID_SOCKET;      
SOCKADDR_IN g_ClientAddr = { 0 };            
int g_iClientAddrLen = sizeof(g_ClientAddr);
int g_bCheckConnect = 0;                
char file[6000] = { 0 };
int file_length = 0;
unsigned char md5_m[1000] = { 0 };
int md5_length = 0;
HANDLE g_hRecv1 = NULL;
HANDLE g_hRecv2 = NULL;

ypedef struct _Client
{
	SOCKET sClient;      
	unsigned char buf[2048];      
	char userName[16];   
	char IP[20];         
	UINT_PTR flag;       
	int stage;
}Client;
Client g_Client[3] = { 0 };

unsigned __stdcall ThreadRecv1(void* param)
{
	//printf("one thread begin1\n");
	SOCKET client = INVALID_SOCKET;
	char temp[128] = { 0 };  
	while (1)
	{
		memset(temp, 0, sizeof(temp));
		int ret = recv(g_Client[0].sClient, temp, sizeof(temp), 0); 
		if (strlen(temp) != 0) {
			//printf("\n----------------%s\n", temp);
			if (temp[0] == 1) {
				printf("\n---packet %d loss----\n", temp[1]);
				//g_Client[0].stage = 1;
				packet_lost[temp[1]] = 1;
				all_state = 2;
			}
			if (temp[4] == '1' && temp[0] == 'r') { //change
				printf("\n----------------%s\n", temp);
				g_Client[0].stage = 1;
			}
			///move to 2
			
			
		}
		//printf("recode erro at %d:%d \n", temp[0], temp[1]);
		//char* message = "message from sever";
		if (ret == SOCKET_ERROR)
			continue;
		
	}

	return 0;
}



unsigned __stdcall ThreadRecv3(void* param)
{
	//printf("one thread begin1\n");
	SOCKET client = INVALID_SOCKET;
	char temp[128] = { 0 };  
	while (1)
	{
		memset(temp, 0, sizeof(temp));
		int ret = recv(g_Client[2].sClient, temp, sizeof(temp), 0); 
		if (strlen(temp) != 0) {
			printf("-----------------%s\n", temp);
			if (temp[4] == '3') {
				g_Client[2].stage = 1;
			}
			if (temp[0] == 2 && temp[1] == '3') {
				int j = 0;
				printf("\n=------");
				while (j < 21) {
					if (temp[2 + j] == 1) {
						frame_lost[j] = 1;
						printf(" frame %d lost ", j);
					}
					j++;
				}
				printf("------\n");
				all_state = 2;
			}
		}
		//printf("recode erro at %d:%d \n", temp[0], temp[1]);
		//char* message = "message from sever";
		if (ret == SOCKET_ERROR)
			continue;
		
	}

	return 0;
}

unsigned __stdcall ThreadRecv4(void* param)
{
	//printf("one thread begin1\n");
	SOCKET client = INVALID_SOCKET;
	char temp[128] = { 0 };  
	while (1)
	{
		memset(temp, 0, sizeof(temp));
		int ret = recv(g_Client[2].sClient, temp, sizeof(temp), 0); 
		if (strlen(temp) != 0) {
			printf("-----------------%s\n", temp);
			if (temp[4] == '3') {
				g_Client[2].stage = 1;
			}
			if (temp[0] == 2 && temp[1] == '3') {
				int j = 0;
				printf("\n=------");
				while (j < 21) {
					if (temp[2 + j] == 1) {
						frame_lost[j] = 1;
						printf(" frame %d lost ", j);
					}
					j++;
				}
				printf("------\n");
				all_state = 2;
			}
		}
		//printf("recode erro at %d:%d \n", temp[0], temp[1]);
		//char* message = "message from sever";
		if (ret == SOCKET_ERROR)
			continue;
		
	}

	return 0;
}

void getFile() {
	FILE* fp;
	char ch;
	//char str[2000] = { 0 };
	fp = fopen("main.c", "r");
	if (fp == NULL)
		printf("file cannot open \n");
	int i = 0;
	while ((ch = fgetc(fp)) != EOF && i < 6000)
	{
		file[i] = ch;
		i++;
		//printf("%c\n",ch);
	}
	printf("the new program is=:\n");
	printf("%s\n\n the size of file is %d\n", file, i);
	file_length = i;
	printf("-------------------------------\n");
	getch();
	fclose(fp);

	int j = 0, k = 0;
	unsigned char encrypt[257] = { 0 };
	unsigned char decrypt[16];
	MD5_CTX md5;
	MD5Init(&md5);
	while (j < i) {
		int counter = 0;
		//printf("%d;\n", j / 32);
		while (counter < 256 && j < i) {
			encrypt[counter] = file[j];
			j++;
			counter++;
		}

		MD5Init(&md5);
		MD5Update(&md5, encrypt, strlen((char*)encrypt));
		MD5Final(&md5, decrypt);
		printf("text:%s\n ", encrypt);

		printf("result_32bit:");
		int o;
		for (o = 0; o < 16; o++) {
			md5_m[k] = decrypt[o];
			printf("%02x", md5_m[k]);
			k++;
			//md5_length++;
		}
		md5_length = k;
		printf("-----------md5_length:%d---------------\n\n\n\n", md5_length);

	}
	printf("\n");
	printf("\n");

}


unsigned __stdcall ThreadAccept(void* param)
{
	_beginthreadex(NULL, 0, ThreadRecv1, &g_Client[0].sClient, 0, NULL);
	_beginthreadex(NULL, 0, ThreadRecv2, &g_Client[1].sClient, 0, NULL);
	_beginthreadex(NULL, 0, ThreadRecv3, &g_Client[1].sClient, 0, NULL);
	_beginthreadex(NULL, 0, ThreadRecv4, &g_Client[1].sClient, 0, NULL);
	int i = 0;
	int temp1 = 0, temp2 = 0;
	int flag = 1;
	//_beginthreadex(NULL, 0, ThreadManager, NULL, 0, NULL);
	while (1)
	{
		while (i < 4)
		{
			printf("waiting for connection of all sensors\n");
			if (g_Client[i].flag != 0)
			{
				++i;
				continue;
			}
			
			if ((g_Client[i].sClient = accept(g_ServerSocket, (SOCKADDR*)& g_ClientAddr, &g_iClientAddrLen)) == INVALID_SOCKET)
			{
				printf("accept failed with error code: %d\n", WSAGetLastError());
				closesocket(g_ServerSocket);
				WSACleanup();
				return -1;
			}
			recv(g_Client[i].sClient, g_Client[i].userName, sizeof(g_Client[i].userName), 0); 
			printf("Successfuuly got a connection from IP:%s ,Port: %d,UerName: %s\n",
				inet_ntoa(g_ClientAddr.sin_addr), htons(g_ClientAddr.sin_port), g_Client[i].userName);
			memcpy(g_Client[i].IP, inet_ntoa(g_ClientAddr.sin_addr), sizeof(g_Client[i].IP)); 
			g_Client[i].flag = g_Client[i].sClient; 
			i++;
		}
		i = 0;
		//printf("here\n");
		if (g_Client[0].flag != 0 && g_Client[1].flag != 0 && g_Client[2].flag != 0 ){
		//if (g_Client[0].flag != 0){
			//;_beginthreadex(NULL, 0, ThreadRecv3, &g_Client[2].sClient, 0, NULL);
			if (all_state == 0) {
				printf("-----------------all sensor connectted press to go on------------------------------------\n");
				getch();
				getFile();
				sendFirstP();
				Sleep(1000);
				if (g_Client[0].stage == 1 || g_Client[2].stage == 1){ //&& g_Client[2].stage == 1) {
					all_state++;
				}
			}
			else if (all_state == 1) {
				printf("--------now sending the code image-----------\n");
				getch();
				sendSecond();
				//Sleep(10000);
				getch();
			}
			else if (all_state == 2){//debug
				if (flag) {// send lost packet
					printf("no22\\\n");
					int o=0;
					while (o < 6) {
						if (packet_lost[o] == 1) {
							sendOnePacket(o);
							Sleep(5000);
							break;
						}

						o++;
					}
					flag--;
				}
				else { // send lost code image
					int o = 0;
					while (o < 21) {
						if (frame_lost[o] == 1) {
							sendSomeFrame();
							printf("no\\\n");
							Sleep(5000);
							break;
						}

						o++;
					}
					//sendSomeFrame();
					//printf("no\\\n");
					flag++;
				}
				Sleep(2000);
				printf("\n------------finish------------------\n");
				//Sleep(10000000000);
			
			}
			
		}
	}
	return 0;
}

int StartServer()
{
	
	WSADATA wsaData = { 0 };
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
	g_ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (g_ServerSocket == INVALID_SOCKET)
	{
		printf("socket failed with error code: %d\n", WSAGetLastError());
		return -1;
	}

	
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(uPort);
	ServerAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	
	if (SOCKET_ERROR == bind(g_ServerSocket, (SOCKADDR*)& ServerAddr, sizeof(ServerAddr)))
	{
		printf("bind failed with error code: %d\n", WSAGetLastError());
		closesocket(g_ServerSocket);
		return -1;
	}
	
	if (SOCKET_ERROR == listen(g_ServerSocket, 20000))
	{
		printf("listen failed with error code: %d\n", WSAGetLastError());
		closesocket(g_ServerSocket);
		WSACleanup();
		return -1;
	}

	_beginthreadex(NULL, 0, ThreadAccept, NULL, 0, 0);
	for (int k = 0; k < 100; k++)
		Sleep(10000000);

	
	for (int j = 0; j < 2; j++)
	{
		if (g_Client[j].sClient != INVALID_SOCKET)
			closesocket(g_Client[j].sClient);
	}
	closesocket(g_ServerSocket);
	WSACleanup();
	printf("server end");
	return 0;
}




int main()
{
	all_state = 0;
	StartServer();
	return 0;
}
