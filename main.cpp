#include <iostream>
#include <conio.h>
#include <cstdlib>
#include <windows.h>
#include <time.h>
#include <vector>
#include <conio.h>
#include <windows.h>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <thread>
#include <chrono>

using namespace std;

// Ukuran arena
bool isBossMode = false;
const int width = 25;
const int height = 20;
int arenaColors[height][width] = {0};
int score = 0;
string playerName;
vector<string> playerNames;
vector<int> scores;
int mana;
string arenaFace[height];

bool isDamaged = false;
bool bossDamaged = false;
bool bossHealing = false;
bool bossUsingSkill = false;

bool skill_1_active = false;
bool skill_2_active = false;
bool skill_3_active = false;
bool doubleDamage = false;
bool shieldActive = false;

int cdUltimate = 0;
int cdSkill2 = 0;
int cdSkill1 = 0;

bool isPlayerSkillUsed = false;
int  playerSkillID = 0;

void setColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

string boss_default[] = {
    "        .-----------------------.     ",
    "      /                         \\    ",
    "     /     ____           ____   \\   ",
    " |    //----\\\\       //----\\\\  |  ",
    "     |    ||    ||       ||    ||  |  ",
    "     |    ||  * ||       || *  ||  |  ",
    "     |    ||____||_______||____||  |  ",
    " |    \\\\____//       \\\\____//  |  ",
    "     \\___         _____        ___/  ",
    "         /__               __//       ",
    "        |   |_____________|   |       ",
    "        |   || * * * * * ||   |       ",
    "        |   || * * * * * ||   |       ",
    "        |___||___________||___|       ",
    "         \\\\_|||      |||_//         ",
    "           \\\\||      |||//          ",
    "             '-|_________|-'          ",
    "               '---------'            ",
    "                                      ",
    "                                      ",
};

string boss_mouth[] = {
    "        .-----------------------.     ",
    "      /                         \\    ",
    "     /     ____           ____   \\   ",
    " |    //----\\\\       //----\\\\  |  ",
    "     |    ||    ||       ||    ||  |  ",
    "     |    ||  * ||       || *  ||  |  ",
    "     |    ||____||_______||____||  |  ",
    " |    \\\\____//       \\\\____//  |  ",
    "     \\___         _____        ___/  ",
    "         /__               __//       ",
    "        |   |_____________|   |       ",
    "        |   || * * * * * ||   |       ",
    "        |   ||           ||   |       ",
    "        |   ||           ||   |       ",
    "        |   || * * * * * ||   |       ",
    "        |___||___________||___|       ",
    "         \\\\_|||      |||_//         ",
    "           \\\\||      |||//          ",
    "             '-|_________|-'          ",
    "               '---------'            ",
};

string boss_look_right[] = {
    "        .-----------------------.     ",
    "      /                         \\    ",
    "     /     ____           ____   \\   ",
    " |    //----\\\\       //----\\\\  |  ",
    "     |    ||    ||       ||    ||  |  ",
    "     |    ||  > ||       ||  > ||  |  ",
    "     |    ||____||_______||____||  |  ",
    " |    \\\\____//       \\\\____//  |  ",
    "     \\___         _____        ___/  ",
    "         /__               __//       ",
    "        |   |_____________|   |       ",
    "        |   || * * * * * ||   |       ",
    "        |   || * * * * * ||   |       ",
    "        |___||___________||___|       ",
    "         \\\\_|||      |||_//         ",
    "           \\\\||      |||//          ",
    "             '-|_________|-'          ",
    "               '---------'            ",
    "                                      ",
    "                                      ",
};

string boss_look_left[] = {
    "        .-----------------------.     ",
    "      /                         \\    ",
    "     /     ____           ____   \\   ",
    " |    //----\\\\       //----\\\\  |  ",
    "     |    ||    ||       ||    ||  |  ",
    "     |    || <  ||       || <  ||  |  ",
    "     |    ||____||_______||____||  |  ",
    " |    \\\\____//       \\\\____//  |  ",
    "     \\___         _____        ___/  ",
    "         /__               __//       ",
    "        |   |_____________|   |       ",
    "        |   || * * * * * ||   |       ",
    "        |   || * * * * * ||   |       ",
    "        |___||___________||___|       ",
    "         \\\\_|||      |||_//         ",
    "           \\\\||      |||//          ",
    "             '-|_________|-'          ",
    "               '---------'            ",
    "                                      ",
    "                                      ",
};

string boss_damaged[] = {
    "        .-----------------------.     ",
    "      /                         \\    ",
    "     /     ____           ____   \\   ",
    " |    //----\\\\       //----\\\\  |  ",
    "     |    ||    ||       ||    ||  |  ",
    "     |    ||  x ||       || x  ||  |  ",
    "     |    ||____||_______||____||  |  ",
    " |    \\\\____//       \\\\____//  |  ",
    "     \\___         _____        ___/  ",
    "         /__               __//       ",
    "        |   |_____________|   |       ",
    "        |   || * * *   * ||   |       ",
    "        |   || *   * * * ||   |       ",
    "        |___||___________||___|       ",
    "         \\\\_|||      |||_//         ",
    "           \\\\||      |||//          ",
    "             '-|_________|-'          ",
    "               '---------'            ",
    "                                      ",
    "                                      ",
};

string boss_skill[] = {
    "        .-----------------------.     ",
    "      /                         \\    ",
    "     /     ____           ____   \\   ",
    " |    //----\\\\       //----\\\\  |  ",
    "     |    ||    ||       ||    ||  |  ",
    "     |    ||  * ||       || *  ||  |  ",
    "     |    ||____||_______||____||  |  ",
    " |    \\\\____//       \\\\____//  |  ",
    "     \\___         _____        ___/  ",
    "         /__               __//       ",
    "        |   |_____________|   |       ",
    "        |   || * * * * * ||   |       ",
    "        |   || * * * * * ||   |       ",
    "        |___||___________||___|       ",
    "         \\\\_|||      |||_//         ",
    "           \\\\||      |||//          ",
    "             '-|_________|-'          ",
    "               '---------'            ",
    "                                      ",
    "                                      ",
};

