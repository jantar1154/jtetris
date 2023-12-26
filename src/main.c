#include "h/main.h"
#include "h/input.h"
#include "h/render.h"
#include "h/tetromino.h"
#include <SDL2/SDL_ttf.h>

SDL_Window * window;
SDL_Renderer * renderer;
SDL_Event event;

long game_score = 0;

Uint64 last;
double delta_time;
float internal_clock;
long rng_seed = 12421421;
int ff = 0; // Fast forward
TTF_Font * font;

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

// Creates and initialises a new tetromino and places it into the top center
void spawn_tetromino() {
    // tet = realloc(tet, sizeof(tetromino));
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
    TTF_Init();
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
    tet = malloc(sizeof(tetromino));

    char * path = "asset/font/impact.ttf";
    font = TTF_OpenFont(path, 48);
    if (!font) {
        fprintf(stderr, "Could not load path (%s)!\n", path);
        exit(1);
    }

    render_init(renderer);

    // init `game_field`
    init_game_field();
    spawn_tetromino();
}

// Checks for obstacles under tetromino
void check_collision() {
    // Iterate through every tile in tetromino and check
    // if there is a baked tile or bottom of play field
    for (int i = 0; i < 8; ++i) {
        tetro_tile * current_tile = &tet->tiles[i];
        game_tile * gtile_under = get_gtile(&field, current_tile, 0, 1);
        if (!current_tile->active) continue;
        if (current_tile->pos_y >= OFFSET_Y + (TILE_H-1) * TILE_SIZE) {
            bake_tiles(&game_score, tet, &field);
            spawn_tetromino();
            ff = 0;
            return;
        } else if (gtile_under && gtile_under->has_tetro_tile) {
            bake_tiles(&game_score, tet, &field);
            spawn_tetromino();
            ff = 0;
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
        move_tetromino(&field, tet, 0, 1);
        check_collision();
    }
}

// Runs every frame, handles ONLY graphics
void render() {
    // Clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Background image
    render_background(renderer);

    // Render tile borders and tiles
    render_field(renderer, &field);

    // Render all tiles in tetromino
    render_tetromino(renderer, tet);

    render_score(renderer, &game_score, font);

    // Swap buffers
    SDL_RenderPresent(renderer);
}

// Calculates delta time
void calc_delta() {
    Uint64 now = SDL_GetPerformanceCounter();
    delta_time = (double) ((now-last) / (double)SDL_GetPerformanceFrequency());
    last = now;
}

// Frees up everything allocated on the heap
void destroy() {
    free(tet);
    render_destroy();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
}

// MAIN
int main () {
    init();

    int quit = 0;

    // Game loop
    while (!quit) {
        while (SDL_PollEvent(&event))
            handle_input(renderer, &field, tet, &event, &quit, &ff);
        calc_delta();
        update();
        render();
    }

    destroy();
    return 0;
}