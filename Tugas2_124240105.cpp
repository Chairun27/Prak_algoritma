#include <iostream>
#include <cstdio>
#include <cstring>
#include <string> 
using namespace std; 

struct Pesanan {
    char nama[50]; 
    char jenis_roti[50]; 
    char total_harga[50];
}; 

struct Queue {
    Pesanan data; 
    Queue* next; 
}; 

Queue *depan = NULL, *belakang = NULL; 

struct Stack {
    Pesanan data; 
    Stack* next; 
}; 

Stack *awal = NULL, *top = NULL; 

void buatQueue() {
    depan = belakang = NULL; 
}

bool queueKosong() {
    return depan == NULL; 
}

void buatStack() {
    awal = top = NULL; 
}

bool stackKosong() {
    return awal == NULL; 
}

void simpanAntrean() {
    FILE* file = fopen("dataPesanan.txt", "w");
    if (!file) return; 

    for (Queue* temp = depan; temp != NULL; temp = temp->next) {
        fprintf(file, "Nama        : %s\n", temp->data.nama); 
        fprintf(file, "Jenis Roti  : %s\n", temp->data.jenis_roti); 
        fprintf(file, "Total Harga : %s\n", temp->data.total_harga); 
        fprintf(file, "================================\n"); 
    }

    fclose(file); 
} 

void tampilkanAntrean() {
    if (!queueKosong()) {
        cout << "\n<===== ANTREAN SAAT INI =====>\n";
        int i = 1; 
        for (Queue* temp = depan; temp != NULL; temp = temp->next, i++) {
            cout << "\nPesanan " << i << ":\n";
            cout << "Nama        : " << temp->data.nama << endl;
            cout << "Jenis Roti  : " << temp->data.jenis_roti << endl;
            cout << "Total Harga : " << temp->data.total_harga << endl;
            cout << "================================\n";
        }
    } else {
        cout << "Tidak ada antrean untuk saat ini\n"; 
    }
}

void enqueue(Pesanan data) {
    Queue* newNode = new Queue; 
    newNode->data = data; 
    newNode->next = NULL; 

    if (queueKosong()) {
        depan = belakang = newNode; 
    } else {
        belakang->next = newNode; 
        belakang = newNode; 
    }

    simpanAntrean(); 
}

void dequeue(Stack* &awal, Stack* &top) {
    if (queueKosong()) {
        cout << "Antrean sedang kosong" << endl; 
        return; 
    }

    Queue* hapus = depan; 
    Pesanan pesanan = hapus->data; 

    // Simpan ke stack 
    Stack* newNode = new Stack{pesanan, NULL}; 
    
    if (awal == NULL) {
        awal = top = newNode; 
    } else {
        top->next = newNode; 
        top = newNode; 
    }

    // Simpan ke file (append) 
    FILE* file = fopen("riwayatPesanan.txt", "a"); 
    if (file != NULL) {
        fprintf(file, "Nama        : %s\n", pesanan.nama); 
        fprintf(file, "Jenis Roti  : %s\n", pesanan.jenis_roti); 
        fprintf(file, "Total Harga : %s\n", pesanan.total_harga); 
        fprintf(file, "================================\n"); 

        fclose(file); 
    } 

    depan = depan->next; 
    delete hapus; 
    if (depan == NULL) belakang = NULL; 

    cout << "Pesanan atas nama " << pesanan.nama << " telah dilayani" << endl; 
    simpanAntrean(); 
}

void tambahPesanan() {
    Pesanan pesanan; 
    cin.ignore(); 

    cout << "<===== TAMBAHKAN DATA PESANAN =====>\n"; 
    cout << "\nNama        : "; cin.getline(pesanan.nama, 50); 
    cout << "Jenis Roti  : "; cin.getline(pesanan.jenis_roti, 50); 
    cout << "Total Harga : "; cin.getline(pesanan.total_harga, 50);  
    enqueue(pesanan); 
    cout << "\nData pesanan berhasil disimpan\n"; 
    cout << "Pesanan telah ditambahkan pada daftar antrean\n";  
}

