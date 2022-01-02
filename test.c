#include <ncurses.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define HEIGH_WINDOW 23
#define WIDTH_WINDOW 40
#define GLASS_HEIGHT 20
#define GLASS_WIDTH 10
#define SPAN 10
#define START_Y_POS (-1)
#define START_X_POS 4
#define FIGURE_SIZE 4

enum Dir {
    DOWN,
    LEFT,
    RIGHT,
    ROTATE,
};

typedef struct {
    int shape[4][4];
    int y, x, prev_y, prev_x;
    bool down, left, right, rotate;
} figure_t;

figure_t figures[] =
{
    {
         {
             {0, 0, 1, 0},
             {0, 0, 1, 0},
             {0, 1, 1, 0},
             {0, 0, 0, 0},
         },
         START_Y_POS, START_X_POS, START_Y_POS, START_X_POS,
     },
     {
         {
            {0, 1, 0, 0},
            {0, 1, 0, 0},
            {0, 1, 1, 0},
            {0, 0, 0, 0},
         },
         START_Y_POS, START_X_POS, START_Y_POS, START_X_POS,
     },
     {
         { //0  1  2  3
            {0, 0, 1, 0}, //0
            {0, 1, 1, 1}, //1
            {0, 0, 0, 0}, //2
            {0, 0, 0, 0}, //3
         },
         START_Y_POS, START_X_POS, START_Y_POS, START_X_POS,
     },
     {
         {
            {0, 1, 1, 0},
            {0, 1, 1, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0},
         },
         START_Y_POS, START_X_POS, START_Y_POS, START_X_POS,
     },
     {
         {
            {0, 1, 0, 0},
            {0, 1, 1, 0},
            {0, 0, 1, 0},
            {0, 0, 0, 0},
         },
         START_Y_POS, START_X_POS, START_Y_POS, START_X_POS,
     },
     {
         {
            {0, 0, 1, 0},
            {0, 1, 1, 0},
            {0, 1, 0, 0},
            {0, 0, 0, 0},
         },
         START_Y_POS, START_X_POS, START_Y_POS, START_X_POS,
     },
     {
         {
            {0, 0, 1, 0},
            {0, 0, 1, 0},
            {0, 0, 1, 0},
            {0, 0, 1, 0},
         },
         START_Y_POS, START_X_POS, START_Y_POS, START_X_POS,
     },
};

int glass[GLASS_HEIGHT + 1][GLASS_WIDTH + 2] = {
                   //0  1  2  3  4  5  6  7  8  9  10 11
                    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, //0
                    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, //1
                    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, //2
                    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, //3
                    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, //4
                    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, //5
                    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, //6
                    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, //7
                    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, //8
                    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, //9
                    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, //10
                    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, //11
                    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, //12
                    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, //13
                    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, //14
                    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, //15
                    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, //16
                    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, //17
                    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, //18
                    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, //19
                    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, //20
                };

char *info[] = {"NEXT:            ",
                "                 ",
                "                 ",
                "                 ",
                "SCORE:     000000"};

WINDOW *win;

void fill_glass(figure_t *tetramino) {
    
    for(int i = 0; i < FIGURE_SIZE; i++) {
       for(int j = 0; j < FIGURE_SIZE; j++) {
           if (tetramino->shape[i][j]) {
               glass[(tetramino -> y) + i][(tetramino -> x) + j + 1] = 1;
           }
       }
    }
}

void paint_figure(figure_t *tetramino) {

    /*
    for(int i = 0; i < FIGURE_SIZE; i++) {
        for(int j = 0; j < FIGURE_SIZE; j++) {
            if (tetramino->shape[i][j]) {
                wmove(win, (tetramino -> prev_y) + i + 1, (tetramino -> prev_x) + j + 2);
                waddch(win, ' ');
            }
        }
    }
    */
    
    for(int i = 0; i < FIGURE_SIZE; i++) {
       for(int j = 0; j < FIGURE_SIZE; j++) {
           if (tetramino -> shape[i][j]) {
               wmove(win, tetramino -> y + i + 1 , tetramino -> x + j + 2);
               waddch(win, '#' | COLOR_PAIR(2));
           }
       }
    }

    tetramino -> prev_y = tetramino -> y;
    tetramino -> prev_x = tetramino -> x;
}

void clean_figure(figure_t *tetramino) {
    
    for(int i = 0; i < FIGURE_SIZE; i++) {
        for(int j = 0; j < FIGURE_SIZE; j++) {
            if (tetramino->shape[i][j]) {
                wmove(win, (tetramino -> prev_y) + i + 1, (tetramino -> prev_x) + j + 2);
                waddch(win, ' ');
            }
        }
    }
    
}

