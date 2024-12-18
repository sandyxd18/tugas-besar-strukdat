#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void alokasiMemoriAwal() {
    buku = (Buku *)malloc(kapasitasBuku * sizeof(Buku));
    if (buku == NULL) {
        printf("Gagal mengalokasikan memori untuk buku.\n");
        exit(1);
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

void upKapasitasBuku() {
    kapasitasBuku *= 2;
    buku = (Buku *)realloc(buku, kapasitasBuku * sizeof(Buku));
    if (buku == NULL) {
        printf("Gagal mengalokasikan memori tambahan untuk buku.\n");
        exit(1);
    }
    printf("Kapasitas buku diperbesar menjadi %d.\n", kapasitasBuku);
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
    getchar();
    fgets(new_buku.judul, 100, stdin);
    removeNewline(new_buku.judul);
    
    printf("Masukkan pengarang: ");
    fgets(new_buku.pengarang, 100, stdin);
    removeNewline(new_buku.pengarang);
    
    printf("Masukkan tahun terbit: ");
    scanf("%d", &new_buku.tahun_terbit);
    
    printf("Masukkan kategori: ");
    getchar();
    fgets(new_buku.kategori, 50, stdin);
    removeNewline(new_buku.kategori);
    
    printf("Masukkan harga: ");
    scanf("%f", &new_buku.harga);
    
    printf("Masukkan stok: ");
    scanf("%d", &new_buku.stok);
    
    printf("Masukkan status (ready/preorder): ");
    getchar();
    fgets(new_buku.status, 10, stdin);
    removeNewline(new_buku.status);

    buku[hitungBuku] = new_buku;
    hitungBuku++;
    printf("Buku berhasil ditambahkan!\n");
}

void deleteBuku() {
    char judul[100];
    printf("Masukkan judul buku yang ingin dihapus: ");
    getchar();
    fgets(judul, 100, stdin);
    removeNewline(judul);

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
    getchar();
    fgets(judul, 100, stdin);
    removeNewline(judul);
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
    getchar();
    fgets(judul, 100, stdin);
    removeNewline(judul);
    char status[10];
    printf("Masukkan status baru (ready/preorder): ");
    fgets(status, 10, stdin);
    removeNewline(status);

    for (int i = 0; i < hitungBuku; i++) {
        if (strcmp(buku[i].judul, judul) == 0) {
            strcpy(buku[i].status, status);
            printf("Status buku %s berhasil diperbarui menjadi '%s'.\n", judul, status);
            return;
        }
    }
    printf("Buku dengan judul %s tidak ditemukan.\n", judul);
}

void freeMem() {
    free(user);
    free(buku);
}

int main() {
    int pilihan;

    alokasiMemoriAwal();

    while (1) {
        printf("\nMenu Admin:\n");
        printf("1. Lihat Daftar Buku\n2. Tambah Buku\n3. Hapus Buku\n4. Update Stok\n5. Update Status Buku\n6. Lihat Antrian Preorder\n7. Tambah User Pembeli\n8. List User Pembeli\n9. Logout\n");
        printf("Pilih opsi: ");
        scanf("%d", &pilihan);

        if (pilihan == 1) displayBuku();
        else if (pilihan == 2) tambahBuku();
        else if (pilihan == 3) deleteBuku();
        else if (pilihan == 4) updateStokBuku();
        else if (pilihan == 5) updateStatusBuku();
        // else if (pilihan == 6) displayQueue();
        else if (pilihan == 7) registerUser();
        else if (pilihan == 8) displayUser();
        else if (pilihan == 9) exit(0);
        else printf("Pilihan tidak valid.\n");
    }
    
    freeMem();
    return 0;
}