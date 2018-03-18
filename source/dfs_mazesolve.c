#include "dfs_mazesolve.h"

// gets the bets possible direction for the cell to move onto
// by taking into account the possible directions and the
// distance from cell to end cell
// populates the dir array, where dir[0] is best direction (left, right, up, down)
// and dir[3] is worst direction 
void get_best_dir(int dir[4], int x, int y)
{
	int shortest_d[4] = {infinity, infinity, infinity, infinity}; 

	// eval left dir
	if (x - 1 >= 0 && (array[y][x-1] == pathc))
	{
		shortest_d[0] = man_distance(x - 1, y, endx, endy); 
		dir[0] = 1; 	
	}

	// eval right dir	
	if (x + 1 <= cols - 1 && (array[y][x+1] == pathc))
	{
		shortest_d[1] = man_distance(x + 1, y, endx, endy); 
		dir[1] = 2; 
	}
	
	// eval up dir	
    if (y - 1 >= 0 && (array[y-1][x] == pathc))
	{
		shortest_d[2] = man_distance(x, y - 1, endx, endy); 
		dir[2] = 3; 
	}
	
	// eval down dir 
	if (y + 1 <= rows-1 && (array[y+1][x] == pathc))
	{
		shortest_d[3] = man_distance(x, y + 1, endx, endy); 
		dir[3] = 4; 
	}

	// now all possible distances are in shortest_d. however, it is unsorted. 
	// it must be sorted in ascending order. At the same time, however, 
	// the direction must be kept track of, so the dir[] array must be sorted 
	// in the same manner as the shortest_d array
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

// starts maze solver depth first search 
void solve_maze()
{
	// set start path
	array[starty][startx] = solvc; 
	
	// prime the dfs_solve algorithm for jump starting it
	if (startx == 0) 
		dfs_solve(1, starty, 1);
	else
		dfs_solve(startx, starty + 1, 3); 
	
	// send end path
	array[endy][endx] = solvc; 
}

// solves the maze with depth first search, correcting itself if 
// dead end reached and brute forcing until finish reached. 
// returns true if end reached
// from: 1=left ; 2=right; 3=up; 4=down 
char dfs_solve(int currentX, int currentY, int from)
{
	array[currentY][currentX] = solvc; 
		
	if (solv_delay > 0) 
	{
		print_maze();
		usleep(solv_delay); 
	}

	if (currentX == endx && currentY == endy - 1) 
		return 1; 
	if (currentY == endy && currentX == endx - 1)
		return 1; 
	
	// populate dir array; holds best direction in lowest index
	int dir[4] = {infinity, infinity, infinity, infinity}; 
	get_best_dir(dir, currentX, currentY);		

	// loop through each possible move left, right, top, down 
	// moves are ordered according to their distance to end
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
				if (array[currentY][currentX - 2] != pathc) 
					continue; 
				
				// if not, move 1 cell left
				array[currentY][currentX - 1] = solvc; 
				
				// recurse deeper
				if (dfs_solve(currentX - 2, currentY, 2))
					return 1; 

				// if this code is reached, it means that the path that was previously
				// taking was a bad path (dead end). Thus, we must reverse the process
				// by setting current block back to empty path
				array[currentY][currentX - 1] = pathc; 
				array[currentY][currentX - 2] = pathc; 
				break;

			// go right
			case 2:
				// if best direction is the direction we came from, return
				if (from == 2)
					continue; 

				// check right bounds
				if (currentX + 2 > cols-1) 
					continue; 
				
				// check if path already taken
				if (array[currentY][currentX + 2] != pathc) 
					continue; 
				
				// if not, move 1 cell right
				array[currentY][currentX + 1] = solvc; 
				
				// recurse deeper
				if (dfs_solve(currentX + 2, currentY, 1))
					return 1; 
				
				// dead end, reverse
				array[currentY][currentX + 1] = pathc; 
				array[currentY][currentX + 2] = pathc; 
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
				if (array[currentY - 2][currentX] != pathc) 
					continue; 
				
				// if not, move 1 cell up
				array[currentY - 1][currentX] = solvc; 
				
				// recurse deeper
				if (dfs_solve(currentX, currentY - 2, 4))
					return 1; 

				// failed, go back
				array[currentY-1][currentX] = pathc; 
				array[currentY-2][currentX] = pathc; 
				break;

			// go down
			case 4: 
				// if best direction is the direction we came from, return
				if (from == 4)
					continue; 

				// check down bounds
				if (currentY + 2 > rows-1) 
					continue; 
				
				// check if path already taken
				if (array[currentY + 2][currentX] != pathc) 
					continue; 
				
				// if not, move 1 cell down
				array[currentY + 1][currentX] = solvc; 
				
				// recurse deeper
				if(dfs_solve(currentX, currentY + 2, 3))
					return 1; 
				
				// dead end, reverse
				array[currentY + 1][currentX] = pathc; 
				array[currentY + 2][currentX] = pathc; 
				break;
			default: 
				printf("Fatal error in direction calculation\n"); 
		}
	}
	return 0; 
}

