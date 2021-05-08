#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8080

int main(int argc, char const *argv[])
{
    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char *hello = "Hello from client";
    char buffer[1024] = {0};
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
    while (1)
    {
        char choice[100];
        char user[100], pass[100];
        printf("Ketik 1 untuk register, Ketik 2 untuk login\n");
        scanf("%s", choice);
        send(sock, choice, strlen(choice), 0);
        printf("Masukkan user dan password anda\n");
        printf("Username :");
        scanf("%s", user);
        //printf("%s\n", user);
        printf("Password :");
        scanf("%s", pass);
        //printf("%s\n", pass);

        strcat(user, ":");
        // printf("%s\n", user);

        strcat(user, pass);
        printf("%s", user);
        send(sock, user, strlen(user), 0);
        valread = read(sock, buffer, 1024);
        printf("%s\n", buffer);
    }
    return 0;
}
