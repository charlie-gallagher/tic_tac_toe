#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <tic_cpu.h>

/* Function declarations */
#include "tic.h"


/* Initialize globals */
char winner = '\0', x = '\0', o = '\0'; // Winner, X, and O
int n_players;

int (*player_one)(int turn, char mark);
int (*player_two)(int turn, char mark);

/* Mark struct defined in tic.h */
struct mark board[9];  // Allocate space for board; initialize to zero



int main(void)
{
	int i = 0;
	int move;

	srand(time(0)); // Set seed for random numbers

	/* Initialize board and run start menu */
	board_init();
	start_menu();

	i = 0;
	while (i < 9) {
		move = player_one(i, 'X');
		if (move == -1) break;		// Break if error
		if (enter_move(i, move, 'X') == -1) break; // Enter move
		print_board();
		if (moderator()) break;

		if (i == 8) break;
		i++;


		move = player_two(i, 'O');
		if (move == -1) break;		// Break if error
		if (enter_move(i, move, 'O') == -1) break; // Enter move
		print_board();
		if (moderator()) break;

		i++;
	}

	/* Summarize results of game */
	summary();
}

/* Initialize game board */
void board_init(void) 
{
	int i;
	int row_index[9] = {0, 0, 0, 1, 1, 1, 2, 2, 2};
	int col_index[9] = {0, 1, 2, 0, 1, 2, 0, 1, 2};
	int diag_index[9] = {0, -1, 1, -1, 2, -1, 1, -1, 0};

	/* Initialize marks to spaces for proper printing */
	for (i = 0; i < 9; i++) {
		(board+i)->mark = ' ';
		(board+i)->row = row_index[i];
		(board+i)->col = col_index[i];
		(board+i)->diag = diag_index[i];
	}
}

/* Run start menu */
void start_menu(void) 
{
	char p1 = '\0';

	printf("Welcome to Tic Tac Toe!\n");
	
	/* Get number of players */ 
	while (n_players < 1 || n_players > 2) {
		printf("How many players? ");
		scanf("%d", &n_players);
		getchar();  // clears buffer of newline

		if (n_players < 1 || n_players > 2) 
			printf("\nPlease choose a number between 1 and 2\n");
	}

	/* Assign roles */ 
	while (p1 != 'X' && p1 != 'O') {
		printf("Would you like to play as X or O? ");
		scanf("%c", &p1);
		getchar(); // clears buffer of newline

		if (n_players < 1 || n_players > 2) 
			printf("\nPlease select either X or O\n");
	}


	/* Assign functions to pointers player_one and player_two, set x, o */
	if (p1 == 'X') {
		player_one = user;
		x = 'p';  // For debugging only

		if (n_players == 1) {
			player_two = cpu;
			o = 'c';  // For debugging only
		}
		else {
			player_two = user;
			o = 'q';  // For debugging only
		}
	}
	else {
		if (n_players == 1) {
			player_one = cpu;
			x = 'c';  // For debugging only
		}
		else {
			player_one = user;
			x = 'q';  // For debugging only
		}
		player_two = user;
		o = 'p';  // For debugging only
	}
}


// Check move for validity and enter on board
int enter_move(int turn, int move, char mark) 
{
	if ((board+move)->mark != ' ') {
		fprintf(stderr, 
	"System error: attempting to play a move that has already been played\n");
		return -1;
	}
	(board+move)->turn = turn;
	(board+move)->mark = mark;
	return 0;
}


/* User interface
 *
 * Inputs a turn and a mark and returns a move on the board. 
 * The move is (supposed to be) guaranteed to be valid. 
 *
 */
int user(int turn, char mark) 
{ 
	int move;
	int valid = 0;
	
	while ( !valid ) {
		printf("Coordinate (from 11 to 33): ");
		scanf("%d", &move);

		/* check for valid coordinate; decode move */
		valid = 1;
		switch (move) {
			case 11: {
					move = 0;
					break;
				 }
			case 12: {
					move = 1;
					break;
				 }
			case 13: {
					move = 2;
					break;
				 }
			case 21: {
					move = 3;
					break;
				 }
			case 22: {
					move = 4;
					break;
				 }
			case 23: {
					move = 5;
					break;
				 }
			case 31: {
					move = 6;
					break;
				 }
			case 32: {
					move = 7;
					break;
				 }
			case 33: {
					move = 8;
					break;
				 }
			default: {
					printf("Invalid coordinate\n\n");
					valid = 0;
					break;
				 }
		}

		if (valid && (board+move)->mark != ' ') {
			printf("That move has already been played\n\n");
			valid = 0;
		}
	}


	// Double-check output
	if ((board+move)->mark != ' ') {
		fprintf(stderr, "System error: Player is about to make illegal move.\n");
		return -1;
	}

	
	return move;
}



