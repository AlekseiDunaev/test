#ifndef TETRIS_H
#define TETRIS_H

#include <stdbool.h>
#include <ncurses.h>

#define HEIGH_WINDOW 23
#define WIDTH_WINDOW 35
#define HEIGH_WINDOW_INFO 11
#define WIDTH_WINDOW_INFO 20
#define GLASS_HEIGHT 20
#define GLASS_WIDTH 10
#define NUM_TETRAMINO 7
#define TETRAMINO_POSITIONS 4
#define POINTS 4
#define SPAN 10
#define START_Y_POS 0
#define START_X_POS 4
#define FIGURE_SIZE 4
#define MAX_LEVEL 19

typedef struct {
	int y, x;
} point_t;

typedef struct {
	int type;
	int orientation;
    	int y, x, prev_y, prev_x;
	bool down, left, right, rotate;
} tetramino_t;

typedef struct {
    int next_tetramino;
    int score;
    int level;
    int tick_till_down;
    bool is_paint;
} game_t;

extern int SPEED_LEVELS[MAX_LEVEL + 1];
extern int LINES_BONUS[5];
extern point_t tetraminos[NUM_TETRAMINO][TETRAMINO_POSITIONS][POINTS];

WINDOW *win, *win_info;
game_t *game;
tetramino_t *tetramino;

extern int glass[GLASS_HEIGHT + 1][GLASS_WIDTH + 2];

void glass_shift(int line);
void set_start_glass();
void refresh_glass();
void fix_figure_in_glass();
void paint_figure();
void clean_figure(); 
void paint();
void check_move(tetramino_t *t);
void step();
void tick();
void rotate(tetramino_t *t);
void quit();

#endif //TETRIS_H
