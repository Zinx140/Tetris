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

using namespace std;

// Ukuran arena
const int width = 25;
const int height = 20;
int score = 0;
string playerName;
vector<string> playerNames;
vector<int> scores;

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

void setColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

// set warnanya ganti aja angkanya kalo mau ganti warna (1-15)
int tetrominoColors[7] = {
    9,  // I - Blue
    14, // O - Yellow
    13, // T - Magenta
    6,  // L - Light Red
    3,  // J - Cyan
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
            } else {
                cout << " "; // Arena kosong
            }
        }

        // Spacing diantara arena dan "Next Tetromino"
        cout << string(horizontalSpacing, ' ');

        if(i == 1) {
            cout << "Score : " << score;
        }


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
            score += 100;
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

int main_menu() {
    int control_x = 0; // Position of ">"
    string arr_mainmenu[3] = {"Play", "Leaderboard", "Exit"};
    char input;
    const int menu_size = 3;

    // Get console window dimensions
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int console_width, console_height; //idk how tf does this even work, don't bother asking me
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    console_width = csbi.dwSize.X;
    console_height = csbi.dwSize.Y;

    const int menu_width = 60; // menu width

    do {
        system("cls");

        // hitung horizontal padding
        string horizontal_padding((console_width - menu_width) / 2, ' '); // Spaces for horizontal centering

        cout << horizontal_padding << "############################################################" << endl;
        cout << horizontal_padding << "#                                                          #" << endl;
        cout << horizontal_padding << "#        @@@@@@  @@@@@  @@@@@@  @@@@  @@  @@@@@@@          #" << endl;
        cout << horizontal_padding << "#        @@@@@@  @      @@@@@@  @  @  @@  @@               #" << endl;
        cout << horizontal_padding << "#          @@    @@@@@    @@    @@@@  @@  @@@@@@@          #" << endl;
        cout << horizontal_padding << "#          @@    @@@@@    @@    @@    @@  @@@@@@@          #" << endl;
        cout << horizontal_padding << "#          @@    @        @@    @ @   @@       @@          #" << endl;
        cout << horizontal_padding << "#          @@    @@@@@    @@    @  @  @@  @@@@@@@          #" << endl;
        cout << horizontal_padding << "#                                                          #" << endl;

        // kontrol untuk optionya
        for (int i = 0; i < menu_size; i++) {
            cout << horizontal_padding << "#               ";
            if (control_x == i) {
                cout << "> " << arr_mainmenu[i];
            } else {
                cout << "  " << arr_mainmenu[i];
            }
            cout << string(41 - arr_mainmenu[i].length(), ' ') << "#" << endl;
        }

        cout << horizontal_padding << "#                                                          #" << endl;
        cout << horizontal_padding << "#       Press W or S to move, press ENTER to choose        #" << endl;
        cout << horizontal_padding << "############################################################" << endl;

        input = _getch();
        if (input == 'w' || input == 'W') {
            if (control_x > 0) control_x--; // Move up
        } else if (input == 's' || input == 'S') {
            if (control_x < menu_size - 1) control_x++; // Move down
        }

    } while (input != '\r'); // ini adalah ascii untuk 'ENTER', dia berhenti kalo enter di tekan

    return control_x;
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
        cout << horizontal_padding <<"| " << left << setw(9) << "              Congratulation to " << topName << "             |" << endl;
        cout << horizontal_padding <<"| " << left << setw(9) << "              Your highest score is " << topScore << "               |"<< endl;
    }

    cout << horizontal_padding << "+=======================================================+" << endl;
    cout << horizontal_padding <<"|          Press any key to return to menu...           |" << endl;
    cout << horizontal_padding <<"+=======================================================+" << endl;

    inFile.close();
    getch();
}

int main() {
    srand(time(0));

    while (true){
        int selection = main_menu();

        if (selection==0){ //if selection==0, then start game!

    int dx = 0;
    int tetromino_y = 1;
    int tetromino_x = rand() % 19 + 1;
    int randomTetromino = rand() % 7;
    vector<vector<int>> currentTetromino = tetromino[0]; // vector current tetronimo
    int nextTetrominoIndex = rand() % 7; // Index untuk tetronimo berikutnya
    vector<vector<int>> nextTetromino = tetromino[0]; // vector next tetronimo
    int arena[height][width] = {0};
    score = 0;

    system("cls");

    cout << "Enter Your Name : ";
    cin >> playerName;

    cout << "Have Fun Playing!" << endl;
    Sleep(1500);

    system("cls");

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
            currentTetromino = nextTetromino; // Set current tetromino to the next one
            nextTetrominoIndex = rand() % 7; // Ambil tetronimo berikutnya
            nextTetromino = tetromino[0]; // Set next tetromino
        }

        draw(arena, nextTetromino);
        clearLines(arena);
        Sleep(100);
        system("cls"); // Bersihkan layar untuk menggambar ulang
    }

    cout << "Game Over!" << endl;
    loadScores();
    saveScore(score, playerName);
    Sleep(2000);


        } else if (selection==1){
            showLeaderboard();
        } else if (selection==2){
            break;
        }
    }


    return 0;
}