void paint() {
    
    for (int i = 0; i < GLASS_HEIGHT + 1; i++) {
        for (int j = 0; j < GLASS_WIDTH + 2; j++) {
            if (glass[i][j]) {
                wmove(win, i + 1, j + 1);
                waddch(win, '#');
            }
        }
    }

    for (int i = 0; i < sizeof(info) / sizeof(info[0]); i++) {
        wmove(win, i + 1, GLASS_WIDTH + SPAN);
        waddstr(win, info[i]);
    }

    box(win, 0, 0);

    wrefresh(win);

}

void check_move(figure_t *tetramino) {

    tetramino -> down = TRUE;
    tetramino -> left = TRUE;
    tetramino -> right = TRUE;
    tetramino -> rotate = TRUE;

    for(int i = FIGURE_SIZE - 1; i >= 0; i--) {
        for(int j = FIGURE_SIZE - 1; j >= 0; j--) {
           if (tetramino -> shape[i][j])  {
               if (glass[(tetramino -> y) + i][(tetramino -> x) + j + 1]) {
                   tetramino -> down = FALSE;
               }
               if (glass[(tetramino -> y) + i][(tetramino -> x) + j]) {
                   tetramino -> left = FALSE;
               }
               if (glass[(tetramino -> y) + i][(tetramino -> x) + j + 2]) {
                   tetramino -> right = FALSE;
               }
           }
       }
    }

}

void step(figure_t *tetramino) {

    int step = tetramino -> y;

    tetramino -> y++;
    check_move(tetramino);

    if (tetramino -> down) {
        clean_figure(tetramino);
        paint_figure(tetramino);
    } else {
        tetramino -> y--;
        fill_glass(tetramino);

        *tetramino = figures[rand() % 7];

    }

}

void rotate(figure_t *tetramino) {
    figure_t temp;
    temp = *tetramino;

    for (int i = 0; i < FIGURE_SIZE; i++) {
        for (int j = 0; j < FIGURE_SIZE; j++) {
            if ((i - 2) == 0) temp.shape[3 - j][2] = tetramino -> shape[i][j];
            if ((i - 2) == 1) temp.shape[3 - j][3] = tetramino -> shape[i][j];
            if ((i - 2) == -1) temp.shape[3 - j][1] = tetramino -> shape[i][j];
            if ((i - 2) == -2) temp.shape[3 - j][0] = tetramino -> shape[i][j];
        }
    }

    check_move(&temp);

    if (temp.down && temp.right && temp.left) temp.rotate = TRUE;
    *tetramino = temp;
}

void quit() {

    clear();
    printw("Bay!");
    refresh();
    sleep(1);

    delwin(win);
    endwin();

}

int main(int argc, char** argv) {

    if (!initscr()) {
        fprintf(stderr, "Error initialising ncurses.\n");
        exit(-1);
    }

    if (!has_colors()) {
        fprintf(stderr, "Error inititialising colors.\n");
        exit(-1);
    }

    start_color();

    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);

    curs_set(0);
    noecho();
    cbreak();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
    
    int offsety = (LINES - HEIGH_WINDOW) / 2;
    int offsetx = (COLS - WIDTH_WINDOW) / 2;

    win = newwin(HEIGH_WINDOW, WIDTH_WINDOW, offsety, offsetx);
    wattron(win, A_BOLD | COLOR_PAIR(1));

    srand(time(NULL));
    
    figure_t tetramino = figures[rand() % 7];
    figure_t fantom;
    
    int ch;
    
    while((ch = getch()) != KEY_UP) {
        tetramino.prev_x = tetramino.x;
        check_move(&tetramino);
        switch(ch) {
            case KEY_LEFT:
                if (tetramino.left) {
                    tetramino.x--;
                    clean_figure(&tetramino);
                    paint_figure(&tetramino);
                }
                break;
            case KEY_RIGHT:
                if (tetramino.right) {
                    tetramino.x++;
                    clean_figure(&tetramino);
                    paint_figure(&tetramino);
                }
                break;
            case ' ' :
                fantom = tetramino;
                rotate(&fantom);
                if (fantom.rotate) {
                    clean_figure(&tetramino);
                    tetramino = fantom;
                    paint_figure(&tetramino);
                }
            default:
                break;
        }

        step(&tetramino);
        paint();
        usleep(400000);
    
    }
    
    getch(); 
    quit();

}
