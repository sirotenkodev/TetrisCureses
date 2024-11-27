#include <cstdlib>
#include <ncurses.h>
#include <signal.h>
#include <panel.h>
#include <memory>
#include <iostream>
#include <vector>
#include <utility>
#include <chrono>

int piecesCount = 0;
int linesCount = 0;

struct Piece {
    int x;
    int y;

    int lowerRow = 2;

    int piece_arr[4][4] = {
        {0,1,0,0},
        {1,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    };
};


int field[20][10] {
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0}
};

void mulMatrix(Piece &p) {

    int mul[4][4] = {
        {0,0,0,1},
        {0,0,1,0},
        {0,1,0,0},
        {1,0,0,0}
    };

    int arr[4][4] = {};

    for(int i = 0; i < 4; ++i) {
        for(int j = 0; j < 4; ++j) {
            int sum = 0;
            for(int m = 0; m < 4; ++m) {
                sum += p.piece_arr[i][m] * mul[m][j];
            }
            arr[i][j] = sum;
        }
    }

    std::swap(arr, p.piece_arr);
}

void lowerPixelPiece(Piece &p) {
    int max = 0;
    for(int i = 0; i < 4; ++i) {
        for(int j = 0; j < 4; ++j) {
            if(p.piece_arr[i][j] == 1) {
                if(max < i) {
                    max = i;
                }
            }
        }
    }

    p.lowerRow = max + 1;
}

void deleteOldPiece(Piece &p) {
    for(int i = 0; i < 4; ++i) {
        for(int j = 0; j < 4; ++j) {
            if(p.piece_arr[j][i] == 1) {
                field[p.y + j][p.x + i] = 0;
            } else {
                continue;
            }
        }
    }
}

void rotateRight(Piece &p) {

    deleteOldPiece(p);

    int arr[4][4] = {};
    for(int i = 0; i < 4; ++i) {
        for(int j = 0; j < 4; ++j) {
            arr[j][i] = p.piece_arr[i][j];
        }
    }
    std::swap(arr, p.piece_arr);
    mulMatrix(p);
}


void addPiece(Piece &p) {
    for(int i = 0; i < 4; ++i) {
        for(int j = 0; j < 4; ++j) {
            if(p.piece_arr[j][i] == 1) {
                field[p.y + j][p.x + i] = 1;
            } else {
                continue;
            }
        }
    }
}

void printField(WINDOW *w) {
    mvwprintw(w, 1, 1, "=======TETRIS=======");
    int max_y, max_x;
    getmaxyx(w, max_y, max_x);
    for(int i = 0; i < max_x - 2; ++i) {
        for(int j = 0; j < max_y; ++j) {
            if(j == 5) {
                mvwprintw(w, i + 7, j, "<!");
            } else if(j == 17) {
                mvwprintw(w, i + 7, j, "!>");
            }
        }
    }

    for(int i = 0; i < 20; ++i) {
        for(int j = 0; j < 10; ++j) {
            if(field[i][j] == 0) {
                mvwaddch(w, i + 7, j + 7, '.');
            } else if(field[i][j] == 1) {
                mvwaddch(w, i + 7, j + 7, '#');
            }
        }
        std::cout << "\n";
    }

    mvwprintw(w, 27, 5, "<!==========!>");
}

int maxCol(Piece &p, int col) {
    int max = 0;
    for(int i = 0; i < 4; ++i) {
        if(p.piece_arr[i][col] == 1) {
            max = i;
        }
    }
    return max;
}

int maxRow(Piece &p, int row) {
    int max = 0;
    for(int i = 0; i < 4; ++i) {
        if(p.piece_arr[row][i] == 1) {
            ++max;
        }
    }
    return max;
}

int minRow(Piece &p, int row) {
    int max = 4;
    for(int i = 3; i > 0; --i) {
        if(p.piece_arr[row][i] == 1) {
            --max;
        }
    }
    return max;
}

void checkUnderLine() {
    for(int i = 19; i > 0; --i) {

        bool lineIsFull = true;

        for (int j = 0; j < 10; ++j) {
            if (field[i][j] == 0) {
                lineIsFull = false;
                break;
            }
        }

        if(lineIsFull) {
            for(int j = i; j > 1; --j) {
                for (int k = 0; k < 10; ++k) {
                    field[j][k] = field[j - 1][k];
                }
            }
            ++linesCount;
        }
    }
}

