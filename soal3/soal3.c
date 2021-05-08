#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <wait.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include <syslog.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <pthread.h>
#include <limits.h>
#define MAX_THREAD 500

//char dir[] = "/home/yoursemicolon/";
char p_dir[] = "/home/yoursemicolon/soal3/";
pthread_t tid[MAX_THREAD];

typedef struct arg_struct {
    char asal[1000];
    char cwd[1000];
} arg_struct;

int isRegularFile(const char *path) {
    struct stat path_stat;
    if(stat(path, &path_stat) != 0) return 0; // is not file
    return S_ISREG(path_stat.st_mode);
}

void pindahFile(char *argv, char *cwd) {
    char argument[1000];
    strcpy(argument, argv);

    int fileType = isRegularFile(argument);
    char dot = '.';
    char slash = '/';
    char *type = strrchr(argument, dot) + 1;
    char *name = strrchr(argument, slash) + 1;
    char tipeLow[100];

    if(type) {
        if((type[strlen(type)-1] >= 'a' && type[strlen(type)-1] <= 'z') || (type[strlen(type)-1] >= 'A' && type[strlen(type)-1] <= 'Z')) {
            strcpy(tipeLow, type);
            // for(int i=0; tipeLow[i]; i++) {
            //     tipeLow[i] = tolower(tipeLow[i]);
            // }
        }
    } else {
        if(!fileType) {
            printf("Ini adalah folder, salah argumen\n");
            return;
        } else {
            strcpy(tipeLow, "Unknown"); // tanpa ekstensi
        }
    }

    mkdir(tipeLow, 0777); // buat folder ekstensi

    size_t len = 0;
    char a[1000]; // res
    char b[1000]; // des

    strcpy(a, argv);
    strcpy(b, cwd);

    strcat(b, "/");
    strcat(b, tipeLow);
    strcat(b, "/");
    strcat(b, name);

    // printf("a = %s\n", a);
    // printf("b = %s\n", b);

    rename(a, b);
    remove(a);
}

void *pindah(void *arg) {
    arg_struct args = *(arg_struct*)arg;

    pindahFile(args.asal, args.cwd);
    pthread_exit(0);
}

void sortHere(char *asal) {
    arg_struct args;
    int check;

    char namaProgramIni[260];
    strcpy(namaProgramIni, p_dir);
    strcat(namaProgramIni, "soal3.c");

    strcpy(args.cwd, p_dir);
    DIR *dirp;
    struct dirent *entry;
    dirp = opendir(asal);

    int index = 0;
    while((entry = readdir(dirp)) != NULL) {
        if(entry->d_type == DT_REG) {
            char namaFile[105];

            strcat(namaFile, asal);
            strcat(namaFile, entry->d_name);
            // sprintf(namaFile, "%s%s", asal, entry->d_name);
            strcpy(args.asal, namaFile);

            if(strcmp(namaFile, namaProgramIni) != 0) {
                check = pthread_create(&tid[index], NULL, pindah, (void *)&args);

                if(check == 0) {
                    printf("Direktori sukses disimpan!");
                } else {
                    printf("Yah, gagal disimpan :(\n");
                }
                printf("%s\n", namaFile);
                sleep(1);
                index++;
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if ((chdir(p_dir)) < 0) {
        exit(EXIT_FAILURE);
    }

    arg_struct args;
    getcwd(args.cwd, sizeof(args.cwd));
    int check;

    if(strcmp(argv[1], "-f") == 0) {
        int index = 0;

        for(int i=2; i<argc; i++) {
            strcpy(args.asal, argv[i]);
            check = pthread_create(&tid[index], NULL, pindah, (void *)&args);

            if(check == 0) printf("File %d : Berhasil Dikategorikan\n", i-1);
            else printf("File %d : Sad, Gagal :(\n", i-1);

            index++;
            sleep(1);
        }

        for(int i=0; i<index; i++) {
            pthread_join(tid[i], NULL);
        }
    } else if(strcmp(argv[1], "-d") == 0) {
        char asal[1000];
        strcpy(asal, argv[2]);
        sortHere(asal);
    } else if(strcmp(argv[1], "*") == 0) {
        char asal[] = "/home/yoursemicolon/soal3";
        sortHere(asal);
    } else {
        printf("Salah argumen\n");
        return 0;
    }
    return 0;
}