string boss_heal[] = {
    "        .-----------------------.     ",
    "      /                         \\    ",
    "     /     ____           ____   \\   ",
    " |    //----\\\\       //----\\\\  |  ",
    "     |    ||    ||       ||    ||  |  ",
    "     |    ||  * ||       || *  ||  |  ",
    "     |    ||____||_______||____||  |  ",
    " |    \\\\____//       \\\\____//  |  ",
    "     \\___         _____        ___/  ",
    "         /__               __//       ",
    "        |   |_____________|   |       ",
    "        |   || * * * * * ||   |       ",
    "        |   || * * * * * ||   |       ",
    "        |___||___________||___|       ",
    "         \\\\_|||      |||_//         ",
    "           \\\\||      |||//          ",
    "             '-|_________|-'          ",
    "               '---------'            ",
    "                                      ",
    "                                      ",
};

void fillFace(string arenaFace[], string bossFace[]) {
    for (int i = 0; i < height; i++) {
        arenaFace[i] = bossFace[i];
    }
}

void display_boss_random(string arenaFace[], bool bossDamaged, bool bossHealing, bool bossUsingSkill) {
        static auto lastUpdateTime = chrono::steady_clock::now();
        auto currentTime = chrono::steady_clock::now();
        auto elapsed = chrono::duration_cast<chrono::seconds>(currentTime - lastUpdateTime).count();
        int randomMovement = rand() % 2;
        system("cls");

        if (bossDamaged){
            fillFace(arenaFace, boss_damaged);
            bossDamaged = false;
        }

        if (bossHealing) {
        setColor(10); // Set color to green
        fillFace (arenaFace, boss_heal);
        setColor(7); // Reset color
        bossHealing = false;
        }

        if (bossUsingSkill) {
        setColor(12); // Set text color to red
        fillFace (arenaFace, boss_skill);
        setColor(7); // Reset color
    }

        if (elapsed >= 4){
//            if (randomMovement == 1){
//            cout << endl;
//            }
            int randomAction = rand() % 4;
            switch (randomAction) {
            case 0:
                fillFace(arenaFace, boss_look_left);
                break;
            case 1:
                fillFace(arenaFace, boss_look_right);
                break;
            case 2:
                fillFace(arenaFace, boss_mouth);
                break;
            case 3:
                fillFace(arenaFace, boss_default);
                break;
        }
        lastUpdateTime = currentTime; // Reset the timer
    }
}

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

void playSound(const string &filename) {
    if (!PlaySound(filename.c_str(), NULL, SND_FILENAME | SND_ASYNC)) {
        cerr << "Error playing sound: " << filename << endl;
    }
}

void playMusic(const string &filename) {
    if (!PlaySound(filename.c_str(), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP)) {
        cerr << "Error playing sound: " << filename << endl;
    }
}

void stopMusic() {
    PlaySound(NULL, NULL, 0);  // Stop any sound that is playing
}

// set warnanya ganti aja angkanya kalo mau ganti warna (1-15)
int tetrominoColors[7] = {
    9,  // I - Light-Blue
    12, // O - Light-Red
    13, // T - Magenta
    6,  // L - Light Red
    3,  // J - Aqua
    2,  // S - Green
    4   // Z - Red
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
void summonTetromino(int arena[height][width], int arenaColors[height][width], int position_x, int position_y, vector<vector<int>> &currentTetromino, int colorIndex) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (i == 0 || j == 0 || i == height - 1 || j == width - 1) {
                arena[i][j] = 2; // Border
            } else {
                if (arena[i][j] != 7) {
                    arena[i][j] = 0; // Arena kosong
                    arenaColors[i][j] = 0; // Reset color
                }
            }
        }
    }
    for (int i = 0; i < currentTetromino.size(); i++) {
        for (int j = 0; j < currentTetromino[i].size(); j++) {
            if (currentTetromino[i][j] == 1) {
                arena[position_y + i][position_x + j] = currentTetromino[i][j];
                arenaColors[position_y + i][position_x + j] = tetrominoColors[colorIndex];
            }
        }
    }
}

// notif buat game
void notification(string &message, const string &newMessage) {
    message = newMessage;
}

void maximizeConsole() {
    HWND consoleWindow = GetConsoleWindow(); // Get the console window handle
    if (consoleWindow == NULL) {
        std::cerr << "Failed to retrieve the console window handle." << std::endl;
        return;
    }

    // Maximize the console window
    ShowWindow(consoleWindow, SW_MAXIMIZE);

    // Resize the buffer size to avoid scrolling and to fill more of the screen
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
    GetConsoleScreenBufferInfo(hConsole, &bufferInfo);

    COORD newSize;
    newSize.X = bufferInfo.dwMaximumWindowSize.X; // Maximum width
    newSize.Y = 300;                             // Set height to a large value to avoid scrolling
    SetConsoleScreenBufferSize(hConsole, newSize);

    std::cout << "Console maximized to fill screen." << std::endl;
}

