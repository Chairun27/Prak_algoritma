#include <iostream>
#include <cstring>
using namespace std; 

// Struktur video binary tree 
struct VideoNode {
    string judul;
    int durasi;
    string status;
    VideoNode *left; 
    VideoNode *right; 
}; 

// Struktur queue untuk playlist 
struct QueueNode {
    VideoNode* video;
    QueueNode* next;
}; 

// Struktur stack untuk riwayat dan undo 
struct StackNode {
    string aksi; // tambah, hapus, playlist, tonton 
    VideoNode* video;
    StackNode* next;
}; 

// Deklarasi pointer 
VideoNode* root = nullptr; // akar (root)
QueueNode* frontQ = nullptr; // penunjuk awal 
QueueNode* rearQ = nullptr; // penunjuk belakang 
StackNode* riwayatTop = nullptr; // puncak stack 
StackNode* undoTop = nullptr; // aksi terakhir 

// Membuat node video baru 
VideoNode* buatVideo(string judul, int durasi) {
    VideoNode* v = new VideoNode{judul, durasi, "tersedia", nullptr, nullptr}; 
    return v; 
}

// Menambahkan video baru ke binary tree 
VideoNode* tambahBinaryTree(VideoNode* node, VideoNode* newVideo) {
    if (!node) {
        return newVideo; 
    } 
    if (newVideo->judul < node->judul) {
        node->left = tambahBinaryTree(node->left, newVideo); 
    } else if (newVideo->judul > node->judul) {
        node->right = tambahBinaryTree(node->right, newVideo);
    }
    return node; 
}

// Menampilkan video secara ascending 
void inorderBinaryTree(VideoNode* node) {
    if (!node) {
        return; 
    }

    inorderBinaryTree(node->left); 
    cout << "Judul  : " << node->judul << endl; 
    cout << "Durasi : " << node->durasi << " menit" << endl; 
    cout << "Status : " << node->status << endl; 
    cout << "==========================" << endl; 
    inorderBinaryTree(node->right);  
} 

// Mencari node berdasarkan judul 
VideoNode* cariBinaryTree(VideoNode* node, string judul) {
    if (!node || node->judul == judul) { 
        return node; 
    } 

    if (judul < node->judul) {
        return cariBinaryTree(node->left, judul); 
    } else {
        return cariBinaryTree(node->right, judul);
    } 
}

// Fungsi playlist 
void enqueue(VideoNode* video) {
    if (video->status != "tersedia") {
        cout << "Video sudah ada di dalam playlist atau sedang diputar" << endl; 
        return; 
    }

    QueueNode* newNode = new QueueNode{video, nullptr}; 

    if (!frontQ) {
        frontQ = rearQ = newNode;
        video->status = "sedang diputar"; 
    } else {
        rearQ->next = newNode; 
        rearQ = newNode; 
        video->status = "dalam antrean"; 
    }

    // Tambah ke undo 
    StackNode* undo = new StackNode{"playlist", video, undoTop}; 
    undoTop = undo; 
}

// Fungsi tonton (dequeue dan simpan ke riwayat)
void dequeue() {
    if (!frontQ) {
        cout << "Playlist kosong" << endl; 
        return; 
    }

    VideoNode* video = frontQ->video; 
    video->status = "tersedia"; 

    // Menambahkan ke riwayat 
    StackNode* riwayat = new StackNode{"tonton", video, riwayatTop};
    riwayatTop = riwayat; 

    // Menambahkan ke undo 
    StackNode* undo = new StackNode{"tonton", video, undoTop};
    undoTop = undo; 

    QueueNode* temp = frontQ;
    frontQ = frontQ->next; 
    if (!frontQ) rearQ = nullptr; 

    delete temp; 
}

// Menampilkan playlist  
void tampilkanPlaylist() {
    if (!frontQ) {
        cout << "Playlist kosong" << endl; 
        return; 
    }

    QueueNode* temp = frontQ;
    cout << "\n<==== Daftar Playlist ====>" << endl; 
    while (temp) {
        cout << "Judul  : " << temp->video->judul << endl;
        cout << "Durasi : " << temp->video->durasi << " menit" << endl; 
        cout << "Status : " << temp->video->status << endl; 
        cout << "==========================" << endl; 
        temp = temp->next; 
    }
} 

// Menampilkan riwayat 
void tampilkanRiwayat() {
    if (!riwayatTop) {
        cout << "Riwayat tontonan video kosong" << endl; 
        return; 
    }

    StackNode* temp = riwayatTop; 
    cout << "\n<==== Riwayat tontonan ====>" << endl; 
    while (temp) {
        cout << "Judul  : " << temp->video->judul << endl; 
        cout << "Durasi : " << temp->video->durasi << " menit" << endl; 
        cout << "==========================" << endl; 
        temp = temp->next; 
    }
}

