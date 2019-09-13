
 /*
#include <stdio.h>                   
#include <winsock2.h>                
#pragma comment (lib, "ws2_32.lib")     

int main()
{
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    SOCKET s=socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in sockaddr;
    sockaddr.sin_family=PF_INET;
    sockaddr.sin_addr.S_un.S_addr=inet_addr("127.0.0.1");   
    sockaddr.sin_port=htons(9000);                          
    bind(s, (SOCKADDR*)&sockaddr, sizeof(SOCKADDR));        
    listen(s, 1);                                           
    printf("listening on port [%d].\n", 9000);
    while(TRUE)
    {
        SOCKADDR clientAddr;
        int size=sizeof(SOCKADDR);
        SOCKET clientsocket;
        printf("***SYS***    New client touched.\n");
        char* msg="Hello, my client.\r\n";
        send(clientsocket, msg, strlen(msg)+sizeof(char), NULL);  
        printf("***SYS***    HELLO.\n");
        while(TRUE)
        {
            char buffer[MAXBYTE]={0};
            recv(clientsocket, buffer, MAXBYTE, NULL);           
            printf("***Client***    %s\n", buffer);
        }
        closesocket(clientsocket);                               
    }
    closesocket(s);                                              
    WSACleanup();                                                
    getchar();                                                  
    exit(0);
	WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    SOCKET s=socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in sockaddr;
    sockaddr.sin_family=PF_INET;
    sockaddr.sin_addr.S_un.S_addr=inet_addr("127.0.0.1");   
    sockaddr.sin_port=htons(9000);                          
    bind(s, (SOCKADDR*)&sockaddr, sizeof(SOCKADDR));        
    listen(s, 1);                                           
    printf("listening on port [%d].\n", 9000);
    while(TRUE)
    {
        SOCKADDR clientAddr;
        int size=sizeof(SOCKADDR);
        SOCKET clientsocket;
        printf("***SYS***    New client touched.\n");
        char* msg="Hello, my client.\r\n";
        send(clientsocket, msg, strlen(msg)+sizeof(char), NULL);  
        printf("***SYS***    HELLO.\n");
        while(TRUE)
        {
            char buffer[MAXBYTE]={0};
            recv(clientsocket, buffer, MAXBYTE, NULL);           
            printf("***Client***    %s\n", buffer);
        }
        closesocket(clientsocket);                               
    }
    closesocket(s);                                              
    WSACleanup();                                                
    getchar();                                                  
    exit(0);
}
/*
#include <stdio.h>                   
#include <winsock2.h>                
#pragma comment (lib, "ws2_32.lib")     

int main()
{
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    SOCKET s=socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in sockaddr;
    sockaddr.sin_family=PF_INET;
    sockaddr.sin_addr.S_un.S_addr=inet_addr("127.0.0.1");   
    sockaddr.sin_port=htons(9000);                          
    bind(s, (SOCKADDR*)&sockaddr, sizeof(SOCKADDR));        
    listen(s, 1);                                           
    printf("listening on port [%d].\n", 9000);
    while(TRUE)
    {
        SOCKADDR clientAddr;
        int size=sizeof(SOCKADDR);
        SOCKET clientsocket;
        printf("***SYS***    New client touched.\n");
        char* msg="Hello, my client.\r\n";
        send(clientsocket, msg, strlen(msg)+sizeof(char), NULL);  
        printf("***SYS***    HELLO.\n");
        while(TRUE)
        {
            char buffer[MAXBYTE]={0};
            recv(clientsocket, buffer, MAXBYTE, NULL);           
            printf("***Client***    %s\n", buffer);
        }
        closesocket(clientsocket);                               
    }
    closesocket(s);                                              
    WSACleanup();                                                
    getchar();                                                  
    exit(0);
	WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    SOCKET s=socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in sockaddr;
    sockaddr.sin_family=PF_INET;
    sockaddr.sin_addr.S_un.S_addr=inet_addr("127.0.0.1");   
    sockaddr.sin_port=htons(9000);                          
    bind(s, (SOCKADDR*)&sockaddr, sizeof(SOCKADDR));        
    listen(s, 1);                                           
    printf("listening on port [%d].\n", 9000);
    while(TRUE)
    {
        SOCKADDR clientAddr;
        int size=sizeof(SOCKADDR);
        SOCKET clientsocket;
        printf("***SYS***    New client touched.\n");
        char* msg="Hello, my client.\r\n";
        send(clientsocket, msg, strlen(msg)+sizeof(char), NULL);  
        printf("***SYS***    HELLO.\n");
        while(TRUE)
        {
            char buffer[MAXBYTE]={0};
            recv(clientsocket, buffer, MAXBYTE, NULL);           
            printf("***Client***    %s\n", buffer);
        }
        closesocket(clientsocket);                               
    }
    closesocket(s);                                              
    WSACleanup();                                                
    getchar();                                                  
    exit(0);
}*/




