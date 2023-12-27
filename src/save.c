#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int score;
    int position;
} save_data;

save_data * data;

#define MAX_SAVES 5
#define SAVE_PATH "save.txt"

// Sorts scores so that largest score is on position 1
void sort_data(save_data * d) {
    for (int i = 0; i < MAX_SAVES; ++i) {
        for (int j = i+1; j < MAX_SAVES; ++j) {
            save_data * d1 = &d[i];
            save_data * d2 = &d[j];
            if (d1->score > d2->score) continue;
            int temp = d1->score;
            d1->score = d2->score;
            d2->score = temp;
        }
    }
}

// Prints all `data` (DEBUG)
void save_print(void) {
    for (int i = 0; i < MAX_SAVES; ++i) {
        printf("[%i] %i\n", data[i].position, data[i].score);
    }
}

int * get_save_data(int * n) {
    int * rtn = malloc(sizeof(int) * MAX_SAVES);
    for (int i = 0; i < MAX_SAVES; ++i) {
        rtn[i] = data[i].score;
    }
    *n = MAX_SAVES;
    return rtn;
}

// Returns pointer to a smallest score in `data`
int *get_smallest_score(void) {
    int smallest = 99999999;
    int * rtn;
    for (int i = 0; i < MAX_SAVES; ++i) {
        int * score = &data[i].score;
        if (*score < smallest) {
            smallest = *score;
            rtn = score;
        }
    }
    if (smallest == 99999999) return 0;
    return rtn;
}

// Appends new score into the `savefile` file
// May delete low scores
void save_append(int score) {
    int * lowest = get_smallest_score();
    if (score <= *lowest) return;
    *lowest = score;
    // Write to file
    FILE * savefile = fopen(SAVE_PATH, "w");
    sort_data(data);
    for (int i = 0; i < MAX_SAVES; ++i) {
        fprintf(savefile, "%i,%i\n", data[i].position, data[i].score);
    }
    fclose(savefile);
}

// Initialises `save` system
void save_init() {
    data = (save_data*) malloc(sizeof(save_data) * MAX_SAVES);
    // Init `save_data`
    for (int i = 0; i < MAX_SAVES; ++i) {
        data[i].score = 0;
        data[i].position = i+1;
    }
    FILE * savefile = fopen(SAVE_PATH, "r");
    if (!savefile) return;

    // Load data from `savefile` into `save_data`
    char * line = malloc(sizeof(char) * 100);
    for (int i = 0; i < MAX_SAVES; ++i) {
        if (!fgets(line, 100, savefile)) break;
        char * tk = strtok(line, ",");
        int pos = strtol(tk, NULL, 10);
        tk = strtok(NULL, ",");
        int s = strtol(tk, NULL, 10);
        data[i].position = pos;
        data[i].score = s;
    }
    free(line);
    fclose(savefile);
}

// Frees up everything after `save_init`
void save_destroy() {
    free(data);
}