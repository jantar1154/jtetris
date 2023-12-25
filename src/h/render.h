#ifndef RENDER_H
#define RENDER_H

#include "main.h"

void render_field(SDL_Renderer * r, game_field * f);

void render_tetromino(SDL_Renderer * r, tetromino * t);

#endif // RENDER_H