#ifndef DFS_MAZESOLVER_H_
#define DFS_MAZESOLVER_H_

#define infinity 999999

// starts dfs maze solve procedure
void solve_maze();


// inserts shortest distance into array, orderd from least to greatest a[0] being the least
// and a[n] the greatest
// loops through array, checks if array distance is longer than new distance
// if it is, inserts shortest distance into array with shifting like this:
// example: insert_dir({1, 2, 3, 4}, 1, 4) 
// output: {1, 4, 2, 3} (4 was inserted into pos 1) 
// final array will be ordered from least to greatest
void calc_shortest(int dir[4], int shortest_d[4], int x, int y, int move);


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


#include "dfs_mazesolve.c"
#endif
