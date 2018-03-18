
//
//	file: source/mazesolve.c
//

#include "mazesolve.h"

// gets the bets possible direction for the cell to move onto
// by taking into account the possible directions and the
// distance from cell to mz.end cell
// populates the dir mz.array, where dir[0] is best direction (left, right, up, down)
// and dir[3] is worst direction 
void get_best_dir(int dir[4], int x, int y)
{
	int shortest_d[4] = {infinity, infinity, infinity, infinity}; 

	// eval left dir
	if (x - 1 >= 0 && (mz.array[y][x-1] == mz.pathc))
	{
		shortest_d[0] = man_distance(x - 1, y, mz.endx, mz.endy); 
		dir[0] = 1; 	
	}

	// eval right dir	
	if (x + 1 <= mz.cols - 1 && (mz.array[y][x+1] == mz.pathc))
	{
		shortest_d[1] = man_distance(x + 1, y, mz.endx, mz.endy); 
		dir[1] = 2; 
	}
	
	// eval up dir	
    if (y - 1 >= 0 && (mz.array[y-1][x] == mz.pathc))
	{
		shortest_d[2] = man_distance(x, y - 1, mz.endx, mz.endy); 
		dir[2] = 3; 
	}
	
	// eval down dir 
	if (y + 1 <= mz.rows-1 && (mz.array[y+1][x] == mz.pathc))
	{
		shortest_d[3] = man_distance(x, y + 1, mz.endx, mz.endy); 
		dir[3] = 4; 
	}

	// now all possible distances are in shortest_d. however, it is unsorted. 
	// it must be sorted in ascmz.ending order. At the same time, however, 
	// the direction must be kept track of, so the dir[] mz.array must be sorted 
	// in the same manner as the shortest_d mz.array
	for (int i = 1; i < 4; i++) 
	{
		int j = i; 
		while (j > 0 && shortest_d[j-1] > shortest_d[j]) 
		{
			int t = shortest_d[j]; 
			shortest_d[j] = shortest_d[j-1]; 
			shortest_d[j-1] = t; 

			int t2 = dir[j]; 
			dir[j] = dir[j-1]; 
			dir[j-1] = t2; 

			j--; 
		}		
	}
}

// mz.starts maze solver depth first search 
void solve_maze()
{
	// set mz.start path
	mz.array[mz.starty][mz.startx] = mz.solvc; 
	
	// prime the dfs_solve algorithm for jump mz.starting it
	if (mz.startx == 0) 
		dfs_solve(1, mz.starty, 1);
	else
		dfs_solve(mz.startx, mz.starty + 1, 3); 
	
	// smz.end mz.end path
	mz.array[mz.endy][mz.endx] = mz.solvc; 
}

// solves the maze with depth first search, correcting itself if 
// dead mz.end reached and brute forcing until finish reached. 
// returns true if mz.end reached
// from: 1=left ; 2=right; 3=up; 4=down 
char dfs_solve(int currentX, int currentY, int from)
{
	mz.array[currentY][currentX] = mz.solvc; 
		
	if (mz.solv_delay > 0) 
	{
		print_maze();
		usleep(mz.solv_delay); 
	}

	if (currentX == mz.endx && currentY == mz.endy - 1) 
		return 1; 
	if (currentY == mz.endy && currentX == mz.endx - 1)
		return 1; 
	
	// populate dir mz.array; holds best direction in lowest index
	int dir[4] = {infinity, infinity, infinity, infinity}; 
	get_best_dir(dir, currentX, currentY);		

	// loop through each possible move left, right, top, down 
	// moves are ordered according to their distance to mz.end
	// eg. dir[0] is best possible move, dir[1] is second best, dir[3] is worst move
	for (int d = 0; d < 4; d++) 
	{
		// switch on dir 
		switch (dir[d]) 
		{
			// go left 
			case 1: 
				// if best direction is the direction we came from, return
				if (from == 1)
					continue; 

				// check left bounds
				if (currentX - 2 <= 0) 
					continue; 
				
				// check if path already taken
				if (mz.array[currentY][currentX - 2] != mz.pathc) 
					continue; 
				
				// if not, move 1 cell left
				mz.array[currentY][currentX - 1] = mz.solvc; 
				
				// recurse deeper
				if (dfs_solve(currentX - 2, currentY, 2))
					return 1; 

				// if this code is reached, it means that the path that was previously
				// taking was a bad path (dead mz.end). Thus, we must reverse the process
				// by setting current block back to empty path
				mz.array[currentY][currentX - 1] = mz.pathc; 
				mz.array[currentY][currentX - 2] = mz.pathc; 
				break;

			// go right
			case 2:
				// if best direction is the direction we came from, return
				if (from == 2)
					continue; 

				// check right bounds
				if (currentX + 2 > mz.cols-1) 
					continue; 
				
				// check if path already taken
				if (mz.array[currentY][currentX + 2] != mz.pathc) 
					continue; 
				
				// if not, move 1 cell right
				mz.array[currentY][currentX + 1] = mz.solvc; 
				
				// recurse deeper
				if (dfs_solve(currentX + 2, currentY, 1))
					return 1; 
				
				// dead mz.end, reverse
				mz.array[currentY][currentX + 1] = mz.pathc; 
				mz.array[currentY][currentX + 2] = mz.pathc; 
				break;

			// go up
			case 3:		
				// if best direction is the direction we came from, return
				if (from == 3)
					continue; 

				// check top bounds
				if (currentY - 2 < 0) 
					continue; 
				
				// check if path already taken
				if (mz.array[currentY - 2][currentX] != mz.pathc) 
					continue; 
				
				// if not, move 1 cell up
				mz.array[currentY - 1][currentX] = mz.solvc; 
				
				// recurse deeper
				if (dfs_solve(currentX, currentY - 2, 4))
					return 1; 

				// failed, go back
				mz.array[currentY-1][currentX] = mz.pathc; 
				mz.array[currentY-2][currentX] = mz.pathc; 
				break;

			// go down
			case 4: 
				// if best direction is the direction we came from, return
				if (from == 4)
					continue; 

				// check down bounds
				if (currentY + 2 > mz.rows-1) 
					continue; 
				
				// check if path already taken
				if (mz.array[currentY + 2][currentX] != mz.pathc) 
					continue; 
				
				// if not, move 1 cell down
				mz.array[currentY + 1][currentX] = mz.solvc; 
				
				// recurse deeper
				if(dfs_solve(currentX, currentY + 2, 3))
					return 1; 
				
				// dead mz.end, reverse
				mz.array[currentY + 1][currentX] = mz.pathc; 
				mz.array[currentY + 2][currentX] = mz.pathc; 
				break;
	//		default: 
				//printf("Fatal error in direction calculation\n"); 
		}
	}
	return 0; 
}

