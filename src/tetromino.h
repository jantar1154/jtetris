#ifndef TETROMINO_H
#define TETROMINO_H
#include "main.h"

game_tile * get_gtile(
    game_field * f,
    tetro_tile * t_tile,
    int x_offset,
    int y_offset);

void update_tetro_tiles(tetromino * t);

void rotate_tetromino(SDL_Renderer * r, tetromino * t);

void move_tetromino(game_field * f, tetromino * t, int x_offset, int y_offset);

#endif // TETROMINO_H