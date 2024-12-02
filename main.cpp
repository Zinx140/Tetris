#include <iostream>
#include <conio.h>
#include <cstdlib>
#include <windows.h>
#include <time.h>
#include <vector>

using namespace std;

//ukuran arena
const int width = 25;
const int height = 20;

// 7 bentuk tetromino ukuran 4x4 dirandom index depannya [random][4][4]
// buat angka 0 artinya itu blank space kalau angka 1 itu nanti keluarnya jadi kotak tetromino nya
vector<vector<vector<int>>> tetromino = {
                                            {{1, 1, 1, 1}},                 // I
                                            {{1, 1}, {1, 1}},               // O
                                            {{0, 1, 0}, {1, 1, 1}},         // T
                                            {{1, 0, 0}, {1, 1, 1}},         // L
                                            {{0, 0, 1}, {1, 1, 1}},         // J
                                            {{0, 1, 1}, {1, 1, 3}},         // S
                                            {{1, 1, 0}, {3, 1, 1}}          // Z
                                        };

//Buat ngegambar tetromino di dalam arena
void summonTetromino(int arena[height][width], int position_x, int position_y, int randomTetromino) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (i == 0 || j == 0 || i == height-1 || j == width-1) {
                arena[i][j] = 2; //ngosongkan arena dulu buat nge hapus jejak tetromino yang belum jatuh atau dipindah
            } else {
                if (arena[i][j] != 7) {
                    arena[i][j] = 0;
                }
            }
        }
    }

    for (int i = 0; i < tetromino[randomTetromino].size(); i++) { // buat ngegambar ukuran tinggi tetromino pakai vektor biar gk nembus
        for (int j = 0; j < tetromino[randomTetromino][i].size(); j++) { // buat ngegambar lebar tetromino vektor
            arena[position_y + i][position_x + j] = tetromino[randomTetromino][i][j]; // buat ngeganti tempat di peta biar ngebentuk tetromino nya
        }
    }

}

//buat ngegambar arena yang lokasi tetromino nya udah di geser
void draw(int arena[height][width]) {
    char block = 219;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (arena[i][j] == 1 || arena[i][j] == 7) {
                cout << "@"; //untuk ngegambar tetromino
            } else if (arena[i][j] == 2) {
                cout << "."; //untuk ngegambbar border arena
            } else {
                cout << " "; //untuk ngegambbar arena kosong
            }
        }
        cout << endl;
    }
}

bool canMove(int arena[height][width], int &position_x, int position_y, int randomTetromino, int dx) {
    int newY = tetromino[randomTetromino].size();
    int newX = position_x + tetromino[randomTetromino][0].size();
    bool flag = false;
    int solid = -1;
    int blank;

    for (int i = 0; i < tetromino[randomTetromino][0].size(); i++) {
        if (tetromino[randomTetromino][newY - 1][i] == 3) {
            flag = true;
            blank = i;
        }
    }

    for (int i = position_x + dx; i < newX; i++) {
        if ((arena[newY + position_y - 1][i] == 2 || arena[newY + position_y - 1][i + dx] == 7) && !flag) {
            for (int j = position_y; j < tetromino[randomTetromino].size() + position_y; j++) {
                for (int k = position_x; k < newX; k++) {
                    if (arena[j - 1][k] == 1) {
                        arena[j - 1][k] = 7;
                    }
                }
            }
            return false;
        } else if (arena[newY + position_y - 1][i] == 2 && flag) {
            for (int j = position_y; j < tetromino[randomTetromino].size() + position_y; j++) {
                for (int k = position_x; k < newX; k++) {
                    if (arena[j - 1][k] == 1) {
                        arena[j - 1][k] = 7;
                    }
                }
            }
            return false;
        } else if (arena[newY + position_y - 1][i] == 7 && flag) {
            if (arena[newY + position_y - 1][blank + position_x] == 7 && arena[newY + position_y - 1][position_x + 1] != 7) {
                for (int j = position_y - 1; j < tetromino[randomTetromino].size() + position_y; j++) {
                    for (int k = position_x; k < newX; k++) {
                        if (arena[j - 1][k] == 1) {
                            arena[j][k] = 7;
                            arena[j][position_x + 1] = 7;
                        }
                    }
                }
                return false;
            } else {
                for (int j = position_y; j < tetromino[randomTetromino].size() + position_y; j++) {
                    for (int k = position_x; k < newX; k++) {
                        if (arena[j - 1][k] == 1) {
                            arena[j - 1][k] = 7;
                        }
                    }
                }
                return false;
            }
        }
    }
    position_x += dx;
    return true;

}

//belum diuji karena hitbox dan tetromino belum bisa muter
void clearLines (int arena[height][width]) {
    for (int i = 1; i < height; i ++) {
    bool flag = true;
        for (int j = 1; j < width; j++) {
            if (arena[i][j] != 7) {
                flag = false;
            }
        }
        if (flag) {
            for (int j = 1; j < width; j++) {
                arena[i][j] = 0;
            }
        }
    }
}

bool gameOver (int arena[height][width]) { // menghentikan game kalo sudah diujung
    for (int j = 1; j < width; j++) {
        if (arena[3][j] == 7) {
            return false;
        }
    }
    return true;
}

int main () {
    srand(time(0));
    int dx = 0;
    int tetromino_y = 1; // inisialisasi posisi y tetromino
    int tetromino_x = rand() % 21 + 1; // inisialisasi posisi x tetromino
    int randomTetromino = rand() % 7; // random untuk bentuk tetromino
    int arena[height][width] = {0}; // inisialisasi buat arena nya

    while (gameOver(arena)) {
        if (canMove(arena, tetromino_x, tetromino_y, randomTetromino, dx)) {
            dx = 0;
            if (kbhit()) { // kontrol untuk geser tetromino nya kalau bisa buat biar tetromino nya nggak melebihi border
                char control = getch();

                if (control == 'a') { // geser kiri
                    if (tetromino_x != 1) { // mbatesi gerakan
                        dx = -1;
                    }
                } else if (control == 'd') { // geser kanan
                    if (tetromino_x + tetromino[randomTetromino][0].size() < width - 1) { // mbatasi gerakan;
                        dx = 1;
                    }
                }
            }

            summonTetromino(arena, tetromino_x, tetromino_y, randomTetromino);
            tetromino_y++; //nurunin tetromino

        } else {
            tetromino_y = 1; // ngembaliin posisi y tetromino ke atas karena udah jatuh ke bawah biar summon lagi
            tetromino_x = rand() % 21 + 1; // ngerandom ulang posisi x tetromino
            randomTetromino = rand() % 7; // ngerandom bentuk tetromino lagi
        }
        draw(arena); // gambar game
        clearLines(arena);
        Sleep(100); //ngelambatin loop
        system("cls"); // ngehapus yang udah tertulis
    }

    return 0;
}


