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
Kita diminta untuk membuat sistem database buku dimana :
1. (a) Dapat melakukan login dan register dimana keduanya membutuhkan username dan password, menerima multi connection, dan menyimpan data username dan password disimpan di akun.txt. 
2. (b)
3. (c)
4. (d)
5. (e)
6. (f)
7. (g)
8. (h)


### Soal 1a
Server
```
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
...
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
            bReadCommand();
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
            ...
```
Client
```
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
```
Di sini untuk mencapai ketentuan soal dimana hanya satu client bisa login pada satu waktu maka menggunakan array yang bersifat seperti antrian dari geeks for geeks. Pada awalnya client membaca input dari user berupa pilihan login atau register
```
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
```
Kemudian mengirim input tersebut ke server. Untuk memanage client yang sedang terhubung menggunakan fungsi pembantu ```checkconnect()``` yang akan menggeser array antrian client saat menerima pesan "0"
```
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
```
Kemudian saat user memilih register maka akan masuk ke fungsi ```addUser()``` yang akan menambahkan data user baru ke file ```akun.txt``` dan mengirim pesan registrasi berhasil ke client
```
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
...
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
```
Ketika memilih login maka ketika menerima username dan password dari client server akan mengecek apakah username dan pass yang diterima sesuai dengan file ```akun.txt```
```
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
...
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
 ```
Fungsi ```checkuser()``` akan loop through file ```akun.txt``` untuk mencari line yang sesuai dengan pesan dari client.

<a name="soal2"></a>
## Soal 2
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

### Soal 2a
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
Hasil perkalian:<br>
<img src="https://github.com/yoursemicolon/soal-shift-sisop-modul-3-B05-2021/blob/main/screenshots/soal2a.png"></img>

### Soal 2b
Melakukan perhitungan matriks output soal 1a dengan matriks input baru. Perhitungannya adalah setiap sel yang berasal dari matriks A menjadi angka untuk faktorial, lalu sel dari matriks B menjadi batas maksimal faktorialnya matriks. Melakukan perhitungan:
```C
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
 ```
 Pengecekan kondisi:
 ```C
 void *condition(void *arg) {
    if(temp_mat_a == 0 || temp_mat_b == 0) printf("0 ");
    else if(temp_mat_a >= temp_mat_b) {
        printf("%llu ", factorial(temp_mat_a,temp_mat_a-temp_mat_b));
    }
    else {
        printf("%llu ", factorial(temp_mat_a, 0));
    }
}
```
Perhitungan faktorial:
```C
unsigned long long factorial(int a, int b) {
    if(a == b) return 1;
    else return a*factorial(a-1, b);
}
```
Hasil perhitungan:<br>
<img src="https://github.com/yoursemicolon/soal-shift-sisop-modul-3-B05-2021/blob/main/screenshots/soal2b.png"></img>

### Soal 2c
Mengecek 5 proses teratas yang memakan resource komputer dengan command ```ps aux | sort -nrk 3,3 | head -5``` menggunakan IPC Pipes. Inisialiasi array pipe1 dan pipe2 sebagai file descriptor (key to access to a file). pipe1 untuk menyimpan two ends dari pipe pertama. pipe2 untuk menyimpan two ends dari pipe kedua.
```C
int main() {
    int pipe1[2], pipe2[2];
    pid_t pid;

    if(pipe(pipe1) == -1) {
        fprintf(stderr, "Pipe failed");
        return 1;
    }
    if(pipe(pipe2) == -1) {
        fprintf(stderr, "Pipe failed");
        return 1;
    }
.....
}
```
Kemudian lakukan fork. Parent akan menggunakan pipe untuk output sehingga kita harus mengganti standard output file descriptor (1) ke writing end dari pipe1[1]. Lalu eksekusi ```argv```.
```C
    // input from stdin (already done)
    // output to pipe1
    dup2(pipe1[1], 1);

    // close fds
    close(pipe1[0]);
    close(pipe1[1]);

    char *argv[] = {"ps", "aux", NULL};
    execv("/usr/bin/ps", argv);

    // exec didn't work
    perror("Bad exec ps");
    _exit(1);
 ```
 Child akan menggunakan pipe untuk input sehingga harus mengganti standard input file descriptor (0) ke reading end dari pipe1[0].
```C
else if(pid == 0) { // child process
        // input from pipe1
        dup2(pipe1[0], 0);

        // output to pipe2
        dup2(pipe2[1], 1);

        // close fds
        close(pipe1[0]);
        close(pipe1[1]);
        close(pipe2[0]);
        close(pipe2[1]);

        char *argv[] = {"sort", "-nrk", "3,3", NULL};
        execv("/usr/bin/sort", argv);

        // exec didn't work
        perror("Bad exec sort");
        _exit(1);
    } else { // parent process
        // close unused fds
        close(pipe1[0]);
        close(pipe1[1]);

        // input from pipe2
        dup2(pipe2[0], 0);

        // output to stdout (already done)
        // close fds
        close(pipe2[0]);
        close(pipe2[1]);

        char *argv[] = {"head", "-5", NULL};
        execv("/usr/bin/head", argv);

        // exec didn't work
        perror("Bad exec head");
        _exit(1);
  ```
 
