#include "h/score.h"

void add_score(long * score, int level, const int rows) {
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
    *score += level * multiplier;
}