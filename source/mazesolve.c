
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
	// set appropriate start char if fancy unicode is on
	if (mz.fancyp) {

		// allocate space for the fancy path characters
		unipath = malloc(6 * sizeof(char*)); 

		// set the unipath character style. This is chosen from commandline
		static char* doubled[] = {"═", "║", "╔", "╗", "╚", "╝"}; 
		static char* thin[] = {"─", "│", "┌", "┐", "└", "┘"}; 
		static char* bold[] = {"━", "┃", "┏", "┓", "┗", "┛"}; 
		static char** styles[] = { bold, thin, doubled }; 

		unipath = styles[mz.fancyp - 1]; 

		if (mz.startx == 0)
			mz.array[mz.starty][0] = unipath[0]; 
		else if (mz.starty == 0) 
			mz.array[0][mz.startx] = unipath[1]; 
	}
	else 
		// set mz.start path
		mz.array[mz.starty][mz.startx] = mz.solvc; 
	
	// prime the dfs_solve algorithm for jump mz.starting it
	if (mz.startx == 0) 
		dfs_solve(1, mz.starty, 1);
	else
		dfs_solve(mz.startx, mz.starty + 1, 3); 
}

// places appropriate corner if fancy path is enabled, given from and to 
// from/to: 1=left ; 2=right; 3=up; 4=down 
void put_corner(int y, int x, char from, char to) {

	// convert from direction to unipath index
	if (from == RIGHT) from = I_RIGHT; 
	else if (from == LEFT) from = I_LEFT; 
	else if (from == DOWN) from = I_DOWN; 
	else if (from == UP) from = I_UP; 

	// find appropriate corner to place. Use clever pre-processor arithemtic to condense 
	// the cases into just 2 decisions. 
	if (from == I_LEFT || from == I_RIGHT) {
		if (to == LEFT) 
			mz.array[y][x] = unipath[I_HOR]; 
		else if (to == RIGHT) 
			mz.array[y][x] = unipath[I_HOR]; 
		else if (to == UP) 
			mz.array[y][x] = unipath[I_UP + from]; 
		else if (to == DOWN) 
			mz.array[y][x] = unipath[I_DOWN + from]; 
	}
	else if (from == I_UP || from == I_DOWN) {
		if (to == LEFT) 
			mz.array[y][x] = unipath[I_LEFT + from];
		else if (to == RIGHT) 
			mz.array[y][x] = unipath[I_RIGHT + from];
		else if (to == UP) 
			mz.array[y][x] = unipath[I_VERT]; 
		else if (to == DOWN) 
			mz.array[y][x] = unipath[I_VERT]; 		
	}
}	

