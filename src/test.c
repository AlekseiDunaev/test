#include <ncurses.h> 
#include <stdlib.h>

#include <string.h>
#include <unistd.h>
#include <time.h>

#include "tetris.h"

void init_ncurses() {

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
    
}

int main(int argc, char** argv) {

    init_ncurses();

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

    game = (game_t *)malloc(sizeof(game_t));

    game -> next_tetramino = next_figure_number;
    game -> score = 0;
    game -> level = 0;
    game -> tick_till_down = SPEED_LEVELS[0];
    game -> is_paint = FALSE;
    
    tetramino = (figure_t *)malloc(sizeof(figure_t));
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
                    game -> is_paint = TRUE;
                }
                break;
            case KEY_RIGHT:
                if (tetramino -> right) {
                    tetramino -> x++;
                    clean_figure();
                    paint_figure();
                    game -> is_paint = TRUE;
                }
                break;
            case KEY_UP:
                fantom = *tetramino;
                rotate(&fantom);
                if (fantom.rotate) {
                    clean_figure();
                    *tetramino = fantom;
                    paint_figure();
                    game -> is_paint = TRUE;
                }
            case KEY_DOWN:
                step();
            default:
                break;
        }

        tick(tetramino);

        if (game -> is_paint) {
            paint();
            game -> is_paint = FALSE;
        }

        usleep(10000);
    
    }
    
    getch(); 
    quit();

    return 0;

}