bool checkCollision(Piece &p) {

    for(int i = 0; i < 4; ++i) {
        int max = maxCol(p, i);
        if(max != 0) {
            if(field[p.y + max + 1][p.x + i] == 1) {
                return false;
            }
        }
    }
    return true;
}

bool moveDone(Piece &p) {
    bool isDone = false;
    lowerPixelPiece(p);
    if(p.y + p.lowerRow >= 20 || !checkCollision(p)) {
        isDone = true;
    }
    checkUnderLine();
    return isDone;
}

void movePiece(Piece &p, short lr, short d) {
    int x = p.x + lr;
    int y = p.y + d;
    deleteOldPiece(p);
    p.x = x;
    p.y = y;
}

void printGameMenu(WINDOW *w) {
    mvwprintw(w, 0, 0, "=========MENU=======");
    mvwprintw(w, 1, 2, "1) Arrow left");
    mvwprintw(w, 3, 2, "2) Arrow right");
    mvwprintw(w, 5, 2, "3) Arrow up - turn");
    mvwprintw(w, 7, 2, "4) Arrow down - 1 line");
    mvwprintw(w, 9, 2, "Pece count: %d", piecesCount);
    mvwprintw(w, 11, 2, "Lines count: %d", linesCount);
}


int main(int argc, char *argv[]) {

    initscr();
    nonl();
    cbreak();
    noecho();
    timeout(700);

    WINDOW *w = newwin(30,22, 2,2);
    WINDOW *menu = newwin(15,30, 2,30);

    scrollok(w, TRUE);

    keypad(stdscr, TRUE);
    keypad(w, TRUE);

    start_color();

    init_pair(COLOR_BLACK, COLOR_BLACK, COLOR_BLACK);
    init_pair(COLOR_GREEN, COLOR_GREEN, COLOR_BLACK);
    init_pair(COLOR_RED, COLOR_RED, COLOR_BLACK);
    init_pair(COLOR_CYAN, COLOR_CYAN, COLOR_BLACK);
    init_pair(COLOR_WHITE, COLOR_WHITE, COLOR_BLACK);
    init_pair(COLOR_MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(COLOR_BLUE, COLOR_BLUE, COLOR_BLACK);
    init_pair(COLOR_YELLOW, COLOR_YELLOW, COLOR_BLACK);


    color_set(COLOR_YELLOW, nullptr);
    box(stdscr, '.', '.');

    wcolor_set(w, COLOR_GREEN, nullptr);
    wborder(w,'|','|','-','-','*','*','*','*');

    wcolor_set(menu, COLOR_MAGENTA, nullptr);
    wborder(menu,'<','>','-','-','*','*','*','*');

    printField(w);

    refresh();
    wrefresh(w);
    wrefresh(menu);


    auto start = std::chrono::high_resolution_clock::now();
    Piece p = {3, 10};
    ++piecesCount;

    int input = ' ';
    while(input != 'q') {

        addPiece(p);

        printField(w);
        wrefresh(w);

        printGameMenu(menu);
        wrefresh(menu);
        input = getch();

        if(moveDone(p)) {
            p = {4, 2};
            ++piecesCount;
            continue;
        }

        if(input == KEY_UP) {
            rotateRight(p);
            continue;
        } else if(input == KEY_LEFT) {
            movePiece(p, -1, 0);
        } else if(input == KEY_RIGHT) {
            movePiece(p, 1, 0);
        } else if(input == KEY_DOWN) {
            while(!moveDone(p)) {
                movePiece(p, 0, 1);
            }
            checkUnderLine();
            continue;
        } else if(input == 'n') {

            p = {4, 2};
        }

        auto now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = now - start;

        if (elapsed.count() >= 1.0) {
            start = now;
            movePiece(p, 0, 1);
        }

        refresh();
        wrefresh(w);
        wrefresh(menu);
    }
    delwin(w);
    delwin(menu);
    endwin();
    exit(0);
}

