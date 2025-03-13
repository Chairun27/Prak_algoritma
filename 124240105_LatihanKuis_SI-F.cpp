#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <limits>
#include <cctype>

using namespace std;

struct mahasiswa {
    string nama;
    int nim;
    string jurusan;
    int tahun_masuk;
    float ipk;
};

string trim(const string& str) {
    size_t start = str.find_first_not_of(" \t\n\r");  // Mencari karakter pertama yang bukan spasi
    size_t end = str.find_last_not_of(" \t\n\r");    // Mencari karakter terakhir yang bukan spasi
    if (start == string::npos || end == string::npos) return ""; // Jika tidak ada karakter selain spasi
    return str.substr(start, end - start + 1);
}

string to_lowercase(const string& str) {
    string result = str;
    transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

void tambah_mhs(vector<mahasiswa>& data) {
    ofstream file("MahasiswaData.dat", ios::app | ios::binary);
    if (!file) {
        cout << "File tidak dapat dibuka!" << endl;
        return;
    }

    mahasiswa* mhs = new mahasiswa;

    cout << "Masukkan Data Mahasiswa" << endl;
    cout << "Nama : ";
    cin.ignore();
    getline(cin, mhs->nama);  // Input string
    cout << "NIM : ";
    cin >> mhs->nim;
    cout << "Jurusan : ";
    cin.ignore();
    getline(cin, mhs->jurusan);  // Input string
    cout << "Tahun Masuk : ";
    cin >> mhs->tahun_masuk;

    mhs->ipk = 0.0;  // IPK awal

    // Menyimpan panjang string nama dan jurusan terlebih dahulu
    size_t nama_len = mhs->nama.size();
    size_t jurusan_len = mhs->jurusan.size();

    // Menulis panjang string nama dan jurusan ke file
    file.write(reinterpret_cast<char*>(&nama_len), sizeof(nama_len));
    file.write(mhs->nama.c_str(), nama_len);  // Menulis string nama

    file.write(reinterpret_cast<char*>(&jurusan_len), sizeof(jurusan_len));
    file.write(mhs->jurusan.c_str(), jurusan_len);  // Menulis string jurusan

    // Menulis data lainnya (NIM, Tahun Masuk, IPK)
    file.write(reinterpret_cast<char*>(&mhs->nim), sizeof(mhs->nim));
    file.write(reinterpret_cast<char*>(&mhs->tahun_masuk), sizeof(mhs->tahun_masuk));
    file.write(reinterpret_cast<char*>(&mhs->ipk), sizeof(mhs->ipk));

    delete mhs;
    file.close();
}

void tampilkan_mhs(vector<mahasiswa>& data) {
    ifstream file("MahasiswaData.dat", ios::in | ios::binary);
    if (!file) {
        cout << "File tidak dapat dibuka!" << endl;
        return;
    }

    mahasiswa mhs;

    cout << "Berikut Adalah Data Mahasiswa" << endl;
    cout << "--------------------------------------" << endl;

    while (file) {
        size_t nama_len, jurusan_len;

        // Membaca panjang string nama dan jurusan
        file.read(reinterpret_cast<char*>(&nama_len), sizeof(nama_len));
        if (file.eof()) break;  // Jika sudah mencapai akhir file

        mhs.nama.resize(nama_len);  // Menyesuaikan ukuran nama
        file.read(&mhs.nama[0], nama_len);  // Membaca string nama

        file.read(reinterpret_cast<char*>(&jurusan_len), sizeof(jurusan_len));
        mhs.jurusan.resize(jurusan_len);  // Menyesuaikan ukuran jurusan
        file.read(&mhs.jurusan[0], jurusan_len);  // Membaca string jurusan

        // Membaca data lainnya
        file.read(reinterpret_cast<char*>(&mhs.nim), sizeof(mhs.nim));
        file.read(reinterpret_cast<char*>(&mhs.tahun_masuk), sizeof(mhs.tahun_masuk));
        file.read(reinterpret_cast<char*>(&mhs.ipk), sizeof(mhs.ipk));

        if (file) {
        // Menampilkan data mahasiswa
        cout << "Nama        : " << mhs.nama << endl; 
        cout << "NIM         : " << mhs.nim << endl;
        cout << "Jurusan     : " << mhs.jurusan << endl;
        cout << "Tahun Masuk : " << mhs.tahun_masuk << endl;
        cout << "IPK         : " << mhs.ipk << endl;
        cout << "--------------------------------------" << endl;
        }
    }

    file.close();
}

void simpan_file (const vector<mahasiswa>& arr) {
    ofstream file("MahasiswaData.dat", ios::out | ios::binary);
    if (!file) {
        cout << "Gagal membuka file untuk menulis!" << endl;
        return;
    }

    for (const auto& mhs : arr) {
        size_t nama_len = mhs.nama.size();
        size_t jurusan_len = mhs.jurusan.size();

        file.write(reinterpret_cast<const char*>(&nama_len), sizeof(nama_len));
        file.write(mhs.nama.c_str(), nama_len); 

        file.write(reinterpret_cast<const char*>(&jurusan_len), sizeof(jurusan_len));
        file.write(mhs.jurusan.c_str(), jurusan_len); 

        file.write(reinterpret_cast<const char*>(&mhs.nim), sizeof(mhs.nim));
        file.write(reinterpret_cast<const char*>(&mhs.tahun_masuk), sizeof(mhs.tahun_masuk));
        file.write(reinterpret_cast<const char*>(&mhs.ipk), sizeof(mhs.ipk));
    }
    file.close(); 
}

int binary_search(vector<mahasiswa>& arr, int nim) {
    int left = 0;
    int right = arr.size() - 1; 

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (arr[mid].nim == nim) {
            return mid; // NIM ditemukan
        } else if (arr[mid].nim < nim) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return -1;
} 

//Fungsi untuk melakukan partisi pada array 
int partition (vector<mahasiswa>& arr, int low, int high) {
    int pivot = arr[high].nim; 
    int i = (low - 1); 

    for (int j = low; j <= high - 1; j++) {
        if (arr[j].nim < pivot) {
            i++; 
            swap (arr[i], arr[j]); 
        }
    }
    swap (arr[i + 1], arr[high]); 
    return (i + 1); 
}

//Pengurutan atau sorting ipk dengan quick sort 
void quick_sort (vector<mahasiswa>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high); 
        quick_sort (arr, low, pi - 1); 
        quick_sort (arr, pi + 1, high); 
    }
}

