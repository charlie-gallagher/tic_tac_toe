#ifndef TICCPU
#define TICCPU
#include "tic.h"

int try_to_win(struct mark *board, char mark);
int two_and_blank(struct mark *board, char mark,
		int trio[], int (*trio_f)(int init, int index));

int cpu_win(struct mark *board, int move, char mark);
int cpu_twos(struct mark *board, int move, char mark);
int cpu_heuristics(struct mark *board, int n_moves, int *possible_moves, char mark);
#endif