// Fungsi untuk menggambar arena
void draw(int arena[height][width], int arenaColors[height][width], vector<vector<int>> &nextTetromino, int currentTetrominoIndex, int nextTetrominoIndex, bool isBossMode, string messages, int bossHealth) {
    const int boxHeight = 4;
    const int boxWidth = 4;
    const int horizontalSpacing = 5;
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    int consoleWidth = 120;
    int arenaWidth = width;
    int horizontalPadding = isBossMode ? (consoleWidth - arenaWidth) / 2 : 0;
    const int instructionPadding = 26;
    display_boss_random(arenaFace, isDamaged, bossHealing, bossUsingSkill);

    if (isBossMode) {
        if (bossHealth < 0) {
            bossHealth = 0;
        }
        cout << setw(45) << "Mana : " << mana << endl;
        cout << setw(45) << "Boss Health : " << bossHealth << endl;
    }

    for (int i = 0; i < height; i++) {
        // Add horizontal padding for boss mode
        if (isBossMode == true) {
            cout << setw(60) << arenaFace[i];
        }

        for (int j = 0; j < width; j++) {
            if (arena[i][j] == 1 || arena[i][j] == 7) {
                setColor(arenaColors[i][j]); // ngeset warna tetromino
                cout << "\u2589";
                setColor(7); // Reset color to default
            } else if (arena[i][j] == 2) {
                cout << "#";
            } else if (arena[i][j] == 4) {
                setColor(arenaColors[i][j]);
                cout << ".";
                setColor(7);
            } else {
                cout << " ";
            }
        }

        // Draw the "Next Tetromino" box
            cout << string(horizontalSpacing, ' ');
             if(i == 1) {
            cout << "Score : " << score;
            }
            if (i == (height / 2 - boxHeight / 2 - 2)) {
                cout << "Next Tetromino:";
            }
            if (i >= (height / 2 - boxHeight / 2) && i < (height / 2 + boxHeight / 2)) {
                int tetrominoRow = i - (height / 2 - boxHeight / 2);
                cout << "#";

                if (tetrominoRow < nextTetromino.size()) {
                    int paddingLeft = (boxWidth - nextTetromino[tetrominoRow].size()) / 2;
                    int paddingRight = boxWidth - nextTetromino[tetrominoRow].size() - paddingLeft;
                    cout << string(paddingLeft, ' ');

                    for (int j = 0; j < nextTetromino[tetrominoRow].size(); j++) {
                        if (nextTetromino[tetrominoRow][j] == 1) {
                            setColor(tetrominoColors[nextTetrominoIndex]); // Color for next tetromino
                            cout << "\u2589";
                            setColor(7); // Reset
                        } else {
                            cout << " ";
                        }
                    }
                    cout << string(paddingRight, ' ');
                } else {
                    cout << string(boxWidth, ' ');
                }
                cout << "#";
            } else if (i == (height / 2 - boxHeight / 2 - 1) || i == (height / 2 + boxHeight / 2)) {
                cout << string(boxWidth + 2, '#');
            }
        cout << endl;
    }

    // Instructions

    if (isBossMode == true){
    cout << messages << endl;
    cout << string(horizontalPadding + instructionPadding, ' ') << "'A' to Move tetromino to the left" << endl;
    cout << string(horizontalPadding + instructionPadding, ' ') << "'D' to Move tetromino to the right" << endl;
    cout << string(horizontalPadding + instructionPadding, ' ') << "'W' to Rotate tetromino" << endl;
    cout << string(horizontalPadding + instructionPadding, ' ') << "'S' to Drop the tetromino" << endl;
    } else if (isBossMode == false) {
    cout << "'A' to Move tetromino to the left" << endl;
    cout << "'D' to Move tetromino to the right" << endl;
    cout << "'W' to Rotate tetromino" << endl;
    cout << "'S' to Drop the tetromino" << endl;
    }
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
    int newHeight = currentTetromino.size();
    int newWidth = currentTetromino[0].size();
    for (int i = 0; i < newHeight; i++) {
        for (int j = 0; j < newWidth; j++) {
            if (currentTetromino[i][j] == 1) {
                if (arena[position_y + i][position_x + j - 1] == 7) {
                    return false;
                }
            }
        }
    }
    return true;
}

bool canTurnRight(int arena[height][width], int position_x, int position_y, vector<vector<int>> &currentTetromino, int dx) {
    int newHeight = currentTetromino.size();
    int newWidth = currentTetromino[0].size();
    for (int i = 0; i < newHeight; i++) {
        for (int j = 0; j < newWidth; j++) {
            if (currentTetromino[i][j] == 1) {
                if (arena[position_y + i][position_x + j + 1] == 7) {
                    return false;
                }
            }
        }
    }
    return true;
}

void fillBottomRow(int arena[height][width], int tetromino_y, int tetromino_x, int randomTetromino) {
    int i = height - 2;

    for (int j = 1; j < width - 1; j++) {
        arena[i][j] = 7;
    }
}

void clearLines(int arena[height][width], int &bossHealth, int arenaColors[height][width], bool &bossDamaged) {
     for (int i = 1; i < height - 1; i++) {
         bool fullLine = true;
         for (int j = 1; j < width - 1; j++) {
             if (arena[i][j] != 7) {
                 fullLine = false;
                 break;
             }
         }
         if (fullLine) {
              if(doubleDamage == true) {
                  score += 200;
                  bossHealth -= 200;
                  doubleDamage = false;
              } else {
                  score += 100;
                  bossHealth -= 100;
              }
              mana++;
              bossDamaged = true;
             for (int j = 1; j < width - 1; j++) {
                 arena[i][j] = 0;
             }
             for (int k = i; k > 1; k--) {
                 for (int j = 1; j < width - 1; j++) {
                     arena[k][j] = arena[k - 1][j];
                     arenaColors[k][j] = arenaColors[k - 1][j];

                 }
             }
         }
     }
 }

