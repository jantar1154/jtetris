#include "h/render.h"
#include <SDL2/SDL_ttf.h>

// Renders tile borders and `game_tile`s
void render_field(SDL_Renderer * r, game_field * f) {
    for (int y = 0; y < TILE_H; ++y) {
        for (int x = 0; x < TILE_W; ++x) {
            SDL_SetRenderDrawColor(r, 100, 100, 100, 255);
            game_tile * tile = &f->tile_arr[y][x];
            SDL_Rect rect = {
                .x = tile->pos_x,
                .y = tile->pos_y,
                .w = TILE_SIZE,
                .h = TILE_SIZE
            };
            SDL_RenderDrawRect(r, &rect);

            if (!tile->has_tetro_tile) continue;
            SDL_Colour * c = &tile->tetromino_tile.colour;
            // Base colour
            SDL_SetRenderDrawColor(r, c->r, c->g, c->b, c->a);
            SDL_RenderFillRect(r, &rect);
            // Borders
            SDL_SetRenderDrawColor(r, c->r/3, c->g/3, c->b/3, c->a/3);
            SDL_RenderDrawRect(r, &rect);
        }
    }
}

// Renders currently falling tetromino
void render_tetromino(SDL_Renderer * r, tetromino * t) {
    SDL_Rect rect;
    // Top tiles
    for (int i = 0; i < 8; ++i) {
        tetro_tile * tile = &t->tiles[i];
        if (!tile->active) continue;
        SDL_Colour * c = &tile->colour;
        rect.x = tile->pos_x;
        rect.y = tile->pos_y;
        rect.h = rect.w = TILE_SIZE;
        SDL_SetRenderDrawColor(r, c->r, c->g, c->b, c->a);
        SDL_RenderFillRect(r, &rect);
        // Draw outline
        SDL_SetRenderDrawColor(r, c->r/3, c->g/3, c->b/3, c->a);
        SDL_RenderDrawRect(r, &rect);
    }
}

// https://mrlvsb.github.io/upr-skripta/c/aplikovane_ulohy/sdl/kresleni.html#text
void render_text(
    SDL_Renderer * r,
    TTF_Font * font,
    SDL_Colour * colour,
    SDL_Rect * rect,
    const char * text) {
    
    SDL_Surface * s = TTF_RenderText_Solid(font, text, *colour);
    SDL_Texture * t = SDL_CreateTextureFromSurface(r, s);

    int text_length = strlen(text);
    rect->w = 22 * text_length;

    SDL_RenderCopy(r, t, NULL, rect);

    SDL_DestroyTexture(t);
    SDL_FreeSurface(s);
}

void render_score(SDL_Renderer * r, long * score, TTF_Font * font) {
    // Border rectangle
    SDL_Rect rect1 = {
        .x = OFFSET_X + TILE_W*TILE_SIZE + 10,
        .y = OFFSET_Y + 50,
        .w = 380,
        .h = 50
    };
    SDL_Rect rect2 = rect1;
    SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
    SDL_RenderDrawRect(r, &rect1);

    // Text
    SDL_Colour clr = {255, 255, 255};
    char text[30];

    rect1.x += 10;
    snprintf(text, 30, "Score: ");
    render_text(r, font, &clr, &rect1, text);

    snprintf(text, 30, "%li", *score);
    rect2.x = (rect2.x + rect2.w) - 22*strlen(text) - 10; // Align right
    render_text(r, font, &clr, &rect2, text);
}