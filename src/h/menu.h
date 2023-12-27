#ifndef MENU_H
#define MENU_H

void main_menu(int * q);

void game_over_menu(int * q, int score);

void menu_init(SDL_Renderer * r, SDL_Event * e, TTF_Font * f);

#endif // MENU_H