#ifndef TICTACTOE
#define TICTACTOE

// Mark structure; each move is a mark
struct mark {
	int row;
	int col;
	int diag;
	char mark;
	char player;
	int turn;	
};

int row(int init, int index);
int col(int init, int index);
int diag(int init, int index);



/* Function declarations */
void start_menu(void);
void board_init(void);

// board positions
int enter_move(int turn, int move, char mark);

// Player and computer moves
int user(int turn, char mark);
/* For more computer moves, see the file tic_cpu.c */
int cpu(int turn, char mark);
int find_moves(struct mark *board, int moves[]);

// Moderator
int moderator(void);
int check_win(int trio[], int (*trio_f)(int init, int index));
int check_rows(void);
int check_cols(void);
int check_diag(void);

// End of game
void summary(void);
void print_board(void);


#endif
