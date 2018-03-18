
//
//	file: header/mazesolve.h
//

#ifndef MAZESOLVE_H_
#define MAZESOLVE_H_

#include "maze.h"
#include "helpers.h"

#define infinity 999999

// direction pre-processors
// from/to: 1=left ; 2=right; 3=up; 4=down 
#define LEFT 1
#define RIGHT 2
#define UP 3
#define DOWN 4

// unipath index pre-processors
#define I_HOR 0
#define I_VERT 1
#define I_DOWN 2
#define I_UP 4
#define I_RIGHT 0
#define I_LEFT 1


// array of array of characters for fancy unicode maze path
// 0: horizontal;  1: vertical;  2: down-right;  3: down-left;  4: up-right;  5: up-left
char** unipath; 

// starts dfs maze solve procedure
void solve_maze();

// places appropriate corner if fancy path is enabled, given from and to 
// from/to: 1=left ; 2=right; 3=up; 4=down 
void put_corner(int y, int x, char from, char to);

// gets the bets possible direction for the cell to move onto
// by taking into account the possible directions and the
// distance from cell to end cell
// dir [index] 
// 0=left; 1=right; 2=up; 3=down 
void get_best_dir(int dir[4], int x, int y);


// solves the maze with depth first search, correcting itself if 
// dead end reached and brute forcing until finish reached. 
// returns true if end reached
char dfs_solve(int currentX, int currentY, int from);


#endif