// solves the maze with depth first search, correcting itself if 
// dead mz.end reached and brute forcing until finish reached. 
// returns true if mz.end reached
// from: 1=left ; 2=right; 3=up; 4=down 
char dfs_solve(int currentX, int currentY, int from)
{
	if (!mz.fancyp)
		mz.array[currentY][currentX] = mz.solvc; 
		
	if (mz.solv_delay > 0 && mz.fileout == 0) 
	{
		print_maze();
		usleep(mz.solv_delay); 
	}

	if (currentX == mz.endx && currentY == mz.endy - 1) 
	{
		if (mz.fancyp) 
		{ 
			put_corner(currentY, currentX, from, DOWN); 
			mz.array[mz.endy][mz.endx] = unipath[I_VERT]; 
		}
		else 	
			mz.array[mz.endy][mz.endx] = mz.solvc; 

		return 1; 
	}
	if (currentY == mz.endy && currentX == mz.endx - 1)
	{
		if (mz.fancyp)
	   	{
			put_corner(currentY, currentX, from, RIGHT); 
			mz.array[mz.endy][mz.endx] = unipath[I_HOR]; 
		}
		else 
			mz.array[mz.endy][mz.endx] = mz.solvc; 

		return 1; 
	}
	
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
			case LEFT: 
				// if best direction is the direction we came from, return
				if (from == LEFT)
					continue; 

				// check left bounds
				if (currentX - 2 <= 0) 
					continue; 
				
				// check if path already taken
				if (mz.array[currentY][currentX - 2] != mz.pathc) 
					continue; 
				
				// now we must move 2 cell to left. We know from cell = from and to cell = left
				if (!mz.fancyp) 	
					mz.array[currentY][currentX - 1] = mz.solvc; 
				else {
					put_corner(currentY, currentX, from, LEFT);			// place appropriate corner or horizontal / vertical
					mz.array[currentY][currentX - 1] = unipath[I_HOR];	// place horizontal since to is always left 
				}
			
				// recurse deeper
				if (dfs_solve(currentX - 2, currentY, RIGHT))
					return 1; 

				// if this code is reached, it means that the path that was previously
				// taking was a bad path (dead mz.end). Thus, we must reverse the process
				// by setting current block back to empty path
				mz.array[currentY][currentX - 1] = mz.pathc; 
				mz.array[currentY][currentX - 2] = mz.pathc; 
				break;

			// go right
			case RIGHT:
				// if best direction is the direction we came from, return
				if (from == RIGHT)
					continue; 

				// check right bounds
				if (currentX + 2 > mz.cols-1) 
					continue; 
				
				// check if path already taken
				if (mz.array[currentY][currentX + 2] != mz.pathc) 
					continue; 

				if (!mz.fancyp) 	
					mz.array[currentY][currentX + 1] = mz.solvc; 
				else {
					put_corner(currentY, currentX, from, RIGHT);		// place appropriate corner or horizontal / vertical
					mz.array[currentY][currentX + 1] = unipath[I_HOR];	// place horizontal since to is always right 
				}
				
				// recurse deeper
				if (dfs_solve(currentX + 2, currentY, LEFT))
					return 1; 
				
				// dead mz.end, reverse
				mz.array[currentY][currentX + 1] = mz.pathc; 
				mz.array[currentY][currentX + 2] = mz.pathc; 
				break;

			// go up
			case UP:		
				// if best direction is the direction we came from, return
				if (from == UP)
					continue; 

				// check top bounds
				if (currentY - 2 < 0) 
					continue; 
				
				// check if path already taken
				if (mz.array[currentY - 2][currentX] != mz.pathc) 
					continue; 
	
				// place path
				if (!mz.fancyp) 	
					mz.array[currentY - 1][currentX] = mz.solvc; 
				else {
					put_corner(currentY, currentX, from, UP);			// place appropriate corner or horizontal / vertical
					mz.array[currentY - 1][currentX] = unipath[I_VERT];	// place vertical since to is always up 
				}			

				// recurse deeper
				if (dfs_solve(currentX, currentY - 2, DOWN))
					return 1; 

				// failed, go back
				mz.array[currentY-1][currentX] = mz.pathc; 
				mz.array[currentY-2][currentX] = mz.pathc; 
				break;

			// go down
			case DOWN: 
				// if best direction is the direction we came from, return
				if (from == 4)
					continue; 

				// check down bounds
				if (currentY + 2 > mz.rows-1) 
					continue; 
				
				// check if path already taken
				if (mz.array[currentY + 2][currentX] != mz.pathc) 
					continue; 
				
				// place path
				if (!mz.fancyp) 	
					mz.array[currentY + 1][currentX] = mz.solvc; 
				else {
					put_corner(currentY, currentX, from, DOWN);			// place appropriate corner or horizontal / vertical
					mz.array[currentY + 1][currentX] = unipath[I_VERT];	// place vertical since to is always down 
				}			

				// recurse deeper
				if(dfs_solve(currentX, currentY + 2, UP))
					return 1; 
				
				// dead mz.end, reverse
				mz.array[currentY + 1][currentX] = mz.pathc; 
				mz.array[currentY + 2][currentX] = mz.pathc; 
				break;
		}
	}
	return 0; 
}

