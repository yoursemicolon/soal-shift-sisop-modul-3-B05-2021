
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <pthread.h>
#define ROW_SIZE 4
#define MIDDLE_SIZE 3
#define COLUMN_SIZE 6
#define MAX_THREADS 4

// deklarasi matriks
int *value;
int first[ROW_SIZE][MIDDLE_SIZE], second[MIDDLE_SIZE][COLUMN_SIZE], multiply[ROW_SIZE][COLUMN_SIZE];

void sharedMemo() {
    for(int i=0; i<ROW_SIZE; i++) {
        for(int j=0; j<COLUMN_SIZE; j++) {
            *value = multiply[i][j];
            sleep(1);
            printf("Terkirim %d\n", *value);
        }
    }
}

void prtScreen() {
    printf("\nHasil Perkalian Matriks:\n");
    for(int i=0; i<ROW_SIZE; i++) {
        for(int j=0; j<COLUMN_SIZE; j++) {
            printf("%d\t", multiply[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void *multiplyMatrices(void *arg) {
    int sum = 0;
    // Each thread computes 1/4th of matrix multiplication
    for(int i=0; i<ROW_SIZE; i++) {
        for(int j=0; j<COLUMN_SIZE; j++) {
            for(int k=0; k<MIDDLE_SIZE; k++) {
                sum += first[i][k] * second[k][j];
            }
            multiply[i][j] = sum;
            sum = 0;
        }
    }
}

void inputMatriksB() {

     // input matriks 3 x 6
    for(int i=0; i<MIDDLE_SIZE; i++) {
        for(int j=0; j<COLUMN_SIZE; j++) {
            scanf("%d", &second[i][j]);
        }
    }
}

void inputMatriksA() {

    // input matriks 4 x 3
    for(int i=0; i<ROW_SIZE; i++) {
        for(int j=0; j<MIDDLE_SIZE; j++) {
            scanf("%d", &first[i][j]);
        }
    }
}

int main(void) {

    key_t key = 1234;

    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    value = shmat(shmid, NULL, 0);

    inputMatriksA();
    inputMatriksB();

    // declaring 4 threads for multiplying
    pthread_t tid[MAX_THREADS];

    // creating threads, each evaluating its own part
    for(int i=0; i<MAX_THREADS; i++) {
        pthread_create(&tid[i], NULL, multiplyMatrices, NULL);
    }

    // joining and waiting for all threads to complete
    for(int i=0; i<MAX_THREADS; i++) {
        pthread_join(tid[i], NULL);
    }

    prtScreen();
    sharedMemo();

    shmdt(value);
    shmctl(shmid, IPC_RMID, NULL);
}
