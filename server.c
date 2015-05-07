/*
    C socket server example
*/

#include<stdio.h>
#include<string.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include <pthread.h>

#define BUFFER_SIZE 1024

void *thread_func(void *arg)
{
    int received, read_size;
    char send_buf[BUFFER_SIZE];
    char filename[128];
    int client_sock = (int)arg;
    FILE *fin;

    printf("Connection accepted\n");

    if ((received = recv(client_sock, filename, 128, 0)) <= 0) {
        printf("Client disconnected\n");
        close(client_sock);
        return;
    }

    filename[received] = '\0';
    printf("filename: %s\n", filename);
    //Send file back to client
    fin = fopen(filename, "rb");
    if (!fin) {
        printf("error: cannot open file %s\n", filename);
        close(client_sock);
        return;
    }
    while ((read_size = fread(send_buf, 1, BUFFER_SIZE, fin)) > 0)
        write(client_sock, send_buf, read_size);
    fclose(fin);

    close(client_sock);
}

int main(int argc , char *argv[])
{
    int sock, client_sock;
    struct sockaddr_in server;
    pthread_t thread;

    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1) {
        printf("Could not create socket\n");
        return 1;
    }
    printf("Socket created\n");

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );

    //Bind
    if (bind(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        //print the error message
        perror("bind failed. Error\n");
        return 1;
    }

    //Listen
    listen(sock, 3);

    //Accept and incoming connection
    puts("Waiting for incoming connection...\n");

    //Receive filename from client
    while (1) {
        //accept connection from an incoming client
        client_sock = accept(sock, 0, 0);
        if (client_sock < 0) {
            printf("accept failed\n");
            return 1;
        }

        pthread_create(&thread, NULL, thread_func, (void*)client_sock);

    }

    close(sock);
    return 0;
}
