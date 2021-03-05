# Tic-Tac-Toe
This is a basic tic-tac-toe program where the computer will never lose. The 
computer follows a set of heuristic rules that make strategically almost perfect. 

### Build
There's nothing special about this build. 

```
gcc -o tic tic.c tic_cpu.c
```

### Demonstration
Simply call the program and the game will start. You can choose 1 or 2 players,
and to start as X or O. After each turn, the board is printed. I made a fair
attempt to handle most user errors, but nothing complete. 

```
~$ ./tic

Welcome to Tic Tac Toe!
How many players? 1
Would you like to play as X or O? O

Computer is thinking...
Found 9 possible moves
Taking center
|-----------|
|   |   |   |
|-----------|
|   | X |   |
|-----------|
|   |   |   |
|-----------|
```

The user types in coordinates from 11 to 33, each corresponding to a coordinate
counting from the top left. So 11 is top-left, 13 is top-right, and 22 is
center, and so on. 


---
Charlie Gallagher, 2021