/* CPU move-maker
 *
 * Inputs turn and mark and returns a (hopefully) valid move. Moves are
 * double-checked for validity in the move output function. 
 *
 * Most of the functionality of the CPU is written in the file tic_cpu.c.
 * Still, the functions are implemented here for consistency. 
 *
 * As of March 5, the CPU generates a random move first. Then, it goes through
 * a series of heuristics to check for a better option. If it finds one, then
 * the random move is replaced by a heuristic move. The actual heuristics are
 * based on the strategy on the Wikipedia page for Tic-Tac-Toe. Probably, 
 * the heuristics should be further exported to the CPU file tic_cpu.c, but
 * for now it's just a series of options. 
 *
 * As long as there are no problems with the heuristics, it will be impossible
 * for the CPU to play the random move it generated. 
 *
 * UPDATE: Moved the heuristics to a function in the tic_cpu.c file. 
 */
int cpu(int turn, char mark) 
{
	int i;
	int rand_move, move;  // randomly generated position

	printf("\nComputer is thinking...\n");
	
	/* Choose random move 
	 * Using (%9) gives you remainders zero through eight
	 */
	rand_move = (rand() % 9);
	while ((board+rand_move)->mark != ' ') {
		rand_move = (rand() % 9);
	}

	/* Find possible moves */
	int possible_moves[9], n_moves;

	n_moves = find_moves(board, possible_moves);
	printf("Found %d possible moves\n", n_moves);


	/* Heuristics section -------------------- */
	move = cpu_heuristics(board, n_moves, possible_moves, mark);

	// Return move or random move
	if (move == -1) move = rand_move;
	return move;
}



/* Find open moves */
int find_moves(struct mark *board, int moves[]) 
{
	int i;
	int *pmoves;
	pmoves = moves;

	for (i = 0; i < 9; i++) {
		if ((board+i)->mark == ' ') {
			*pmoves = i;
			pmoves++;
		}
	}

	// Return the number of available moves
	// I don't love this, but it does work
	return (int) (pmoves - moves);
}




/* REWRITING MODERATOR FUNCTION */

/* Moderator helper functions and arrays for indexing all rows/cols/diags */

int row(int init, int index) {
	return init + index;
}

int col(int init, int index) {
	return init + 3 * index;
}

int diag(int init, int index) {
	if (init == 0) return index * 4;
	else if (init == 2) return 2 + 2*index;
	else return -1;
}


int moderator(void)
{
	
	int irows[3] = {0, 3, 6};
	int icols[3] = {0, 1, 2};
	int idiags[3] = {0, 2, -1};

	/* Check for winner in rows, cols, and diags */
	if (check_win(irows, row) == 1) return 1;
	else if (check_win(icols, col) == 1) return 1;
	else if (check_win(idiags, diag) == 1) return 1;

	return 0;
}

/* Check for winner in given trio */
int check_win(int trio[], int (*trio_f)(int init, int index)) 
{
	int i, j;
	int checksum = 0;

	for (i = 0; i < 3; i++) {
		checksum = 0;  // Reset checksum
		for (j = 0; j < 3; j++) {
			// Since there are only 2 diagonals, third value is -1
			if (trio[i] == -1) break; 

			checksum += (board + trio_f(trio[i], j))->mark;
		}

		switch (checksum) {
			case 264: {
					winner = 'X';
					return 1;
				  }
			case 237: {
					winner = 'O';
					return 1;
				  }
			default: break;
		}
	}
	return 0;
}


/* Summary functions */
void print_board(void)
{
	printf("|-----------|\n");
	printf("| %c | %c | %c |\n", board->mark, (board+1)->mark, 
			(board+2)->mark);
	printf("|-----------|\n");
	
	printf("| %c | %c | %c |\n", (board+3)->mark, (board+4)->mark, 
			(board+5)->mark);
	printf("|-----------|\n");
	
	printf("| %c | %c | %c |\n", (board+6)->mark, (board+7)->mark, 
			(board+8)->mark);
	printf("|-----------|\n");
}

/* Generate summary of end of game */
void summary(void)
{	
	printf("\n\n~~~~~~~~~~~~~\n");
	if (winner != '\0') 
	{
		printf("Winner is %c! ", winner);
		/* Add 'you won' if player won */
		putchar('\n');
	}

	else printf("It's a draw!\n");
	printf("~~~~~~~~~~~~~\n");
}