void clearLines(int arena[height][width], int arenaColors[height][width]) {
    for (int i = 1; i < height - 1; i++) {
        bool fullLine = true;
        for (int j = 1; j < width - 1; j++) {
            if (arena[i][j] != 7) {
                fullLine = false;
                break;
            }
        }
        if (fullLine) {
            score += 100;
            for (int j = 1; j < width - 1; j++) {
                arena[i][j] = 0;
            }
            for (int k = i; k > 1; k--) {
                for (int j = 1; j < width - 1; j++) {
                    arena[k][j] = arena[k - 1][j];
                    arenaColors[k][j] = arenaColors[k - 1][j];
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

void hardDrop(int arena[height][width], vector<vector<int>> currentTetromino, int position_x, int position_y, bool drop, int &temp, int randomTetromino) {
    int shadowColor = tetrominoColors[randomTetromino];
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
                    arenaColors[position_y + i - 1][position_x + j] = shadowColor; // Set color for shadow
                }
            }
            if (drop) {
                temp = position_y;
            }
        }
    }
}

void moveCursorTo(int x, int y) {
    COORD coord = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void clearBottomPart(int start_line, int console_width, int num_lines) {
    string blank_line(console_width, ' ');
    for (int i = 0; i < num_lines; ++i) {
        moveCursorTo(0, start_line + i);
        cout << blank_line;
    }
    moveCursorTo(0, start_line); // Reset cursor to start of cleared section
}

void draw_menu_selection(const int console_width, const int menu_width, const string options[], int menu_size, int control_x, int start_line) {
    string horizontal_padding((console_width - menu_width) / 2, ' ');

    for (int i = 0; i < menu_size; i++) {
        moveCursorTo(0, start_line + i);
        cout << horizontal_padding << "#               ";
        if (control_x == i) {
            cout << "> " << options[i];
        } else {
            cout << "  " << options[i];
        }
        cout << string(41 - options[i].length(), ' ') << "#" << endl;
    }

    // Add bottom padding and instructions
    moveCursorTo(0, start_line + menu_size);
    cout << horizontal_padding << "#                                                          #" << endl;
    moveCursorTo(0, start_line + menu_size + 1);
    cout << horizontal_padding << "#       Press W or S to move, press ENTER to choose        #" << endl;
    moveCursorTo(0, start_line + menu_size + 2);
    cout << horizontal_padding << "############################################################" << endl;
}

int difficulty_menu(const int console_width, const int menu_width, int start_line) {
    int control_x = 0;
    string arr_difficulty[3] = {"Easy", "Medium", "Hard"};
    char input;
    const int menu_size = 3;

    string horizontal_padding((console_width - menu_width) / 2, ' ');
    system("cls");
    cout << horizontal_padding << "############################################################" << endl;
    cout << horizontal_padding << "#                                                          #" << endl;
    cout << horizontal_padding << "#        @@@@@@  @@@@@  @@@@@@  @@@@  @@  @@@@@@@          #" << endl;
    cout << horizontal_padding << "#        @@@@@@  @      @@@@@@  @  @  @@  @@               #" << endl;
    cout << horizontal_padding << "#          @@    @@@@@    @@    @@@@  @@  @@@@@@@          #" << endl;
    cout << horizontal_padding << "#          @@    @@@@@    @@    @@    @@  @@@@@@@          #" << endl;
    cout << horizontal_padding << "#          @@    @        @@    @ @   @@       @@          #" << endl;
    cout << horizontal_padding << "#          @@    @@@@@    @@    @  @  @@  @@@@@@@          #" << endl;
    cout << horizontal_padding << "#                                                          #" << endl;
    cout << horizontal_padding << "#                                                          #" << endl;
    cout << horizontal_padding << "#                                                          #" << endl;

    start_line = 11;

    do {
        clearBottomPart(start_line, console_width, menu_size + 3);
        draw_menu_selection(console_width, menu_width, arr_difficulty, menu_size, control_x, start_line);

        input = _getch();
        if (input == 'w' || input == 'W') {
            if (control_x > 0) {
                control_x--; // Move up
                playSound("sfx/move.wav");
            }
        } else if (input == 's' || input == 'S') {
            if (control_x < menu_size - 1) {
                control_x++; // Move down
                playSound("sfx/move.wav");
            }
        }

    } while (input != '\r'); // Enter key
    playSound("sfx/select.wav");

    return control_x;
}

int play_submenu(const int console_width, const int menu_width, int& difficulty, int start_line) {
    int control_x = 0;
    string arr_submenu[3] = {"Normal Mode", "Boss Mode", "Back to Main Menu"};
    char input;
    const int menu_size = 3;  // JANGAN DI GANTI NANTI ERROR

    string horizontal_padding((console_width - menu_width) / 2, ' ');
    system("cls");
    cout << horizontal_padding << "############################################################" << endl;
    cout << horizontal_padding << "#                                                          #" << endl;
    cout << horizontal_padding << "#        @@@@@@  @@@@@  @@@@@@  @@@@  @@  @@@@@@@          #" << endl;
    cout << horizontal_padding << "#        @@@@@@  @      @@@@@@  @  @  @@  @@               #" << endl;
    cout << horizontal_padding << "#          @@    @@@@@    @@    @@@@  @@  @@@@@@@          #" << endl;
    cout << horizontal_padding << "#          @@    @@@@@    @@    @@    @@  @@@@@@@          #" << endl;
    cout << horizontal_padding << "#          @@    @        @@    @ @   @@       @@          #" << endl;
    cout << horizontal_padding << "#          @@    @@@@@    @@    @  @  @@  @@@@@@@          #" << endl;
    cout << horizontal_padding << "#                                                          #" << endl;
    cout << horizontal_padding << "#                                                          #" << endl;
    cout << horizontal_padding << "#                                                          #" << endl;

    start_line = 11;

    do {
        clearBottomPart(start_line, console_width, menu_size + 3);
        draw_menu_selection(console_width, menu_width, arr_submenu, menu_size, control_x, start_line);

        input = _getch();
        if (input == 'w' || input == 'W') {
            if (control_x > 0) {
                control_x--; // Move up
                playSound("sfx/move.wav");
            }
        } else if (input == 's' || input == 'S') {
            if (control_x < menu_size - 1) {
                control_x++; // Move down
                playSound("sfx/move.wav");
            }
        }

    } while (input != '\r');
    playSound("sfx/select.wav");

    if (control_x == 0){
        return 0;
    }

    if (control_x == 1) {
        return 1;
    }

    return -1; // kembali ke main menu
}

int main_menu(const int console_width, const int menu_width) {
    string arr_mainmenu[3] = {"Play", "Leaderboard", "Exit"};
    const int menu_size = 3;
    int control_x = 0;
    int difficulty = -1;
    // Draw static top part
    string horizontal_padding((console_width - menu_width) / 2, ' ');
    system("cls");
    cout << horizontal_padding << "############################################################" << endl;
    cout << horizontal_padding << "#                                                          #" << endl;
    cout << horizontal_padding << "#        @@@@@@  @@@@@  @@@@@@  @@@@  @@  @@@@@@@          #" << endl;
    cout << horizontal_padding << "#        @@@@@@  @      @@@@@@  @  @  @@  @@               #" << endl;
    cout << horizontal_padding << "#          @@    @@@@@    @@    @@@@  @@  @@@@@@@          #" << endl;
    cout << horizontal_padding << "#          @@    @@@@@    @@    @@    @@  @@@@@@@          #" << endl;
    cout << horizontal_padding << "#          @@    @        @@    @ @   @@       @@          #" << endl;
    cout << horizontal_padding << "#          @@    @@@@@    @@    @  @  @@  @@@@@@@          #" << endl;
    cout << horizontal_padding << "#                                                          #" << endl;
    cout << horizontal_padding << "#                                                          #" << endl;
    cout << horizontal_padding << "#                                                          #" << endl;

    int start_line = 11; // Line where menu starts
    while (true) {
        draw_menu_selection(console_width, menu_width, arr_mainmenu, menu_size, control_x, start_line);

        char input = _getch();
        if (input == 'w' || input == 'W') {
            if (control_x > 0) {
                control_x--; // Move up
                playSound("sfx/move.wav");
            }
        } else if (input == 's' || input == 'S') {
            if (control_x < menu_size - 1) {
                control_x++; // Move down
                playSound("sfx/move.wav");
            }
        } else if (input == '\r') { // ENTER key
            playSound("sfx/select.wav");
            if (control_x == 0) {
                return 0;
            } else if (control_x == 1) {
                return 1;
            } else if (control_x == 2) {
                system("cls");
                cout << "Exiting..." << endl;
                return 2;
                break;
            }
        }
    }
}

void loadScores() {
    ifstream infile("leaderboard.txt");
    string name;
    int score;

    if (!infile) {
        cout << "Failed to open leaderboard file." << endl;
        return;
    }

    while (infile >> name >> score) {
        playerNames.push_back(name);
        scores.push_back(score);
    }
    infile.close();
}

void saveScores() {
    ofstream outfile("leaderboard.txt");

    for (int i = 0; i < playerNames.size(); ++i) {
        outfile << playerNames[i] << " " << scores[i] << endl;
    }
    outfile.close();
}

void saveScore(int score, const string& playerName) {
    bool playerExists = false;
    for (int i = 0; i < playerNames.size(); ++i) {
        if (playerNames[i] == playerName) {
            scores[i] += score;
            playerExists = true;
            break;
        }
    }

    if (!playerExists) {
        playerNames.push_back(playerName);
        scores.push_back(score);
    }

    for (size_t i = 0; i < scores.size(); ++i) {
        for (size_t j = i + 1; j < scores.size(); ++j) {
            if (scores[i] < scores[j]) {
                swap(scores[i], scores[j]);
                swap(playerNames[i], playerNames[j]);
            }
        }
    }

    saveScores();
}

void showLeaderboard() {
    ifstream inFile("leaderboard.txt");
    string name;
    int score;
    int count = 1;
    string topName;
    int topScore = 0;

    if (!inFile) {
        cout << "Failed to open leaderboard file." << endl;
        Sleep(2000);
        return;
    }

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int console_width, console_height;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    console_width = csbi.dwSize.X;
    console_height = csbi.dwSize.Y;

    const int menu_width = 60;

    system("cls");

    playMusic("music/leaderboard.wav");

    string horizontal_padding((console_width - menu_width) / 2, ' ');

    cout << horizontal_padding << "=========================================================" << endl;
    cout << horizontal_padding << "| o                                                   o |" << endl;
    cout << horizontal_padding << "|      @      @@@     @@@     @@@      @@@     @@@      |" << endl;
    cout << horizontal_padding << "|      @      @@      @@@     @  @     @@      @@@      |" << endl;
    cout << horizontal_padding << "|      @@@    @@@     @ @     @@@      @@@     @  @     |" << endl;
    cout << horizontal_padding << "| o                                                   o |" << endl;
    cout << horizontal_padding << "|          @@@@    @@@     @@@     @@@      @@@         |" << endl;
    cout << horizontal_padding << "|          @ @     @ @     @@@     @@@      @  @        |" << endl;
    cout << horizontal_padding << "|          @@@@    @@@     @ @     @  @     @@@         |" << endl;
    cout << horizontal_padding << "| o                                                   o |" << endl;
    cout << horizontal_padding << "=========================================================" << endl;

    cout << endl;

    cout << horizontal_padding <<"+=======================================================+" << endl;
    cout << horizontal_padding <<"|                 TOP 5 HIGHEST SCORERS                 |" << endl;
    cout << horizontal_padding <<"+=======================================================+" << endl;
    cout << horizontal_padding <<"| " << left << setw(4) << "No"
         << "| " << left << setw(15) << "Name"
         << "| " << right << setw(10) << "Score" << "   |                 |" << endl;
    cout << horizontal_padding <<"+=======================================================+" << endl;

    while (inFile >> name >> score && count <= 5) {
        cout << horizontal_padding <<"| " << left << setw(4) << count
             << "| " << left << setw(15) << name
             << "| " << right << setw(10) << score << "   |                 |" << endl;

        if (score > topScore) {
            topScore = score;
            topName = name;
        }

        count++;
    }

    while (count <= 5) {
        cout << horizontal_padding <<"| " << left << setw(4) << count
             << "| " << left << setw(15) << "-"
             << "| " << right << setw(10) << "0" << "   |                 |" << endl;
        count++;
    }

    cout << horizontal_padding <<"+=======================================================+" << endl;

    if (!topName.empty()) {
        cout << horizontal_padding <<"| " << left << setw(9) << "              Congratulation to " << setw(16) << topName << "      |" << endl;
        cout << horizontal_padding <<"| " << left << setw(9) << "              Your highest score is " << setw(16) << topScore << "  |"<< endl;
    }

    cout << horizontal_padding << "+=======================================================+" << endl;
    cout << horizontal_padding <<"|          Press any key to return to menu...           |" << endl;
    cout << horizontal_padding <<"+=======================================================+" << endl;

    inFile.close();
    getch();
}

bool canRandom(int randomTetromino, int arena[height][width], int positionX, int positionY) {
    vector<vector<int>> currentTetromino = tetromino[randomTetromino];
    if (canMove(arena, positionX, positionY, currentTetromino)) {
        return true;
    } else {
        return false;
    }
}

string intro() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int console_width, console_height;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    console_width = csbi.dwSize.X;
    console_height = csbi.dwSize.Y;

    string names[] = {"Han", "Gilbert", "Reynard"};
    vector<vector<string>> blocks = {
        {"#####", "#\u2589\u2589\u2589#", "# \u2589 #", "# \u2589 #", "# \u2589 #", "# \u2589 #", "#####"},
        {"#####", "#\u2589\u2589\u2589#", "#\u2589  #", "#\u2589\u2589\u2589#", "#\u2589  #", "#\u2589\u2589\u2589#", "#####"},
        {"#####", "#\u2589\u2589\u2589#", "# \u2589 #", "# \u2589 #", "# \u2589 #", "# \u2589 #", "#####"},
        {"#####", "#\u2589\u2589\u2589#", "#\u2589 \u2589#", "#\u2589\u2589\u2589#", "#\u2589\u2589 #", "#\u2589 \u2589#", "#####"},
        {"#####", "# \u2589 # ", "# \u2589 #", "# \u2589 #", "# \u2589 #", "# \u2589 #", "#####"},
        {"#####", "#\u2589\u2589\u2589#", "#\u2589  #", "#\u2589\u2589\u2589#", "#  \u2589#", "#\u2589\u2589\u2589#", "#####" }
    };

    vector<string> nameblock = {
        "############################################################",
        "#                                                          #",
        "#        @@@@@@  @@@@@  @@@@@@  @@@@  @@  @@@@@@@          #",
        "#        @@@@@@  @      @@@@@@  @  @  @@  @@               #",
        "#          @@    @@@@@    @@    @@@@  @@  @@@@@@@          #",
        "#          @@    @@@@@    @@    @@    @@  @@@@@@@          #",
        "#          @@    @        @@    @ @   @@       @@          #",
        "#          @@    @@@@@    @@    @  @  @@  @@@@@@@          #",
        "#                                                          #",
        "#                                                          #",
        "#                                                          #",
        "#                                                          #",
        "#                                                          #",
        "#                     ENTER YOUR NAME:                     #",
        "############################################################"
    };

    const int menu_width = 40;
    cout << "Press 'ENTER' to skip the intro" << endl;
    string horizontal_padding((console_width - menu_width) / 2, ' ');

    string playerName = "";

    playMusic("music/intro.wav");
    cout << endl;
    cout << horizontal_padding << "Group project made by:" << endl;
    cout << endl;

    for (const string& name : names) {
        if (_kbhit() && _getch() == 13) { // Check if Enter is pressed
            goto skip_intro;
        }
        cout << horizontal_padding << name << endl;
        this_thread::sleep_for(chrono::seconds(1)); // 1 second delay
    }

    // Display block sequences
    for (const auto& block_set : blocks) {
        if (_kbhit() && _getch() == 13) { // Check if Enter is pressed
            goto skip_intro;
        }
        system("cls");
        cout << "Press 'ENTER' to skip the intro" << endl;
        cout << endl;
        for (const string& block : block_set) {
            cout << horizontal_padding << block << endl;
            this_thread::sleep_for(chrono::milliseconds(500)); // 0.5 second delay
        }
    }

skip_intro: // Skip to insert name
    system("cls");

    while (true) {
        system("cls");
        for (size_t i = 0; i < nameblock.size(); i++) {
            if (i == 13) {
                string nameLine = "#                 ENTER YOUR NAME: " + playerName + "_                        #";
                if (nameLine.size() > nameblock[13].size()) {
                    nameLine = nameLine.substr(0, nameblock[13].size() - 1) + "#"; // Truncate if too long
                }
                cout << horizontal_padding << nameLine << endl;
            } else {
                cout << horizontal_padding << nameblock[i] << endl;
            }
        }

        char ch = _getch();
        if (ch == 13) { // Enter key
            break;
        } else if (ch == 8) { // Backspace
            if (!playerName.empty()) {
                playerName.pop_back();
            }
        } else if (isprint(ch)) { // Printable characters
            if (playerName.size() < 20) { // Limit nama biar ga terlalu panjang
                playerName += ch;
            }
        }
    }

    // Display final box with the entered name
    system("cls");
    for (size_t i = 0; i < nameblock.size(); i++) {
        if (i == 13) {
            string nameLine = "#                 ENTER YOUR NAME: " + playerName + "_                        #";
            if (nameLine.size() > nameblock[13].size()) {
                nameLine = nameLine.substr(0, nameblock[13].size() - 1) + "#";
            }
            cout << horizontal_padding << nameLine << endl;
        } else {
            cout << horizontal_padding << nameblock[i] << endl;
        }
    }

    // Indicate game is starting
    cout << "\nGame starting..." << endl;
    return playerName;
    Sleep(2000);
}

void intro_boss() {
    string line_1[11] = {"As", "you", "clear", "your", "last Tetrimino,", "the ground", "shakes", "and a giant", "skull head", "rises before you.", "A voice booms:"};
    string line_2[4] = {"WELCOME,", "PLAYER.", "YOU'VE", "GONE TOO FAR."};
    string line_3[9] = {"Meet TETROFENDER-X,", "once a guardian of Tetriminos,", "now", "a deranged machine", "obsessed with eliminating chaos.", "Its arsenal of tools,", "rotating walls,", "and super-powered blocks are ready to crush you."};
    string line_4[5] = {"MY", "BLOCKS", "WILL", "DESTROY", "YOU!"};
    string line_5[4] = {"Can the Player", "defeat the mighty Tetrofender-X", "and", "restore order to the Tetris world?"};

    bool skip = false;  // Flag to detect if Enter is pressed

    playMusic("music/intro.wav");

    // print a line with delay or all at once if Enter is pressed
    auto printLine = [&](const string arr[], int size, int delay_ms) {
        for (int i = 0; i < size; i++) {
            cout << arr[i] << " ";
            if (!skip && _kbhit() && (getchar() == '\n')) {
                skip = true;  // Set the skip flag when Enter is pressed
            }
            if (!skip) {
                this_thread::sleep_for(chrono::milliseconds(delay_ms));  // Delay only if skip is false
            }
        }
        cout << endl;
    };

    // Print intro lines
    printLine(line_1, 11, 500);
    printLine(line_2, 4, 1000);
    printLine(line_3, 9, 525);
    printLine(line_4, 5, 1000);
    printLine(line_5, 4, 500);

    cout << endl << "The boss fight is starting..." << endl;
}

void runNormalGame(const string& playerName) {
    int dx = 0;
    int tetromino_y = 1;
    int tetromino_x = rand() % 19 + 1;
    int randomTetromino = rand() % 7;
    vector<vector<int>> currentTetromino = tetromino[randomTetromino]; // vector current tetronimo
    int nextTetrominoIndex = rand() % 7; // Index untuk tetronimo berikutnya
    vector<vector<int>> nextTetromino = tetromino[nextTetrominoIndex]; // vector next tetronimo
    int arena[height][width] = {0};
    bool flag = true;
    int tempY, bossHealth, counter = 0;
    score = 0;
    string bossSkillMessage="";
    system("cls");

    stopMusic();

    Sleep(1500);

    system("cls");

    playMusic("music/MainTheme.wav");

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
                } else if (int(control) == 27) {
                    break;
                }

            }

            summonTetromino(arena, arenaColors, tetromino_x, tetromino_y, currentTetromino, randomTetromino);

            if (counter % 4 == 0) {
                tetromino_y++;
            }
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
                        arenaColors[tetromino_y + i - 1][tetromino_x + j] = tetrominoColors[randomTetromino];
                    }
                }
            }

            tetromino_y = 1;
            tetromino_x = rand() % 19 + 1;
            randomTetromino = nextTetrominoIndex; // Update the current Tetromino index
            currentTetromino = nextTetromino;
            nextTetrominoIndex = rand() % 7;     // Generate tindex tetronimo berikutnya
            nextTetromino = tetromino[nextTetrominoIndex]; // Set next Tetromino
            flag = true;
        }

        counter++;
        hardDrop(arena, currentTetromino, tetromino_x, tetromino_y, drop, tempY, randomTetromino);
        draw(arena, arenaColors, nextTetromino, randomTetromino, nextTetrominoIndex, false, bossSkillMessage, bossHealth);
        clearLines(arena, arenaColors);
        Sleep(100);
        system("cls");
    }

    cout << "Game Over!" << endl;
    saveScore(score, playerName);
    Sleep(2000);
}

