#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>
#include <unistd.h>

#define TABLE_SIZE 1000
#define FILE_NAME "data.txt"

struct Node {
    int id;
    char nama[25];
    int usia;
    char gender;
    float tinggi, berat;
    struct Node* next;
};

// modulus untuk menghasilkan key berdasarkan ID
int key(int id){
    return id % TABLE_SIZE;
}

// validasi input usia
int checkID(int id){
    if(id < 1000 || id > 9999){
        return 0;
    }
    return 1;
}

// validasi input nama
int checkNama(char nama[]){
    int length = strlen(nama);
    if(length < 3 || length > 25){
        return 0;
    }
    for(int i = 0; i < length; i++){
        if (!(isalpha(nama[i])) && (nama[i] != '\n') && (nama[i] != ' ')) {
            return 0;
        }
    }
    return 1;
}

// validasi input gender
int checkGender(char gender){
    if(gender != 'M' && gender != 'F'){
        return 0;
    }
    return 1;
}

// validasi input usia
int checkUsia(int usia){
    if(usia < 1 || usia > 5){
        return 0;
    }
    return 1;
}

// validasi input tinggi badan
int checkTinggi(float tinggi){
    if(tinggi < 50 || tinggi > 99){
        return 0;
    }
    return 1;
}

int checkBerat(float berat){
    if(berat < 5 || berat > 25){
        return 0;
    }
    return 1;
}

// menambahkan data ke tabel hash
void insert(struct Node* hashTable[], int id, char nama[], int usia, char gender, float tinggi, float berat){
    int i = key(id);

    // membuat node baru
    struct Node* newNode = (struct Node*) malloc(sizeof(struct Node));
    newNode->id = id;
    strcpy(newNode->nama, nama);
    newNode->usia = usia;
    newNode->gender = gender;
    newNode->tinggi = tinggi;
    newNode->berat = berat;
    newNode->next = NULL;

    // memeriksa apakah posisi kosong
    if (hashTable[i] == NULL){
        hashTable[i] = newNode; // node ditambah pada posisi kosong
    } else{
        // menambahkan node baru ke akhir jika terjadi collision
        struct Node* current = hashTable[i];
        while (current->next != NULL) { // mencari posisi kosong pada chain
            current = current->next;
        }
        current->next = newNode; // menambah node pada akhir chain
    }
}

// mencari data berdasarkan ID balita
struct Node* searchID(struct Node* hashTable[], int id){
    int i = key(id);

    // mencari node dengan ID yang cocok
    struct Node* current = hashTable[i];
    while(current != NULL){
        if(current->id == id){
            return current;
        }
        current = current->next;
    }

    // node tidak ditemukan
    return NULL;
}

// menampilkan seluruh data
void viewData(struct Node* hashTable[]){
    printf("\n\t\t\t-------------------------------------------------------------------------------------\n");
    printf("\t\t\t| ID Balita |        Nama Balita       | Usia | Gender | Tinggi Badan | Berat Badan |\n");
    printf("\t\t\t-------------------------------------------------------------------------------------\n");

    // melakukan iterasi di seluruh tabel hash
    for(int i = 0; i < TABLE_SIZE; i++){
        struct Node* current = hashTable[i];
        while(current != NULL){
            printf("\t\t\t| %-9d | %-24s |  %d   |   %c    |   %.2f cm   |  %.2f kg   |\n", current->id, current->nama, current->usia, current->gender, current->tinggi, current->berat);
            current = current->next;
        }
    }
    printf("\t\t\t-------------------------------------------------------------------------------------\n");
}

