#include <iostream>
#include <conio.h>
#include <cstdlib>
#include <windows.h>
#include <time.h>
#include <vector>

using namespace std;

// Ukuran arena
const int width = 25;
const int height = 20;

int input_menu;

// Tetromino menggunakan vector
vector<vector<vector<int>>> tetromino = {
    {{1, 1, 1, 1}},                  // I
    {{1, 1}, {1, 1}},                // O
    {{0, 1, 0}, {1, 1, 1}},          // T
    {{1, 0, 0}, {1, 1, 1}},          // L
    {{0, 0, 1}, {1, 1, 1}},          // J
    {{0, 1, 1}, {1, 1, 0}},          // S
    {{1, 1, 0}, {0, 1, 1}}           // Z
};

// Fungsi untuk memutar tetromino ke kanan (90 derajat searah jarum jam)
vector<vector<int>> rotateMatrix(const vector<vector<int>> &matrix) {
    int n = matrix.size();
    int m = matrix[0].size();
    vector<vector<int>> rotated(m, vector<int>(n, 0));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            rotated[j][n - 1 - i] = matrix[i][j];
        }
    }
    return rotated;
}

// Fungsi untuk memeriksa apakah tetromino bisa diputar
bool canRotate(int arena[height][width], int position_x, int position_y, vector<vector<int>> &rotatedTetromino) {
    int newHeight = rotatedTetromino.size();
    int newWidth = rotatedTetromino[0].size();
    for (int i = 0; i < newHeight; i++) {
        for (int j = 0; j < newWidth; j++) {
            if (rotatedTetromino[i][j] == 1) {
                int newY = position_y + i;
                int newX = position_x + j;
                if (newY >= height || newX < 0 || newX >= width || arena[newY][newX] == 7 || arena[newY][newX] == 2) {
                    return false; // Tidak bisa diputar
                }
            }
        }
    }
    return true;
}

// Fungsi untuk menggambar tetromino di dalam arena
void summonTetromino(int arena[height][width], int position_x, int position_y, vector<vector<int>> &currentTetromino) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (i == 0 || j == 0 || i == height - 1 || j == width - 1) {
                arena[i][j] = 2; // Border
            } else {
                if (arena[i][j] != 7) {
                    arena[i][j] = 0; // Arena kosong
                }
            }
        }
    }
    for (int i = 0; i < currentTetromino.size(); i++) {
        for (int j = 0; j < currentTetromino[i].size(); j++) {
            if (currentTetromino[i][j] == 1) {
                arena[position_y + i][position_x + j] = currentTetromino[i][j];
            }
        }
    }
}

// Fungsi untuk menggambar arena
void draw(int arena[height][width], vector<vector<int>> &nextTetromino) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (arena[i][j] == 1 || arena[i][j] == 7) {
                cout << "@"; // Tetromino
            } else if (arena[i][j] == 2) {
                cout << "#"; // Border
            } else {
                cout << " "; // Arena kosong
            }
        }
        cout << endl;
    }
    cout << endl;

    // Gambar Tetris berikutnya di bagian kanan
    cout << "Next Tetromino: " << endl;
    for (int i = 0; i < nextTetromino.size(); i++) {
        for (int j = 0; j < nextTetromino[i].size(); j++) {
            if (nextTetromino[i][j] == 1) {
                cout << "@";
            } else {
                cout << " ";
            }
        }
        cout << endl;
    }
}

bool canMove(int arena[height][width], int position_x, int position_y, vector<vector<int>> &currentTetromino, int dx) {
    int newHeight = currentTetromino.size();
    int newWidth = currentTetromino[0].size();
    for (int i = 0; i < newHeight; i++) {
        for (int j = 0; j < newWidth; j++) {
            if (currentTetromino[i][j] == 1) {
                int newY = position_y + i;
                int newX = position_x + j;
                if (newY >= height || newX < 0 || newX >= width || arena[newY][newX] == 7 || arena[newY][newX] == 2) {
                    return false; // Tidak bisa bergerak
                }
            }
        }
    }
    return true;
}

