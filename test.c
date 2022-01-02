#include <ncurses.h> 
#include <stdlib.h>

#include <string.h>
#include <unistd.h>
#include <time.h>

#define HEIGH_WINDOW 23
#define WIDTH_WINDOW 40
#define HEIGH_WINDOW_INFO 11
#define WIDTH_WINDOW_INFO 25
#define GLASS_HEIGHT 20
#define GLASS_WIDTH 10
#define SPAN 10
#define START_Y_POS 0
#define START_X_POS 4
#define FIGURE_SIZE 4
#define MAX_LEVEL 19

int SPEED_LEVELS[MAX_LEVEL + 1] = {
//  0,   1,  2,  3,  4,  5,  6,  7,  8,  9
    50, 48, 46, 44, 42, 40, 38, 36, 34, 32,
//  10, 11, 12, 13, 14, 15, 16, 17, 18, 19
    30, 28, 26, 24, 22, 20, 16, 12, 8,  4
};

int LINES_BONUS[5] = {0, 100, 200, 400, 800 };

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

typedef struct {
    int next_tetramino;
    int score;
    int level;
    int tick_till_down;
} game_t;

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
         FALSE, FALSE, FALSE, FALSE,
     },
     {
         {
            {0, 1, 0, 0},
            {0, 1, 0, 0},
            {0, 1, 1, 0},
            {0, 0, 0, 0},
         },
         START_Y_POS, START_X_POS, START_Y_POS, START_X_POS,
         FALSE, FALSE, FALSE, FALSE,
     },
     {
         { //0  1  2  3
            {0, 0, 1, 0}, //0
            {0, 1, 1, 1}, //1
            {0, 0, 0, 0}, //2
            {0, 0, 0, 0}, //3
         },
         START_Y_POS, START_X_POS, START_Y_POS, START_X_POS,
         FALSE, FALSE, FALSE, FALSE,
     },
     {
         {
            {0, 1, 1, 0},
            {0, 1, 1, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0},
         },
         START_Y_POS, START_X_POS, START_Y_POS, START_X_POS,
         FALSE, FALSE, FALSE, FALSE,
     },
     {
         {
            {0, 1, 0, 0},
            {0, 1, 1, 0},
            {0, 0, 1, 0},
            {0, 0, 0, 0},
         },
         START_Y_POS, START_X_POS, START_Y_POS, START_X_POS,
         FALSE, FALSE, FALSE, FALSE,
     },
     {
         {
            {0, 0, 1, 0},
            {0, 1, 1, 0},
            {0, 1, 0, 0},
            {0, 0, 0, 0},
         },
         START_Y_POS, START_X_POS, START_Y_POS, START_X_POS,
         FALSE, FALSE, FALSE, FALSE,
     },
     {
         {
            {0, 0, 1, 0},
            {0, 0, 1, 0},
            {0, 0, 1, 0},
            {0, 0, 1, 0},
         },
         START_Y_POS, START_X_POS, START_Y_POS, START_X_POS,
         FALSE, FALSE, FALSE, FALSE,
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


WINDOW *win, *win_info;
game_t *game = NULL;
figure_t *tetramino = NULL;

void glass_shift(int line) {

    for (int i = line; i > 0; i--) {
        for (int j = GLASS_WIDTH; j > 0; j--) {
            glass[i][j] = glass[i - 1][j];
        }
    }

    for (int j = 0; j < GLASS_WIDTH; j++) {
        glass[0][j + 1] = 0;
    }

}

void refresh_glass() {
    int lines = 0;
    for (int i = GLASS_HEIGHT - 1; i > 0; i--) {
        int blocks = 0;
        for (int j = GLASS_WIDTH; j > 0; j--) {
            if (!glass[i][j]) break;
            blocks++;
        }
        if (blocks == 10) {
            lines++;
            glass_shift(i);
        }
    }
    game -> score += LINES_BONUS[lines];
}

void fill_glass() {
    
    for(int i = 0; i < FIGURE_SIZE; i++) {
       for(int j = 0; j < FIGURE_SIZE; j++) {
           if (tetramino->shape[i][j]) {
               glass[(tetramino -> y) + i][(tetramino -> x) + j + 1] = 1;
           }
       }
    }

    refresh_glass();

}

void paint_figure() {

    for(int i = 0; i < FIGURE_SIZE; i++) {
       for(int j = 0; j < FIGURE_SIZE; j++) {
           if (tetramino -> shape[i][j]) {
               wmove(win, tetramino -> y + i + 1 , tetramino -> x + j + 2);
               waddch(win, ' ' | A_REVERSE | COLOR_PAIR(2));
           }
       }
    }

    tetramino -> prev_y = tetramino -> y;
    tetramino -> prev_x = tetramino -> x;
}

void clean_figure() {
    
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
            wmove(win, i + 1, j + 1);
            if (glass[i][j]) {
                waddch(win, ' ' | A_REVERSE | COLOR_PAIR(1));
            } else {
                waddch(win, ' ');
            }
            paint_figure();
        }
    }

    figure_t next_tetramino = figures[game -> next_tetramino];

    wclear(win_info);
 
    wmove(win_info, 1, 1);
    waddstr(win_info, "NEXT");

    for(int i = 0; i < FIGURE_SIZE; i++) {
       for(int j = 0; j < FIGURE_SIZE; j++) {
           if (next_tetramino.shape[i][j]) {
               wmove(win_info, next_tetramino.y + i + 3, next_tetramino.x + j + 1);
               waddch(win_info, ' ' | A_REVERSE | COLOR_PAIR(3));
           }
       }
    }
 
    wmove(win_info, 7, 1);
    waddstr(win_info, "SCORE");
 
    wmove(win_info, 7, 7);
    wattron(win_info, COLOR_PAIR(2));
    wprintw(win_info, "%d", (game -> score));
    wattroff(win_info, COLOR_PAIR(2));
    wattron(win_info, A_BOLD | COLOR_PAIR(1));

    box(win, 0, 0);
    box(win_info, 0, 0);

    wrefresh(win);
    wrefresh(win_info);

}