void activatePlayerSkills(char control, int arena[height][width], int &bossHealth, int tetrominoX, int tetrominoY, int randomTetromino) {
    if (control == '1' && mana >= 1 && cdSkill1 == 0) {
            doubleDamage = true;
            cdSkill1 = 60;
            mana--;
    } else if (control == '2' && mana >= 3 && cdSkill2 == 0) {
        fillBottomRow(arena, tetrominoY, tetrominoX, randomTetromino);
        clearLines(arena, bossHealth, arenaColors, bossDamaged);
        cdSkill2 = 5;
        mana -= 4;
    } else if (control == '3' && mana >= 5 && cdUltimate == 0) {
            shieldActive = true;
            cdUltimate = 120;
            mana -= 5;
    }
}

void displaySkillStatus(bool skill_1_active, bool skill_2_active, bool skill_3_active) {
    const int instructionPadding = 73;
    int consoleWidth = 120;
    int arenaWidth = width;
    int horizontalPadding = isBossMode ? (consoleWidth - arenaWidth) / 2 : 0;
    // Cek skill 1
    if (skill_1_active && cdSkill1 == 0) {
        cout << string(horizontalPadding + instructionPadding, ' ') << "Skill 1 (Double Damage) | mana cost 1: Active" << endl;
    } else if (cdSkill1 > 0) {
        cout << string(horizontalPadding + instructionPadding, ' ') << "Skill 1 (Double Damage) | mana cost 1: Inactive - Coldown : " << cdSkill1 << endl;
    } else {
        cout << string(horizontalPadding + instructionPadding, ' ') << "Skill 1 (Double Damage) | mana cost 1: Inactive" << endl;
    }

    //Cek Skill 2
    if (skill_2_active) {
        cout << string(horizontalPadding + instructionPadding, ' ') << "Skill 2 (Destroy Lower Lines) | mana cost 3: Active" << endl;
    } else if (cdSkill2 > 0) {
        cout << string(horizontalPadding + instructionPadding, ' ') << "Skill 2 (Destroy Lower Lines) | mana cost 3: Inactive - Coldown : " << cdSkill2 << endl;
    } else {
        cout << string(horizontalPadding + instructionPadding, ' ') << "Skill 2 (Destroy Lower Lines) | mana cost 3: Inactive" << endl;
    }

    // Cek skill 3
    if (skill_3_active && cdUltimate == 0) {
        cout << string(horizontalPadding + instructionPadding, ' ') << "Skill 3 (Shield/Defense) | mana cost 5: Active" << endl;
    } else if (cdUltimate > 0) {
        cout << string(horizontalPadding + instructionPadding, ' ') << "Skill 3 (Shield/Defense) | mana cost 5: Active" << " - Coldown : " << cdUltimate << endl;
    } else {
        cout << string(horizontalPadding + instructionPadding, ' ') << "Skill 3 (Shield/Defense) | mana cost 5: Inactive" << endl;
    }
}

