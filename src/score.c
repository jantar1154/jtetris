#include "h/score.h"
#include "h/main.h"

void add_score(long * score, int * level, const int rows) {
    int multiplier = 1;
    switch (rows) {
        case 0:
            return;
        case 1:
            multiplier = 40;
            break;
        case 2:
            multiplier = 100;
            break;
        case 3:
            multiplier = 300;
            break;
        case 4:
        default:
            multiplier = 1200;
            break;
    }
    *score += *level * multiplier;
    change_limit(0.05 * (rows/2));
    if (0 == *score % (100*(1+(int)(*level/3)))) add_level();
}