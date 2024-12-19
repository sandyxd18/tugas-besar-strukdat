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
// char antrian[MAX_QUEUE][MAX_QUEUE];
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
    int front, rear;
} Queue;

Queue queue;

void initQueue() {
    queue.front = -1;
    queue.rear = -1;
}

int isQueueFull() {
    return (queue.rear + 1) % MAX_QUEUE == queue.front;
}

int isQueueEmpty() {
    return queue.front == -1;
}

void enqueue(User user) {
    if (isQueueFull()) {
        printf("Antrian penuh. Tidak dapat menambahkan %s.\n", user.nama);
        return;
    }
    if (isQueueEmpty()) {
        queue.front = 0;
    }
    queue.rear = (queue.rear + 1) % MAX_QUEUE;
    queue.antrian[queue.rear] = user;
    printf("%s telah ditambahkan ke antrian.\n", user.nama);
}

void dequeue() {
    if (isQueueEmpty()) {
        printf("Antrian kosong. Tidak ada yang dapat dihapus.\n");
        return;
    }
    printf("%s telah dihapus dari antrian.\n", queue.antrian[queue.front].nama);
    if (queue.front == queue.rear) {
        queue.front = -1;
        queue.rear = -1;
    } else queue.front = (queue.front + 1) % MAX_QUEUE;
}

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

