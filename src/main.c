#include "main.h"
#include "input.h"
#include "render.h"

SDL_Window * window;
SDL_Renderer * renderer;
SDL_Event event;

Uint64 last;
double delta_time;
float internal_clock;
long rng_seed = 12421421;
int ff = 0; // Fast forward

tetromino * tet; // A tetromino that is currenty falling

game_field field;

// Prints `message` to `stdout` and exits with code `1`
void err(const char * message) {
    fprintf(stderr, "%s\n", message);
    exit(1);
}

// Generates a random integer in the range of `from` and `to`
int rnd(const int from, const int to) {
    int seed = time(NULL) + rng_seed;
    srand(seed);

    int num = from + rand() % (1+to-from);
    rng_seed = seed;
    return num;
}

// Initialises game field
void init_game_field() {
    for (int y = 0; y < TILE_H; ++y) {
        for (int x = 0; x < TILE_W; ++x) {
            game_tile * this_tile = &field.tile_arr[y][x];
            this_tile->has_tetro_tile = 0;
            this_tile->pos_x = OFFSET_X + x * TILE_SIZE;
            this_tile->pos_y = OFFSET_Y + y * TILE_SIZE;
        }
    }
}

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
        default:
            err("Invalid random at tetro tile initialisation");
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
    rand_col.r = rnd(150,255);
    rand_col.g = rnd(150,255);
    rand_col.b = rnd(150,255);

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

// Creates and initialises a new tetromino and places it into the top center
void spawn_tetromino() {
    // Free because there was a memory leak after spawning more
    // than one tetrmino
    free(tet);
    tet = malloc(sizeof(tetromino));
    tet->pos_x = OFFSET_X + (TILE_SIZE * (TILE_W/2-1));
    tet->pos_y = OFFSET_Y;
    init_tetro_tiles(tet);
    update_tetro_tiles(tet);
}

// Initialisations
void init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        err("Could not initialise SDL video");
    }
    Uint32 window_flags =
        SDL_WINDOW_SHOWN |
        SDL_WINDOW_OPENGL
    ;
    Uint32 renderer_flags =
        SDL_RENDERER_PRESENTVSYNC |
        SDL_RENDERER_ACCELERATED
    ;
    window = SDL_CreateWindow(
        "Tetris",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        window_flags
    );
    renderer = SDL_CreateRenderer(
        window,
        -1,
        renderer_flags
    );
    last = SDL_GetPerformanceCounter();
    internal_clock = 0.0f;
    // init `game_field`
    init_game_field();
    spawn_tetromino();
}

// finds `game_tile` from `field` located on the same position as `t_tile`
game_tile * get_gtile(tetro_tile * t_tile, int x_offset, int y_offset) {
    if (!t_tile->active) return 0;
    for (int y = 0; y < TILE_H; ++y) {
        for (int x = 0; x < TILE_W; ++x) {
            game_tile * curr_g_tile = &field.tile_arr[y][x];
            if (curr_g_tile->pos_y == t_tile->pos_y+TILE_SIZE*y_offset &&
                curr_g_tile->pos_x == t_tile->pos_x+TILE_SIZE*x_offset) {
                return curr_g_tile;
            }
        }
    }
    return 0;
}
// Moves tetromino
void move_tetromino(tetromino * t, int x_offset, int y_offset) {
    y_offset *= TILE_SIZE;
    x_offset *= TILE_SIZE;

    // Iterates through every tile in `t` until it sees obstacle
    // And stops tetromino from moving towards the obstacle
    for (int i = 0; i < 8; ++i) {
        tetro_tile * tile = &t->tiles[i];
        game_tile * gt = get_gtile(tile, x_offset/TILE_SIZE, 0);
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
void bake_tiles() {
    for (int i = 0; i < 8; ++i) {
        tetro_tile * current_tile = &tet->tiles[i];
        game_tile * gtile = get_gtile(current_tile, 0, 0);
        if (!gtile) continue; 
        gtile->has_tetro_tile = 1;
        memcpy(&gtile->tetromino_tile, current_tile, sizeof(tetro_tile));
    }
    spawn_tetromino();
}

// Checks for obstacles under tetromino
void check_collision() {
    // Iterate through every tile in tetromino and check
    // if there is a baked tile or bottom of play field
    for (int i = 0; i < 8; ++i) {
        tetro_tile * current_tile = &tet->tiles[i];
        game_tile * gtile_under = get_gtile(current_tile, 0, 1);
        if (current_tile->pos_y+TILE_SIZE >= OFFSET_Y + TILE_H * TILE_SIZE) {
            bake_tiles();
            return;
        } else if (gtile_under && gtile_under->has_tetro_tile) {
            bake_tiles();
            return;
        }
    }
}

// Runs every frame, handles logic and NO graphics
void update() {
    internal_clock += delta_time;
    // call two times every second
    if (internal_clock > 0.5f || ff) {
        internal_clock = 0;
        move_tetromino(tet, 0, 1);
        check_collision();
    }
}

// Runs every frame, handles ONLY graphics
void render() {
    // Clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Render tile borders and tiles
    render_field(renderer, &field);

    // Render all tiles in tetromino
    render_tetromino(renderer, tet);

    // Swap buffers
    SDL_RenderPresent(renderer);
}

// Calculates delta time
void calc_delta() {
    Uint64 now = SDL_GetPerformanceCounter();
    delta_time = (double) ((now-last) / (double)SDL_GetPerformanceFrequency());
    last = now;
}

// Rotates tetromino 90 degrees
void rotate_tetromino(tetromino * t) {
    for (int i = 0; i < 8; ++i) {
        tetro_tile * tile = &t->tiles[i];
        int old_y = tile->rel_y;
        tile->rel_y = tile->rel_x;
        tile->rel_x = 1-old_y;
    }
    update_tetro_tiles(tet);
    render_tetromino(renderer, tet);
}

// Frees up everything allocated on the heap
void destroy() {
    free(tet);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

// MAIN
int main () {
    init();

    int quit = 0;

    // Game loop
    while (!quit) {
        while (SDL_PollEvent(&event))
            handle_input(tet, &event, &quit, &ff);
        calc_delta();
        update();
        render();
    }

    destroy();
    return 0;
}