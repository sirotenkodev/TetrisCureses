#include <cstdlib>
#include <curses.h>
#include <signal.h>
#include <panel.h>
#include <memory>
#include <iostream>
#include <vector>
#include <utility>
#include <chrono>
#include <random>
#include <chrono>

int piecesCount = 0;
int linesCount = 0;
int linesGameOver = 0;

bool isGameOver = false;
bool isDeleteUnderLine = false;

enum class Shape {
    I,
    J,
    L,
    O,
    Z,
    T,
    S
};

struct Piece {
    int x;
    int y;

    Shape s;

    int lowerRow = 2;
    int tPieceArr[4][4] = {
        {0,1,0,0},
        {1,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    };

    int oPieceArr[4][4] = {
        {0,1,1,0},
        {0,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    };

    int lPieceArr[4][4] = {
        {0,0,1,0},
        {1,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    };
    int jPieceArr[4][4] = {
        {0,0,0,0},
        {1,1,1,0},
        {0,0,1,0},
        {0,0,0,0}
    };
    int sPieceArr[4][4] = {
        {0,1,1,0},
        {1,1,0,0},
        {0,0,0,0},
        {0,0,0,0}
    };
    int zPieceArr[4][4] = {
        {1,1,0,0},
        {0,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    };
    int iPieceArr[4][4] = {
        {0,0,0,0},
        {1,1,1,1},
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


int gofield[20][10] {
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,1,1,1},
    {0,0,0,0,0,0,0,0,1,0},
    {0,0,0,0,0,0,0,0,1,0},
    {0,0,1,1,0,0,0,0,0,0},
    {0,0,1,0,0,0,0,0,0,0},
    {0,0,1,1,0,0,1,1,1,0},
    {0,0,1,0,0,0,0,1,0,0},
    {0,0,1,1,0,0,0,1,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,1,1,1,0,0,0,0,0},
    {0,0,1,0,1,0,0,1,0,0},
    {0,0,1,1,1,0,0,0,0,0},
    {0,0,1,1,0,0,0,1,0,0},
    {0,0,1,0,1,0,0,1,0,0},
    {0,0,0,0,0,0,0,1,0,0},
    {0,0,0,1,1,1,0,0,0,0},
    {0,0,0,1,0,1,0,0,0,0},
    {0,1,0,1,0,0,0,0,0,0},
    {0,1,1,1,0,0,0,0,0,0}
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
                switch (p.s) {
                case Shape::I:
                    sum += p.iPieceArr[i][m] * mul[m][j];
                    break;
                case Shape::J:
                    sum += p.jPieceArr[i][m] * mul[m][j];
                    break;
                case Shape::L:
                    sum += p.lPieceArr[i][m] * mul[m][j];
                    break;
                case Shape::O:
                    sum += p.oPieceArr[i][m] * mul[m][j];
                    break;
                case Shape::Z:
                    sum += p.zPieceArr[i][m] * mul[m][j];
                    break;
                case Shape::T:
                    sum += p.tPieceArr[i][m] * mul[m][j];
                    break;
                case Shape::S:
                    sum += p.sPieceArr[i][m] * mul[m][j];
                    break;

                }
            }
            arr[i][j] = sum;
        }
    }

    switch (p.s) {
    case Shape::I:
        std::swap(arr, p.iPieceArr);
        break;
    case Shape::J:
        std::swap(arr, p.jPieceArr);
        break;
    case Shape::L:
        std::swap(arr, p.lPieceArr);
        break;
    case Shape::O:
        std::swap(arr, p.oPieceArr);
        break;
    case Shape::Z:
        std::swap(arr, p.zPieceArr);
        break;
    case Shape::T:
        std::swap(arr, p.tPieceArr);
        break;
    case Shape::S:
        std::swap(arr, p.sPieceArr);
        break;
    }
}

void lowerPixelPiece(Piece &p) {
    int max = 0;
    for(int i = 0; i < 4; ++i) {
        for(int j = 0; j < 4; ++j) {
            switch (p.s) {
            case Shape::I:
                if(p.iPieceArr[i][j] == 1) {
                    if(max < i) {
                        max = i;
                    }
                }
                break;
            case Shape::J:
                if(p.jPieceArr[i][j] == 1) {
                    if(max < i) {
                        max = i;
                    }
                }
                break;
            case Shape::L:
                if(p.lPieceArr[i][j] == 1) {
                    if(max < i) {
                        max = i;
                    }
                }
                break;
            case Shape::O:
                if(p.oPieceArr[i][j] == 1) {
                    if(max < i) {
                        max = i;
                    }
                }
                break;
            case Shape::Z:
                if(p.zPieceArr[i][j] == 1) {
                    if(max < i) {
                        max = i;
                    }
                }
                break;
            case Shape::T:
                if(p.tPieceArr[i][j] == 1) {
                    if(max < i) {
                        max = i;
                    }
                }
                break;
            case Shape::S:
                if(p.sPieceArr[i][j] == 1) {
                    if(max < i) {
                        max = i;
                    }
                }
                break;

            }
            if(p.tPieceArr[i][j] == 1) {
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
            switch (p.s) {
            case Shape::I:
                if(p.iPieceArr[j][i] == 1) {
                    field[p.y + j][p.x + i] = 0;
                } else {
                    continue;
                }
                break;
            case Shape::J:
                if(p.jPieceArr[j][i] == 1) {
                    field[p.y + j][p.x + i] = 0;
                } else {
                    continue;
                }
                break;
            case Shape::L:
                if(p.lPieceArr[j][i] == 1) {
                    field[p.y + j][p.x + i] = 0;
                } else {
                    continue;
                }
                break;
            case Shape::O:
                if(p.oPieceArr[j][i] == 1) {
                    field[p.y + j][p.x + i] = 0;
                } else {
                    continue;
                }
                break;
            case Shape::Z:
                if(p.zPieceArr[j][i] == 1) {
                    field[p.y + j][p.x + i] = 0;
                } else {
                    continue;
                }
                break;
            case Shape::T:
                if(p.tPieceArr[j][i] == 1) {
                    field[p.y + j][p.x + i] = 0;
                } else {
                    continue;
                }
                break;
            case Shape::S:
                if(p.sPieceArr[j][i] == 1) {
                    field[p.y + j][p.x + i] = 0;
                } else {
                    continue;
                }
                break;
            }

        }
    }
}

void rotateRight(Piece &p) {

    deleteOldPiece(p);

    int arr[4][4] = {};
    for(int i = 0; i < 4; ++i) {
        for(int j = 0; j < 4; ++j) {
            switch (p.s) {
            case Shape::I:
                arr[j][i] = p.iPieceArr[i][j];
                break;
            case Shape::J:
                arr[j][i] = p.jPieceArr[i][j];
                break;
            case Shape::L:
                arr[j][i] = p.lPieceArr[i][j];
                break;
            case Shape::O:
                arr[j][i] = p.oPieceArr[i][j];
                break;
            case Shape::Z:
                arr[j][i] = p.zPieceArr[i][j];
                break;
            case Shape::T:
                arr[j][i] = p.tPieceArr[i][j];
                break;
            case Shape::S:
                arr[j][i] = p.sPieceArr[i][j];
                break;
            }

        }
    }

    switch (p.s) {
    case Shape::I:
        std::swap(arr, p.iPieceArr);
        break;
    case Shape::J:
        std::swap(arr, p.jPieceArr);
        break;
    case Shape::L:
        std::swap(arr, p.lPieceArr);
        break;
    case Shape::O:
        std::swap(arr, p.oPieceArr);
        break;
    case Shape::Z:
        std::swap(arr, p.zPieceArr);
        break;
    case Shape::T:
        std::swap(arr, p.tPieceArr);
        break;
    case Shape::S:
        std::swap(arr, p.sPieceArr);
        break;
    }


    mulMatrix(p);
}


void addPiece(Piece &p) {
    for(int i = 0; i < 4; ++i) {
        for(int j = 0; j < 4; ++j) {
            switch (p.s) {
            case Shape::I:
                if(p.iPieceArr[j][i] == 1) {
                    field[p.y + j][p.x + i] = 1;
                } else {
                    continue;
                }
                break;
            case Shape::J:
                if(p.jPieceArr[j][i] == 1) {
                    field[p.y + j][p.x + i] = 1;
                } else {
                    continue;
                }
                break;
            case Shape::L:
                if(p.lPieceArr[j][i] == 1) {
                    field[p.y + j][p.x + i] = 1;
                } else {
                    continue;
                }
                break;
            case Shape::O:
                if(p.oPieceArr[j][i] == 1) {
                    field[p.y + j][p.x + i] = 1;
                } else {
                    continue;
                }
                break;
            case Shape::Z:
                if(p.zPieceArr[j][i] == 1) {
                    field[p.y + j][p.x + i] = 1;
                } else {
                    continue;
                }
                break;
            case Shape::T:
                if(p.tPieceArr[j][i] == 1) {
                    field[p.y + j][p.x + i] = 1;
                } else {
                    continue;
                }
                break;
            case Shape::S:
                if(p.sPieceArr[j][i] == 1) {
                    field[p.y + j][p.x + i] = 1;
                } else {
                    continue;
                }
                break;
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
            if(j == 4) {
                mvwprintw(w, i + 6, j, "<!");
            } else if(j == 16) {
                mvwprintw(w, i + 6, j, "!>");
            }
        }
    }

    for(int i = 0; i < 20; ++i) {
        for(int j = 0; j < 10; ++j) {
            if(field[i][j] == 0) {
                mvwprintw(w, i + 6, j + 6, ".");
            } else if(field[i][j] == 1) {
                mvwprintw(w, i + 6, j + 6, "#");
            }
        }
        std::cout << "\n";
    }

    mvwprintw(w, 26, 4, "<!==========!>");
}


void printEndField(WINDOW *w) {
    mvwprintw(w, 1, 1, "=======TETRIS=======");
    int max_y, max_x;
    getmaxyx(w, max_y, max_x);
    for(int i = 0; i < max_x - 2; ++i) {
        for(int j = 0; j < max_y; ++j) {
            if(j == 4) {
                mvwprintw(w, i + 6, j, "<!");
            } else if(j == 16) {
                mvwprintw(w, i + 6, j, "!>");
            }
        }
    }

    for(int i = 0; i < 20; ++i) {
        for(int j = 0; j < 10; ++j) {
            if(gofield[i][j] == 0) {
                mvwprintw(w, i + 6, j + 6, " ");
            } else if(gofield[i][j] == 1) {
                mvwprintw(w, i + 6, j + 6, "#");
            }
        }
        std::cout << "\n";
    }

    mvwprintw(w, 26, 4, "<!==========!>");
}

void printGameOver(WINDOW *w) {

    int sCount = 0;
    int eCount = 9;
    int osCount = 0;
    int oeCount = 19;
    for(int i = 0; i < 5 ; ++i) {
        for(int j = sCount; j < eCount ; ++j) {
            mvwprintw(w, sCount + 6, j + 6, "#");
            wrefresh(w);
        }
                     usleep(200000);
        for(int j = osCount; j < oeCount ; ++j) {
            mvwprintw(w, j + 6, eCount + 6, "#");
        }
                    usleep(200000);
        for(int j = eCount; j >= sCount ; --j) {
            mvwprintw(w, oeCount + 6, j + 6, "#");
            wrefresh(w);
        }
                        usleep(200000);
        for(int j = oeCount; j >= osCount ; --j) {
            mvwprintw(w, j + 6, sCount + 6, "#");
            wrefresh(w);
        }
        ++sCount; --eCount;
        ++osCount; --oeCount;
                        usleep(200000);
    }

}

int maxCol(Piece &p, int col) {
    int max = 0;
    for(int i = 0; i < 4; ++i) {
        switch (p.s) {
        case Shape::I:
            if(p.iPieceArr[i][col] == 1) {
                max = i;
            }
            break;
        case Shape::J:
            if(p.jPieceArr[i][col] == 1) {
                max = i;
            }
            break;
        case Shape::L:
            if(p.lPieceArr[i][col] == 1) {
                max = i;
            }
            break;
        case Shape::O:
            if(p.oPieceArr[i][col] == 1) {
                max = i;
            }
            break;
        case Shape::Z:
            if(p.zPieceArr[i][col] == 1) {
                max = i;
            }
            break;
        case Shape::T:
            if(p.tPieceArr[i][col] == 1) {
                max = i;
            }
            break;
        case Shape::S:
            if(p.sPieceArr[i][col] == 1) {
                max = i;
            }
            break;
        }
    }
    return max;
}

int maxRow(Piece &p, int row) {
    int max = 0;
    for(int i = 0; i < 4; ++i) {
        switch (p.s) {
        case Shape::I:
            if(p.iPieceArr[row][i] == 1) {
                ++max;
            }
            break;
        case Shape::J:
            if(p.jPieceArr[row][i] == 1) {
                ++max;
            }
            break;
        case Shape::L:
            if(p.lPieceArr[row][i] == 1) {
                ++max;
            }
            break;
        case Shape::O:
            if(p.oPieceArr[row][i] == 1) {
                ++max;
            }
            break;
        case Shape::Z:
            if(p.zPieceArr[row][i] == 1) {
                ++max;
            }
            break;
        case Shape::T:
            if(p.tPieceArr[row][i] == 1) {
                ++max;
            }
            break;
        case Shape::S:
            if(p.sPieceArr[row][i] == 1) {
                ++max;
            }
            break;
        }
    }
    return max;
}

int minRow(Piece &p, int row) {
    int max = 4;
    for(int i = 3; i > 0; --i) {
        switch (p.s) {
        case Shape::I:
            if(p.iPieceArr[row][i] == 1) {
                --max;
            }
            break;
        case Shape::J:
            if(p.jPieceArr[row][i] == 1) {
                --max;
            }
            break;
        case Shape::L:
            if(p.lPieceArr[row][i] == 1) {
                --max;
            }
            break;
        case Shape::O:
            if(p.oPieceArr[row][i] == 1) {
                --max;
            }
            break;
        case Shape::Z:
            if(p.zPieceArr[row][i] == 1) {
                --max;
            }
            break;
        case Shape::T:
            if(p.tPieceArr[row][i] == 1) {
                --max;
            }
            break;
        case Shape::S:
            if(p.sPieceArr[row][i] == 1) {
                --max;
            }
            break;
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

bool checkGameOver() {
    linesGameOver = 0;
    for(int i = 19; i > 0; --i) {

        for (int j = 0; j < 10; ++j) {
            if (field[i][j] == 1) {
                ++linesGameOver;
                break;
            }
        }
    }

    return linesGameOver >= 19;
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
    halfdelay(10);

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

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0, 7);

    Piece p = {3, 10};
    p.s = static_cast<Shape>(distr(gen));
    ++piecesCount;

    int input = ' ';
    while(input != 'q') {

        addPiece(p);

        checkUnderLine();

        if(checkGameOver()) {

            printGameOver(w);
            isGameOver = true;

            usleep(200000);
            printEndField(w);

            wrefresh(w);
            wrefresh(menu);
            break;
        }

        if(moveDone(p)) {
            p = {4, 0};
            p.s = static_cast<Shape>(distr(gen));
            lowerPixelPiece(p);
            p.x = p.lowerRow + 1;
            ++piecesCount;
        }

        printField(w);
        wrefresh(w);

        printGameMenu(menu);
        wrefresh(menu);
        input = getch();

        if(input == KEY_UP) {
            rotateRight(p);
        } else if(input == KEY_LEFT) {
            movePiece(p, -1, 0);
        } else if(input == KEY_RIGHT) {
            movePiece(p, 1, 0);
        } else if(input == KEY_DOWN) {
            movePiece(p, 0, 1);
            while(!moveDone(p)) {
                movePiece(p, 0, 1);
            }
            refresh();
            continue;
        }

        auto now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = now - start;

        if (elapsed.count() >= 0.3 && !moveDone(p)) {
            start = now;
            movePiece(p, 0, 1);
        }
        refresh();
    }

    while(true) {
        printEndField(w);

        wrefresh(w);
        wrefresh(menu);
    }

    delwin(w);
    delwin(menu);
    endwin();

    exit(0);
}