// mengupdate data berdasarkan ID balita
void update(struct Node* hashTable[], int id){
    struct Node* current = searchID(hashTable, id);
    if(current != NULL){
        printf("\033[0;32m");
        printf("\n\t\t\t\tData ditemukan!\n");
        printf("\033[0m");
        printf("\n\t\t\t\tID\t\t: %d\n", current->id);
        printf("\t\t\t\tNama\t\t: %s\n", current->nama);
        printf("\t\t\t\tUsia\t\t: %d tahun\n", current->usia);
        printf("\t\t\t\tGender\t\t: %c\n", current->gender);
        printf("\t\t\t\tTinggi Badan\t: %.2f cm\n", current->tinggi);
        printf("\t\t\t\tBerat Badan\t: %.2f kg\n", current->berat);

        printf("\n\t\t\t\tMasukkan data yang ingin diupdate:\n");
        int usia;
        char gender;
        float tinggi, berat;

        do{
            printf("\n\t\t\t\tUsia (1-5 tahun)\t: ");
            scanf("%d", &usia);
        }while(!checkUsia(usia));
        current->usia = usia;
        do{
            printf("\t\t\t\tGender (M/F)\t\t: ");
            scanf(" %c", &gender);
        }while(!checkGender(gender));
        current->gender = gender;
        do{
            printf("\t\t\t\tTinggi Badan (50-99 cm)\t: ");
            scanf("%f", &tinggi);
        }while(!checkTinggi(tinggi));
        current->tinggi = tinggi;
        do{
            printf("\t\t\t\tBerat Badan (5-25 kg)\t: ");
            scanf("%f", &berat);
        }while(!checkBerat(berat));
        current->berat = berat;

        printf("\033[0;32m");
        printf("\n\t\t\t\tData berhasil diupdate!\n");
        printf("\033[0m");
    } else {
        printf("\033[0;31m");
        printf("\n\t\t\t\tData tidak ditemukan.\n");
        printf("\033[0m");
    }
}

// menghapus data berdasarkan ID balita
void delete(struct Node* hashTable[], int id){
    int i = key(id);

    // mencari node dengan ID yang cocok
    struct Node* current = hashTable[i];
    struct Node* previous = NULL;
    while(current != NULL){
        if(current->id == id){
            if(previous == NULL){
                // jika node pertama pada chain
                hashTable[i] = current->next;
            } else {
                // jika node berada di tengah atau akhir chain
                previous->next = current->next;
            }
            free(current);
            printf("\033[0;32m");
            printf("\n\t\t\t\tData berhasil dihapus!\n\n");
            printf("\033[0m");
            return;
        }
        previous = current;
        current = current->next;
    }

    printf("\033[0;31m");
    printf("\n\t\t\t\tData tidak ditemukan.\n\n");
    printf("\033[0m");
}

// menghitung hasil rata-rata data
void meanData(struct Node* hashTable[]){
    int j = 0;
    float totalBerat = 0, totalTinggi = 0;

    for (int i = 0; i < TABLE_SIZE; i++){
        struct Node* current = hashTable[i];
        while(current != NULL){
            totalBerat += current->berat;
            totalTinggi += current->tinggi;
            j++;
            current = current->next;
        }
    }

    if(j > 0){
        float meanBerat, meanTinggi;
        meanBerat = totalBerat / j;
        meanTinggi = totalTinggi / j;

        printf("\n\t\t\t\tRata-rata Tinggi Badan\t: %.2f cm\n", meanTinggi);
        printf("\t\t\t\tRata-rata Berat Badan\t: %.2f kg\n\n", meanBerat);
        printf("\t\t\t\tDari %d data yang tersedia.", j);
    } else{
        printf("\033[0;31m");
        printf("\n\t\t\t\tTidak ada data yang tersedia.\n");
        printf("\033[0m");
    }
}

//header untuk tiap menu
void header(){
	system("cls");
    printf("\t\t\t===========================================================================");
	printf("\n\t\t\t=x=x=x=x=x=x                                                   x=x=x=x=x=x=");
    printf("\n\t\t\t=x=x=x=x=x=x                ");
    printf("\033[0;31m");
    printf("<3");
    printf("\033[0;34m");
    printf("   Data Balita   ");
    printf("\033[0;31m");
    printf("<3");
    printf("\033[0m");
    printf("              x=x=x=x=x=x=");
    printf("\n\t\t\t=x=x=x=x=x=x                                                   x=x=x=x=x=x=");
    printf("\n\t\t\t===========================================================================");
}

void loading() {
    printf("\n\n\t\t\t\t\t\tLoading... ");
    for (int i = 0; i <= 4; i++) {
        printf("%d%% ", (i * 100) / 4);
        fflush(stdout);
        sleep(1); // Sleep for 1 second
        printf("\b\b\b\b");
    }
    printf("\n\n\t\t\t\t\tLoading complete!\n");
}

