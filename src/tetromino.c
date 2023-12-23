#include "render.h"
#include "tetromino.h"

// Refreshes positions of tiles in `tetromino`
void update_tetro_tiles(tetromino * t) {
    for (int i = 0 ; i < 8; ++i) {
        tetro_tile * tile = &t->tiles[i];
        tile->pos_x = t->pos_x + tile->rel_x * TILE_SIZE;
        tile->pos_y = t->pos_y + tile->rel_y * TILE_SIZE;
    }
}

// Rotates tetromino 90 degrees
void rotate_tetromino(SDL_Renderer * r, tetromino * t) {
    for (int i = 0; i < 8; ++i) {
        tetro_tile * tile = &t->tiles[i];
        int old_y = tile->rel_y;
        tile->rel_y = tile->rel_x;
        tile->rel_x = 1-old_y;
    }
    update_tetro_tiles(t);
    render_tetromino(r, t);
}