Hasil eksekusi:<br>
<img src="https://github.com/yoursemicolon/soal-shift-sisop-modul-3-B05-2021/blob/main/screenshots/soal2c.png"></img>

## Kendala Pengerjaan
* Mendapatkan segmentation fault pada saat penggunaan shared memory
* Salah rumus pada fungsi untuk perhitungan faktorial

<a name="soal3"></a>
## Soal 3
**Penjelasan Nomor 3**

Seorang mahasiswa bernama Alex sedang mengalami masa gabut. Di saat masa gabutnya, ia memikirkan untuk merapikan sejumlah file yang ada di laptopnya. Karena jumlah filenya terlalu banyak, Alex meminta saran ke Ayub. Ayub menyarankan untuk membuat sebuah program C agar file-file dapat dikategorikan. Program ini akan memindahkan file sesuai ekstensinya ke dalam folder sesuai ekstensinya yang folder hasilnya terdapat di working directory ketika program kategori tersebut dijalankan.

Contoh apabila program dijalankan:
```
# Program soal3 terletak di /home/izone/soal3
$ ./soal3 -f path/to/file1.jpg path/to/file2.c path/to/file3.zip
#Hasilnya adalah sebagai berikut
/home/izone
|-jpg
|--file1.jpg
|-c
|--file2.c
|-zip
|--file3.zip

```
Catatan: 
- Kategori folder tidak dibuat secara manual, harus melalui program C
- Program ini tidak case sensitive. Contoh: JPG dan jpg adalah sama
- Jika ekstensi lebih dari satu (contoh “.tar.gz”) maka akan masuk ke folder dengan titik terdepan (contoh “tar.gz”)
- Dilarang juga menggunakan fork-exec dan system()
- Bagian b dan c berlaku rekursif

Dibawah ini merupakan source code yang digunakan untuk mengecek file dapat diakses atau tidak dan mengecek jenis tipe file atau bukan. Jika argument tidak dapat diakses, atau  argument tidak merupakan argument reguler file, maka jika false akan mengembalikan nilai 0 ke fungsi main.
```C
getcwd(cwd, sizeof(cwd));
strcpy(argument, (char*) arg);
int fileType = isRegularFile(argument);
if(access(argument, F_OK) == -1){ //F_OK flag : Used to check for existence of file.
  return (void *) 0;
}
if(!fileType){
  return (void *) 0;
}
```

Kemudian, dibawah ini digunakan untuk mencari ekstensi jika berhasil mendapat eksistensi akan digunakan sebagai nama folder.

```C
strcpy(buff4, argument);
Ext3 = strchr(buff4, dot1); 
```

```strchr``` digunakan untuk mengembalikan kemudian dipotong nilai dot(.) yang pertama ditemukan.

Delimiter "/" digunakan untuk mendapatkan nama-nama file. Kemudian, untuk melakukannya menggunakan looping dan strtok. Loopingnya digunakan untuk menghilangkan ``/`` dan memotong string.
```C
strcpy(buff, argument);
char *tok=strtok(buff, "/");

while(tok != 0){
  sprintf(name, "%s", tok);
  tok = strtok(0, "/");
}
```

Untuk menghitung kemunculan ```.``` pada dan mencari nama file tanpa adanya ekstensi code dibawah dilakukan. Menggunakan ``strtok`` dan juga looping. ``hitung`` digunakan untuk menghitung ``.`` yang muncul pada string.
```C
strcpy(buff, argument);strcpy(buff2, name);strcpy(buff3, name);
char *tok2=strtok(buff2, ".");
sprintf(Ext2, "%s", tok2);
while(tok2 != 0){
  hitung++;
  sprintf(Ext, "%s", tok2);
  tok2=strtok(0, ".");
}
```

Kemudian, dibawah ini digunakan untuk mengecek file yang nantinya akan dimasukkan ke dalam kategori yang sesuai. Case pertama digunakan untuk mengkategorikan file hidden agar nantinya bisa masuk ke dalam folder ``hidden``. Untuk case kedua, jika ``.`` muncul lebih dari 1  maka akan masuk ke dalam folder titik terdepan. Case ketiga, digunakan jika tidak terdapat ekstensi sehingga akan dibuat folder ```unknown``` untuk file tanpa ekstensi.
```C
if(dot == pertama){
    strcpy(Ext, "hidden");
  }
  if(hitung >= 3){
    strcpy(Ext, Ext3+1);
  }
  else if (hitung <= 1){
    strcpy(Ext, "unknown");
  }
```

Kemudian, dibawah ini terdapat perulangan untuk mengubah ekstensi menjadi lower case. Lalu, akan membuat folder dan akan dilakukan pemindahan file.
```C
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
```

