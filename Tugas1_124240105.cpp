#include <iostream> 
#include <string>
#include <algorithm> //Untuk std::sort mengurutkan data menggunakan binary search 
using namespace std;

struct mahasiswa
{
    string nama;
    int nim;
    float ipk; 
};

//Fungsi untuk melakukan pencarian linear search 
int linearsearch(mahasiswa data[], int size, int nim) {
    for (int i = 0; i < size; i++) {
        if (data[i].nim == nim) {
            return i; //Mengembalikan index jika NIM ditemukan 
        }
    }
    return -1; //Mengembalikan -1 jika NIM tidak ditemukan 
} 

//Pengurutan dengan quick sort 
void quick_sort(mahasiswa data[], int awal, int akhir) {
    int low = awal, high = akhir; 
    float pivot = data[(awal + akhir) / 2].ipk; 
    mahasiswa temp; 

    do {
        while (data[low].ipk < pivot)
            low++; 
        while (data[high].ipk > pivot)
            high--;
        
            if (low <= high) {
                temp = data[low]; 
                data[low] = data[high]; 
                data[high] = temp; 

                low++;
                high--; 
            }
    } while (low <= high); 

    if (awal < high) 
        quick_sort(data, awal, high); 
    if (low < akhir) 
        quick_sort(data, low, akhir);
} 

//Pengurutan dengan bubble sort 
void bubble_sort(mahasiswa mhs[], int n) {
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (mhs[j].ipk < mhs[j + 1].ipk) {
                mahasiswa temp = mhs[j];
                mhs[j] = mhs[j + 1];
                mhs[j + 1] = temp; 
            }
        }
    }
}

int main ()
{
    int n = 5;  
    mahasiswa mhs[5] = 
    {
        {"Budi", 124240001, 3.5},
        {"Joko", 124240002, 3},
        {"Ferry", 124240003, 3.7},
        {"Bella", 124240004, 3.3},
        {"Khansa", 124240005, 3.9}
    }; 

    //Mengurutkan mahasiswa berdasarkan nama 
    sort(mhs, mhs + n, [](mahasiswa a, mahasiswa b) {
        return a.nama < b.nama; //Dengan binary search 
    }); 

    int i; 
    int pilih_menu;
    string cari_nama;
    int cari_nim; 
    int awal, akhir, tengah; 
    bool found = false; 

    do {
        cout << "\n==== PILIHAN MENU ====" <<endl; 
        cout << "1. Tampilkan Data Mahasiswa" << endl; 
        cout << "2. Cari Berdasarkan NIM" << endl; 
        cout << "3. Cari Berdasarkan Nama" << endl; 
        cout << "4. Sort IPK Mahasiswa (asc)" << endl;
        cout << "5. Sort IPK Mahasiswa (desc)" << endl; 
        cout << "6. Exit" << endl; 
        cout << "Pilih menu (1-6) : ";  
        cin >> pilih_menu; 
        cout << endl; 
        system("cls");

        if (pilih_menu == 1) {
            cout << "Daftar Mahasiswa :" << endl;
            for (int i = 0; i < 5; i++) {
                cout << "Nama : " << mhs[i].nama;  
                cout << ", NIM : " << mhs[i].nim; 
                cout << ", IPK : " << mhs[i].ipk << endl; 
            } 
        }
        else if (pilih_menu == 2) {
            cout << "Masukkan NIM yang akan dicari : ";
            cin >> cari_nim; 
            
            int index = linearsearch(mhs, n, cari_nim); 

            if (index != -1) {
                cout << "Nama : " << mhs[index].nama << ", NIM : " << mhs[index].nim << ", IPK : " << mhs[index].ipk << endl; 
            }else {
                cout << "Mahasiswa dengan NIM " << cari_nim << " tidak ditemukan" << endl; 
            }
        }
        else if (pilih_menu == 3) {
            cout << "Masukkan Nama yang akan dicari : "; 
            cin >> cari_nama; 
            
            awal = 0; 
            akhir = 4; 
            found = false;  

            while (awal <= akhir && !found) {
                tengah = (awal + akhir) / 2; 
                if (mhs[tengah].nama == cari_nama) {
                    found = true; 
                }else if (mhs[tengah].nama < cari_nama) {
                    awal = tengah + 1; 
                }else {
                    akhir = tengah - 1; 
                }
            }
            if (found) {
                cout << "Nama : " << mhs[tengah].nama << ", NIM : " << mhs[tengah].nim << ", IPK : " << mhs[tengah].ipk << endl;   
            }else {
                cout << "Mahasiswa dengan nama " << cari_nama << " tidak ditemukan" << endl; 
            }
        }
        else if (pilih_menu == 4) {
            quick_sort(mhs, 0, n - 1); 
            cout << "Daftar Mahasiswa Setelah Diurutkan IPK Berdasarkan (Ascending) : " << endl; 
            for (int i = 0; i < n; i++) {
                cout << "Nama : " << mhs[i].nama << ", NIM : " << mhs[i].nim << ", IPK : " << mhs[i].ipk << endl; 
            }
        }
        else if (pilih_menu == 5) {
            bubble_sort(mhs, n);
            cout << "Daftar Mahasiswa Setelah Diurutkan IPK Berdasarkan (Descending) : " << endl; 
            for (int i = 0; i < n; i++) {
                cout << "Nama : " << mhs[i].nama << ", NIM : " << mhs[i].nim << ", IPK : " << mhs[i].ipk << endl; 
            }
        }
        else if (pilih_menu == 6) {
            cout << "=== Program Selesai ===" << endl;
            cout << "Terima Kasih Telah Menggunakan Program Ini" << endl; 
            system("pause"); 
        }
    }while (pilih_menu != 6); 
    return 0;  
} 