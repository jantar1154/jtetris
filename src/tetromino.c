#include "render.h"
#include "gamefield.h"
#include "tetromino.h"

// Converts `char *` to values to be put to `bot_row` and `top_row`
void str_to_row(const char str[8], layout * lo) {
    // Top row
    for (int i = 0; i < 4; ++i) {
        lo->top_row[i] = str[i] - '0';
    }
    // Bottom row
    for (int i = 0; i < 4; ++i) {
        lo->bot_row[i] = str[4+i] - '0';
    }
}

// Chooses a random layout for the tetromino tiles
// TODO: make more layouts
void init_tetro_tiles(tetromino * tet) {
    layout * lo = malloc(sizeof(layout));
    switch (rnd(0, 3)) {
        case 0:
            str_to_row("01100110", lo);
            break;
        case 1:
            str_to_row("11101000", lo);
            break;
        case 2:
            str_to_row("01110001", lo);
            break;
        case 3:
            str_to_row("00001111", lo);
            break;
    }
    // Set active tiles based on values in `lo`
    // Top row
    for (int i = 0; i < 4; ++i) {
        tetro_tile * tile = &tet->tiles[i];
        tile->active = lo->top_row[i];
        tile->pos_x = tet->pos_x + (i * TILE_SIZE);
        tile->pos_y = tet->pos_y;
        tile->rel_x = i-1;
        tile->rel_y = 1;
    }
    // Bottom row
    for (int i = 0; i < 4; ++i) {
        tetro_tile * tile = &tet->tiles[i+4];
        tile->active = lo->bot_row[i];
        tile->pos_x = tet->pos_x + (i * TILE_SIZE);
        tile->pos_y = tet->pos_y + TILE_SIZE;
        tile->rel_x = i-1;
        tile->rel_y = 0;
    }

    SDL_Colour rand_col;
    rand_col.r = rnd(50,255);
    rand_col.g = rnd(50,255);
    rand_col.b = rnd(50,255);

    for (int i = 0; i < 8; ++i) {
        tetro_tile * tile = &tet->tiles[i];
        memcpy(&tile->colour, &rand_col, sizeof(SDL_Colour));
        tile->colour.a = 255;
    }
    free(lo);
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
void rotate_tetromino(SDL_Renderer * r, game_field * f, tetromino * t) {
    tetromino temp;
    memcpy(&temp, t, sizeof(tetromino));
    // Creates a temporary rotated tetromino and checks if it there is an
    // obstacle, otherwise rotates `t`.
    for (int i = 0; i < 8; ++i) {
        tetro_tile * tile = &temp.tiles[i];
        int old_y = tile->rel_y;
        tile->rel_y = tile->rel_x;
        tile->rel_x = 1-old_y;
        update_tetro_tiles(&temp);

        // Check for baked tiles
        game_tile * gt = get_gtile(f, tile, 0, 0);
        if (gt && gt->has_tetro_tile) return;

        // Check for edges
        if (tile->pos_x >= OFFSET_X + TILE_W * TILE_SIZE) return;
        if (tile->pos_y >= OFFSET_Y + TILE_H * TILE_SIZE) return;
        if (tile->pos_x <= OFFSET_X+1) return;
    }

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

// Transfers tiles from tetromino into `game_field`,
// deletes tetromino, runs check for filled row
void bake_tiles(tetromino * t, game_field * f) {
    for (int i = 0; i < 8; ++i) {
        tetro_tile * current_tile = &t->tiles[i];
        game_tile * gtile = get_gtile(f, current_tile, 0, 0);
        if (!gtile) continue; 
        gtile->has_tetro_tile = 1;
        memcpy(&gtile->tetromino_tile, current_tile, sizeof(tetro_tile));
    }
    check_filled_row(f);
}