void check_move(figure_t *t) {

    t -> down = TRUE;
    t -> left = TRUE;
    t -> right = TRUE;
    t -> rotate = FALSE;

    for(int i = FIGURE_SIZE - 1; i >= 0; i--) {
        for(int j = FIGURE_SIZE - 1; j >= 0; j--) {
           if (t -> shape[i][j])  {
               if (glass[(t -> y) + i][(t -> x) + j + 1]) {
                   t -> down = FALSE;
               }
               if (glass[(t -> y) + i][(t -> x) + j]) {
                   t -> left = FALSE;
               }
               if (glass[(t -> y) + i][(t -> x) + j + 2]) {
                   t -> right = FALSE;
               }
           }
       }
    }

}

void step() {

    tetramino -> y++;
    check_move(tetramino);

    if (tetramino -> down) {
        clean_figure(tetramino);
        paint_figure(tetramino);
    } else {
        tetramino -> y--;
        fill_glass(tetramino);

        *tetramino = figures[game -> next_tetramino];
        game -> next_tetramino = rand() % 7;

    }

}

void tick() {
    game -> tick_till_down--;
    if (!(game -> tick_till_down)) {
        step(tetramino);
        game -> tick_till_down = SPEED_LEVELS[game -> level];
    }
}

void rotate(figure_t *t) {
    figure_t temp;
    temp = *t;

    for (int i = 0; i < FIGURE_SIZE; i++) {
        for (int j = 0; j < FIGURE_SIZE; j++) {
            if ((i - 2) == 0) temp.shape[3 - j][2] = t -> shape[i][j];
            if ((i - 2) == 1) temp.shape[3 - j][3] = t -> shape[i][j];
            if ((i - 2) == -1) temp.shape[3 - j][1] = t -> shape[i][j];
            if ((i - 2) == -2) temp.shape[3 - j][0] = t -> shape[i][j];
        }
    }

    *t = temp;
    check_move(t);

    if (t -> down && t -> right && t -> left) t -> rotate = TRUE;
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
    init_pair(3, COLOR_GREEN, COLOR_BLACK);

    curs_set(0);
    noecho();
    cbreak();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
    
    int offsety = (LINES - HEIGH_WINDOW) / 2;
    int offsetx = (COLS - WIDTH_WINDOW) / 2;

    win = newwin(HEIGH_WINDOW, WIDTH_WINDOW, offsety, offsetx);
    wattron(win, A_BOLD | COLOR_PAIR(1));

    win_info = newwin(HEIGH_WINDOW_INFO, WIDTH_WINDOW_INFO, offsety + 1, offsetx + GLASS_WIDTH + 4);
    wattron(win_info, A_BOLD | COLOR_PAIR(1));

    srand(time(NULL));

    int figure_number = rand() % 7;
    int next_figure_number = rand() % 7;

    figure_t fantom;

    game = (game_t *) malloc(sizeof(game_t));

    game -> next_tetramino = next_figure_number;
    game -> score = 0;
    game -> level = 0;
    game -> tick_till_down = SPEED_LEVELS[0];
    
    tetramino = (figure_t *) malloc(sizeof(figure_t));
    *tetramino = figures[figure_number];
    
    int ch;
    
    while((ch = getch()) != 'q') {
        tetramino -> prev_x = tetramino -> x;
        check_move(tetramino);
        switch(ch) {
            case KEY_LEFT:
                if (tetramino -> left) {
                    tetramino -> x--;
                    clean_figure();
                    paint_figure();
                }
                break;
            case KEY_RIGHT:
                if (tetramino -> right) {
                    tetramino -> x++;
                    clean_figure();
                    paint_figure();
                }
                break;
            case KEY_UP:
                fantom = *tetramino;
                rotate(&fantom);
                if (fantom.rotate) {
                    clean_figure();
                    *tetramino = fantom;
                    paint_figure();
                }
            case KEY_DOWN:
                step();
            default:
                break;
        }

        tick(tetramino);
        paint();
        usleep(10000);
    
    }
    
    getch(); 
    quit();

}