void cari_jurusan (vector<mahasiswa>& arr, const string& jurusan) {
    bool found = false;
    string jurusanLower = to_lowercase(trim(jurusan)); 

    for (auto& mhs : arr) {
        if (to_lowercase(trim(mhs.jurusan)) == jurusanLower) {
            cout << "\nData mahasiswa ditemukan" << endl;
            cout << "Nama        : " << mhs.nama << endl;
            cout << "NIM         : " << mhs.nim << endl;
            cout << "Jurusan     : " << mhs.jurusan << endl;
            cout << "Tahun Masuk : " << mhs.tahun_masuk << endl;
            cout << "IPK         : " << mhs.ipk << endl;
            cout << "--------------------------------------" << endl;
            found = true; 
        }
    }

    if (!found) {
        cout << "\nMahasiswa dengan jurusan " << jurusan << " tidak ditemukan!" << endl; 
    }
}

void update_ipk (vector<mahasiswa>& arr, int nim) {
    int index = binary_search(arr, nim); 
    if (index != -1) {
        cout << "\nData mahasiswa ditemukan" << endl;
        cout << "Nama         : " << arr[index].nama << endl;
        cout << "NIM          : " << arr[index].nim << endl;
        cout << "Jurusan      : " << arr[index].jurusan << endl;
        cout << "Tahun Masuk  : " << arr[index].tahun_masuk << endl;
        cout << "IPK          : " << arr[index].ipk << endl; 
        
        float new_ipk;
        cout << "\nMasukkan nilai IPK baru (0-4) : ";
        cin >> new_ipk;

        if (new_ipk < 0 || new_ipk > 4) {
            cout << "Nilai IPK tidak valid! Harus antara 0-4" << endl; 
        }
        else {
            arr[index].ipk = new_ipk;
            cout << "Nilai IPK berhasil diupdate!" << endl; 

            simpan_file(arr);
        }
    }
    else {
        cout << "\nMahasiswa dengan NIM " << nim << " tidak ditemukan!" << endl; 
    }
}