// Fungsi undo
void undo() {
    if (!undoTop) {
        cout << "Tidak ada aksi yang bisa diundo" << endl; 
        return; 
    }

    StackNode* aksi = undoTop; 
    string type = aksi->aksi; 
    VideoNode* video = aksi->video; 

    if (type == "playlist") {
        // Hapus dari playlist 
        QueueNode* prev = nullptr, *curr = frontQ; 
        while (curr && curr->video != video) {
            prev = curr;
            curr = curr->next; 
        }

        if (curr) {
            if (curr == frontQ) frontQ = curr->next;
            else if (prev) prev->next = curr->next;
            if (curr == rearQ) rearQ = prev; 
            delete curr; 
        }

        video->status = "tersedia"; 
        cout << "Undo playlist berhasil" << endl; 
    } else if (type == "tonton") {
        // Kembalikan ke playlist 
        video->status = "sedang diputar"; 
        QueueNode* newNode = new QueueNode{video, frontQ}; 
        frontQ = newNode;
        if (!rearQ) rearQ = newNode;

        // Hapus dari riwayat 
        if (riwayatTop && riwayatTop->video == video) {
            StackNode* temp = riwayatTop;
            riwayatTop = riwayatTop->next; 
            delete temp;
        }
        cout << "Undo tontonan berhasil" << endl; 
    }

    undoTop = undoTop->next;
    delete aksi; 
}

// Fungsi bantu mencari node paling kiri 
VideoNode* cariMinimum(VideoNode* node) {
    while (node && node->left) {
        node = node->left; 
    }
    return node; 
}

// Fungsi rekursif untuk menghapus node dari binary tree
VideoNode* hapusBinaryTree(VideoNode* node, string judul, bool& berhasil, VideoNode*& deletedNode) {
    if (!node) {
        berhasil = false; 
        return nullptr; 
    } 

    if (judul < node->judul) {
        node->left = hapusBinaryTree(node->left, judul, berhasil, deletedNode); 
    } else if (judul > node->judul) {
        node->right = hapusBinaryTree(node->right, judul, berhasil, deletedNode); 
    } else {
        // Ketemu node yang ingin dihapus 
        berhasil = true;
        deletedNode = node; 

        // Tidak memiliki anak 
        if (!node->left && !node->right) {
            return nullptr; 
        }

        // Memiliki 1 anak
        if (!node->left) return node->right; 
        if (!node->right) return node->left;

        // Memiliki 2 anak
        VideoNode* pengganti = cariMinimum(node->right); 
        node->judul = pengganti->judul;
        node->durasi = pengganti->durasi;
        node->status = pengganti->status; 

        // Hapus node pengganti dari subtree kanan
        bool dummy;
        VideoNode* dummyDeleted;
        node->right = hapusBinaryTree(node->right, pengganti->judul, dummy, dummyDeleted); 
    }
    return node; 
}

void hapusVideo(string judul) {
    VideoNode* target = cariBinaryTree(root, judul); 
    if (!target) {
        cout << "Video tidak ditemukan" << endl; 
        return; 
    }

    // Mengecek status video 
    if (target->status == "dalam antrean" || target->status == "sedang diputar") {
        cout << "Video sudah ada di dalam playlist atau sedang diputar" << endl;
        cout << "Yakin ingin menghapus? (y/t) : "; 
        char jawab; 
        cin >> jawab; 
        if (jawab == 't') {
            cout << "Penghapusan dibatalkan" << endl;
            return;
        }
    }

    QueueNode* prev = nullptr, *curr = frontQ; 
        while (curr) {
            if (curr->video == target) {
                if (curr == frontQ) frontQ = curr->next;
                else if (prev) prev->next = curr->next;
                if (curr == rearQ) rearQ = prev; 
                delete curr; 
                break; 
            }

            prev = curr;
            curr = curr->next;
        }

        // Hapus dari binary tree
        bool berhasil = false; 
        VideoNode* deletedNode = nullptr;
        root = hapusBinaryTree(root, judul, berhasil, deletedNode); 

        if (berhasil && deletedNode) {
            // Tambahkan ke undo stack 
            StackNode* undo = new StackNode{"hapus", deletedNode, undoTop}; 
            undoTop = undo; 
            cout << "Video " << judul << " berhasil dihapus" << endl; 
        } else {
            cout << "Gagal menghapus video" << endl; 
        }
}

void menghapusVideo() {
    string judul;
    cout << "Masukkan judul video yang ingin dihapus : "; 
    cin.ignore(); 
    getline(cin, judul); 
    hapusVideo(judul); 
}

