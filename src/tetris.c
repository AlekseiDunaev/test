//#include <ncurses.h> 
#include <stdlib.h>

#include <string.h>
#include <unistd.h>

#include "tetris.h"

int SPEED_LEVELS[MAX_LEVEL + 1] = {
//  0,   1,  2,  3,  4,  5,  6,  7,  8,  9
    50, 48, 46, 44, 42, 40, 38, 36, 34, 32,
//  10, 11, 12, 13, 14, 15, 16, 17, 18, 19
    30, 28, 26, 24, 22, 20, 16, 12, 8,  4
};

int LINES_BONUS[5] = {0, 100, 200, 400, 800 };

point_t tetraminos[NUM_TETRAMINO][TETRAMINO_POSITIONS][POINTS] = {

	{{{0, 1},{1, 0},{1, 1},{1, 2}},
	{{0, 1},{1, 0},{1, 1},{2, 1}},
	{{1, 0},{1, 1},{1, 2},{2, 1}},
	{{0, 1},{1, 1},{1, 2},{2, 1}}},

	{{{0, 1},{1, 1},{2, 0},{2, 1}},
	{{1, 0},{1, 1},{1, 2},{2, 2}},
	{{0, 1},{0, 2},{1, 1},{2, 1}},
	{{0, 0},{1, 0},{1, 1},{1, 2}}},

	{{{0, 1},{1, 0},{1, 1},{2, 0}},
	{{0, 0},{0, 1},{1, 1},{1, 2}},
	{{0, 1},{1, 0},{1, 1},{2, 0}},
	{{0, 0},{0, 1},{1, 1},{1, 2}}},
	
	{{{0, 1},{1, 1},{1, 2},{2, 2}},
	{{0, 1},{0, 2},{1, 0},{1, 1}},
	{{0, 1},{1, 1},{1, 2},{2, 2}},
	{{0, 1},{0, 2},{1, 0},{1, 1}}},
	
	{{{0, 1},{1, 1},{2, 1},{3, 1}},
	{{1, 0},{1, 1},{1, 2},{1, 3}},
	{{0, 1},{1, 1},{2, 1},{3, 1}},
	{{1, 0},{1, 1},{1, 2},{1, 3}}},

	{{{0, 1},{1, 1},{2, 1},{2, 2}},
	{{0, 2},{1, 0},{1, 1},{1, 2}},
	{{0, 0},{0, 1},{1, 1},{2, 1}},
	{{1, 0},{1, 1},{1, 2},{2, 0}}},

	{{{0, 0},{0, 1},{1, 0},{1, 1}},
	{{0, 0},{0, 1},{1, 0},{1, 1}},
	{{0, 0},{0, 1},{1, 0},{1, 1}},
	{{0, 0},{0, 1},{1, 0},{1, 1}}},
	
};

int glass_init[GLASS_HEIGHT + 1][GLASS_WIDTH + 2] = {
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

int glass[GLASS_HEIGHT + 1][GLASS_WIDTH + 2];

void set_start_glass() {
    for (int i = 0; i < GLASS_HEIGHT + 1; i++) {
        for (int j = 0; j < GLASS_WIDTH + 2; j++) {
            glass[i][j] = glass_init[i][j];
        }
    }

}

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
            i++;
        }
    }
    game -> score += LINES_BONUS[lines];
    game -> is_paint = TRUE;
}

void fix_figure_in_glass() {
   
    for(int i = 0; i < FIGURE_SIZE; i++) {
	    glass[tetraminos[tetramino -> type][tetramino -> orientation][i].y + tetramino -> y] \
                [tetraminos[tetramino -> type][tetramino -> orientation][i].x + tetramino -> x + 1]  = 1;
    }

    refresh_glass();

}

