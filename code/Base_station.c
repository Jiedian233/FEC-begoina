#include <stdio.h>                                
#include <stdlib.h>                                
#include <string.h>                                
#include <sys/socket.h>                                                   
#include <sys/types.h>
#include <unistd.h> 
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <errno.h>
#include <pthread.h>

void pthread_func(int*);

int main(int argc,char *argv[])                    
{                                                  
    int server_sockfd;
    int client_sockfd;
    int ret;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    socklen_t server_addrlen;
    socklen_t client_addrlen;
    int on = 1;

    char buf[20];

    /*1,create a socket*/
    /*IPPROTO=0,dumserver protocol for TCP*/
    server_sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(-1 == server_sockfd)
    {
        perror("socket"),exit(-1);
    }
    /*1.5 enable address reuse*/
    ret = setsockopt(server_sockfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
    /*2,bind the socket with an ip*/
    memset(&server_addr,0,sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(3490);
    server_addr.sin_addr.s_addr = inet_addr("172.23.7.179");
    server_addrlen = sizeof(struct sockaddr);
    ret = bind(server_sockfd,(const struct sockaddr*)&server_addr,server_addrlen);
    if(-1 == ret)
    {
        perror("bind"),exit(-1);
    }
    /*3,listen the socket*/
    ret = listen(server_sockfd,6);
    if(-1 == ret)
    {
        perror("listen"),exit(-1);
    }
    while(1)
    {
        /*4,accept the data from the client*/
        client_addrlen = sizeof(struct sockaddr);
        client_sockfd = accept(server_sockfd,(struct sockaddr *)&client_addr,&client_addrlen);
        if(client_sockfd > 0)
        {    
            printf("IP is %s\n",inet_ntoa(client_addr.sin_addr));
            printf("Port is %d\n",ntohs(client_addr.sin_port));
            pthread_t pt;
            printf("%d\n",client_sockfd);
            ret = pthread_create(&pt,NULL,(void *)pthread_func,&client_sockfd);
            if(0 != ret)
            {
                printf("ip:%s connect failed",inet_ntoa(client_addr.sin_addr));
            }

           pthread_join(pt,(void*)buf);
        }
    }
    close(server_sockfd);
    return 0;                                      
}                                                  
void pthread_func(int* p_client_sockfd)            
{
    int client_sockfd = *p_client_sockfd;
    printf("%d\n",client_sockfd);
    int ret;
    char data[100];
    char data1[100];
    while(1)
    {
        /*5,read the data from client*/
        ret = recv(client_sockfd,data,100,0);
        if(-1 == ret)
        {
            perror("recv"),exit(-1);
        }
        printf("data from client:%s\n",data);
        /*6,send the data to client*/
        printf("server send data:");
        scanf("%s",data1);
        ret = send(client_sockfd,data1,100,0);
        if(-1 == ret)
        {
            perror("send"),exit(-1);
        }                                                  
    }
   pthread_exit(0);
}