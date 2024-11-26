#include <iostream>
#include <conio.h>
#include <cstdlib>
#include <windows.h>
#include <time.h>

using namespace std;

const int width = 30;
const int height = 20;

int tetrominoes[7][4][4] = {
    {{0, 0, 0, 0}, {0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}}, // I
    {{1, 1}, {1, 1}},                                         // O
    {{0, 1, 0}, {1, 1, 1}, {0, 0, 0}},                        // T
    {{1, 0, 0}, {1, 1, 1}, {0, 0, 0}},                        // L
    {{0, 0, 1}, {1, 1, 1}, {0, 0, 0}},                        // J
    {{0, 1, 1}, {1, 1, 0}, {0, 0, 0}},                        // S
    {{1, 1, 0}, {0, 1, 1}, {0, 0, 0}}                         // Z
};

void summonTetromino(int arena[height][width], int position_x, int position_y, int randomTetromino) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            arena[i][j] = 0;
        }
    }

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            arena[position_y + i][position_x + j] = tetrominoes[randomTetromino][i][j];
        }
    }

}

void draw(int arena[height][width]) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (arena[i][j] == 1) {
                cout << "@";
            } else {
                cout << ".";
            }
        }
        cout << endl;
    }
}


int main () {
    srand(time(0));
    int tetromino_y = 0;
    int tetromino_x = rand() % 27;
    int randomTetromino = rand() % 7;
    int arena [height][width];

    while (true) {
        if (tetromino_y < 19) {
            if (kbhit()) {
                char control = getch();

                if (control == 'a') {
                    if (tetromino_x > 0) {
                        tetromino_x -= 1;
                    }
                } else if (control == 'd') {
                    if (tetromino_x < 27) {
                        tetromino_x += 1;
                    }
                }
            }

            summonTetromino(arena, tetromino_x, tetromino_y, randomTetromino);
            tetromino_y++;
        } else {
            tetromino_y = 0;
            tetromino_y = 0;
            tetromino_x = rand() % 27;
            randomTetromino = rand() % 7;
        }
        draw(arena);
        Sleep(100);
        system("cls");
    }

    return 0;
}


