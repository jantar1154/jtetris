#ifndef INPUT_H
#define INPUT_H

#include "main.h"

void handle_input(
    SDL_Renderer * r,
    game_field * f,
    tetromino * t,
    SDL_Event * e,
    int * quit,
    int * ff);

#endif // INPUT_H