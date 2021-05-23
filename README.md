# Laporan Resmi Soal Shift 3
Kelompok B05
* Amanda Rozi Kurnia 05111940000094
* Melanchthon Bonifacio Butarbutar 05111940000097
* Nadia Tiara Febriana 05111940000217

[Soal Shift 3](https://docs.google.com/document/d/1ud1JyncoSDAo5hA03wPvjn7QBHPUeUG1eBJ8ETtq2dQ/edit)

## Table of Contents
1. [Soal 1](#soal1)
2. [Soal 2](#soal2)
3. [Soal 3](#soal3)

<a name="soal1"></a>
## Soal 1
Pada soal ini, kami diminta untuk membuat program dengan ketentuan sebagai berikut.
1. (a) Melakukan perkalian matrix (4x3 dengan 3x6) dan menampilkan hasilnya
2. (b) Melakukan perhitungan matrix dari matriks hasil 1a dan matriks baru (input user). Setiap sel dari matriks A menjadi angka untuk faktorial, sel dari matriks B menjadi batas faktorialnya, dengan ketentuan sebagai berikut.
```
If a >= b  -> a!/(a-b)!
If b > a -> a!
If 0 -> 0
```
3. (c) Mengecek 5 proses teratas yang memakan resource komputer dengan command ```ps aus| sort -nrk 3,3 | head -5``` menggunakan IPC Pipes
**Note:** semua matriks berasal dari input ke program

### Soal 1a
Melakukan perkalian matriks ukuran 4x3 dengan 3x6, hasilnya adalah matriks 4x6. Matriks A dan matriks B merupakan input dari user.
```C
.....
void *multiplyMatrices(void *arg) {
    int sum = 0;

    // Each thread computes 1/4th of matrix multiplication
    for(int i=0; i<ROW_SIZE; i++) {
        for(int j=0; j<COLUMN_SIZE; j++) {
            for(int k=0; k<MIDDLE_SIZE; k++) {
                sum += first[i][k] * second[k][j];
            }
            value[i][j] = sum;
            sum = 0;
        }
    }
}
.....
```
Membuat 4 thread untuk melakukan perkalian matriks
```C
.....
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
 .....
 ```
Menampilkan hasil perkalian

### Soal 1b
### Soal 1c

<a name="soal2"></a>
# Soal 2
### Soal 2a
### Soal 2b
### Soal 2c

<a name="soal3"></a>
## Soal 3
