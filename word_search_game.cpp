
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <cstddef>

using namespace std;

#define SIZE 8
#define WORDS 5

#define RESET   "\033[0m"
#define CYAN    "\033[1;36m"
#define YELLOW  "\033[1;33m"
#define GREEN   "\033[1;32m"
#define RED     "\033[1;31m"
#define MAGENTA "\033[1;35m"
#define BLUE    "\033[1;34m"
#define WHITE   "\033[1;37m"

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void printCentered(const string &text, const string &color = RESET) {
    int width = 80;
    int len = (int)text.length();
    int spaces = (width - len) / 2;
    if (spaces < 0) spaces = 0;
    cout << color;
    for (int i = 0; i < spaces; i++) cout << " ";
    cout << text << RESET << "\n";
}

class Board {
    char grid[SIZE][SIZE];
    string words[WORDS];
    bool found[WORDS];
    vector<pair<int,int>> cellsOfWord[WORDS];

public:
    void createGrid() {
        for(int i = 0; i < SIZE; i++) {
            for(int j = 0; j < SIZE; j++) {
                grid[i][j] = '*';
            }
        }
    }

    void placeWords(string w[WORDS]) {
        for(int i = 0; i < WORDS; i++) {
            words[i] = w[i];
            found[i] = false;
            cellsOfWord[i].clear();
        }

        // All 8 directions (horizontal, vertical, diagonal) for better mix
        int dr[8] = {0, 0, 1, -1, 1, 1, -1, -1};
        int dc[8] = {1, -1, 0, 0, 1, -1, 1, -1};

        for(int i = 0; i < WORDS; i++) {
            int len = (int)words[i].length();
            bool done = false;
            int tries = 0;
            const int maxTries = 5000;

            while(!done && tries < maxTries) {
                tries++;
                int d = rand() % 8;
                int row = rand() % SIZE;
                int col = rand() % SIZE;
                int endRow = row + dr[d] * (len - 1);
                int endCol = col + dc[d] * (len - 1);

                if(endRow < 0 || endRow >= SIZE || endCol < 0 || endCol >= SIZE)
                    continue;

                bool clash = false;
                for(int k = 0; k < len; k++) {
                    int nr = row + dr[d] * k;
                    int nc = col + dc[d] * k;
                    char cCell = grid[nr][nc];
                    if(cCell != '*' && cCell != words[i][k]) {
                        clash = true;
                        break;
                    }
                }

                if(clash) continue;

                for(int k = 0; k < len; k++) {
                    int nr = row + dr[d] * k;
                    int nc = col + dc[d] * k;
                    grid[nr][nc] = words[i][k];
                    cellsOfWord[i].push_back({nr, nc});
                }
                done = true;
            }
        }

        for(int i = 0; i < SIZE; i++) {
            for(int j = 0; j < SIZE; j++) {
                if(grid[i][j] == '*') {
                    grid[i][j] = 'A' + (rand() % 26);
                }
            }
        }
    }

    bool isFoundCell(int r, int c) {
        for(int i = 0; i < WORDS; i++) {
            if(!found[i]) continue;
            for(const auto &p : cellsOfWord[i]) {
                if(p.first == r && p.second == c) return true;
            }
        }
        return false;
    }

    void displayGrid() {
        cout << "     ";
        for(int j = 1; j <= SIZE; j++) {
            cout << "  " << j << "  ";
        }
        cout << "\n";

        for(int i = 0; i < SIZE; i++) {
            cout << "  " << i + 1 << "  ";
            for(int j = 0; j < SIZE; j++) {
                if(isFoundCell(i, j)) {
                    cout << "[ " << GREEN << grid[i][j] << RESET << " ]";
                } else {
                    cout << "[ " << grid[i][j] << " ]";
                }
            }
            cout << "\n";
        }
    }

    void showWordList() {
        cout << "Words to find: ";
        for(int i = 0; i < WORDS; i++) {
            if(found[i]) {
                cout << GREEN << "[" << words[i] << "]" << RESET << " ";
            } else {
                cout << words[i] << " ";
            }
        }
        cout << "\n\n";
    }

    string getSelected(int r1, int c1, int r2, int c2) {
        int dr = 0, dc = 0;
        if(r2 > r1) dr = 1;
        if(r2 < r1) dr = -1;
        if(c2 > c1) dc = 1;
        if(c2 < c1) dc = -1;

        int stepsRow = abs(r2 - r1);
        int stepsCol = abs(c2 - c1);
        int steps = stepsRow > stepsCol ? stepsRow : stepsCol;

        string s = "";
        int r = r1, c = c1;
        for(int i = 0; i <= steps; i++) {
            if(r >= 0 && r < SIZE && c >= 0 && c < SIZE) {
                s += grid[r][c];
            }
            r += dr;
            c += dc;
        }
        return s;
    }

