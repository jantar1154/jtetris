#ifndef TETROMINO_H
#define TETROMINO_H
#include "main.h"

void update_tetro_tiles(tetromino * t);

void rotate_tetromino(SDL_Renderer * r, tetromino * t);

#endif // TETROMINO_H