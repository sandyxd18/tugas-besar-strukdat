#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUKU 100
#define MAX_USER 100

typedef struct {
    char nama[100];
    char no_telp[15];
    char password[50];
    char role[10];
} User;

typedef struct {
    int id;
    char nama_pembeli[100];
    float total;
} Transaksi;

typedef struct {
    char judul[100];
    char pengarang[100];
    int tahun_terbit;
    char kategori[50];
    float harga;
    int stok;
    char status[10];
} Buku;

typedef struct QueueNode {
    char pembeli[50];
    struct QueueNode *next;
} QueueNode;

typedef struct {
    char judul[100];
    QueueNode *depan;
    QueueNode *belakang;
} PreorderQueue;

Buku buku_buku[MAX_BUKU];
int hitungBuku = 0;
User user[MAX_USER];
int hitungUser = 0;
PreorderQueue preorderQueues[MAX_BUKU];
int hitungpreorderQueue = 0;

int cekUsername(const char *username) {
    FILE *file = fopen("users.dat", "rb");
    if (file == NULL) return 0;

    User user;
    while (fread(&user, sizeof(User), 1, file)) {
        if (strcmp(user.nama, username) == 0) {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

// void tambahAdmin(const char *username, const char *password, const char *no_telp) {
//     if (!cekUsername(username)) {
//         FILE *file = fopen("users.dat", "ab");
//         if (file == NULL) {
//             printf("Gagal membuka file!\n");
//             return;
//         }

//         User user;
//         strcpy(user.nama, username);
//         strcpy(user.password, password);
//         strcpy(user.no_telp, no_telp);

//         fwrite(&user, sizeof(User), 1, file);
//         fclose(file);
//         printf("User %s berhasil ditambahkan.\n", username);
//     }
// }

void tambahUser(const char *username, const char *password, const char *role, const char *no_telp) {
    if (cekUsername(username)) {
        printf("Username %s sudah ada. Tidak dapat ditambahkan.\n", username);
        return;
    }

    User user;
    strcpy(user.nama, username);
    strcpy(user.password, password);
    strcpy(user.role, role);
    strcpy(user.no_telp, no_telp);
    
    FILE *file = fopen("users.dat", "ab");
    if (file == NULL) {
        printf("Gagal membuka file!\n");
        return;
    }

    fwrite(&user, sizeof(User), 1, file);
    fclose(file);

    printf("User %s berhasil ditambahkan sebagai %s.\n", username, role);
}

void registerUser() {
    char username[100], password[50], no_telp[15];

    printf("=== Register ===\n");
    printf("Masukkan username: ");
    scanf("%s", username);

    if (cekUsername(username)) {
        printf("Username sudah terdaftar, coba gunakan username lain.\n");
        return;
    }

    printf("Masukkan password: ");
    scanf("%s", password);
    printf("Masukkan nomor telepon: ");
    scanf("%s", no_telp);

    tambahUser(username, password, "pembeli", no_telp);
    printf("Registrasi berhasil!\n");
}

void loginUser() {
    char username[100], password[50];
    User user;

    printf("=== Login ===\n");
    printf("Masukkan username: ");
    scanf("%s", username);
    printf("Masukkan password: ");
    scanf("%s", password);

    FILE *file = fopen("users.dat", "rb");
    if (file == NULL) {
        printf("Belum ada user yang terdaftar. Silahkan registrasi terlebih dahulu!\n");
        return;
    }

    int cari = 0;
    while (fread(&user, sizeof(User), 1, file)) {
        if (strcmp(user.nama, username) == 0 && strcmp(user.password, password) == 0) {
            cari = 1;
            break;
        }
    }
    fclose(file);

    if (!cari) {
        printf("Login gagal! Username atau password salah.\n");
    }
}

void tambahBuku() {
    Buku buku;

    printf("\n=== Tambahkan Buku ===\n");
    printf("Judul buku: ");
    scanf(" %[^\n]s", buku.judul);
    printf("Nama pengarang: ");
    scanf(" %[^\n]s", buku.pengarang);
    printf("Tahun terbit: ");
    scanf("%d", &buku.tahun_terbit);
    printf("Kategori buku: ");
    scanf(" %[^\n]s", buku.kategori);
    printf("Harga: ");
    scanf("%f", &buku.harga);
    printf("Stok awal: ");
    scanf("%d", &buku.stok);

    strcpy(buku.status, "ready");
    buku_buku[hitungBuku++] = buku;

    strcpy(preorderQueues[hitungpreorderQueue].judul, buku.judul);
    preorderQueues[hitungpreorderQueue].depan = preorderQueues[hitungpreorderQueue].belakang = NULL;
    hitungpreorderQueue++;

    printf("Buku berhasil ditambahkan!\n");
}

void updateStok() {
    char judul[100];
    int tambahStok;

    printf("\n=== Tambahkan Stok Buku ===\n");
    printf("Judul buku: ");
    scanf(" %[^\n]s", judul);

    for (int i = 0; i < hitungBuku; i++) {
        if (strcmp(buku_buku[i].judul, judul) == 0) {
            printf("Stok saat ini: %d\n", buku_buku[i].stok);
            printf("Tambahkan stok: ");
            scanf("%d", &tambahStok);
            buku_buku[i].stok += tambahStok;
            printf("Stok berhasil ditambah! Stok baru: %d\n", buku_buku[i].stok);
            return;
        }
    }
    printf("Buku tidak ditemukan.\n");
}

void updateStatus() {
    char judul[100];
    char statusBaru[10];

    printf("\n=== Ubah Status Buku ===\n");
    printf("Judul buku: ");
    scanf(" %[^\n]s", judul);

    for (int i = 0; i < hitungBuku; i++) {
        if (strcmp(buku_buku[i].judul, judul) == 0) {
            printf("Status saat ini: %s\n", buku_buku[i].status);
            printf("Ubah status ke (ready/preorder): ");
            scanf("%s", statusBaru);
            strcpy(buku_buku[i].status, statusBaru);
            printf("Status berhasil diperbarui ke '%s'.\n", statusBaru);
            return;
        }
    }
    printf("Buku tidak ditemukan.\n");
}

void tambahPreorder(char *pembeli, char *judul) {
    for (int i = 0; i < hitungpreorderQueue; i++) {
        if (strcmp(preorderQueues[i].judul, judul) == 0) {
            QueueNode *newNode = (QueueNode *)malloc(sizeof(QueueNode));
            strcpy(newNode->pembeli, pembeli);
            newNode->next = NULL;

            if (preorderQueues[i].belakang == NULL) {
                preorderQueues[i].depan = preorderQueues[i].belakang = newNode;
            } else {
                preorderQueues[i].belakang->next = newNode;
                preorderQueues[i].belakang = newNode;
            }
            printf("Preorder berhasil ditambahkan untuk '%s'.\n", judul);
            return;
        }
    }
    printf("Buku preorder tidak ditemukan.\n");
}

void tampilkanPreorderQueue() {
    printf("\n=== Antrian Preorder ===\n");
    for (int i = 0; i < hitungpreorderQueue; i++) {
        printf("Buku: %s\n", preorderQueues[i].judul);

        QueueNode *current = preorderQueues[i].depan;
        if (current == NULL) {
            printf("  Tidak ada antrian preorder.\n");
        } else {
            printf("  Antrian preorder:\n");
            while (current != NULL) {
                printf("  - %s\n", current->pembeli);
                current = current->next;
            }
        }
    }
}

int main() {
    int pilihan;
    char userSaatini[50];
    char passwordSaatini[50];
    int cekAdmin = 0;

    addUser("admin", "admin123", "admin", "081234567890");

    while (1) {
        printf("\n=== Toko Buku Digital ===\n");
        printf("1. Register\n2. Login\n3. Keluar\nPilih opsi: ");
        scanf("%d", &pilihan);

        if (pilihan == 1){
            registerUser();
        } else if (pilihan == 2) {
            loginUser();
            
            int cari = 0;
            for (int i = 0; i < hitungUser; i++) {
                if (strcmp(user[i].nama, userSaatini) == 0 && strcmp(user[i].password, passwordSaatini) == 0) {
                    cari = 1;
                    cekAdmin = (strcmp(user[i].role, "admin") == 0);
                    break;
                }
            }

            if (!cari) {
                printf("Login gagal! Username atau password salah.\n");
                continue;
            }

            if (cekAdmin) {
                printf("\nLogin sebagai Admin.\n");
                while (1) {
                    printf("\n=== Menu Admin ===\n");
                    printf("1. Tambahkan Buku\n2. Tambahkan Stok Buku\n3. Ubah Status Buku\n4. Lihat Antrian Preorder\n5. Logout\nPilih opsi: ");
                    scanf("%d", &pilihan);

                    if (pilihan == 1) tambahBuku();
                    else if (pilihan == 2) updateStok();
                    else if (pilihan == 3) updateStatus();
                    else if (pilihan == 4) tampilkanPreorderQueue();
                    else if (pilihan == 5) break;
                    else printf("Pilihan tidak valid.\n");
                }
            } else {
                printf("\nLogin sebagai Pembeli.\n");
                while (1) {
                    printf("\n=== Menu Pembeli ===\n");
                    printf("1. Lihat Daftar Buku\n");
                    printf("2. Preorder Buku\n");
                    printf("3. Logout\n");
                    printf("Pilih opsi: ");
                    scanf("%d", &pilihan);

                    if (pilihan == 1) {
                        for (int i = 0; i < hitungBuku; i++) {
                            printf("Judul: %s, Status: %s\n", buku_buku[i].judul, buku_buku[i].status);
                        }
                    } else if (pilihan == 2) {
                        char judul[100];
                        printf("Masukkan judul buku: ");
                        scanf(" %[^\n]s", judul);
                        addPreorder(userSaatini, judul);
                    } else if (pilihan == 3) break;
                    else printf("Pilihan tidak valid.\n");
                }
            }
        } else if (pilihan == 3) {
            printf("Keluar dari program.\n");
            exit(0);
        } else {
            printf("Pilihan tidak valid, silahkan coba lagi.\n");
        }
    }
    return 0;
}