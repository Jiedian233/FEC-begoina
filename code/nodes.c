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

int main(int argc,char *argv[])                    
{                                                  
    int client_sockfd;
    struct sockaddr_in client_addr;
    socklen_t client_addrlen;
    char data[20];
    char data1[20];
    int ret;

    /*1,create a socket*/
    client_sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(-1 == client_sockfd)
    {
        perror("socket"),exit(-1);
    }
    /*2,connect to the server*/
    memset(&client_addr,0,sizeof(struct sockaddr));
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(3490);
    client_addr.sin_addr.s_addr = inet_addr("172.23.7.179");
    client_addrlen = sizeof(struct sockaddr);
    ret = connect(client_sockfd,(const struct sockaddr *)&client_addr,client_addrlen);
    if(-1 == ret)
    {
        perror("connect"),exit(-1);
    }
    while(1)
    {
        /*3,send the data to server*/
        printf("client data are :");
        scanf("%s",data);
        ret = send(client_sockfd,data,20,0);
        if(-1 == ret)
        {
            perror("write"),exit(-1);
        }
        /*4,receive the data from server*/
        ret = recv(client_sockfd,data1,100,0);
        if(-1 == ret)
        {
            perror("recv"),exit(-1);
        }
        printf("data from server:%s\n",data1);
    }
    close(client_sockfd);
    return 0;                                      
}