//display welcome message
void welcomeMessage(){
    printf("\n\n\n\n\n");
    printf("\033[0;35m");
    printf("\n\t\t\t        =x=x=x=x=x=x=x=x=x=x=x=x=x=x=x=x=x=x=x=x=x=x=");
    printf("\n\t\t\t        =                                           =");
    printf("\n\t\t\t        =                 ");
    printf("\033[0;33m");
    printf("WELCOME TO");
    printf("\033[0;35m");
    printf("                =");
    printf("\n\t\t\t        =                  ");
    printf("\033[1;33m");
    printf("POSYANDU");
    printf("\033[0;35m");
    printf("                 =");
    printf("\n\t\t\t        =                                           =");
    printf("\n\t\t\t        =x=x=x=x=x=x=x=x=x=x=x=x=x=x=x=x=x=x=x=x=x=x=");
    printf("\033[0m");
    loading();
}

void login(){
	//default login
	system("cls");
    header();
	const char defaultUsername[] = "admin";
	const char defaultPassword[] = "admin";
	char username[20];
    char password[20];
    int tryL = 0;
    do{
        printf("\n\n\n\t\t\t\tUsername: ");
        scanf(" %[^\n]", username);
        printf("\n\t\t\t\tPassword: ");
        scanf(" %[^\n]", password);
		if((!strcmp(username, defaultUsername)) && (!strcmp(password, defaultPassword))){
            return;
        }
        else{
            printf("\033[0;31m");
            printf("\t\t\t\tMaaf, informasi yang Anda masukkan salah");
            printf("\033[0m");
            tryL++;
        }
    }
    while(tryL<=3);
    if(tryL>3){	//jika percobaan gagal lebih dari 3 kali
        system("cls");
        header();
        printf("\033[0;31m");
        printf("\n\t\t\t\tLogin Gagal");
        printf("\n\t\t\t\tMaaf, Anda belum terdaftar.");
        printf("\033[0m");
        getchar();
        exit(1);
    }
}



