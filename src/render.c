#include "h/render.h"

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