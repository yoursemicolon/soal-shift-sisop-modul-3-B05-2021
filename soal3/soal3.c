#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
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
char p_dir[] = "/Users/nadiatiara/praktikum_sisop/modul3/soal3";
pthread_t tid[MAX_THREAD];
char keepFile[2000][2000];

typedef struct arg_struct {
    char asal[1000];
    char cwd[1000];
} arg_struct;

int isRegularFile(const char *path) { 
    struct stat path_stat;
    if(stat(path, &path_stat) != 0) 
      return 0; // is not file
    return S_ISREG(path_stat.st_mode);
}

void *pindahFile(void *arg){
  char str[999];
  char buff[999],buff2[999],buff3[999],buff4[999];
  char argument[1000],Dest[1000];
  char cwd[1000],name[1000];
  char Ext[1000],Ext2[1000];
  char dot = '.',dot1 = '.';
  char pertama = buff3[0];
  char *Ext3;
  int hitung = 0;

  
  getcwd(cwd, sizeof(cwd));
  strcpy(argument, (char*) arg);
  int fileType = isRegularFile(argument);

  if(access(argument, F_OK) == -1){ //F_OK flag : Used to check for existence of file.
    return (void *) 0;
  }
  if(!fileType){
    return (void *) 0;
  }

  strcpy(buff4, argument);
  Ext3 = strchr(buff4, dot1);
  strcpy(buff, argument);
  char *tok=strtok(buff, "/");

  while(tok != 0){
    sprintf(name, "%s", tok);
    tok = strtok(0, "/");
  }

  strcpy(buff, argument);strcpy(buff2, name);strcpy(buff3, name);
  char *tok2=strtok(buff2, ".");
  sprintf(Ext2, "%s", tok2);

  while(tok2 != 0){
    hitung++;
    sprintf(Ext, "%s", tok2);
    tok2=strtok(0, ".");
  }
  if(dot == pertama){
    strcpy(Ext, "hidden");
  }
  if(hitung >= 3){
    strcpy(Ext, Ext3+1);
  }
  else if (hitung <= 1){
    strcpy(Ext, "unknown");
  }
  for (int i = 0; i < sizeof(Ext); i++){
    Ext[i] = tolower(Ext[i]);
  }

  strcpy(buff, (char*) arg);
  mkdir(Ext, 0777);
  strcat(cwd, "/");strcat(cwd,Ext);
  strcat(cwd, "/");strcat(cwd, name);
  strcpy(Dest, cwd);
  rename(buff, Dest);
  return (void *) 1;
}

void sortHere(char *asal, int *ab) {
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
  while ((entry = readdir(dirp)) != 0)
  {
    if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
      {
        char namaFile[105];

        strcpy(namaProgramIni, asal);
        strcat(namaProgramIni, "/");
        strcat(namaProgramIni, entry->d_name);

        if(isRegularFile(namaProgramIni)){
          strcpy(keepFile[*ab], namaProgramIni);
          *ab = *ab + 1;
        }
        else{
          sortHere(namaProgramIni, ab);
        }
      }
    }
  closedir(dirp);
}

int main(int argc, char *argv[]) 
{
  if ((chdir(p_dir)) < 0) {
    exit(EXIT_FAILURE);
  }

void *status;
int x;
int index;
char asal[1000];
arg_struct args;
    getcwd(args.cwd, sizeof(args.cwd));
    int check;if(strcmp(argv[1], "-f") == 0) {
  index = 0;
  for(int i=2; i<argc; i++) {
    strcpy(args.asal, argv[i]);
    check = pthread_create(&tid[index], NULL, pindahFile, (void *)&args);

    if(check == 0) 
      printf("File %d : Berhasil Dikategorikan\n", i-1);
    else 
      printf("File %d : Sad, Gagal :(\n", i-1);

      index++;
      sleep(1);
  }

    for(int i=0; i<index; i++) {
      pthread_join(tid[i], NULL);
    }
  }

  else if(strcmp(argv[1], "-d") == 0) {
  if(argc != 3){
  printf("Argument salah\n");
  exit(1);
}

strcpy(asal, argv[2]);
sortHere(asal, &index);
pthread_t t_id[index];

  for(int i = 0; i<index; i++){
    pthread_create(&t_id[i], 0, pindahFile, (void*)keepFile[i]);
  }
  for(int i = 0; i<index; i++){
    pthread_join(t_id[i], &status);
    x = (int) status;
  }
  if(x) {
    printf("Direktori sudah disimpan!\n");
  }
  if(!x) {
    printf("Yah, gagal disimpan:(\n");
  }
}else if(strcmp(argv[1], "*")==0){
  if (argc != 2) {
    printf("Argument salah\n");
    exit(1);
  }

  char cwd [1000];
  getcwd(cwd, sizeof(cwd));
  strcpy(asal, cwd);
  sortHere(asal, &index);

  pthread_t t_id[index];

  for(int i = 0; i<index; i++){
    pthread_create(&t_id[i], 0, pindahFile, (void*)keepFile[i]);
    pthread_join(t_id[i], &status);
  }
exit(0);
}}