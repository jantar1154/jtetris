#ifndef RENDER_H
#define RENDER_H

#include "main.h"
#include <SDL2/SDL_ttf.h>

void render_field(SDL_Renderer * r, game_field * f);

void render_next_tetromino(SDL_Renderer * r, tetromino * t);

void render_tetromino(SDL_Renderer * r, tetromino * t);

void render_text(
    SDL_Renderer * r,
    TTF_Font * font,
    const int w,
    SDL_Colour * colour,
    SDL_Rect * rect,
    const char * text);

void render_level(SDL_Renderer * r, TTF_Font * font, int * lv);

void render_score(SDL_Renderer * r, long * score, TTF_Font * font);

void render_best_score(SDL_Renderer * r, TTF_Font * f);

void render_background(SDL_Renderer * r);

void render_init(SDL_Renderer * r);

void render_destroy(void);

#endif // RENDER_H