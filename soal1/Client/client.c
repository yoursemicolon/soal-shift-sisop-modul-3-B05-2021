#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <fcntl.h>
#define PORT 8080

char msg[1024];
char receive[1024];
bool isloggedin = false;
int sock;

void regis()
{
    read(sock, receive, 1024);
    printf("%s\n", receive);
    memset(receive, 0, sizeof(receive));
    char uname[100];
    char pass[100];
    scanf("%s %s", uname, pass);
    sprintf(msg, "%s:%s\n", uname, pass);
    send(sock, msg, strlen(msg), 0);
    memset(msg, 0, sizeof(msg));
    read(sock, receive, 1024);
    printf("%s\n", receive);
    memset(receive, 0, sizeof(receive));
}

void login()
{
    read(sock, receive, 1024);
    printf("%s\n", receive);
    memset(receive, 0, sizeof(receive));
    char uname[100];
    char pass[100];
    scanf("%s %s", uname, pass);
    sprintf(msg, "%s:%s\n", uname, pass);
    send(sock, msg, strlen(msg), 0);
    memset(msg, 0, sizeof(msg));
    read(sock, receive, 1024);
    printf("%s\n", receive);
    if (receive[0] == 'L')
    {
        isloggedin = true;
    }
    memset(receive, 0, sizeof(receive));
}

void resetbuffer()
{
    read(sock, receive, 1024);
    printf("%s\n", receive);
    memset(receive, 0, sizeof(receive));
}

void sends(char data[])
{
    send(sock, data, strlen(data), 0);
    memset(msg, 0, sizeof(msg));
}

void add()
{
    char temp[1024];
    for (int i = 0; i < 3; i++)
    {
        resetbuffer();
        scanf("%s", temp);
        temp[strcspn(temp, "\n")] = 0;
        sends(temp);
    }
    FILE *sfd = fopen(temp, "rb");
    char data[1024] = {0};

    while (1)
    {
        memset(data, 0, 1024);
        size_t size = fread(data, sizeof(char), 1024, sfd);
        send(sock, data, 1024, 0);
        break;
    }
    printf("break");
    fclose(sfd);
    resetbuffer();
}

void download()
{
    resetbuffer();
    char temp[1024];
    scanf("%s", temp);
    temp[strcspn(temp, "\n")] = 0;
    sends(temp);
    read(sock, receive, 1024);
    printf("%s\n", receive);
    if (receive[0] == 'F')
    {
        char dir[300] = "/home/kyu/Documents/Praktikum/Modul3/soal1/";
        strcat(dir, temp);
        FILE *file = fopen(dir, "w");
        char buffer[4096] = {0};
        while (1)
        {
            memset(buffer, 0, sizeof(buffer));
            int len = read(sock, buffer, 4096);
            fprintf(file, "%s", buffer);
            break;
        }
        printf("break\n");
        fclose(file);
    }
}

void del()
{
    resetbuffer();
    char temp[1024];
    scanf("%s", temp);
    temp[strcspn(temp, "\n")] = 0;
    sends(temp);
    resetbuffer();
}

void see()
{
    char buff[10000];
    read(sock, buff, 10000);
    printf("%s\n", buff);
    memset(buff, 0, sizeof(buff));
}

void find()
{
    printf("Tulis nama file anda\n");
    char find[200] = {0};
    scanf("%s", find);
    find[strcspn(find, "\n")] = 0;
    sends(find);
    resetbuffer();
}

int main(int argc, char const *argv[])
{
    struct sockaddr_in address;
    struct sockaddr_in serv_addr;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    else
    {
        read(sock, receive, 1024);
        printf("%s\n", receive);
        memset(receive, 0, sizeof(receive));
    }

    while (1)
    {
        pthread_t thread;
        char command[100];
        scanf("%s", command);
        strcpy(msg, command);
        send(sock, msg, strlen(msg), 0);
        memset(msg, 0, sizeof(msg));
        if (strcmp(command, "register") == 0)
        {
            regis();
            continue;
        }
        else if (strcmp(command, "login") == 0)
        {
            login();
        }
        else if (strcmp(command, "add") == 0 && isloggedin)
        {
            add();
        }
        else if (strcmp(command, "download") == 0 && isloggedin)
        {
            download();
        }
        else if (strcmp(command, "delete") == 0 && isloggedin)
        {
            del();
        }
        else if (strcmp(command, "see") == 0 && isloggedin)
        {
            see();
        }
        else if (strcmp(command, "find") == 0 && isloggedin)
        {
            find();
        }
        else
        {
            printf("Command not found\n");
            continue;
        }
    }
    return 0;
}
