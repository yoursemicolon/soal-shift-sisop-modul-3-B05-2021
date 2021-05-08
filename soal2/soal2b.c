#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <pthread.h>
#define ROW_SIZE 4
#define COLUMN_SIZE 6

int temp_mat_a, temp_mat_b;
int input[ROW_SIZE][COLUMN_SIZE];

unsigned long long factorial(int a, int b) {
    if(a == b) return 1;
    else return a*factorial(a-1, b);
}

void *condition(void *arg) {
    if(temp_mat_a == 0 || temp_mat_b == 0) printf("0 ");
    else if(temp_mat_a > temp_mat_b) {
        printf("%llu ", factorial(temp_mat_a, temp_mat_b));
    }
    else {
        printf("%llu ", factorial(temp_mat_a, 0));
    }
}

void inputMatriks() {
    // input matriks 4 x 6
    for(int i=0; i<ROW_SIZE; i++) {
        for(int j=0; j<COLUMN_SIZE; j++) {
            scanf("%d", &input[i][j]);
        }
    }
}

void main() {
    key_t key = 12345;
    int (*value)[6];

    int shmid = shmget(key, sizeof(int[4][6]), IPC_CREAT | 0666);
    value = shmat(shmid, NULL, 0);

    inputMatriks();

    pthread_t tid[ROW_SIZE*COLUMN_SIZE];
    int count_thread = 0;

    printf("\nHasil:\n");
    for(int i=0; i<ROW_SIZE; i++) {
        for(int j=0; j<COLUMN_SIZE; j++) {
            temp_mat_a = value[i][j];
            temp_mat_b = input[i][j];

            pthread_create(&tid[count_thread], NULL, &condition, NULL);

            count_thread++;
            sleep(1);
        }
        printf("\n");
    }

    for(int i=0; i<count_thread; i++) {
        pthread_join(tid[i], NULL);
    }

    shmdt(value);
    shmctl(shmid, IPC_RMID, NULL);
}
