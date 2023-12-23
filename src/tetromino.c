#include "render.h"
#include "tetromino.h"

// finds `game_tile` from `field` located on the same position as `t_tile`
game_tile * get_gtile(game_field * f, tetro_tile * t_tile, int x_offset, int y_offset) {
    if (!t_tile->active) return 0;
    for (int y = 0; y < TILE_H; ++y) {
        for (int x = 0; x < TILE_W; ++x) {
            game_tile * curr_g_tile = &f->tile_arr[y][x];
            if (curr_g_tile->pos_y == t_tile->pos_y+TILE_SIZE*y_offset &&
                curr_g_tile->pos_x == t_tile->pos_x+TILE_SIZE*x_offset) {
                return curr_g_tile;
            }
        }
    }
    return 0;
}

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

// Moves tetromino
void move_tetromino(game_field * f, tetromino * t, int x_offset, int y_offset) {
    y_offset *= TILE_SIZE;
    x_offset *= TILE_SIZE;

    // Iterates through every tile in `t` until it sees obstacle
    // And stops tetromino from moving towards the obstacle
    for (int i = 0; i < 8; ++i) {
        tetro_tile * tile = &t->tiles[i];
        game_tile * gt = get_gtile(f, tile, x_offset/TILE_SIZE, 0);
        if (!tile->active) continue;
        if (gt && gt->has_tetro_tile) x_offset = 0;
        if (tile->pos_x + x_offset >= OFFSET_X + TILE_W*TILE_SIZE) {
            x_offset = 0;
        } else if (tile->pos_x + x_offset <= OFFSET_X - TILE_SIZE) {
            x_offset = 0;
        }
    }

    t->pos_y += y_offset;
    t->pos_x += x_offset;
    update_tetro_tiles(t);
}