void batalkanPesanan() {
    if (queueKosong()) {
        cout << "Antrean kosong, tidak ada pesanan untuk dibatalkan" << endl; 
        return; 
    }

    if (depan == belakang) {
        delete depan; 
        depan = belakang = NULL; 

        FILE *file = fopen("dataPesanan.txt", "w"); // Kosongkan file 
        if (file != NULL) fclose(file); 
    } else {
        Queue* temp = depan; 
        while (temp->next != belakang) {
            temp = temp->next; 
        } 
        delete belakang; 
        belakang = temp; 
        belakang->next = NULL; 

        simpanAntrean(); 
    }
    
    cout << "Pesanan terakhir berhasil dibatalkan" << endl;  
    remove("dataPesanan.txt");  
}

void tampilkanRiwayat() {
    FILE* file = fopen("riwayatPesanan.txt", "r"); 
    if (file == NULL) {
        cout << "Belum ada riwayat pesanan!" << endl; 
        return; 
    } 

    char baris[100]; 
    int nomor = 1; 
    cout << "\n<===== RIWAYAT PESANAN =====>\n"; 
    bool tampil = false; 

    while (fgets(baris, sizeof(baris), file)) {
        if (strncmp(baris, "Nama", 4) == 0) {
            cout << "\nPesanan " << nomor++ << endl;  
            tampil = true;  
        } 
        if (tampil) {
            cout << baris; 
        }
    }

    fclose(file); 
}

void muatAntrean() {
    FILE* file = fopen("dataPesanan.txt", "r");
    if (!file) return; 

    Pesanan pesanan;
    char buffer[100]; 
    while (fgets(buffer, sizeof(buffer), file)) {
        if (strncmp(buffer, "Nama", 4) == 0) {
            sscanf(buffer, "Nama : %[^\n]", pesanan.nama); 
        } else if (strncmp(buffer, "Jenis", 5) == 0) {
            sscanf(buffer, "Jenis Roti : %[^\n]", pesanan.jenis_roti);
        } else if (strncmp(buffer, "Total", 5) == 0) {
            sscanf(buffer, "Total Harga : %[^\n]", pesanan.total_harga);
            enqueue(pesanan); 
        } 
    }

    fclose(file); 
}

int main () {
    int pilihan; 

    buatQueue(); 
    muatAntrean(); 
    buatStack(); 

    do {
        cout << "\n<===== MENU SISTEM TOKO ROTI MANIS LEZAT =====>\n"; 
        cout << "\n1. Menambah Pesanan\n"; 
        cout << "2. Melihat Daftar Antrean Pesanan\n"; 
        cout << "3. Melayani Pembeli\n"; // Melayani pembeli berdasarkan urutan yang paling atas 
        cout << "4. Membatalkan Pesanan\n"; // Membatalkan pesanan paling akhir 
        cout << "5. Menampilkan History Pesanan\n"; 
        cout << "0. Keluar\n"; 
        cout << "Masukkan Pilihan : "; 
        cin >> pilihan; 
        system("cls"); 

        switch(pilihan) {
            case 1: 
                system("cls"); 
                tambahPesanan(); 
                break; 
            case 2: 
                system("cls"); 
                tampilkanAntrean(); 
                system("pause"); 
                break; 
            case 3: 
                system("cls");   
                dequeue(awal, top); 
                system("pause"); 
                break; 
            case 4: 
                system("cls"); 
                batalkanPesanan(); 
                system("pause"); 
                break; 
            case 5: 
                system("cls"); 
                tampilkanRiwayat(); 
                system("pause"); 
                break; 
            case 0: 
                cout << "Terima kasih telah menggunakan sistem Toko Roti Manis Lezat\n"; 
                break; 
            default: 
            cout << "Pilihan menu tidak valid. Silakan coba lagi!\n"; 
        }
    } while (pilihan != 0); 

    return 0; 
} 