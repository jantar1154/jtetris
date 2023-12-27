#ifndef SAVE_H
#define SAVE_H

void save_append(int score);

void save_init(void);

void save_destroy(void);

int * get_save_data(int * n);

void save_print(void);

#endif // SAVE_H