    bool checkWord(string sel) {
        if(sel.empty()) return false;
        string rev = sel;
        int n = (int)rev.length();
        for(int i = 0; i < n / 2; i++) {
            char tmp = rev[i];
            rev[i] = rev[n - 1 - i];
            rev[n - 1 - i] = tmp;
        }

        for(int i = 0; i < WORDS; i++) {
            if(!found[i] && (words[i] == sel || words[i] == rev)) {
                found[i] = true;
                return true;
            }
        }
        return false;
    }

    bool allFound() {
        for(int i = 0; i < WORDS; i++) {
            if(!found[i]) return false;
        }
        return true;
    }
};

class Level {
public:
    int levelNo;
    string words[WORDS];

    Level(int no, string w[WORDS]) {
        levelNo = no;
        for(int i = 0; i < WORDS; i++) {
            words[i] = w[i];
        }
    }

    void generateLevel(Board &b) {
        b.createGrid();
        b.placeWords(words);
    }
};

class Game {
    int level;
    int maxLevel;
    bool over;
    vector<string> pool;
    Board board;

    void buildPool() {
        string list[] = {
            "CAT","SUN","MAP","RUN","BIG","DOG","PEN","CUP","BAT","HAT",
            "RED","BLUE","PINK","GOLD","MOON","STAR","TREE","LEAF","ROCK","SAND",
            "LAKE","RIVER","OCEAN","CLOUD","STORM","LIGHT","NIGHT","MUSIC","DANCE","HAPPY",
            "SMILE","DREAM","WORLD","HOUSE","MONEY","PAPER","PENCIL","ERASER","MARKER","WINDOW",
            "GARDEN","FLOWER","PUZZLE","PLANET","SILVER","BRONZE","ANIMAL","INSECT","RABBIT","TURTLE",
            "SPIDER","MONKEY","GIRAFFE","PANTHER","JACKET","SWEATER","SANDALS","GLASSES","BALLOON","CAMERA",
            "GUITAR","VIOLIN","TRUMPET","KEYBOARD","MONITOR","LAPTOP","TABLET","PRINTER","SPEAKER","HEADSET",
            "BACKPACK","NOTEBOOK","CRAYON","MARBLE","WHISTLE","CANDLE","BOTTLE","BASKET","BLANKET","PILLOW",
            "CURTAIN","MIRROR","KITCHEN","BEDROOM","HALLWAY","CEILING","CHIMNEY","STAIRS","ELEVATOR","CORRIDOR",
            "BALCONY","TERRACE","DRIVEWAY","SIDEWALK","HIGHWAY","TRAFFIC","AIRPORT","STATION","HARBOR","ISLAND",
            "DESERT","FOREST","CANYON","VOLCANO","GLACIER","TSUNAMI","THUNDER","RAINBOW","SUNSHINE","SNOWFALL",
            "WINTER","SUMMER","AUTUMN","SPRING","SEASON","WEATHER","FORECAST","TORNADO","BLIZZARD","FLOODING",
            "DROUGHT","WILDFIRE","MUDSLIDE","CATFISH","DOLPHIN","STARFISH","SEAHORSE","OCTOPUS","LOBSTER","SHRIMP"
        };
        int n = (int)(sizeof(list) / sizeof(list[0]));
        for(int i = 0; i < n; i++) {
            pool.push_back(list[i]);
        }

        unsigned seed = (unsigned)chrono::system_clock::now().time_since_epoch().count();
        shuffle(pool.begin(), pool.end(), default_random_engine(seed));
    }

    void loadLevel() {
        string w[WORDS];
        int start = (level - 1) * WORDS;
        for(int i = 0; i < WORDS; i++) {
            if(start + i < (int)pool.size()) {
                w[i] = pool[start + i];
            } else {
                w[i] = "WORD";
            }
        }

        Level lv(level, w);
        lv.generateLevel(board);
    }

public:
    Game() {
        buildPool();
        maxLevel = (int)pool.size() / WORDS;
        if(maxLevel < 1) maxLevel = 1;
        level = 1;
        over = false;
        loadLevel();
    }

    bool isOver() { return over; }
    int currentLevel() { return level; }
    int totalLevels() { return maxLevel; }

    void showBoard() {
        cout << YELLOW << "Level " << level << " / " << maxLevel << RESET << "\n\n";
        board.showWordList();
        board.displayGrid();
    }

