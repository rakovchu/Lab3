/*
    C ECHO client example using sockets
*/
#include <stdio.h> //printf
#include <string.h>    //strlen
#include <sys/socket.h>    //socket
#include <arpa/inet.h> //inet_addr
#include <unistd.h>

#define BUFFER_SIZE 1024

int main(int argc , char *argv[])
{
    int sock;
    struct sockaddr_in server;
    char filename[128], recv_buf[BUFFER_SIZE];
    int received, total;
    FILE *fout;

    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1) {
        printf("Could not create socket\n");
        return 1;
    }
    printf("Socket created\n");

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(8888);

    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0) {
        printf("connect failed. Error\n");
        return 1;
    }
    printf("Connected\n");

    printf("Enter fielname : ");
    scanf("%s" , filename);

    if (send(sock , filename, strlen(filename), 0) < 0) {
        puts("send failed\n");
        return 1;
    }

    fout = fopen(filename, "wb");
    if (!fout) {
        printf("error opening file %s\n", filename);
        return -1;
    }

    total = 0;
    // Receive file
    while ((received = recv(sock, recv_buf, BUFFER_SIZE, 0)) > 0)
        total += fwrite(recv_buf, 1, received, fout);

    printf("done, %d bytes written\n", total);

    close(sock);
    return 0;
}
