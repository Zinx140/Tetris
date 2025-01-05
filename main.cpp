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

void playSound(const string &filename) {
    if (!PlaySound(filename.c_str(), NULL, SND_FILENAME | SND_ASYNC)) {
        cerr << "Error playing sound: " << filename << endl;
    }
}

bool playBackgroundMusic(const string &filename) {
    if (!PlaySound(filename.c_str(), NULL, SND_FILENAME | SND_LOOP | SND_ASYNC)) {
        cerr << "Error playing music: " << filename << endl;
        return false;
    }
    return true;
}

int arenaColors[height][width] = {0};

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

// Fungsi untuk menggambar arena
void draw(int arena[height][width], int arenaColors[height][width], vector<vector<int>> &nextTetromino, int currentTetrominoIndex, int nextTetrominoIndex) {
    const int boxHeight = 4;
    const int boxWidth = 4;
    const int horizontalSpacing = 5;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (arena[i][j] == 1 || arena[i][j] == 7) {
                setColor(arenaColors[i][j]); // ngeset warna tetromino
                cout << "@";
                setColor(7); // Reset color ke default
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
                        cout << "@";
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
    cout << endl;
    cout << "'A' to move tetromino to the left" << endl;
    cout << "'D' to move tetromino to the right" << endl;
    cout << "'W' to rotate tetromino" << endl;
    cout << "'S' to Drop the tetromino" << endl;
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
                if (arena[position_y + i][position_x - 1] == 7) {
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
                if (arena[position_y + i][position_x + newWidth] == 7) {
                    return false;
                }
            }
        }
    }
    return true;
}

void clearLines(int arena[height][width], int &bossHealth) {
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
            bossHealth -= 100;
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

bool canRandom(int randomTetromino, int arena[height][width], int positionX, int positionY) {
    vector<vector<int>> currentTetromino = tetromino[randomTetromino];
    if (canMove(arena, positionX, positionY, currentTetromino)) {
        return true;
    } else {
        return false;
    }
}

void runNormalGame() {
    int dx = 0;
    int tetromino_y = 1;
    int tetromino_x = rand() % 19 + 1;
    int randomTetromino = rand() % 7;
    vector<vector<int>> currentTetromino = tetromino[randomTetromino]; // vector current tetronimo
    int nextTetrominoIndex = rand() % 7; // Index untuk tetronimo berikutnya
    vector<vector<int>> nextTetromino = tetromino[nextTetrominoIndex]; // vector next tetronimo
    int arena[height][width] = {0};
    bool flag = true;
    int tempY, counter = 0;
    score = 0;

    system("cls");

    cout << "Enter Your Name : ";
    cin >> playerName;

    cout << "Have Fun Playing!" << endl;
    Sleep(1500);

    system("cls");

    playBackgroundMusic("music/MainTheme.wav");

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
        draw(arena, arenaColors, nextTetromino, randomTetromino, nextTetrominoIndex);
        clearLines(arena);
        Sleep(100);
        system("cls"); // Bersihkan layar untuk menggambar ulang
    }

    cout << "Game Over!" << endl;
    saveScore(score, playerName);
    Sleep(2000);
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

    system("cls");

    cout << "Enter Your Name : ";
    cin >> playerName;

    cout << "Have Fun Playing!" << endl;
    Sleep(1500);

    system("cls");

    while (!gameOver(arena)) {
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
                int changeTetromino = rand() % 7;
                muteMove = true;
                do {
                    randomSkill = rand() % 4 + 1;
                } while((randomSkill == 3 && bossHealth + 50 > limitHealth) || (randomSkill == 1 && !canRandom(changeTetromino, arena, tetromino_x, tetromino_y)));

                if (randomSkill == 1) { // Skill1: ngubah bentuk tetromino
                    currentTetromino = tetromino[changeTetromino];
                } else if (randomSkill == 2) { // Skill2: Langsung jatuhin Tetromino
                    drop = true;
                    flag = false;
                } else if (randomSkill == 3) { // Skill3: Nambah darah boss
                    bossHealth += 50;
                } else { // Skill4: blokir player move
                    muteMove = false;
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
        cout << "Boss Health: " << bossHealth << endl;
        hardDrop(arena, currentTetromino, tetromino_x, tetromino_y, drop, tempY, randomTetromino);
        draw(arena, arenaColors, nextTetromino, randomTetromino, nextTetrominoIndex);
        clearLines(arena, bossHealth);
        Sleep(100);
        system("cls"); // Bersihkan layar untuk menggambar ulang
    }

    cout << "Game Over!" << endl;
    Sleep(2000);
}

int main() {
    srand(time(0));
    loadScores();

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    int console_width = csbi.dwSize.X;
    int console_height = csbi.dwSize.Y;

    const int menu_width = 60;

    int difficulty = 0; // Default temporary value
    int start_line = 5; // Starting line for the submenu

    while (true){
    int selection_menu = main_menu(console_width, menu_width);

    int easy = 3000, intervalEasy = 60; // easy, medium,
    int medium = 4500, intervalMed = 45;
    int hard = 6000, intervalHard = 5;

    if (selection_menu == 0){
        int selection_submenu = play_submenu(console_width, menu_width, difficulty, start_line);

        if (selection_submenu == 0){
            runNormalGame();
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
        break;
    }
    }
    return 0;
}