    int takeInput() {
        int vals[4] = {0, 0, 0, 0};
        string prompts[4] = {
            "Enter Start Row: ",
            "Enter Start Column: ",
            "Enter End Row: ",
            "Enter End Column: "
        };
        int step = 0;

        while(step < 4) {
            cout << "\n" << prompts[step];
            if(step > 0) cout << "(0 = go back) ";
            int val;
            cin >> val;

            if(val == 0) {
                if(step == 0) {
                    cout << RED << "Already at the first step." << RESET << "\n";
                    continue;
                }
                step--;
                continue;
            }

            if(val < 1 || val > SIZE) {
                cout << RED << "Value must be between 1 and " << SIZE << "..!" << RESET << "\n";
                continue;
            }

            vals[step] = val;
            step++;
        }

        string sel = board.getSelected(vals[0] - 1, vals[1] - 1, vals[2] - 1, vals[3] - 1);

        if(board.checkWord(sel)) {
            cout << "\n" << GREEN << ">> Found: " << sel << "!" << RESET << "\n";

            if(board.allFound()) {
                cout << "\n";
                printCentered("========================================", GREEN);
                printCentered("      LEVEL " + to_string(level) + " COMPLETE!", GREEN);
                printCentered("========================================", GREEN);
                cout << "\n";

                level++;
                if(level > maxLevel) {
                    cout << "\n";
                    printCentered("========================================", YELLOW);
                    printCentered("        YOU WIN! ALL LEVELS COMPLETE!", YELLOW);
                    printCentered("========================================", YELLOW);
                    cout << "\n";
                    over = true;
                    cout << "Press Enter to return to menu...";
                    cin.ignore();
                    cin.get();
                    return 2;
                }

                cout << "Press Enter to continue to Level " << level << "...";
                cin.ignore();
                cin.get();
                loadLevel();
                return 1;
            }
        } else {
            cout << "\n" << RED << ">> Not a match, try again..!" << RESET << "\n";
        }
        return 0;
    }
};

void showWelcome() {
    clearScreen();
    cout << "\n\n";
    printCentered("W E L C O M E   T O   T H E   W O R D   G A M E", YELLOW);
    cout << "\n\n\n";
    printCentered(":: Developed by ::", CYAN);
    printCentered("Asima Asim", MAGENTA);
    cout << "\n\n\n";
    printCentered("Press Enter to continue...", WHITE);
    cin.ignore();
    cin.get();
}

int main() {
    srand((unsigned)time(0));
    int choice;
    Game *g = nullptr;

    do {
        clearScreen();
        cout << "\n";
        printCentered("=== WORD SEARCH GAME ===", CYAN);
        cout << "\n\n";
        cout << CYAN << "  |1. Information about the game" << RESET << "\n";
        if(g != nullptr && !g->isOver()) {
            cout << CYAN << "  |2. Continue Game" << RESET << "\n";
        } else {
            cout << CYAN << "  |2. Start the Game" << RESET << "\n";
        }
        cout << CYAN << "  |3. Exit" << RESET << "\n";
        cout << "\n  Enter Choice: ";
        cin >> choice;

        if(choice == 1) {
            clearScreen();
            cout << "\n";
            printCentered("ABOUT THE WORD GAME", YELLOW);
            cout << "\n";
            cout << "  This is a word search game played on an 8x8 grid\n";
            cout << "  of letters. Hidden inside the grid are 5 words\n";
            cout << "  that you must find.\n\n";
            cout << "  The grid is labeled with row numbers (1-8) on the\n";
            cout << "  left and column numbers (1-8) on top, so you can\n";
            cout << "  locate any letter easily.\n\n";
            cout << "  To find a word, enter the row/column of its\n";
            cout << "  starting letter and the row/column of its ending\n";
            cout << "  letter. Words can be placed in any direction:\n";
            cout << "  horizontal, vertical or diagonal (forward and backward).\n\n";
            cout << "  Enter 0 at any step to go back one step.\n\n";
            cout << "  There are many levels, each with 5 new words and\n";
            cout << "  a new grid. Find every word to move to the next\n";
            cout << "  level. Clear every level to win!\n\n";
            cout << "  Press Enter to continue...";
            cin.ignore();
            cin.get();
        }
        else if(choice == 2) {
            bool freshGame = (g == nullptr);
            if(freshGame) {
                showWelcome();
                g = new Game();
            }

            bool playing = true;
            while(playing && !g->isOver()) {
                clearScreen();
                g->showBoard();
                int status = g->takeInput();
                if(status == 1 || status == 2) {
                    playing = false;
                }
            }
        }
        else if(choice == 3) {
            cout << "\n";
            printCentered("Thank you for playing!", GREEN);
            cout << "\n";
        }
        else {
            cout << "\n" << RED << "  Invalid choice..!" << RESET << "\n";
            cin.ignore();
            cin.get();
        }

    } while(choice != 3);

    delete g;
    return 0;
}