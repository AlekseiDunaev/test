#ifndef TETRIS_H
#define TETRIS_H

//#include <stdlib.h>
#include <stdbool.h>

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

extern int SPEED_LEVELS[MAX_LEVEL + 1];

extern int LINES_BONUS[5];

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
    bool is_paint;
    bool game_over;
} game_t;

extern figure_t figures[];

extern int glass[GLASS_HEIGHT + 1][GLASS_WIDTH + 2];

WINDOW *win, *win_info;
game_t *game;
figure_t *tetramino;

void glass_shift(int line);
void set_start_glass();
void refresh_glass();
void fix_figutre_in_glass();
void paint_figure();
void clean_figure(); 
void paint();
void check_move(figure_t *t);
void step();
void tick();
void rotate(figure_t *t);
void quit();

#endif //TETRIS_H