void hapus_mahasiswa (vector<mahasiswa>& arr, int nim) {
    int index = binary_search(arr, nim); 
    if (index != -1) {
        arr.erase(arr.begin() + index);
        cout << "\nMahasiswa dengan NIM " << nim << " berhasil dihapus!" << endl; 
    }
    else {
        cout << "\nMahasiswa dengan NIM " << nim << " tidak temukan!" << endl; 
    }
}

void muat_data (vector<mahasiswa>& data) {
    ifstream file("MahasiswaData.dat", ios::in | ios::binary);
    if (!file) {
        cout << "File tidak dapat dibuka!" << endl; 
        return; 
    }

    mahasiswa mhs;
    while(file) {
        size_t nama_len, jurusan_len;

        file.read(reinterpret_cast<char*>(&nama_len), sizeof(nama_len));
        if (file.eof()) break;  // Jika sudah mencapai akhir file

        mhs.nama.resize(nama_len);  // Menyesuaikan ukuran nama
        file.read(&mhs.nama[0], nama_len);  // Membaca string nama

        file.read(reinterpret_cast<char*>(&jurusan_len), sizeof(jurusan_len));
        mhs.jurusan.resize(jurusan_len);  // Menyesuaikan ukuran jurusan
        file.read(&mhs.jurusan[0], jurusan_len);  // Membaca string jurusan

        // Membaca data lainnya
        file.read(reinterpret_cast<char*>(&mhs.nim), sizeof(mhs.nim));
        file.read(reinterpret_cast<char*>(&mhs.tahun_masuk), sizeof(mhs.tahun_masuk));
        file.read(reinterpret_cast<char*>(&mhs.ipk), sizeof(mhs.ipk));

        if (file) {
            data.push_back(mhs);
        } 
    }
    file.close();
}

void simpan_file (const vector<mahasiswa>& arr); 

int main() {
    vector<mahasiswa> data; 
    int menu;
    int nim;
    string jurusan; 

    muat_data(data);
    
    do {
        cout << "\n==== Selamat Datang pada Menu SmartCampus++ ====" << endl;
        cout << "| 1. Input Data Mahasiswa                      |" << endl;
        cout << "| 2. Tampilkan Data Mahasiswa                  |" << endl;
        cout << "| 3. Cari Mahasiswa Berdasarkan NIM            |" << endl;
        cout << "| 4. Cari Mahasiswa Berdasarkan Jurusan        |" << endl;
        cout << "| 5. Update IPK Mahasiswa                      |" << endl;
        cout << "| 6. Hapus Data Mahasiswa                      |" << endl; 
        cout << "| 7. Exit                                      |" << endl;
        cout << "------------------------------------------------" << endl;
        cout << "Pilih menu (1-7) : ";
        cin >> menu;
        system("cls");

        if (menu == 1) {
            tambah_mhs(data);
            muat_data(data);
        }
        else if (menu == 2) {
            tampilkan_mhs(data);
        }
        else if (menu == 3) {
            cout << "Masukkan NIM yang akan dicari : ";
            cin >> nim;
            cout << endl; 

            quick_sort(data, 0, data.size() - 1);
            int index = binary_search(data, nim);
            if (index != -1) {
                cout << "Data mahasiswa ditemukan" << endl;
                cout << "Nama         : " << data[index].nama << endl;
                cout << "NIM          : " << data[index].nim << endl;
                cout << "Jurusan      : " << data[index].jurusan << endl;
                cout << "Tahun Masuk  : " << data[index].tahun_masuk << endl;
                cout << "IPK          : " << data[index].ipk << endl; 
            }
            else {
                cout << "Mahasiswa dengan NIM " << nim << " tidak ditemukan!" << endl; 
            }
        }
        else if (menu == 4) {
            cout << "Masukkan jurusan yang akan dicari : "; 
            cin.ignore();
            getline(cin, jurusan);
            cari_jurusan(data, jurusan);  
        }
        else if (menu == 5) {
            cout << "Masukkan NIM mahasiswa yang ingin diupdate nilai IPK-nya : "; 
            cin >> nim; 
            update_ipk(data, nim);
        }
        else if (menu == 6) {
            cout << "Masukkan NIM mahasiswa yang ingin dihapus : ";
            cin >> nim; 
            hapus_mahasiswa(data, nim); 
        }
        else if (menu == 7) {
            simpan_file(data); 
            cout << "Terima kasih telah menggunakan SmartCampus++! Semoga sukses dalam studi Anda!" << endl;
            system("pause"); 
        }
    } while (menu != 7);

    return 0;
}