int main() {
    int pilihan;
    string judul; 
    int durasi; 

    do {
        cout << "\n<==== MENU STREAMING VIDEO IDLIX TUBE ====>\n"; 
        cout << "\n1. Tambah Video\n"; 
        cout << "2. Tampilkan Daftar Video\n"; 
        cout << "3. Tambahkan ke Playlist\n"; 
        cout << "4. Tonton Video\n"; 
        cout << "5. Riwayat Tontonan\n"; 
        cout << "6. Hapus Video\n"; 
        cout << "7. Undo Tindakan Terakhir\n"; 
        cout << "0. Keluar\n"; 
        cout << "Silakan pilih menu : "; 
        cin >> pilihan; 

        switch (pilihan) {
            case 1: {
                system("cls"); 
                cout << "Masukkan judul video : "; 
                cin.ignore(); 
                getline(cin, judul); 
                cout << "Masukkan durasi video (dalam menit) : "; 
                cin >> durasi; 
                cout << endl; 

                if (cariBinaryTree(root, judul)) {
                    cout << "Judul video sudah ada, tidak boleh terdapat duplikat judul" << endl; 
                } else {
                    VideoNode* video = buatVideo(judul, durasi); 
                    root = tambahBinaryTree(root, video); 
                    undoTop = new StackNode{"tambah", video, undoTop}; 
                    cout << "Video berhasil ditambahkan" << endl; 
                }

                system("pause"); 
                break;
            } 

            case 2: {
                system("cls"); 
                if (!root) {
                    cout << "Tidak ada video yang tersedia saat ini" << endl; 
                } else {
                    cout << "Berikut adalah daftar video secara ascending : " << endl; 
                    cout << "===============================================" << endl; 
                    inorderBinaryTree(root); 
                    cout << endl;

                    cout << "Ingin mencari video berdasarkan judul? (y/t) : "; 
                    char cari;
                    cin >> cari; 
                    if (cari == 'y') {
                        cout << "Masukkan judul video yang ingin dicari : "; 
                        cin.ignore(); 
                        getline(cin, judul); 
                        VideoNode* ditemukan = cariBinaryTree(root, judul); 
                        if (ditemukan) {
                            cout << endl; 
                            cout << "<==== Video ditemukan ====>" << endl; 
                            cout << "Judul  : " << ditemukan->judul << endl; 
                            cout << "Durasi : " << ditemukan->durasi << " menit" << endl; 
                            cout << "Status : " << ditemukan->status << endl; 
                            cout << "==========================" << endl; 
                        } else {
                            cout << "Video dengan judul " << judul << " tidak ditemukan" << endl; 
                        }
                    }
                }
                
                system("pause"); 
                break; 
            } 

            case 3: {
                system("cls"); 
                cout << "Masukkan judul video yang ingin ditambahkan ke playlist : "; 
                cin.ignore(); 
                getline(cin, judul); 
                VideoNode* video = cariBinaryTree(root, judul); 
                if (video) {
                    enqueue(video); 
                    cout << "Video berhasil ditambahkan ke playlist" << endl; 
                } else {
                    cout << "Video tidak ditemukan" << endl; 
                }

                system("pause"); 
                break;
            } 

            case 4: { 
                system("cls");
                if (!frontQ) {
                    cout << "Playlist kosong. Tambahkan video terlebih dahulu" << endl; 
                } else {
                    tampilkanPlaylist();

                    cout << "Ingin menonton video terdepan dari playlist? (y/t) : "; 
                    char tonton;
                    cin >> tonton;
                
                    if (tonton == 'y') {
                        string judulDitonton = frontQ->video->judul; // menyimpan nama sebelum dequeue 
                        dequeue(); 
                        cout << "Video " << judulDitonton << " telah selesai ditonton" << endl; 
                    } else {
                        cout << "Tontonan dibatalkan" << endl; 
                    }
                }

                system("pause"); 
                break; 
            } 

            case 5: {
                system("cls"); 
                tampilkanRiwayat();  
                system("pause"); 
                break;
            } 

            case 6: {
                system("cls");
                menghapusVideo();  
                system("pause");
                break;
            } 

            case 7: {
                system("cls");
                undo();  
                system("pause");
                break;
            } 

            case 0: {
                system("cls");
                cout << "Terima kasih telah menggunakan program streaming video IDLIX Tube ini" << endl;  
                system("pause");
                break;
            } 

            default: {
                system("cls");
                cout << "Pilihan menu yang dimasukkan tidak valid. Silakan coba lagi" << endl;   
                system("pause");
            } 
        }
    } while (pilihan != 0); 
    return 0; 
}