int main(){
    struct Node* hashTable[TABLE_SIZE];
    struct Node hash[1000];

    // Inisialisasi tabel hash dengan NULL
    for(int i = 0; i < TABLE_SIZE; i++){
        hashTable[i] = NULL;
    }

    FILE* fp = fopen(FILE_NAME, "r");

    if (fp == NULL) {
        printf("\n\t\t\t\tData tidak tersedia\n");
        exit(1);
    }

    int j = 0;
    while(!feof(fp)){
        fscanf(fp, "%d\t%s\t%d\t%c\t%f\t%f\n", &hash[j].id, hash[j].nama, &hash[j].usia, &hash[j].gender, &hash[j].tinggi, &hash[j].berat);
        int i = key(hash[j].id);

        // membuat node baru
        struct Node* newNode = (struct Node*) malloc(sizeof(struct Node));
        newNode->id = hash[j].id;
        strcpy(newNode->nama, hash[j].nama);
        newNode->usia = hash[j].usia;
        newNode->gender = hash[j].gender;
        newNode->tinggi = hash[j].tinggi;
        newNode->berat = hash[j].berat;
        newNode->next = NULL;

        // memeriksa apakah posisi kosong
        if (hashTable[i] == NULL){
            hashTable[i] = newNode; // node ditambah pada posisi kosong
        } else{
            // menambahkan node baru ke akhir jika terjadi collision
            struct Node* current = hashTable[i];
            while (current->next != NULL) { // mencari posisi kosong pada chain
                current = current->next;
            }
            current->next = newNode; // menambah node pada akhir chain
        }
        j++;
	}

    fclose(fp);

    welcomeMessage();
    login();

    int choice;
    do{
        system("cls");
        header();
        printf("\n\t\t\t\t1. Insert\n");
        printf("\t\t\t\t2. Update\n");
        printf("\t\t\t\t3. Search\n");
        printf("\t\t\t\t4. Display\n");
        printf("\t\t\t\t5. Delete\n");
        printf("\t\t\t\t6. Rata-Rata\n");
        printf("\t\t\t\t7. Exit\n");
        printf("\n\t\t\t\tEnter your choice: ");
        scanf("%d", &choice);

        switch(choice){
            case 1:{
                system("cls");
                header();
                int id;
                char nama[50];
                int usia;
                char gender;
                float tinggi, berat;
                printf("\n\t\t\t\tMasukkan data balita:\n");
                do{
                    printf("\n\t\t\t\tID (4-digit)\t\t: ");
                    scanf("%d", &id);
                }while(!checkID(id));
                do{
                    printf("\t\t\t\tNama (3-25 karakter)\t: ");
                    scanf(" %[^\n]", nama);
                }while(!checkNama(nama));
                do{
                    printf("\t\t\t\tUsia (1-5 tahun)\t: ");
                    scanf("%d", &usia);
                }while(!checkUsia(usia));
                do{
                    printf("\t\t\t\tGender (M/F)\t\t: ");
                    scanf(" %c", &gender);
                }while(!checkGender(gender));
                do{
                    printf("\t\t\t\tTinggi Badan (50-99 cm)\t: ");
                    scanf("%f", &tinggi);
                }while(!checkTinggi(tinggi));
                do{
                    printf("\t\t\t\tBerat Badan (5-25 kg)\t: ");
                    scanf("%f", &berat);
                }while(!checkBerat(berat));

                insert(hashTable, id, nama, usia, gender, tinggi, berat);
                printf("\033[0;32m");
                printf("\n\t\t\t\tData berhasil ditambahkan!\n\n");
                printf("\033[0m");
                printf("\n\n\n\t\t\t\t Tekan tombol enter untuk lanjut.....");
                getch();
                break;
            }

            case 2:{
                system("cls");
                header();
                int id;
                printf("\n\t\t\t\tMasukkan ID untuk diupdate: ");
                scanf("%d", &id);
                update(hashTable, id);
                printf("\n\n\n\t\t\t\t Tekan tombol enter untuk lanjut.....");
                getch();
                break;
            }

            case 3:{
                system("cls");
                header();
                int id;
                printf("\n\t\t\t\tMasukkan ID untuk dicari: ");
                scanf("%d", &id);

                struct Node* result = searchID(hashTable, id);
                if(result != NULL){
                    printf("\033[0;32m");
                    printf("\n\t\t\t\tData ditemukan!\n");
                    printf("\033[0m");
                    printf("\n\t\t\t\tID\t\t: %d\n", result->id);
                    printf("\t\t\t\tNama\t\t: %s\n", result->nama);
                    printf("\t\t\t\tUsia\t\t: %d tahun\n", result->usia);
                    printf("\t\t\t\tGender\t\t: %c\n", result->gender);
                    printf("\t\t\t\tTinggi Badan\t: %.2f cm\n", result->tinggi);
                    printf("\t\t\t\tBerat Badan\t: %.2f kg\n\n", result->berat);
                } else{
                    printf("\033[0;31m");
                    printf("\n\t\t\t\tData tidak ditemukan.\n");
                    printf("\033[0m");
                }
                printf("\n\n\n\t\t\t\t Tekan tombol enter untuk lanjut.....");
                getch();
                break;
            }

            case 4:{
                system("cls");
                header();
                printf("\n\t\t\tData Balita:\n");
                viewData(hashTable);
                printf("\n");
                printf("\n\n\n\t\t\t\t Tekan tombol enter untuk lanjut.....");
                getch();
                break;
            }

            case 5:{
                system("cls");
                header();
                int id;
                printf("\n\t\t\t\tMasukkan ID untuk dihapus: ");
                scanf("%d", &id);
                delete(hashTable, id);
                printf("\n\n\n\t\t\t\t Tekan tombol enter untuk lanjut.....");
                getch();
                break;
            }

            case 6:{
                system("cls");
                header();
                printf("\n\t\t\t\tHasil Rata-Rata Data:\n");
                meanData(hashTable);
                printf("\n");
                printf("\n\n\n\t\t\t\t Tekan tombol enter untuk lanjut.....");
                getch();
                break;
            }

            case 7:{
                system("cls");
                header();
                printf("\n\n\n\t\t\t\tThank you!\n\n\n");
                break;
            }

            default:{
                printf("\n\t\t\t\tInvalid choice!\n\n");
                break;
            }
        }

    }while(choice != 7);

    return 0;
}