void paint_figure() {

    for(int i = 0; i < FIGURE_SIZE; i++) {
	wmove(win, tetraminos[tetramino -> type][tetramino -> orientation][i].y + (tetramino -> y) + 1, \
	tetraminos[tetramino -> type][tetramino -> orientation][i].x + (tetramino -> x) + 2);
	waddch(win, ' ' | A_REVERSE | COLOR_PAIR(2));
    }
    
    wrefresh(win);
    wrefresh(win_info);

    tetramino -> prev_y = tetramino -> y;
    tetramino -> prev_x = tetramino -> x;
}

void clean_figure() {

    for(int i = 0; i < FIGURE_SIZE; i++) {
	wmove(win, tetraminos[tetramino -> type][tetramino -> orientation][i].y + (tetramino -> prev_y) + 1, \
	tetraminos[tetramino -> type][tetramino -> orientation][i].x + (tetramino -> prev_x) + 2);
	waddch(win, ' ');
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
        }
    }

    wclear(win_info);
 
    wmove(win_info, 1, 1);
    waddstr(win_info, "NEXT");

    for(int i = 0; i < FIGURE_SIZE; i++) {
	wmove(win_info, tetraminos[game -> next_tetramino][0][i].y + 1, \
	tetraminos[game -> next_tetramino][0][i].x + 6);
	waddch(win_info, ' ' | A_REVERSE | COLOR_PAIR(3));
    }
 
    wmove(win_info, 5, 1);
    waddstr(win_info, "SCORE");
 
    wmove(win_info, 5, 7);
    wattron(win_info, COLOR_PAIR(2));
    wprintw(win_info, "%d", (game -> score));
    wattroff(win_info, COLOR_PAIR(2));

	wmove(win_info, 7, 1);
	waddstr(win_info, "QUIT: q");
	wmove(win_info, 8, 1);
	waddstr(win_info, "PAUSED: p");
	wmove(win_info, 9, 1);
	waddstr(win_info, "CHANGE FIGURE: c");
    
	wattron(win_info, A_BOLD | COLOR_PAIR(1));

    box(win, 0, 0);
    box(win_info, 0, 0);

    wrefresh(win);
    wrefresh(win_info);

}

void check_move(tetramino_t *t) {

    t -> down = TRUE;
    t -> left = TRUE;
    t -> right = TRUE;
    t -> rotate = FALSE;

    for(int i = TETRAMINO_POSITIONS - 1; i >= 0; i--) {
	if (glass[(t -> y) + tetraminos[t -> type][t -> orientation][i].y] \
		[(t -> x) + tetraminos[t -> type][t -> orientation][i].x + 1]) {
		t -> down = FALSE;
	};
	if (glass[(t -> y) + tetraminos[t -> type][t -> orientation][i].y] \
		[(t -> x) + tetraminos[t -> type][t -> orientation][i].x + 2]) {
		t -> right = FALSE;
	};
	if (glass[(t -> y) + tetraminos[t -> type][t -> orientation][i].y] \
		[(t -> x) + tetraminos[t -> type][t -> orientation][i].x]) {
		t -> left = FALSE;
	};
    }

}

void step() {

    tetramino -> y++;
    check_move(tetramino);

    if (tetramino -> down) {
        clean_figure();
        paint_figure();
        game -> is_paint = FALSE;
    } else {
        tetramino -> y--;
        fix_figure_in_glass();

        tetramino -> type = game -> next_tetramino;
        tetramino -> orientation = 0;
        tetramino -> y = START_Y_POS;
        tetramino -> x = START_X_POS;
        game -> next_tetramino = rand() % 7;
    }

}

void tick() {
    game -> tick_till_down--;
    if (!(game -> tick_till_down)) {
        step();
        game -> tick_till_down = SPEED_LEVELS[game -> level];
    }
}

void rotate(tetramino_t *t) {
    (t -> orientation == 3) ? t -> orientation = 0: t -> orientation++;

    check_move(t);

    if (t -> down && t -> right && t -> left) t -> rotate = TRUE;
}

void quit() {

    clear();
    printw("Bay!");
    refresh();
    sleep(1);

    delwin(win);
    delwin(win_info);

    free(tetramino);
    free(game);

    endwin();
}