bool canTurnLeft(int arena[height][width], int position_x, int position_y, vector<vector<int>> &currentTetromino, int dx) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int newHeight = currentTetromino.size();
            int newWidth = currentTetromino[0].size();
            for (int i = position_y; i <= position_y + newHeight + 1; i++) {
                if (arena[i][position_x - 1] == 7) {
                    return false;
                }
            }
        }
    }
    return true;
}

bool canTurnRight(int arena[height][width], int position_x, int position_y, vector<vector<int>> &currentTetromino, int dx) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int newHeight = currentTetromino.size();
            int newWidth = currentTetromino[0].size();
            for (int i = position_y; i <= position_y + newHeight + 1; i++) {
                if (arena[i][position_x + newWidth] == 7) {
                    return false;
                }
            }
        }
    }
    return true;
}

void clearLines(int arena[height][width]) {
    for (int i = 1; i < height - 1; i++) {
        bool fullLine = true;
        for (int j = 1; j < width - 1; j++) {
            if (arena[i][j] != 7) {
                fullLine = false;
                break;
            }
        }
        if (fullLine) {
            for (int j = 1; j < width - 1; j++) {
                arena[i][j] = 0;
            }
            for (int k = i; k > 1; k--) {
                for (int j = 1; j < width - 1; j++) {
                    arena[k][j] = arena[k - 1][j];
                }
            }
        }
    }
}

bool gameOver(int arena[height][width]) {
    for (int j = 1; j < width - 1; j++) {
        if (arena[1][j] == 7) {
            return true;
        }
    }
    return false;
}

//main menu (cuma sementara)
void main_menu(){
    cout << "=== Tetris ===" <<  endl;
    cout << "1. Play" << endl;
    cout << "2. leaderboard" << endl;
    cout << "0. Exit" << endl;
    cout << ">> "; cin >> input_menu;

    while (input_menu<0 || input_menu>2){
        cout << ">> "; cin >> input_menu;
    }
}

int main() {
    srand(time(0));
    int dx = 0;
    int tetromino_y = 1;
    int tetromino_x = rand() % 19 + 1;
    int randomTetromino = rand() % 7;
    vector<vector<int>> currentTetromino = tetromino[randomTetromino];
    vector<vector<int>> nextTetromino = tetromino[rand() % 7];
    int arena[height][width] = {0};


    main_menu();

    switch (input_menu){
    case 1:
        while (!gameOver(arena)) {
        if (canMove(arena, tetromino_x, tetromino_y, currentTetromino, dx)) {
            dx = 0;
            if (kbhit()) {
                char control = getch();
                if (control == 'a') {
                    if (tetromino_x > 1 && canTurnLeft(arena, tetromino_x, tetromino_y, currentTetromino, dx)) {
                        dx = -1;
                        tetromino_x += dx;
                    }
                } else if (control == 'd') {
                    if (tetromino_x + currentTetromino[0].size() < width - 1 && canTurnRight(arena, tetromino_x, tetromino_y, currentTetromino, dx)) {
                        dx = 1;
                        tetromino_x += dx;
                    }
                } else if (control == 'w') { // Rotasi
                    vector<vector<int>> rotatedTetromino = rotateMatrix(currentTetromino);
                    if (canRotate(arena, tetromino_x, tetromino_y, rotatedTetromino)) {
                        currentTetromino = rotatedTetromino;
                    }
                }
            }

            summonTetromino(arena, tetromino_x, tetromino_y, currentTetromino);
            tetromino_y++;
        } else {
            for (int i = 0; i < currentTetromino.size(); i++) {
                for (int j = 0; j < currentTetromino[i].size(); j++) {
                    if (currentTetromino[i][j] == 1) {
                        arena[tetromino_y + i - 1][tetromino_x + j] = 7;
                    }
                }
            }
            tetromino_y = 1;
            tetromino_x = rand() % 19 + 1;
            currentTetromino = nextTetromino; // Set the current Tetromino to the next
            nextTetromino = tetromino[rand() % 7]; // Generate a new next Tetromino

        draw(arena, nextTetromino);
        clearLines(arena);
        Sleep(150);
        system("cls"); // Bersihkan layar untuk menggambar ulang
    }

    cout << "Game Over!" << endl;

    break;
    }

}
return 0;
}