void runGame(int bossHealth, int interval) {
    int dx = 0;
    int tetromino_y = 1;
    int tetromino_x = rand() % 19 + 1;
    int randomTetromino = rand() % 7;
    vector<vector<int>> currentTetromino = tetromino[randomTetromino]; // vector current tetronimo
    int nextTetrominoIndex = rand() % 7; // Index untuk tetronimo berikutnya
    vector<vector<int>> nextTetromino = tetromino[nextTetrominoIndex]; // vector next tetronimo
    int arena[height][width] = {0};
    bool flag = true, muteMove = true;
    int limitHealth = bossHealth;
    string skillName;
    int tempY, randomSkill, counter = 1; // counter buat ngitung detik atau ms
    // jadi interval boss nyerang itu nanti rumus nya counter % interval
    score = 0;
    string bossSkillMessage = "";
    string playerSkillMessage = "";
    bool bossDamaged = false;
    bool bossHalfHealthMusicPlayed = false; // Track if boss2.wav has been played
    bool bossRageMusicPlayed = false;      // Track if bossberserk.wav has been played
    system("cls");

    intro_boss();

    stopMusic();

    Sleep(1500);

    system("cls");

    playMusic("music/boss1.wav");

    while (!gameOver(arena) && bossHealth > 0) {

        if (bossHealth <= limitHealth / 2 && !bossHalfHealthMusicPlayed) {
            playMusic("music/boss2.wav");
            bossHalfHealthMusicPlayed = true;
        }

        bool drop = false;
        if (canMove(arena, tetromino_x, tetromino_y, currentTetromino) && flag) {
            dx = 0;
            if (kbhit()) {
                char control = getch();

                if (muteMove) {

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
                    } else if(control == '1' || control == '2' || control == '3') {
                        activatePlayerSkills(control, arena, bossHealth, tetromino_x, tetromino_y, randomTetromino);
                    } else if (int(control) == 27) {
                        break;
                    }

                } else {
                    if (control == 's') {
                        drop = true;
                        flag = false;
                    }
                }
            }

            summonTetromino(arena, arenaColors, tetromino_x, tetromino_y, currentTetromino, randomTetromino);

            if (counter % 4 == 0) {
                tetromino_y++;
            }

            if (counter % interval == 0) { // interval kapan boss nyerang
                    if (randomSkill == 1 || randomSkill == 2 || randomSkill == 3){
                        bossUsingSkill = true;
                    }
                int changeTetromino = rand() % 7;
                muteMove = true;
                do {
                    randomSkill = rand() % 4 + 1;
                } while((randomSkill == 3 && (bossHealth + 50 >= limitHealth)) || (randomSkill == 1 && !canRandom(changeTetromino, arena, tetromino_x, tetromino_y)));

                if (shieldActive) {
                      bossSkillMessage = "Boss skill blocked by Shield!";
                } else {
                    if (randomSkill == 1) { // Skill1: ngubah bentuk tetromino
                        currentTetromino = tetromino[changeTetromino];
                        bossSkillMessage = "Boss used a Skill: Change Tetromino!";
                    } else if (randomSkill == 2) { // Skill2: Langsung jatuhin Tetromino
                        drop = true;
                        flag = false;
                        bossSkillMessage = "Boss used a Skill: Force Drop!";
                    } else if (randomSkill == 3) { // Skill3: Nambah darah boss
                        bossHealth += 50;
                        bossHealing = true;
                        bossSkillMessage = "Boss used a Skill: Healing!";
                    } else { // Skill4: blokir player move
                        muteMove = false;
                        bossSkillMessage = "Boss used a Skill: Movement Block!";
                    }
                }
            }

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
                        arenaColors[tetromino_y + i - 1][tetromino_x + j] = tetrominoColors[randomTetromino];
                    }
                }
            }

            tetromino_y = 1;
            tetromino_x = rand() % 19 + 1;
            randomTetromino = nextTetrominoIndex; // Update the current Tetromino index
            currentTetromino = nextTetromino;
            nextTetrominoIndex = rand() % 7;     // Generate index tetronimo berikutnya
            nextTetromino = tetromino[nextTetrominoIndex]; // Set next Tetromino
            muteMove = true;
            flag = true;
        }



        counter++; // buat nambah detik
        hardDrop(arena, currentTetromino, tetromino_x, tetromino_y, drop, tempY, randomTetromino);
        draw(arena, arenaColors, nextTetromino, randomTetromino, nextTetrominoIndex, true, bossSkillMessage + "\n" + playerSkillMessage, bossHealth);
        clearLines(arena, bossHealth, arenaColors, bossDamaged);

        //cd berkurang
        if(shieldActive) {
            if (counter % 10 == 0) {
                if(cdUltimate > 0) {
                    cdUltimate--;
                }
                else {
                    shieldActive = false;
                }
            }
        }

        if (counter % 10 == 0) {
            if(cdSkill1 > 0) {
                cdSkill1--;
            }
            if (cdSkill2 > 0) {
                cdSkill2--;
            }
        }

        //kriteria skill
        if (mana >= 1) {
            skill_1_active = true;
        } else {
            skill_1_active = false;
        }

        if (mana >= 3 && cdSkill2 == 0) {
            skill_2_active = true;
        } else {
            skill_2_active = false;
        }

        if (mana >= 5) {
            if (cdUltimate == 0) {
                skill_3_active = true; // Still active while cooldown runs
            } else {
                skill_3_active = false; // Inactive once cooldown finishes
            }
        } else {
            skill_3_active = false; // Inactive if mana < 5
        }

        displaySkillStatus(skill_1_active, skill_2_active, skill_3_active);
        bossUsingSkill = false;
        Sleep(100);
        system("cls");
    }

    if (bossHealth > 0) {
        cout << "Better luck next time" << endl;
    } else {
        cout << "Congratulations You WIn" << endl;
    }
    Sleep(2000);
}

