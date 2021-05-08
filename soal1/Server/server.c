#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <limits.h>

#define PORT 8080
int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    char *regsuccess = "Register sukses";
    char *logsuccess = "Login sukses";
    char *logfailed = "Password salah";

    while (1)
    {

        FILE *fp;
        fp = fopen("akun.txt", "a+");
        valread = read(new_socket, buffer, 1024);
        printf("%s\n", buffer);
        if (strcmp(buffer, "1") == 0)
        {
            valread = read(new_socket, buffer, 1024);
            fprintf(fp, "%s\n", buffer);
            send(new_socket, regsuccess, strlen(regsuccess), 0);
        }
        else if (strcmp(buffer, "2") == 0)
        {
            char info[200];
            int check = 0;
            while (fgets(info, 1024, fp) != NULL)
            {
                if ((strstr(info, buffer)) != NULL)
                {
                    printf("Login Berhasil\n");
                    send(new_socket, logsuccess, strlen(logsuccess), 0);
                    break;
                }
            }

            if (check == 0)
            {
                send(new_socket, logfailed, strlen(logfailed), 0);
            }
        }
    }

    return 0;
}
