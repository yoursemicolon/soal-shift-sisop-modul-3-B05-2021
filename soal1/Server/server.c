#define _GNU_SOURCE
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdbool.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <fcntl.h>
#include <dirent.h>
#define PORT 8080

char msg[1024];
char receive[1024];
char user[30];
char upass[100];
int master_socket, new_socket, client_socket[30] = {0}, max_clients = 30, activity, max_sd, i, valread, sd;
char command[100];
bool isconnected = false;

fd_set readfds;
bool isloggedin = false;

void addUser(char str[])
{
    printf("ADDING USER\n");
    char idpass[100];
    strcpy(idpass, str);
    printf("%s\n", idpass);
    FILE *file = fopen("akun.txt", "a");
    fputs(idpass, file);
    fclose(file);
}

void resetbuffer()
{
    memset(receive, 0, sizeof(receive));
}

void checkconnect()
{ // check Disconnects + Read vals
    resetbuffer();
    int check;
    if ((check = read(sd, receive, 1024)) == 0)
    {
        isconnected = false;
        isloggedin = false;
        close(sd);
        for (i = 0; i < 30; i++)
        {
            client_socket[i] = client_socket[i + 1];
        }
        client_socket[29] = 0;
    }
}
char *getFileName(char file[])
{
    char *ptr;
    char tok = '/';
    ptr = strrchr(file, tok);
    ptr++;
    return ptr;
}
char *getFileExt(char file[])
{
    char *ptr;
    char tok = '.';
    ptr = strrchr(file, tok);
    ptr++;
    return ptr;
}

void checkconnectc()
{ // check Disconnects + Read vals
    resetbuffer();
    int check;
    if ((check = read(sd, command, 100)) == 0)
    {
        isconnected = false;
        isloggedin = false;
        close(sd);
        ;
        for (i = 0; i < 30; i++)
        {
            client_socket[i] = client_socket[i + 1];
        }
        client_socket[29] = 0;
    }
}

bool checkuser(char str[])
{
    printf("checkuser\n");
    char idpass[100];
    strcpy(idpass, str);
    printf("%s\n", idpass);
    char *id;
    char tok[2] = ":";
    char find[100];
    FILE *file = fopen("akun.txt", "r");
    while (fgets(find, 100, file))
    {
        printf("%s%s\n", find, idpass);
        if (strcmp(find, idpass) == 0)
        {
            strcpy(upass, idpass);
            fclose(file);
            id = strtok(idpass, tok);
            strcpy(user, id);
            isloggedin = true;
            return true;
        }
    }
    return false;
}

void sends(char data[])
{
    send(sd, data, strlen(data), 0);
    memset(msg, 0, sizeof(msg));
}

void writefile(char dir[])
{
    FILE *file = fopen(dir, "w");
    char buffer[1024] = {0};
    while (1)
    {
        memset(buffer, 0, sizeof(buffer));
        int len = read(sd, buffer, 1024);
        fprintf(file, "%s", buffer);
        break;
    }
    printf("break\n");
    fclose(file);
}

void addFiles()
{
    char publisher[1024] = {0};
    char tahun[1024] = {0};
    char path[1024] = {0};
    sends("Publisher:\n");
    checkconnect();
    strcpy(publisher, receive);
    sends("Tahun Publish:\n");
    checkconnect();
    strcpy(tahun, receive);
    sends("Filepath:\n");
    checkconnect();
    strcpy(path, receive);
    char *ptr1;
    char slash = '/';
    ptr1 = strrchr(path, slash);
    ptr1++;
    char fname[100];
    strcpy(fname, ptr1);
    char ext[10];
    char *ptr2;
    char dot = '.';
    ptr2 = strrchr(path, dot);
    ptr2++;
    strcpy(ext, ptr2);
    char dir[300] = "/home/kyu/Documents/Praktikum/Modul3/soal1/files/";
    strcat(dir, fname);
    FILE *tsv = fopen("files.tsv", "a");
    char info[5000];
    sprintf(info, "%s\t%s\t%s\t%s\t%s\n", fname, publisher, tahun, ext, path);
    fputs(info, tsv);
    fclose(tsv);
    writefile(dir);
    sends("File berhasil ditambahkan\n");
    FILE *log = fopen("running.log", "a");
    fprintf(log, "Tambah: %s %s", fname, upass);
    fclose(log);
}

