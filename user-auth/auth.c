#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char nama[100];
    char no_telp[20];
    char password[50];
    char role[10];
} User;

User *user = NULL;
int hitungUser = 0;
int kapasitasUser = 2;

void addAdmin() {
    user = (User  *)malloc(kapasitasUser * sizeof(User));
    if (user == NULL) {
        printf("Gagal mengalokasikan memori.\n");
        exit(1);
    }

    strcpy(user[hitungUser].nama, "admin");
    strcpy(user[hitungUser].no_telp, "08123456789");
    strcpy(user[hitungUser].password, "admin123");
    strcpy(user[hitungUser].role, "admin");
    hitungUser++;
}

void upKapasitasUser() {
    kapasitasUser *= 2;
    user = (User  *)realloc(user, kapasitasUser * sizeof(User));
    if (user == NULL) {
        printf("Gagal mengalokasikan memori.\n");
        exit(1);
    }
}

int cekUser(const char *nama) {
    for (int i = 0; i < hitungUser; i++) {
        if (strcmp(user[i].nama, nama) == 0) {
            return 1;
        }
    }
    return 0;
}

void removeNewline(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

void registerUser() {
    if (hitungUser >= kapasitasUser) {
        upKapasitasUser();
    }

    User new_user;
    printf("Masukkan username(nama): ");
    getchar();
    fgets(new_user.nama, 100, stdin);
    removeNewline(new_user.nama);

    if (cekUser(new_user.nama)) {
        printf("Pengguna dengan nama '%s' sudah terdaftar. Silakan gunakan nama lain.\n", new_user.nama);
        return;
    }

    printf("Masukkan nomor telepon: ");
    fgets(new_user.no_telp, 20, stdin);
    removeNewline(new_user.no_telp);
    
    printf("Masukkan password: ");
    fgets(new_user.password, 50, stdin);
    removeNewline(new_user.password);
    
    strcpy(new_user.role, "pembeli");

    user[hitungUser] = new_user;
    hitungUser++;
    printf("Registrasi berhasil!\n");
}

void loginUser() {
    char nama[100];
    char password[50];

    printf("Masukkan username(nama): ");
    getchar();
    fgets(nama, 100, stdin);
    removeNewline(nama);
    printf("Masukkan password: ");
    fgets(password, 50, stdin);
    removeNewline(password);

    for (int i = 0; i < hitungUser; i++) {
        if (strcmp(user[i].nama, nama) == 0 && strcmp(user[i].password, password) == 0) {
            printf("Login berhasil! Selamat datang %s!\n", user[i].nama);
            return;
        }
    }
    printf("Login gagal! Nama atau password salah.\n");
}

void freeMem() {
    free(user);
}

int main() {
    int pilihan;

    addAdmin();

    while (1) {
        printf("\n=== Toko Buku Digital ===\n");
        printf("1. Register\n2. Login\n3. Keluar\nPilih opsi: ");
        scanf("%d", &pilihan);

        if (pilihan == 1) {
            registerUser();
        } else if (pilihan == 2) {
            loginUser();
        } else if (pilihan == 3) {
            printf("Keluar dari program.\n");
            exit(0);
        } else {
            printf("Pilihan tidak valid, silahkan coba lagi.\n");
        }
    }

    freeMem();
    return 0;
}