void addAdmin() {
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

void upKapasitasBuku() {
    kapasitasBuku *= 2;
    buku = (Buku *)realloc(buku, kapasitasBuku * sizeof(Buku));
    if (buku == NULL) {
        printf("Gagal mengalokasikan memori untuk buku.\n");
        exit(1);
    }
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
    if (hitungUser == 0) {
        printf("Belum ada user yang ditambahkan.\n");
        return;
    }
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

void linearSearch(const char *key, const char *value) {
    int found = 0;

    for (int i = 0; i < hitungBuku; i++) {
        if ((strcmp(key, "judul") == 0 && strcasecmp(buku[i].judul, value) == 0) ||
            (strcmp(key, "pengarang") == 0 && strcasecmp(buku[i].pengarang, value) == 0) ||
            (strcmp(key, "kategori") == 0 && strcasecmp(buku[i].kategori, value) == 0)) {
            printf("\nBuku ditemukan:\n");
            printf("Judul: %s\nPengarang: %s\nTahun Terbit: %d\nKategori: %s\nHarga: %.2f\nStok: %d\nStatus: %s\n",
                   buku[i].judul, buku[i].pengarang, buku[i].tahun_terbit, buku[i].kategori, buku[i].harga, buku[i].stok, buku[i].status);
            found = 1;
        }
    }

    if (!found) {
        printf("\nBuku dengan %s '%s' tidak ditemukan.\n", key, value);
    }
}

void sortTahunTerbit(int descending) {
    for (int i = 1; i < hitungBuku; i++) {
        Buku key = buku[i];
        int j = i - 1;

        while (j >= 0 && ((descending && buku[j].tahun_terbit < key.tahun_terbit) ||
                          (!descending && buku[j].tahun_terbit > key.tahun_terbit))) {
            buku[j + 1] = buku[j];
            j--;
        }
        buku[j + 1] = key;
    }
}

void sortHarga(int ascending) {
    for (int i = 1; i < hitungBuku; i++) {
        Buku key = buku[i];
        int j = i - 1;

        while (j >= 0 && ((ascending && buku[j].harga > key.harga) ||
                          (!ascending && buku[j].harga < key.harga))) {
            buku[j + 1] = buku[j];
            j--;
        }
        buku[j + 1] = key;
    }
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

void displayQueue() {
    if (isQueueEmpty()) {
        printf("Antrian kosong.\n");
        return;
    }
    printf("\nAntrian Preorder:\n");
    int i = queue.front;
    while (1) {
        printf("%s\n", queue.antrian[i].nama);
        if (i == queue.rear) break;
        i = (i + 1) % MAX_QUEUE;
    }
}

// void addToQueue(const char *judul) {
//     if (hitungAntrian >= MAX_QUEUE) {
//         printf("Antrian penuh.\n");
//         return;
//     }
//     strcpy(antrian[hitungAntrian], judul);
//     hitungAntrian++;
//     printf("Buku %s telah ditambahkan ke antrian.\n", judul);
// }

void beliBuku() {
    char judul[100];
    printf("Masukkan judul buku yang ingin dibeli: ");
    getchar();
    fgets(judul, 100, stdin);
    removeNewline(judul);

    for (int i = 0; i < hitungBuku; i++) {
        if (strcmp(buku[i].judul, judul) == 0) {
            if (strcmp(buku[i].status, "preorder") == 0) {
                buku[i].stok = 0;
                if (userSaatIni != NULL) {
                    enqueue(*userSaatIni);
                }
                printf("Buku dalam status pre-order. Buku dengan judul %s berhasil ditambahkan ke list preorder.\n", judul);
            } else if (buku[i].stok > 0) {
                buku[i].stok--;
                int jumlah;
                printf("Masukkan jumlah buku yang ingin dibeli: ");
                scanf("%d", &jumlah);

                float total = buku[i].harga * jumlah;
                printf("Total harga: %.2f\n", total);

                int metode;
                Transaksi transaksi;
                transaksi.id_transaksi = rand() % 10000;
                strcpy(transaksi.judul, judul);
                transaksi.jumlah = jumlah;
                transaksi.total = total;
                
                printf("Pilih metode pembayaran:\n1. Kartu kredit\n2. Dompet digital\n3. Transfer bank\n");
                printf("Pilih metode: ");
                scanf("%d", &metode);
                getchar();

                if (metode == 1) {
                    strcpy(transaksi.metode, "Kartu Kredit");
                    printf("Masukkan nomor kartu kredit: ");
                    fgets(transaksi.pembayaran.kredit.nomor_kartu, 20, stdin);
                    removeNewline(transaksi.pembayaran.kredit.nomor_kartu);

                    printf("Masukkan nama pemilik kartu: ");
                    fgets(transaksi.pembayaran.kredit.nama_pemilik, 100, stdin);
                    removeNewline(transaksi.pembayaran.kredit.nama_pemilik);

                    printf("Masukkan tanggal kadaluarsa (MM/YY): ");
                    fgets(transaksi.pembayaran.kredit.tanggal_kadaluarsa, 10, stdin);
                    removeNewline(transaksi.pembayaran.kredit.tanggal_kadaluarsa);
                } else if (metode == 2) {
                    strcpy(transaksi.metode, "Dompet Digital");
                    printf("Masukkan nama dompet digital: ");
                    fgets(transaksi.pembayaran.dompet_digital.nama_dompet_digital, 50, stdin);
                    removeNewline(transaksi.pembayaran.dompet_digital.nama_dompet_digital);

                    printf("Masukkan nomor akun: ");
                    fgets(transaksi.pembayaran.dompet_digital.nomor_akun, 50, stdin);
                    removeNewline(transaksi.pembayaran.dompet_digital.nomor_akun);
                } else if (metode == 3) {
                    strcpy(transaksi.metode, "Transfer Bank");
                    printf("Masukkan nama bank: ");
                    fgets(transaksi.pembayaran.transfer_bank.nama_bank, 50, stdin);
                    removeNewline(transaksi.pembayaran.transfer_bank.nama_bank);

                    printf("Masukkan nomor rekening: ");
                    fgets(transaksi.pembayaran.transfer_bank.nomor_rekening, 50, stdin);
                    removeNewline(transaksi.pembayaran.transfer_bank.nomor_rekening);

                    printf("Masukkan nama pengirim: ");
                    fgets(transaksi.pembayaran.transfer_bank.nama_pengirim, 100, stdin);
                    removeNewline(transaksi.pembayaran.transfer_bank.nama_pengirim);
                } else {
                    printf("Metode pembayaran tidak tersedia.\n");
                }

                printf("\nPemabayaran berhasil diproses. Detail transaksi:\n");
                printf("ID Transaksi: %d\nJudul Buku: %s\nJumlah: %d\nTotal: %.2f\nMetode: %s\n", transaksi.id_transaksi, transaksi.judul, transaksi.jumlah, transaksi.total, transaksi.metode);
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
    initQueue();
    addAdmin();

    while (1) {
        printf("\n=== Toko Buku Digital ===\n");
        printf("1. Register\n2. Login\n3. Keluar\nPilih opsi: ");
        scanf("%d", &pilihan);

        if (pilihan == 1) {
            registerUser();
        } else if (pilihan == 2) {
            loginUser();
            if (strcmp(user[hitungUser - 1].role, "admin") == 0)
            {
                while (1)
                {
                    printf("\nMenu Admin:\n");
                    printf("1. Lihat Daftar Buku\n2. Tambah Buku\n3. Hapus Buku\n4. Update Stok\n5. Update Status Buku\n6. Lihat Antrian Preorder\n7. Tambah User Pembeli\n8. List User Pembeli\n9. Logout\n");
                    printf("Pilih opsi: ");
                    scanf("%d", &pilihan);

                    if (pilihan == 1)
                        displayBuku();
                    else if (pilihan == 2)
                        tambahBuku();
                    else if (pilihan == 3)
                        deleteBuku();
                    else if (pilihan == 4)
                        updateStokBuku();
                    else if (pilihan == 5)
                        updateStatusBuku();
                    else if (pilihan == 6)
                        displayQueue();
                    else if (pilihan == 7)
                        registerUser();
                    else if (pilihan == 8)
                        displayUser();
                    else if (pilihan == 9)
                    {
                        userSaatIni == NULL;
                        break;
                    }
                    else
                        printf("Pilihan tidak valid.\n");
                }
            }
            else
            {
                while (1)
                {
                    printf("\nDaftar Toko Buku Digital:\n");
                    printf("1. Lihat Daftar Buku\n2. Lihat Riwayat Pembelian\n3. Lihat Profil\n4. Logout\n");
                    printf("Pilih opsi: ");
                    scanf("%d", &pilihan);

                    if (pilihan == 1)
                    {
                        while (1)
                        {
                            displayBuku();
                            int opsi;
                            printf("1. Beli buku\n2. Cari buku\n3. Sortir Buku\n4. Kembali\n");
                            printf("Pilih opsi: ");
                            scanf("%d", &opsi);
                            if (opsi == 1)
                                beliBuku();
                            else if (opsi == 2) {
                                while (1) {
                                    char key[20], value[100];
                                    int choise;
                                    printf("\nCari buku berdasarkan:\n1. Judul\n2. Pengarang\n3. Kategori\n4. Kembali\nPilih opsi: ");
                                    scanf("%d", &choise);
                                    if (choise == 1) strcpy(key, "judul");
                                    else if (choise == 2) strcpy(key, "pengarang");
                                    else if (choise == 3) strcpy(key, "kategori");
                                    else if (choise == 4) break; 
                                    else printf("Pilihan tidak valid.\n");
                                    printf("Masukkan %s yang ingin dicari: ", key);
                                    getchar();
                                    fgets(value, 100, stdin);
                                    removeNewline(value);
                                    linearSearch(key, value);
                                }
                            }     
                            else if (opsi == 3)
                                while (1) {
                                    int choise;
                                    printf("\nPilih kriteria penyortiran:\n1. Tahun terbit terbaru\n2. Tahun terbit terlama\n3. Harga termurah\n4. Harga termahal\n5. Kembali\nPilih opsi: ");
                                    scanf("%d", choise);
                                    if (choise == 1) {
                                        sortTahunTerbit(1);
                                        displayBuku();
                                    } else if (choise == 2) {
                                        sortTahunTerbit(0);
                                        displayBuku();
                                    } else if (choise == 3) {
                                        sortHarga(1);
                                        displayBuku();
                                    } else if (choise == 4) {
                                        sortHarga(0);
                                        displayBuku();
                                    } else if (choise == 5) {
                                        break;
                                    } else printf("Pilihan tidak valid.\n");
                                }
                            else if (opsi == 4)
                                break;
                            else
                                printf("Pilihan tidak valid.\n");
                        }
                    }
                    else if (pilihan == 2)
                    {
                        displayHistory();
                        int opsi;
                        printf("1. Print History Pembelian\n2. Kembali\n");
                        printf("Pilih opsi: ");
                        scanf("%d", &opsi);
                        if (opsi == 1)
                            simpanHistory();
                        else if (opsi == 2)
                            continue;
                        else
                            printf("Pilihan tidak valid.\n");
                    }
                    else if (pilihan == 3)
                    {
                        displayProfile();
                    }
                    else if (pilihan == 4)
                    {
                        userSaatIni == NULL;
                        break;
                    }
                    else
                    {
                        printf("Pilihan tidak valid.\n");
                    }
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