void download()
{
    sends("namafile.extension\n");
    checkconnect();
    bool flag;
    char find[200];
    receive[strcspn(receive, "\n")] = 0;
    strcpy(find, receive);
    FILE *file = fopen("files.tsv", "r");
    char lines[1024];
    while (fgets(lines, 1024, file))
    {
        char *token;
        char found[100];
        char *linesz = lines;
        char *rest;
        token = strtok_r(linesz, "\t", &rest);
        strcpy(found, token);
        if (strcmp(find, found) == 0)
        {
            flag = true;
            break;
        }
    }
    if (flag == true)
    {
        sends("File found\n");
        char temp[104] = "/home/kyu/Documents/Praktikum/Modul3/soal1/files/";
        strcat(temp, find);
        FILE *sfd = fopen(temp, "rb");
        char data[4096] = {0};
        while (1)
        {
            memset(data, 0, 4096);
            size_t size = fread(data, sizeof(char), 4096, sfd);
            send(sd, data, strlen(data), 0);
            break;
        }
        printf("break");
        fclose(sfd);
    }
    else
    {
        sends("File not found\n");
    }
}

void deletess()
{
    sends("namafile.extension\n");
    checkconnect();
    bool flag = false;
    char finds[1024] = {0};
    strcpy(finds, receive);
    printf("%s\n", finds);
    FILE *fileR = fopen("files.tsv", "r");
    FILE *fileW = fopen("temp.tsv", "w");
    char data[1024] = {0};
    char publisher[200], tahun[200], filepath[200], ext[20], filename[200];
    while (fgets(data, 1024, fileR) != NULL)
    {
        sscanf(data, "%[^\t]\t%s\t%s\t%s\t%s", filename, publisher, tahun, ext, filepath);
        if (strcmp(filename, finds) != 0)
        {
            fprintf(fileW, "%s", data);
        }
        if (strcmp(filename, finds) == 0)
        {
            flag = true;
        }
        bzero(data, 1024);
    }
    fclose(fileW);
    fclose(fileR);
    if (flag == true)
    {
        remove("files.tsv");
        rename("temp.tsv", "files.tsv");
        FILE *log = fopen("running.log", "a");
        fprintf(log, "Hapus: %s %s", finds, upass);
        fclose(log);
        char oldFile[200] = {0};
        char renamed[200] = {0};
        char temp[104] = "/home/kyu/Documents/Praktikum/Modul3/soal1/files/";
        strcat(oldFile, temp);
        strcat(renamed, temp);
        strcat(oldFile, finds);
        strcat(renamed, "old-");
        strcat(renamed, finds);
        //    printf("d");
        rename(oldFile, renamed);
        sends("Delete sukses\n");
    }
    else
    {
        sends("File tidak ditemukan");
    }
}

void see()
{
    FILE *fileR = fopen("files.tsv", "r");
    char data[1024] = {0};
    char tosend[10000] = {0};
    char publisher[200], tahun[200], filepath[200], ext[20], filename[200];
    while (fgets(data, 1024, fileR) != NULL)
    {
        sscanf(data, "%[^\t]\t%s\t%s\t%s\t%s", filename, publisher, tahun, ext, filepath);
        char tosendbuff[1024];
        sprintf(tosendbuff, "Nama: %s\nPublisher: %s\nTahun Publishing: %s\nExtensi File: %s\nFilepath: %s\n\n", filename, publisher, tahun, ext, filepath);
        strcat(tosend, tosendbuff);
        bzero(data, 1024);
    }
    fclose(fileR);
    sends(tosend);
}

