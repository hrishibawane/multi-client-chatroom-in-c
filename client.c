#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>

#define MAX_CLIENTS 100
#define BUFFER_SZ 2048
#define NAME_LEN 32

char* ip = "127.0.0.1";
volatile sig_atomic_t flag = 0;
int sockfd = 0;
char name[NAME_LEN];
int port = 8080;
struct sockaddr_in serv_addr;

void str_overwrite_stdout()
{
    printf("\r%s", "> ");
    fflush(stdout);
}

void str_trim_lf(char *arr, int len)
{
    for (int i = 0; i < len; i++)
    {
        if (arr[i] == '\n')
        {
            arr[i] = '\0';
            break;
        }
    }
}

void catch_exit()
{
    flag = 1;
}

void* recv_msg_handler()
{
    char message[BUFFER_SZ] = "";
    while (1)
    {
        int receive = recv(sockfd, message, BUFFER_SZ, 0);
        if (receive > 0)
        {
            printf("%s", message);
            str_overwrite_stdout();
        }
        else if (receive == 0)
        {
            break;
        }
        bzero(message, BUFFER_SZ);
    }
}

void* send_msg_handler()
{
    char buffer[BUFFER_SZ] = "";
    char message[BUFFER_SZ + NAME_LEN] = "";

    while (1) 
    {
        str_overwrite_stdout();
        fgets(buffer, BUFFER_SZ, stdin);
        str_trim_lf(buffer, BUFFER_SZ);

        if (strcmp(buffer, "exit") == 0)
        {
            break;
        }
        else 
        {
            sprintf(message, "%s: %s\n", name, buffer);
            send(sockfd, message, strlen(message), 0);
        }
        bzero(buffer, BUFFER_SZ);
        bzero(message, BUFFER_SZ + NAME_LEN);
    }
    catch_exit();
}

int main(int argc, char** argv)
{
    if(argc == 2)
    {
        port = atoi(argv[1]);
    }

    signal(SIGINT, catch_exit);

    printf("Enter your name: ");
    fgets(name, NAME_LEN, stdin);
    str_trim_lf(name, strlen(name));

    if(strlen(name) > NAME_LEN-1)
    {
        printf("Enter name correctly!\n");
        exit(EXIT_FAILURE);
    }

    // Socket settings
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(ip);
    serv_addr.sin_port = htons(port);

    // Connect to server
    int err = connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    if (err == -1)
    {
        printf("ERROR: connect()");
        exit(EXIT_FAILURE);
    }

    // Send name
    send(sockfd, name, NAME_LEN, 0);

    printf("\n******WELCOME TO THE CHATROOM******\n");

    pthread_t send_msg_thread;
    if (pthread_create(&send_msg_thread, NULL, &send_msg_handler, NULL) != 0)
    {
        printf("ERROR: pthread_create()");
        exit(EXIT_FAILURE);
    }

    pthread_t recv_msg_thread;
    if (pthread_create(&recv_msg_thread, NULL, &recv_msg_handler, NULL) != 0)
    {
        printf("ERROR: pthread_create()");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        if (flag) 
        {
            printf("\nBye\n");
            break;
        }
    }
    close(sockfd);
    return EXIT_SUCCESS;
}