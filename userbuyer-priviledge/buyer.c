#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_QUEUE 100
#define MAX_HISTORY 100

typedef struct {
    char judul[100];
    char pengarang[100];
    int tahun_terbit;
    char kategori[50];
    float harga;
    int stok;
    char status[10];
} Buku;

Buku *buku = NULL;
int hitungBuku = 0;
int kapasitasBuku = 2;

typedef struct {
    char nama[100];
    char no_telp[20];
    char password[50];
    char role[10];
} User;

User *user = NULL;
int hitungUser = 0;
int kapasitasUser = 2;
int hitungAntrian = 0;
char antrian[MAX_QUEUE][MAX_QUEUE];
User *userSaatIni = NULL;
int hitungHistory = 0;

typedef union {
    struct {
        char nomor_kartu[20];
        char nama_pemilik[100];
        char tanggal_kadaluarsa[10];
    } kredit;
    struct {
        char nama_dompet_digital[50];
        char nomor_akun[50];
    } dompet_digital;
    struct {
        char nama_bank[50];
        char nomor_rekening[50];
        char nama_pengirim[100];
    } transfer_bank;
} Pembayaran;

typedef struct {
    int id_transaksi;
    char judul[100];
    char metode[20];
    int jumlah;
    float total;
    Pembayaran pembayaran;
} Transaksi;

Transaksi history[MAX_HISTORY];

typedef struct {
    User antrian[MAX_QUEUE];
    int front;
    int rear;
} Queue;

void alokasiMemoriAwal() {
    buku = (Buku *)malloc(kapasitasBuku * sizeof(Buku));
    if (buku == NULL) {
        printf("Gagal mengalokasikan memori untuk buku.\n");
        exit(1);
    }
    user = (User  *)malloc(kapasitasUser  * sizeof(User));
    if (user == NULL) {
        printf("Gagal mengalokasikan memori untuk user.\n");
        exit(1);
    }
}

void removeNewline(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

void addToQueue(const char *judul) {
    if (hitungAntrian >= MAX_QUEUE) {
        printf("Antrian penuh.\n");
        return;
    }
    strcpy(antrian[hitungAntrian], judul);
    hitungAntrian++;
    printf("Buku %s telah ditambahkan ke antrian.\n", judul);
}

void displayBuku() {
    if (hitungBuku == 0) {
        printf("Belum ada buku yang ditambahkan.\n");
        return;
    }

    printf("\nDaftar Buku:\n");
    for (int i = 0; i < hitungBuku; i++) {
        printf("Judul: %s, Tahun Terbit: %d, Kategori: %s, Harga: %.2f, Stok: %d, Status: %s\n", buku[i].judul, buku[i].tahun_terbit, buku[i].kategori, buku[i].harga, buku[i].stok, buku[i].status);
    }
}

void beliBuku() {
    char judul[100];
    printf("Masukkan judul buku yang ingin dibeli: ");
    getchar();
    fgets(judul, 100, stdin);
    removeNewline(judul);

    for (int i = 0; i < hitungBuku; i++) {
        if (strcmp(buku[i].judul, judul) == 0) {
            if (strcmp(buku[i].status, "preorder") == 0) {
                addToQueue(judul);
                printf("Buku dalam status pre-order. Buku dengan judul %s berhasil ditambahkan ke list preorder.\n", judul);
            } else if (buku[i].stok > 0) {
                buku[i].stok--;
                printf("Pembelian buku %s berhasil!\n", judul);
            } else {
                printf("Stok buku %s habis.\n", judul);
            }
            return;
        }
    }
    printf("Buku dengan judul %s tidak ditemukan.\n", judul);
}

void simpanHistory() {
    FILE *file = fopen("history.txt", "w");
    if (file == NULL) {
        printf("Gagal membuka file untuk menyimpan riwayat pembelian.\n");
        return;
    }

    for (int i = 0; i < hitungHistory; i++) {
        fprintf(file, "===================\nID: %d\nJudul: %s\nJumlah: %d\nMetode Pembayaran: %s\nTotal: %.2f\n===================\n", i+1, history[i].judul, history[i].jumlah, history[i].metode, history[i].total);
    }
    fclose(file);
    printf("Riwayat pembelian berhasil disimpan ke 'history.txt'.\n");
}

void displayHistory() {
    printf("\nRiwayat Pembelian:\n");
    for (int i = 0; i < hitungHistory; i++) {
        printf("===================\nID: %d\nJudul: %s\nJumlah: %d\nMetode Pembayaran: %s\nTotal: %.2f\n===================\n", i+1, history[i].judul, history[i].jumlah, history[i].metode, history[i].total);
    }
}

void displayProfile() {
    printf("Profile Pengguna:\n");
    printf("Nama: %s\n", userSaatIni->nama);
    printf("No Telepon: %s\n", userSaatIni->no_telp);
}

void updateTelepon() {
    char new_telepon[20];
    printf("Masukkan nomor telepon baru: ");
    scanf("%s", new_telepon);
    strcpy(userSaatIni->no_telp, new_telepon);
    printf("Nomor telepon berhasil diperbarui.\n");
}

void freeMem() {
    free(user);
    free(buku);
}

int main() {
    int pilihan;

    alokasiMemoriAwal();

    while (1) {
        printf("\nDaftar Toko Buku Digital:\n");
        printf("1. Lihat Daftar Buku\n2. Lihat Riwayat Pembelian\n3. Lihat Profil\n4. Logout\n");
        printf("Pilih opsi: ");
        scanf("%d", &pilihan);

        if (pilihan == 1) {
            while (1) {
                displayBuku();
                int opsi;
                printf("1. Beli buku\n2. Kembali\n");
                printf("Pilih opsi: ");
                scanf("%d", &opsi);
                if (opsi == 1) beliBuku();
                else if (opsi == 2) break;
                else printf("Pilihan tidak valid.\n");
            } 
        } else if (pilihan == 2) {
            displayHistory();
            int opsi;
            printf("1. Print History Pembelian\n2. Kembali\n");
            printf("Pilih opsi: ");
            scanf("%d", &opsi);
            if (opsi == 1) simpanHistory();
            else if (opsi == 2) continue;
            else printf("Pilihan tidak valid.\n");
        } else if (pilihan == 3) {
            displayProfile();
        } else if (pilihan == 4) {
            break;
        } else {
            printf("Pilihan tidak valid.\n");
        }
    }
    freeMem();
    return 0;
}