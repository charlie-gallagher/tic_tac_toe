#include <stdio.h>
#include <stdlib.h>
#include "tic_cpu.h"
#include "tic.h"


/* If two in a row of some mark are found, return the position that
 * will win the game for that mark. 
 *
 * Note that the try_to_win function works equally well for blocking a 
 * win by the opposing player. 
 *
 * These return winning move if a row contains two marks and a space 
 * Otherwise, return -1
 *
 */

/* Three useful indices of rows, columns, and diagonals */
static int rows[3][3] = {
	{0, 1, 2},
	{3, 4, 5},
	{6, 7, 8}
};
static int cols[3][3] = {
	{0, 3, 6},
	{1, 4, 7},
	{2, 5, 8}
};
static int diags[2][3] = {
	{0, 4, 8},
	{2, 4, 6}
};


/* Heuristics -----------------------*/
int cpu_heuristics(struct mark *board, int n_moves, int *possible_moves, char mark)
{
	int i;
	int corners[4] = {0, 2, 6, 8};
	int sides[4] = {1, 3, 5, 7};
	char other_mark;

	/* Get other player's mark */
	if (mark == 'X') other_mark = 'O';
	else other_mark = 'X';


	
	// If only one move, make that move
	if (n_moves == 1) {
		return possible_moves[0];
	}
	
	// Try to win 
	for (i = 0; i < n_moves; i++) {
		if (cpu_win(board, possible_moves[i], mark) == 1) {
			return possible_moves[i];
		}
	}

	// Try to block 
	for (i = 0; i < n_moves; i++) {
		if (cpu_win(board, possible_moves[i], other_mark) == 1) {
			return possible_moves[i];
		}
	}

	// Try to make a fork
	for (i = 0; i < n_moves; i++) {
		if (cpu_twos(board, possible_moves[i], mark) == 2) {
			return possible_moves[i];
		}
	}

	// Block opponents fork
	for (i = 0; i < n_moves; i++) {
		if (cpu_twos(board, possible_moves[i], other_mark) == 2) {
			return possible_moves[i];
		}
	}

	// Try to make two in a trio, with the other one blank
	for (i = 0; i < n_moves; i++) {
		if (cpu_twos(board, possible_moves[i], mark) == 1) {
			return possible_moves[i];
		}
	}


	// Try to play in the center
	if ((board+4)->mark == ' ') {
		return 4;
	}

	/* Defaults: First open corner, then first open side */

	// Try to play in a corner
	for (i = 0; i < 4; i++) {
		if ((board+corners[i])->mark == ' ') {
			return corners[i];
		}
	}

	// Try to play a side
	for (i = 0; i < 4; i++) {
		if ((board+sides[i])->mark == ' ') {
			return sides[i];
		}
	}


	return -1;
}




/* Check if move is a winning move
 *
 * The current move argument is guaranteed to be blank. Then, 
 * all that needs to be done is check the other two moves for 
 * being the same mark.
 *
 * Consider converting this to a bunch of checksums and a single
 * loop through them
 */
int cpu_win(struct mark *board, int move, char mark) 
{
	extern int rows[3][3];
	extern int cols[3][3];
	extern int diags[2][3];
	int row, col, diag;
	int checksum;
	int i;

	row = (board + move)->row; // Possible values: 0, 1, 2
	col = (board + move)->col; // Possible values: 0, 1, 2
	diag = (board + move)->diag; // Possible values: -1, 0, 1, 2

	/* Check row */
	checksum = 0;
	for (i = 0; i < 3; i++) {
		checksum += ((board + rows[row][i])->mark == mark);
	}

	if (checksum == 2) return 1;
	

	/* Check col */
	checksum = 0;
	for (i = 0; i < 3; i++) {
		checksum += ((board + cols[col][i])->mark == mark);
	}

	if (checksum == 2) return 1;


	/* Check diag 
	 *
	 * Diagonals are tricker, because the center square is crossed
	 * by both digaonals. The center square thus has a diagonal 
	 * of "2", instead of -1 (none), 0 (top left), or 1 (top right)
	 */
	checksum = 0;
	if (diag == 0 || diag == 1) {
		for (i = 0; i < 3; i++)
			checksum += ((board + diags[diag][i])->mark == mark);

		if (checksum == 2) return 1;
	}
	else if (diag == 2) {
		for (i = 0; i < 3; i++)
			checksum += ((board + diags[0][i])->mark == mark);

		if (checksum == 2) return 1;

		checksum = 0;
		for (i = 0; i < 3; i++) 
			checksum += ((board + diags[1][i])->mark == mark);

		if (checksum == 2) return 1;
	}

	return -1;
}

/*
int cpu_twos(struct mark *board, int move, char mark);
*/

/* Returns one if move would be one of two of the same mark in
 * a trio, the other being blank
 */
int cpu_twos(struct mark *board, int move, char mark) 
{
	
	extern int rows[3][3];
	extern int cols[3][3];
	extern int diags[2][3];
	int row, col, diag;
	int marks, blanks;
	int i;
	int out = 0;	// Output, number of two-in-a-rows made
			// by move passed by move

	row = (board + move)->row; // Possible values: 0, 1, 2
	col = (board + move)->col; // Possible values: 0, 1, 2
	diag = (board + move)->diag; // Possible values: -1, 0, 1, 2

	/* Check row */
	blanks = marks = 0;
	for (i = 0; i < 3; i++) {
		marks += ((board + rows[row][i])->mark == mark);
		blanks += ((board + rows[row][i])->mark == ' ');
	}

	if (blanks == 2 && marks == 1) out++;

	/* Check cols */
	blanks = marks = 0;
	for (i = 0; i < 3; i++) {
		marks += ((board + cols[col][i])->mark == mark);
		blanks += ((board + cols[col][i])->mark == ' ');
	}

	if (blanks == 2 && marks == 1) out++;


	/* Check diagonals */
	blanks = marks = 0;
	if (diag == 0 || diag == 1) {
		for (i = 0; i < 3; i++) {
			marks += ((board + diags[diag][i])->mark == mark);
			blanks += ((board + diags[diag][i])->mark == ' ');
			
		}
		if (blanks == 2 && marks == 1) out++;

	}
	else if (diag == 2) {
		blanks = marks = 0;
		for (i = 0; i < 3; i++) {
			marks += ((board + diags[0][i])->mark == mark);
			blanks += ((board + diags[0][i])->mark == ' ');
		}

		if (blanks == 2 && marks == 1) out++;


		blanks = marks = 0;
		for (i = 0; i < 3; i++) {
			marks += ((board + diags[1][i])->mark == mark);
			blanks += ((board + diags[1][i])->mark == ' ');
		}

		if (blanks == 2 && marks == 1) out++;
	}

	return out;
}


