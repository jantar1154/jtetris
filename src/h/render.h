#ifndef RENDER_H
#define RENDER_H

#include "main.h"
#include <SDL2/SDL_ttf.h>

void render_field(SDL_Renderer * r, game_field * f);

void render_tetromino(SDL_Renderer * r, tetromino * t);

void render_score(SDL_Renderer * r, long * score, TTF_Font * font);

void render_background(SDL_Renderer * r);

void render_init(SDL_Renderer * r);

void render_destroy();

#endif // RENDER_H