#include "render.h"
#include "save.h"
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

SDL_Texture * background_texture;

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

void render_next_tetromino(SDL_Renderer * r, tetromino *t) {
    SDL_SetRenderDrawColor(r, 50, 50, 50, 255);
    SDL_Rect rect = {
        .x = t->pos_x - 10,
        .y = t->pos_y - 10,
        .h = 2*TILE_SIZE + 10,
        .w = 4*TILE_SIZE + 10
    };
    SDL_RenderFillRect(r, &rect);
    render_tetromino(r, t);
}

// Renders tetromino
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
    const int w,
    SDL_Colour * colour,
    SDL_Rect * rect,
    const char * text) {
    
    SDL_Surface * s = TTF_RenderText_Solid(font, text, *colour);
    SDL_Texture * t = SDL_CreateTextureFromSurface(r, s);

    int text_length = strlen(text);
    rect->w = w * text_length;

    SDL_RenderCopy(r, t, NULL, rect);

    SDL_DestroyTexture(t);
    SDL_FreeSurface(s);
}

// Prints current level
void render_level(SDL_Renderer * r, TTF_Font * font, int * lv) {
    SDL_Rect rect1 = {
        .x = OFFSET_X + TILE_W*TILE_SIZE + 5,
        .y = OFFSET_Y + 260,
        .w = 220,
        .h = 50
    };
    SDL_Rect rect2 = rect1;

    SDL_SetRenderDrawColor(r, 40, 40, 40, 255);
    SDL_RenderFillRect(r, &rect1);
    SDL_SetRenderDrawColor(r, 210, 210, 210, 255);
    SDL_RenderDrawRect(r, &rect1);

    // Text
    SDL_Colour clr = {255, 255, 255};
    char text[30];

    rect1.x += 10;
    snprintf(text, 30, "Lv: ");
    render_text(r, font, 22, &clr, &rect1, text);

    snprintf(text, 30, "%i", *lv);
    rect2.x = (rect2.x + rect2.w) - 22*strlen(text) - 10; // Align right
    render_text(r, font, 22, &clr, &rect2, text);
}

void render_score(SDL_Renderer * r, long * score, TTF_Font * font) {
    // Border rectangle
    SDL_Rect rect1 = {
        .x = OFFSET_X + TILE_W*TILE_SIZE + 5,
        .y = OFFSET_Y + 180,
        .w = 220,
        .h = 50
    };
    SDL_Rect rect2 = rect1;
    SDL_SetRenderDrawColor(r, 40, 40, 40, 255);
    SDL_RenderFillRect(r, &rect1);
    SDL_SetRenderDrawColor(r, 210, 210, 210, 255);
    SDL_RenderDrawRect(r, &rect1);

    // Text
    SDL_Colour clr = {255, 255, 255};
    char text[30];

    rect1.x += 10;
    // snprintf(text, 30, "Score: ");
    // render_text(r, font, &clr, &rect1, text);

    snprintf(text, 30, "%li", *score);
    rect2.x = (rect2.x + rect2.w) - 22*strlen(text) - 10; // Align right
    render_text(r, font, 22, &clr, &rect2, text);
}

void render_best_score(SDL_Renderer *r, TTF_Font * font) {
    SDL_Rect rect = {
        .x = OFFSET_X + TILE_SIZE*TILE_W + 5,
        .y = OFFSET_Y + 350,
        .h = 30,
        .w = 220
    };
    SDL_SetRenderDrawColor(r, 20, 20, 20, 255);
    SDL_RenderFillRect(r, &rect);
    int n = 0;
    int * save_data = get_save_data(&n);
    SDL_Colour white = {255, 255, 255};
    for (int i = 0; i < n; ++i) {
        char txt[32];
        snprintf(txt, 32, "%i", save_data[i]);
        render_text(r, font, 25, &white, &rect, txt);
        rect.y += 35;
    }

    free(save_data);
}

// renders background image
void render_background(SDL_Renderer * r) {
    SDL_RenderCopy(r, background_texture, NULL, NULL);
}

// Destroys everything allocated on heap
void render_destroy(void) {
    SDL_DestroyTexture(background_texture);
}


// Initialises global variables
void render_init(SDL_Renderer * r) {
    // Load background
    char * bg_path = "background.jpeg";
    SDL_Surface * background_surface = IMG_Load(bg_path);
    if (!background_surface) return;
    background_texture = SDL_CreateTextureFromSurface(r, background_surface);
    SDL_FreeSurface(background_surface);
}