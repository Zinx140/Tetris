#include <iostream>
#include <conio.h>
#include <cstdlib>
#include <windows.h>
#include <time.h>
#include <vector>
// sudah jalan cuma menunya tak tahan dulu soalnya rusak >3
using namespace std;

// Ukuran arena
const int width = 25;
const int height = 20;

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

int candrop[width];


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
    const int boxHeight = 4;
    const int boxWidth = 4;
    const int horizontalSpacing = 5; // Spacing diantara arena dan "Next Tetromino"

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (arena[i][j] == 1 || arena[i][j] == 7) {
                cout << "@"; // Tetromino
            } else if (arena[i][j] == 2) {
                cout << "#"; // Border
            } else if (arena[i][j] == 4) {
                cout << ".";
            } else {
                cout << " "; // Arena kosong
            }
        }

        // Spacing diantara arena dan "Next Tetromino"
        cout << string(horizontalSpacing, ' ');

        // Print "Next Tetromino" label di atas kotak
        if (i == (height / 2 - boxHeight / 2 - 2)) {
            // Center
            int labelPadding = max(0, (boxWidth - 15) / 2);
            cout << string(labelPadding, ' ') << "Next Tetromino:";
        }

        // Print "Next Tetromino" box baris per baris
        if (i >= (height / 2 - boxHeight / 2) && i < (height / 2 + boxHeight / 2)) {
            int tetrominoRow = i - (height / 2 - boxHeight / 2); // Calculate current row untuk tetromino

            // left border
            cout << "#";

            if (tetrominoRow < nextTetromino.size()) {
                // Center tetromino dalam box 4x4
                int paddingLeft = (boxWidth - nextTetromino[tetrominoRow].size()) / 2;
                int paddingRight = boxWidth - nextTetromino[tetrominoRow].size() - paddingLeft;

                // Print left padding, tetromino contents, dan right padding
                cout << string(paddingLeft, ' ');
                for (int j = 0; j < nextTetromino[tetrominoRow].size(); j++) {
                    if (nextTetromino[tetrominoRow][j] == 1) {
                        cout << "@";
                    } else {
                        cout << " ";
                    }
                }
                cout << string(paddingRight, ' ');
            } else {
                cout << string(boxWidth, ' ');
            }

            // box's right border
            cout << "#";
        } else if (i == (height / 2 - boxHeight / 2 - 1) || i == (height / 2 + boxHeight / 2)) {
            cout << string(boxWidth + 2, '#'); // Full-width border
        }

        cout << endl;
    }

    // instructions
    cout << endl;
    cout << "'A' to move tetromino to the left" << endl;
    cout << "'D' to move tetromino to the right" << endl;
    cout << "'W' to rotate tetromino" << endl;
}

bool canMove(int arena[height][width], int position_x, int position_y, vector<vector<int>> &currentTetromino) {
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
            for (int i = position_y; i <= position_y + newHeight; i++) {
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
            for (int i = position_y; i <= position_y + newHeight; i++) {
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

int minn(int arr[], int n) {
    int low = arr[0];
    for (int i = 1; i < n; i++) {
        if (low > arr[i]) {
            low = arr[i];
        }
    }
    if (low == 0) {
        low = 19;
    }
    return low;
}

void hardDrop(int arena[height][width], vector<vector<int>> currentTetromino, int position_x, int position_y, bool drop, int &temp) {
    while (canMove(arena, position_x, position_y, currentTetromino)) {
        if (canMove(arena, position_x, position_y, currentTetromino)) {
            position_y++;
        }
    }

    for (int i = 0; i < currentTetromino.size(); i++) {
        for (int j = 0; j < currentTetromino[i].size(); j++) {
            if (currentTetromino[i][j] == 1) {
                if (arena[position_y + i - 1][position_x + j] != 1 && arena[position_y + i - 1][position_x + j] != 7 && arena[position_y + i - 1][position_x + j] != 2) {
                    arena[position_y + i - 1][position_x + j] = 4;
                }
            }
            if (drop) {
                if (arena[position_y + i - 1][position_x + j] == 4) {
                    arena[position_y + i - 1][position_x + j] = 7;
                }
                temp = position_y;
            }
        }
    }
}

int main() {
    srand(time(0));
    int dx = 0;
    int tetromino_y = 1;
    int tetromino_x = rand() % 19 + 1;
    int randomTetromino = rand() % 7;
    vector<vector<int>> currentTetromino = tetromino[randomTetromino]; // vector current tetronimo
    int nextTetrominoIndex = rand() % 7; // Index untuk tetronimo berikutnya
    vector<vector<int>> nextTetromino = tetromino[nextTetrominoIndex]; // vector next tetronimo
    int arena[height][width] = {0};
    bool flag = true;
    int tempY;

    while (!gameOver(arena)) {
        bool drop = false;
        if (canMove(arena, tetromino_x, tetromino_y, currentTetromino) && flag) {
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
                } else if (control == 's') {
                    drop = true;
                    flag = false;
                }
            }

            summonTetromino(arena, tetromino_x, tetromino_y, currentTetromino);
            tetromino_y++;
        } else {
            if (!flag) {
                for (int i = 0; i < currentTetromino.size(); i++) {
                    for (int j = 0; j < currentTetromino[0].size(); j++) {
                        if (currentTetromino[i][j] == 1) {
                            arena[tetromino_y + i - 1][tetromino_x + j] = 0;
                        }
                    }
                }
                tetromino_y = tempY;
            }

            for (int i = 0; i < currentTetromino.size(); i++) {
                for (int j = 0; j < currentTetromino[i].size(); j++) {
                    if (currentTetromino[i][j] == 1) {
                        arena[tetromino_y + i - 1][tetromino_x + j] = 7;
                    }
                }
            }


            tetromino_y = 1;
            tetromino_x = rand() % 19 + 1;
            currentTetromino = nextTetromino; // Set current tetromino to the next one
            nextTetrominoIndex = rand() % 7; // Ambil tetronimo berikutnya
            nextTetromino = tetromino[nextTetrominoIndex]; // Set next tetromino
            flag = true;
        }
        hardDrop(arena, currentTetromino, tetromino_x, tetromino_y, drop, tempY);
        draw(arena, nextTetromino);
        clearLines(arena);
        Sleep(150);
        system("cls"); // Bersihkan layar untuk menggambar ulang
    }

    cout << "Game Over!" << endl;
    return 0;
}
