#pragma once
#ifndef MAIN_H
#define MAIN_H

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// SETTINGS
// OG Tetris has 10Hx20W tiles
#define TILE_W 10
#define TILE_H 20
#define TILE_SIZE 28
#define OFFSET_X 80
#define OFFSET_Y 10

#define WINDOW_WIDTH 768
#define WINDOW_HEIGHT 576

typedef struct {
    int top_row[4];
    int bot_row[4];
} layout;

typedef struct {
    int active; // `0` - inactive, `1` - active
    int pos_x, pos_y;
    // Position (in number of tiles) relative to the top left corner
    // of parent tetromino
    int rel_x, rel_y;
    SDL_Colour colour;
} tetro_tile;

typedef struct {
    tetro_tile tiles[8];
    float pos_x, pos_y;
} tetromino;

typedef struct {
    int pos_x, pos_y;
    // Bool, if there is a `tetro_tile` baked into this `game_tile`
    int has_tetro_tile;
    tetro_tile tetromino_tile;
} game_tile;

// An array of tiles
typedef struct {
    game_tile tile_arr[TILE_H][TILE_W];
} game_field;

#endif // MAIN_H