Kemudian, fungsi ``sortHere`` digunakan untuk melakukan pengurutan secara rekursif.

**Source Code**
```C
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
```

**Soal 3a**

Program menerima opsi -f seperti contoh di atas, jadi pengguna bisa menambahkan argumen file yang bisa dikategorikan sebanyak yang diinginkan oleh pengguna. 
Output yang dikeluarkan adalah seperti ini :
```
File 1 : Berhasil Dikategorikan (jika berhasil)
File 2 : Sad, gagal :( (jika gagal)
File 3 : Berhasil Dikategorikan
```
**Opsi -f digunakan untuk mengkategorikan file sesuai argument yang diinginkan**

Jika argument -f  dan argument path yang sesuai, nantinya akan dimasukkan ke dalam thread, kemudian akan dieksekusi pada fungsi ```pindahFile```. Thread akan dijalankan bersamaan sesuai dengan banyaknya thread menggunakan ```pthread_join```.

**Source Code**
```C
char tempdirektori[1024];
    //int x;
    getcwd(tempdirektori, sizeof(tempdirektori));
    if(strcmp(argv[1], "-f") == 0){
        for( x = (1+1); x < argc; x++){
            if(isRegularFile(argv[x])){
                pthread_create(&(tid[x-2]), 0, pindahFile, (void *)argv[x]); //buat thread
                printf("File %d : Berhasil Dikategorikan\n", x-1);
            }else{
                printf("File %d : Sad, Gagal :(\n", x-1);
            }
        }for( x = 0; x < argc - 2; x++){
            pthread_join(tid[x], 0);
        }
```

**Soal 3b**

Program juga dapat menerima opsi -d untuk melakukan pengkategorian pada suatu directory. Namun pada opsi -d ini, user hanya bisa memasukkan input 1 directory saja, tidak seperti file yang bebas menginput file sebanyak mungkin.
Contohnya adalah seperti ini:
```
$ ./soal3 -d /path/to/directory/
```
Perintah di atas akan mengkategorikan file di /path/to/directory, lalu hasilnya akan disimpan di working directory dimana program C tersebut berjalan (hasil kategori filenya bukan di /path/to/directory).
Output yang dikeluarkan adalah seperti ini :
```
Jika berhasil, print “Direktori sukses disimpan!”
Jika gagal, print “Yah, gagal disimpan :(“
```
**Opsi -d digunakan untuk mengkategorikan file pada suatu direktori**

Jika argument -d  dan argument path yang sesuai hanya satu yang harus dilakukan pertama kali yaitu memanggil fungsi ```sortHere```. Kemudian, setiap elemen pada array nantinya akan dibuat thread untuk mengeksekusi fungsi ```pindahFile```.

**Source Code**
```C
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
    thread_join(t_id[i], &status);
    x = (int) status;
  }
  if(x) {
    printf("Direktori sudah disimpan!\n");
  }
  if(!x) {
    printf("Yah, gagal disimpan:(\n");
  }
}
```

**Soal 3c**

Selain menerima opsi-opsi di atas, program ini menerima opsi *, contohnya ada di bawah ini:
```
$ ./soal3 \*
```
Opsi ini akan mengkategorikan seluruh file yang ada di working directory ketika menjalankan program C tersebut.

**Opsi * digunakan untuk mengkategorikan seluruh file yang ada di working directory ketika menjalankan program**

Jika argument * diberikan maka yang pertama dilakukan adalah menyimpan path current working directory dikarenakan akan mengkategorikan file pada direktori yang sedang digunakan. Kemudian akan memanggil ```sortHere``` untuk menyimpan setiap path dalam array lalu akan dibuat thread untuk dieksekusi pada fungsi ```pindahFile``` dan untuk mengkategorikan file thread akan dijalankan secara paralel.

**Source Code**
```C
else if(strcmp(argv[1], "*")==0){
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
```

**Soal 3d**

Semua file harus berada di dalam folder, jika terdapat file yang tidak memiliki ekstensi, file disimpan dalam folder “Unknown”. Jika file hidden, masuk folder “Hidden”. sudah dijelaskan pada ```pindahFile``` terdapat variable ```ext```.

**Soal 3e**

Setiap 1 file yang dikategorikan dioperasikan oleh 1 thread agar bisa berjalan secara paralel sehingga proses kategori bisa berjalan lebih cepat. Semua path akan dijadikan thread dan kemudian dieksekusi secara paralel.

**Output**

- Opsi -f

Berikut adalah gambar jika argument path dimasukkan. Menghasilkan output sebagai berikut.


- Opsi -d

Berikut adalah gambar jika argument path dimasukkan. Menghasilkan output sebagai berikut.



- Opsi *

Berikut adalah gambar jika argument path dimasukkan. Menghasilkan output sebagai berikut.



**Kendala**
- jika penulisan ekstensi salah yang dimana seharusnya akan muncul output ```Sad, gagal``` tetapi yang muncul malah sebaliknya yaitu berhasil.
- 
- 
