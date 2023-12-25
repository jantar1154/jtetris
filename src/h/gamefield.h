#ifndef GAMEFIELD_H
#define GAMEFIELD_H

#include "main.h"

game_tile * get_gtile(
    game_field * f, tetro_tile * t_tile,
    int x_offset, int y_offset);

void check_filled_row(long * sc, game_field * f);

#endif // GAMEFIELD_H