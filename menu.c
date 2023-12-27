#include "main.h"
#include "render.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_rect.h>

typedef struct {
    int x;
    int y;
} pos;

pos mouse_pos = {0,0};
char mouse_in[100];

static TTF_Font * font;
static SDL_Renderer * renderer;
static SDL_Event * ev;

void click(int * q) {
    if (!strcmp(mouse_in, "start")) {
        *q = 1;
    } else if (!strcmp(mouse_in, "exit")) {
        *q = 1;
    } else if (!strcmp(mouse_in, "quit")) {
        destroy();
        exit(0);
    }
}

void menu_input(int * q) {
    switch(ev->type) {
        case SDL_QUIT:
            *q = 1;
            break;
        case SDL_MOUSEMOTION:
            SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
            break;
        case SDL_MOUSEBUTTONDOWN:
            click(q);
            break;
    }
}

void render_btn(const char * text, int width, int height, int y_offset) {
    SDL_Rect rect = {
        .x = WINDOW_WIDTH/2 - width/2,
        .y = y_offset,
        .w = width,
        .h = height
    };
    // If mouse hovers over button, bg is gray
    if (mouse_pos.x >= rect.x && mouse_pos.x <= rect.x + rect.w &&
        mouse_pos.y >= rect.y && mouse_pos.y <= rect.y + rect.h) {
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        strcpy(mouse_in, text);
    } else {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    }

    // Otherwise, bg is black
    SDL_RenderFillRect(renderer, &rect);

    if (!strlen(text)) return;
    SDL_Colour white = {255,255,255};
    int w = 30;
    rect.x = width - w*strlen(text)/2;
    rect.y += 10;
    rect.h -= 20;
    render_text(renderer, font, w, &white, &rect, text);
}

void main_menu(int * q) {
    strcpy(mouse_in, "none");
    // Background
    render_background(renderer);

    // Button start
    render_btn("start", 300, 100, 400);
    // Button quit
    render_btn("quit", 300, 100, 600);

    while (SDL_PollEvent(ev)) menu_input(q);

    SDL_RenderPresent(renderer);
}

void game_over_menu(int * q, int score) {
    // Render background
    render_background(renderer);

    char text[64] = "Game over!";
    SDL_Colour white = {255,255,255};
    SDL_Rect rect = {
        .x = WINDOW_WIDTH/2-30*strlen(text)/2,
        .y = 200,
        .w = 30*strlen(text),
        .h = 60
    };

    render_text(renderer, font, 30, &white, &rect, text);
    rect.y += 100;
    sprintf(text, "Score: %i", score);
    render_text (renderer, font, 30, &white, &rect, text);

    render_btn("exit", 300, 100, 400);

    while (SDL_PollEvent(ev)) menu_input(q);
    
    SDL_RenderPresent(renderer);
}

void menu_init(SDL_Renderer * r, SDL_Event * e, TTF_Font * f) {
    renderer = r;
    ev = e;
    font = f;
}