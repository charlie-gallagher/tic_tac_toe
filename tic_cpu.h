#ifndef TICCPU
#define TICCPU
#include "tic.h"

int cpu_win(struct mark *board, int move, char mark);
int cpu_twos(struct mark *board, int move, char mark);
int cpu_heuristics(struct mark *board, int n_moves, int *possible_moves, char mark);
#endif