void find()
{
    checkconnect();
    FILE *fileR = fopen("files.tsv", "r");
    char find[200];
    bool flag = false;
    char data[1024] = {0};
    char tosend[10000] = {0};
    char publisher[200], tahun[200], filepath[200], ext[20], filename[200];
    strcpy(find, receive);
    while (fgets(data, 1024, fileR) != NULL)
    {
        sscanf(data, "%[^\t]\t%s\t%s\t%s\t%s", filename, publisher, tahun, ext, filepath);
        char tosendbuff[1024];
        sprintf(tosendbuff, "Nama: %s\nPublisher: %s\nTahun Publishing: %s\nExtensi File: %s\nFilepath: %s\n\n", filename, publisher, tahun, ext, filepath);
        char *beep;
        beep = strstr(filename, find);
        if (beep != NULL)
        {
            flag = true;
            strcat(tosend, tosendbuff);
        }
        bzero(data, 1024);
    }
    fclose(fileR);
    if (!flag)
    {
        sends("Tidak ditemukan");
    }
    else
    {
        sends(tosend);
    }
}

int main(int argc, char const *argv[])
{
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    FILE *dir = fopen("files.tsv", "a");
    fclose("files.tsv");
    DIR *dir = opendir("files");
    if (dir)
    {
        closedir(dir);
    }
    else if (ENOENT == errno)
    {
        mkdir("files", 0777);
    }

    if ((master_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(master_socket, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    while (1)
    {
        //clear the socket set
        FD_ZERO(&readfds);

        //add master socket to set
        FD_SET(master_socket, &readfds);
        max_sd = master_socket;

        //add child sockets to set
        for (i = 0; i < max_clients; i++)
        {
            //socket descriptor
            sd = client_socket[i];

            //if valid socket descriptor then add to read list
            if (sd > 0)
                FD_SET(sd, &readfds);

            //highest file descriptor number, need it for the select function
            if (sd > max_sd)
                max_sd = sd;
        }
        //wait for an activity on one of the sockets , timeout is NULL ,
        //so wait indefinitely
        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

        if ((activity < 0) && (errno != EINTR))
        {
            printf("select error");
        }

        //If something happened on the master socket ,
        //then its an incoming connection
        if (FD_ISSET(master_socket, &readfds))
        {
            if ((new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }
            for (i = 0; i < max_clients; i++)
            {
                //if position is empty
                if (client_socket[i] == 0)
                {
                    client_socket[i] = new_socket;
                    printf("Adding to list of sockets as %d\n", i);

                    break;
                }
            }
        }
        sd = client_socket[0];
        isconnected = true;
        printf("CONNECTED\n");
        strcpy(msg, "Register / Login?\n");
        printf("%s\n", msg);
        send(new_socket, msg, strlen(msg), 0);
        memset(msg, 0, sizeof(msg));
        while (isconnected)
        {
            memset(command, 0, sizeof(command));
            checkconnectc();
            printf("Command: %s\n", command);
            //int a;
            //scanf("%d",&a);
            if (isloggedin == false)
            {
                if (strcmp(command, "register") == 0)
                {
                    printf("register\n");
                    strcpy(msg, "Register\nInput ID dan password dipisah oleh spasi");
                    send(new_socket, msg, strlen(msg), 0);
                    checkconnect();
                    addUser(receive);
                    sends("Registrasi berhasil\n");
                    continue;
                }
                if (strcmp(command, "login") == 0)
                {
                    printf("login\n");
                    sends("Login\nInput ID dan password dipisah oleh spasi");
                    checkconnect();
                    if (checkuser(receive))
                    {
                        sprintf(msg, "Login berhasil.\nSelamat datang %s\n", user);
                        sends(msg);
                    }
                    else
                    {
                        sends("ID atau password salah\n");
                    }
                    memset(receive, 0, sizeof(receive));
                    continue;
                }
            }
            if (isloggedin == true)
            {
                if (strcmp(command, "add") == 0)
                {
                    printf("Add\n");
                    addFiles();
                }
                else if (strcmp(command, "download") == 0)
                {
                    printf("download\n");
                    download();
                }
                else if (strcmp(command, "delete") == 0)
                {
                    printf("delete\n");
                    deletess();
                }
                else if (strcmp(command, "see") == 0)
                {
                    printf("see\n");
                    see();
                }
                else if (strcmp(command, "find") == 0)
                {
                    find();
                }
            }
        }
    }
    return 0;
}
