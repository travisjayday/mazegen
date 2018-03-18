
//
//	file: header/mazesolve.h
//

#ifndef MAZESOLVE_H_
#define MAZESOLVE_H_

#include "maze.h"
#include "helpers.h"

#define infinity 999999

// starts dfs maze solve procedure
void solve_maze();

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
