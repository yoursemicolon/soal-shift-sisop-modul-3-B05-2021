#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <syslog.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <dirent.h>
#include <string.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>

#define EOF (-1)

int file__Reg(const char *P){ //cek file atau folder
    struct stat P_stat;
    stat(P, &P_stat);
    return 
        S_ISREG(P_stat.st_mode);
}

pthread_t T[100];
char tempdirektori[1024];

void* pindahtempat(void *arg)
{
    char *P;
    char ekstensi[100];
    char D[100]; //destinasi
    char S[100]; //source
    char potong[100];
    char namafile[100];
    char C[1024]; //cwd
    char *pisah,*pisah2;
    char *A[5];
    char *an[5];
    int x = 0;

    P = (char*) arg;

    strcpy(S, arg);
    strcpy(potong, arg);
    pisah = strtok(P,"."); //split path

    while(pisah != 0){
        A[x] = pisah;x++;
        pisah = strtok(NULL,".");
    }if(x == 1){
        strcpy(ekstensi, "Unknown");
    }else{
        int y;
        for(y = 0; y < strlen(A[x-1]); y++){
            ekstensi[y] = tolower(A[x-1][y]); //nyimpen seluruh ekstensi
        }
    }
    pisah2 = strtok(potong, "/"); //split direktori
    while(pisah2 != 0){
        an[x] = pisah2;x++;
        pisah2 = strtok(0,"/");
    }

    strcpy(namafile, an[x-1]);

    getcwd(C, sizeof(C)); // dpt direktori yang lg dibuka
    strcpy(D, C);
    strcat(D, "/");
    strcat(D, ekstensi);
    __builtin___memset_chk (ekstensi, 0,100, __darwin_obsz0 (ekstensi));
    mkdir(D,0777);
    //memset(ekstensi,0,100);

    FILE *p1;
    FILE *p2;
    // p1 = fopen(S,"r");
    // p2 = fopen(D,"w");
    strcat(D, "/");
    strcat(D, namafile);
    
    p1 = fopen(S,"r");
    p2 = fopen(D,"w");

    int ch;
    if(!p1){
        printf("Unable to open source file to read!!\n");
        fclose(p2);
    }if(!p2){
        printf("Unable to open target file to write\n");
    }while((ch = fgetc(p1)) != -1)
    {
        fputc(ch, p2); //pindah ke file lama
    }
    remove(S); //Hps file lama
    return 0;
}

int main(int argc, char *argv[]){
    int x;
    getcwd(tempdirektori, sizeof(tempdirektori));
    if(strcmp(argv[1], "-f") == 0){
        for( x = (1+1); x < argc; x++){
            if(file__Reg(argv[x])){
                pthread_create(&(T[x-2]), 0, pindahtempat, (void *)argv[x]); //buat thread
                printf("File %d : Berhasil Dikategorikan\n", x-1);
            }else{
                printf("File %d : Sad, Gagal :(\n", x-1);
            }
        }for( x = 0; x < argc - 2; x++){
            pthread_join(T[x], 0);
        }
    }
}
