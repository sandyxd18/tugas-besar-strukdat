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
    float status[10];
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

void addAdmin() {
    user = (User  *)malloc(kapasitasUser * sizeof(User));
    if (user == NULL) {
        printf("Gagal mengalokasikan memori.\n");
        exit(1);
    }

    strcpy(user[hitungUser].nama, "admin");
    strcpy(user[hitungUser].no_telp, "123456789");
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

void registerUser() {
    if (hitungUser >= kapasitasUser) {
        upKapasitasUser();
    }

    User new_user;
    printf("Masukkan nama: ");
    scanf("%s", new_user.nama); //ubah dengan fgets

    if (is_user_exists(new_user.nama)) {
        printf("Pengguna dengan nama '%s' sudah terdaftar. Silakan gunakan nama lain.\n", new_user.nama);
        return;
    }

    printf("Masukkan nomor telepon: ");
    scanf("%s", new_user.no_telp);
    printf("Masukkan password: ");
    scanf("%s", new_user.password); //ubah dengan fgets
    strcpy(new_user.role, "pembeli");

    user[hitungUser] = new_user;
    hitungUser++;
    printf("Registrasi berhasil!\n");
}

void loginUser() {
    char nama[100];
    char password[100];

    printf("Masukkan nama: ");
    scanf("%s", nama); //ubah dengan fgets
    printf("Masukkan password: ");
    scanf("%s", password); //ubah dengan fgets

    for (int i = 0; i < hitungUser; i++) {
        if (strcmp(user[i].nama, nama) == 0 && strcmp(user[i].password, password) == 0) {
            printf("Login berhasil! Selamat datang, %s sebagai %s.\n", user[i].nama, user[i].role);
            return;
        }
    }
    printf("Login gagal! Nama atau password salah.\n");
}

void upKapasitasBuku() {
    kapasitasBuku *= 2;
    buku = (Buku *)realloc(buku, kapasitasBuku * sizeof(Buku));
    if (buku == NULL) {
        printf("Gagal mengalokasikan memori untuk buku.\n");
        exit(1);
    }
}

void displayBuku() {
    printf("\nDaftar Buku:\n");
    for (int i = 0; i < hitungBuku; i++) {
        printf("Judul: %s, Tahun Terbit: %d, Kategori: %s, Harga: %.2f, Stok: %d, Status: %s\n", buku[i].judul, buku[i].pengarang, buku[i].tahun_terbit, buku[i].kategori, buku[i].harga, buku[i].stok, buku[i].status);
    }
}

void displayUser() {
    printf("\nDaftar Pengguna Pembeli:\n");
    for (int i = 0; i < hitungUser; i++) {
        if (strcmp(user[i].role, "pembeli") == 0) {
            printf("Nama: %s, No Telepon: %s, Role: %s\n", user[i].nama, user[i].no_telp, user[i].role);
        }
    }
}

void tambahBuku() {
    if (hitungBuku >= kapasitasBuku) {
        upKapasitasBuku();
    }

    Buku new_buku;
    printf("Masukkan judul: ");
    scanf("%s", new_buku.judul); //ubah dengan fgets
    printf("Masukkan pengarang: ");
    scanf("%s", new_buku.pengarang); //ubah dengan fgets
    printf("Masukkan tahun terbit: ");
    scanf("%d", &new_buku.tahun_terbit);
    printf("Masukkan kategori: ");
    scanf("%s", new_buku.kategori); //ubah dengan fgets
    printf("Masukkan harga: ");
    scanf("%f", &new_buku.harga);
    printf("Masukkan stok: ");
    scanf("%d", &new_buku.stok);
    printf("Masukkan status (ready/preorder): ");
    scanf("%s", new_buku.status);

    buku[hitungBuku] = new_buku;
    hitungBuku++;
    printf("Buku berhasil ditambahkan!\n");
}

void deleteBuku() {
    char judul[100];
    printf("Masukkan judul buku yang ingin dihapus: ");
    scanf("%s", judul); //ubah dengan fgets

    for (int i = 0; i < hitungBuku; i++) {
        if (strcmp(buku[i].judul, judul) == 0) {
            for (int j = i; j < hitungBuku - 1; j++) {
                buku[j] = buku[j + 1];
            }
            hitungBuku--;
            printf("Buku %s berhasil dihapus!\n", judul);
            return;
        }
    }
    printf("Buku dengan judul %s tidak ditemukan.\n", judul);
}

void updateStokBuku() {
    char judul[100];
    int jumlah;
    printf("Masukkan judul buku yang ingin diperbarui stoknya: ");
    scanf("%s", judul); //ubah dengan fgets
    printf("Masukkan jumlah yang ingin ditambahkan/kurangi (negatif untuk mengurangi): ");
    scanf("%d", &jumlah);

    for ( int i = 0; i < hitungBuku; i++) {
        if (strcmp(buku[i].judul, judul) == 0) {
            buku[i].stok += jumlah;
            printf("Stok buku %s berhasil diperbarui. Stok sekarang: %d\n", judul, buku[i].stok);
            return;
        }
    }
    printf("Buku dengan judul %s tidak ditemukan.\n", judul);
}

void updateStatusBuku() {
    char judul[100];
    printf("Masukkan judul buku yang ingin diperbarui statusnya: ");
    scanf("%s", judul); //ubah dengan fgets
    char status[10];
    printf("Masukkan status baru (ready/preorder): ");
    scanf("%s", status);

    for (int i = 0; i < hitungBuku; i++) {
        if (strcmp(buku[i].judul, judul) == 0) {
            strcpy(buku[i].status, status);
            printf("Status buku %s berhasil diperbarui menjadi '%s'.\n", judul, status);
            return;
        }
    }
    printf("Buku dengan judul %s tidak ditemukan.\n", judul);
}

void displayQueue() {
    printf("\nAntrian Preorder:\n");
    for (int i = 0; i < hitungAntrian; i++) {
        printf("%d. %s\n", i + 1, antrian[i]);
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

void beliBuku() {
    char judul[100];
    printf("Masukkan judul buku yang ingin dibeli: ");
    scanf("%s", judul); //ubah dengan fgets

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

    addAdmin();

    while (1) {
        printf("\n=== Toko Buku Digital ===\n");
        printf("1. Register\n2. Login\n3. Keluar\nPilih opsi: ");
        scanf("%d", &pilihan);

        if (pilihan == 1) {
            registerUser();
        } else if (pilihan == 2) {
            loginUser();
            if (strcmp(user[hitungUser - 1].role, "admin") == 0) {
                printf("\nMenu Admin:\n");
                printf("1. Lihat Daftar Buku\n2. Tambah Buku\n3. Hapus Buku\n4. Update Stok\n5. Update Status Buku\n6. Lihat Antrian Preorder\n7. Tambah User Pembeli\n8. List User Pembeli\n9. Logout\n");
                printf("Pilih opsi: ");
                scanf("%d", &pilihan);

                if (pilihan == 1) displayBuku();
                else if (pilihan == 2) tambahBuku();
                else if (pilihan == 3) deleteBuku();
                else if (pilihan == 4) updateStokBuku();
                else if (pilihan == 5) updateStatusBuku();
                else if (pilihan == 6) displayQueue();
                else if (pilihan == 7) registerUser();
                else if (pilihan == 8) displayUser();
                else if (pilihan == 9) break;
                else printf("Pilihan tidak valid.\n");
            } else {
                printf("\nDaftar Toko Buku Digital:\n");
                printf("1. Lihat Daftar Buku\n2. Lihat Riwayat Pembelian\n3. Lihat Profil\n4. Logout\n");
                printf("Pilih opsi: ");
                scanf("%d", &pilihan);

                if (pilihan == 1) {
                    displayBuku();
                    int opsi;
                    printf("1. Beli buku\n2. Kembali\n3. ");
                    printf("Pilih opsi: ");
                    scanf("%d", &opsi);
                    if (opsi == 1) beliBuku();
                    else if (opsi == 2) return; //perlu cek lagi
                    else printf("Pilihan tidak valid.\n");
                } else if (pilihan == 2) {
                    displayHistory();
                    int opsi;
                    printf("1. Print History Pembelian\n2. Kembali\n");
                    printf("Pilih opsi: ");
                    scanf("%d", &opsi);
                    if (opsi == 1) simpanHistory();
                    else if (opsi == 2) return; //perlu cek lagi
                    else printf("Pilihan tidak valid.\n");
                } else if (pilihan == 3) {
                    displayProfile();
                } else if (pilihan == 4) {
                    break;
                } else {
                    printf("Pilihan tidak valid.\n");
                }
            }
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