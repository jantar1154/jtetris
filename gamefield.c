#include "main.h"
#include "gamefield.h"
#include "score.h"

// finds `game_tile` from `field` located on the same position as `t_tile`
game_tile * get_gtile(
    game_field * f, tetro_tile * t_tile,
    int x_offset, int y_offset) {
    if (!t_tile->active) return 0;
    for (int y = 0; y < TILE_H; ++y) {
        for (int x = 0; x < TILE_W; ++x) {
            game_tile * curr_g_tile = &f->tile_arr[y][x];
            if (curr_g_tile->pos_y == t_tile->pos_y + TILE_SIZE*y_offset &&
                curr_g_tile->pos_x == t_tile->pos_x + TILE_SIZE*x_offset) {
                return curr_g_tile;
            }
        }
    }
    return 0;
}

// Creates a copy of current field, creates new one and inserts old active
// tiles on it, but y higher 1
void move_tiles_down(game_field * f, const int y_limit) {
    game_field temp;
    memcpy(&temp, f, sizeof(game_field));
    // Erase everything in original field lower than `y`
    for (int y = 0; y < y_limit+1; ++y) {
        for (int x = 0; x < TILE_W; ++x) {
            game_tile * tile = &f->tile_arr[y][x];
            tile->has_tetro_tile = 0;
        }
    }

    // Copy everything from `temp` to `f` but one lower on y
    for (int y = 1; y < y_limit+1; ++y) {
        for (int x = 0; x < TILE_W; ++x) {
            game_tile * tile = &f->tile_arr[y][x];
            tetro_tile tet_tile = {
                .active = 1,
                .pos_x = tile->pos_x,
                .pos_y = tile->pos_y
            };
            game_tile * tile_above = get_gtile(&temp, &tet_tile, 0, -1);
            if (!tile_above) break;
            if (!tile_above->has_tetro_tile) continue;
            tile->has_tetro_tile = 1;
            tile->tetromino_tile = tile_above->tetromino_tile;
        }
    }
}

void check_filled_row(long * sc, game_field * f, int * lv) {
    int count = 0;
    int * y_vals = malloc(sizeof(int));
    for (int y = 0; y < TILE_H; ++y) {
        for (int x = 0; x < TILE_W; ++x) {
            game_tile * tile = &f->tile_arr[y][x];
            if (!tile->has_tetro_tile) break;
            if (x+1 != TILE_W) continue;
            y_vals[count] = y;
            ++count;
            y_vals = realloc(y_vals, (1+count) * sizeof(int));
        }
    }
    for (int i = 0; i < count; ++i) {
        move_tiles_down(f, y_vals[i]);
    }
    add_score(sc, lv, count);
    free(y_vals);
}