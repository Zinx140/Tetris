#include <iostream>
#include <conio.h>
#include <cstdlib>
#include <windows.h>
#include <time.h>

using namespace std;

//ukuran arena
const int width = 30;
const int height = 20;

// 7 bentuk tetromino ukuran 4x4 dirandom index depannya [random][4][4]
// buat angka 0 artinya itu blank space kalau angka 1 itu nanti keluarnya jadi kotak tetromino nya
int tetrominoes[7][4][4] = {
    {{0, 0, 0, 0}, {0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}}, // I
    {{1, 1}, {1, 1}},                                         // O
    {{0, 1, 0}, {1, 1, 1}, {0, 0, 0}},                        // T
    {{1, 0, 0}, {1, 1, 1}, {0, 0, 0}},                        // L
    {{0, 0, 1}, {1, 1, 1}, {0, 0, 0}},                        // J
    {{0, 1, 1}, {1, 1, 0}, {0, 0, 0}},                        // S
    {{1, 1, 0}, {0, 1, 1}, {0, 0, 0}}                         // Z
};

//Buat ngegambar tetromino di dalam arena
void summonTetromino(int arena[height][width], int position_x, int position_y, int randomTetromino) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            arena[i][j] = 0; //ngosongkan arena dulu buat nge hapus jejak tetromino yang belum jatuh atau dipindah
        }
    }

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            arena[position_y + i][position_x + j] = tetrominoes[randomTetromino][i][j]; // buat ngeganti tempat di peta biar ngebentuk tetromino nya
        }
    }

}

//buat ngegambar arena yang lokasi tetromino nya udah di geser
void draw(int arena[height][width]) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (arena[i][j] == 1) {
                cout << "@"; //untuk ngegambar tetromino
            } else {
                cout << "."; //untuk ngegambbar arena kosong
            }
        }
        cout << endl;
    }
}


int main () {
    srand(time(0));
    int tetromino_y = 0; // inisialisasi posisi y tetromino
    int tetromino_x = rand() % 27; // inisialisasi posisi x tetromino
    int randomTetromino = rand() % 7; // random untuk bentuk tetromino
    int arena [height][width]; // inisialisasi buat arena nya

    while (true) {
        if (tetromino_y < 19) {
            if (kbhit()) { // kontrol untuk geser tetromino nya kalau bisa buat biar tetromino nya nggak melebihi border
                char control = getch();

                if (control == 'a') { // geser kiri
                    if (tetromino_x > 0) {
                        tetromino_x -= 1;
                    }
                } else if (control == 'd') { // geser kanan
                    if (tetromino_x < 27) {
                        tetromino_x += 1;
                    }
                }
            }

            summonTetromino(arena, tetromino_x, tetromino_y, randomTetromino);
            tetromino_y++; //nurunin tetromino
        } else {
            tetromino_y = 0; // ngembaliin posisi y tetromino ke atas karena udah jatuh ke bawah biar summon lagi
            tetromino_x = rand() % 27; // ngerandom ulang posisi x tetromino
            randomTetromino = rand() % 7; // ngerandom bentuk tetromino lagi
        }
        draw(arena);
        Sleep(100);
        system("cls");
    }

    return 0;
}