int main() {
    srand(time(0));
    loadScores();
    maximizeConsole();
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    int console_width = csbi.dwSize.X;
    int console_height = csbi.dwSize.Y;

    const int menu_width = 60;

    int difficulty = 0; // Default temporary value
    int start_line = 5; // Starting line for the submenu


    while (true){
        stopMusic();
        int selection_menu = main_menu(console_width, menu_width);
        int easy = 3000, intervalEasy = 60; // easy, medium,
        int medium = 4500, intervalMed = 45;
        int hard = 6000, intervalHard = 30;
        skill_1_active = false;
        skill_2_active = false;
        skill_3_active = false;
        doubleDamage = false;
        shieldActive = false;
        mana = 2;

        cdUltimate = 0;
        cdSkill2 = 0;
        cdSkill1 = 0;

        if (selection_menu == 0){
            int selection_submenu = play_submenu(console_width, menu_width, difficulty, start_line);

            if (selection_submenu == 0){
                system("cls");
                string playerName = intro();
                Sleep(2500);
                runNormalGame(playerName);
            } else if (selection_submenu == 1){
                int selection_difficulty = difficulty_menu(console_width, menu_width, start_line);

                if (selection_difficulty == 0){
                    runGame(easy, intervalEasy);
                } else if (selection_difficulty == 1){
                    runGame (medium, intervalMed);
                } else if (selection_difficulty == 2){
                    runGame (hard, intervalHard);
                }
            } else if (selection_submenu == -1){
                continue;
            }
        } else if (selection_menu == 1){
            showLeaderboard();
        } else if (selection_menu == 2){
            playSound("sfx/exit.wav");
            this_thread::sleep_for(chrono::milliseconds(1500));
            break;

        }
    }

    return 0;
}
