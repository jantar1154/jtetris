#include "input.h"

// Handles events
void handle_input(tetromino * t, SDL_Event * e, int * quit, int * ff) {
    switch (e->type) {
        // Handle quitting the program
        case SDL_QUIT:
            *quit = 1;
            break;
        case SDL_KEYDOWN:
            switch (e->key.keysym.sym) {
                case SDLK_q:
                    *quit = 1;
                    break;
                case SDLK_UP:
                    rotate_tetromino(t);
                    break;
                case SDLK_LEFT:
                case SDLK_a:
                    move_tetromino(t, -1, 0);
                    break;
                case SDLK_RIGHT:
                case SDLK_d:
                    move_tetromino(t, 1, 0);
                    break;
                case SDLK_DOWN:
                case SDLK_s:
                    *ff = 1;
                    break;
            }
            break;
        case SDL_KEYUP:
            switch (e->key.keysym.sym) {
                case SDLK_DOWN:
                case SDLK_s:
                    *ff = 0;
                    break;
            }
            break;
    }
}