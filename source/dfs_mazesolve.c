
// inserts shortest distance into array
// loops through array, checks if array distance is longer than new distance
// if it is, inserts shortest distance into array with shifting like this:
// example: insert_dir({1, 2, 3, 4}, 1, 4) 
// output: {1, 4, 2, 3} (4 was inserted into pos 1) 
// final array will be ordered from least to greatest
void calc_shortest(int dir[4], int shortest_d[4], int x, int y, int move)
{
	// calc distance 
	int d = man_distance(x, y, endx, endy); 

	for (int j = 0; j < 4; j++)
	{
		// loop through shortest distances, if new distance is shorter
		// than any elems of the arr, then add it to the arr
		// and shift all array elems to the right
		if (shortest_d[j] > d) 
		{
			int p = j; 	// copy position j for later
			// shift elements to the right of p (position)
			// to one to the right and move d into original p
			// basically updating least distance array orderly
			for (int i = 2; i >= 0 && 2 - p >= 0; i--, p++)
			{
				dir[i+1] = dir[i];
				shortest_d[i+1] = shortest_d[i]; 
			}
			dir[j] = move;
			shortest_d[j] = d;
			break;	
		}
	}
}	

// gets the bets possible direction for the cell to move onto
// by taking into account the possible directions and the
// distance from cell to end cell
// dir [index] 
// 0=left ; 1=right; 2=up; 3=down 
void get_best_dir(int dir[4], int x, int y)
{
	int shortest_d[4] = {infinity, infinity, infinity, infinity}; 

	// eval left dir
	if (x -1 >= 0 && (array[y][x-1] == pathc))
		calc_shortest(dir, shortest_d, x-1, y, 1); 

	// eval right dir	
	if (x + 1 <= cols-1 && (array[y][x+1] == pathc))
		calc_shortest(dir, shortest_d, x+1, y, 2); 
	
	// eval up dir	
       	if (y-1 >= 0 && (array[y-1][x] == pathc))
		calc_shortest(dir, shortest_d, x, y-1, 3); 
	
	// eval down dir 
	if (y + 1 <= rows-1 && (array[y+1][x] == pathc))
		calc_shortest(dir, shortest_d, x, y+1, 4); 
}

// starts maze solver depth first search 
void solve_maze()
{
	// send start path
	array[starty][startx] = solvc; 
	
	// prime the dfs_solve algorithm for jump starting it
	if (startx == 0) 
		dfs_solve(startx+1, starty, 1);
	else
		dfs_solve(startx, starty+1, 3); 
	
	// send end path
	array[endy][endx] = solvc; 
}

// solves the maze with depth first search, correcting itself if 
// dead end reached and brute forcing until finish reached. 
// returns true if end reached
char dfs_solve(int currentX, int currentY, int from)
{
	array[currentY][currentX] = solvc; 
		
	if (solv_delay > 0) 
	{
		print_maze();
		usleep(solv_delay); 
	}

	if (endy == rows && currentX == endx && currentY == endy-1) 
	       	return 1; 
	if (endx == cols && currentY == endy && currentX == endx-1) 
		return 1; 

	int dir[4] = {infinity, infinity, infinity, infinity}; 
	get_best_dir(dir, currentX, currentY);		
	// loop through each possible move left, right, top, down 
	// moves are ordered according to their distance to end
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
				if (array[currentY][currentX-2] != pathc) 
					continue; 
				
				// if not, move 1 cell left
				array[currentY][currentX-1] = solvc; 
				
				// recurse deeper
				if (dfs_solve(currentX-2, currentY, 2))
					return 1; 
				array[currentY][currentX-1] = pathc; 
				array[currentY][currentX-2] = pathc; 
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
				if (array[currentY][currentX+2] != pathc) 
					continue; 
				
				// if not, move 1 cell right
				array[currentY][currentX+1] = solvc; 
				
				// recurse deeper
				if (dfs_solve(currentX+2, currentY, 1))
					return 1; 
				
				array[currentY][currentX+1] = pathc; 
				array[currentY][currentX+2] = pathc; 
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
				if (array[currentY-2][currentX] != pathc) 
					continue; 
				
				// if not, move 1 cell up
				array[currentY-1][currentX] = solvc; 
				
				// recurse deeper
				if (dfs_solve(currentX, currentY-2, 4))
					return 1; 
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
				if (array[currentY+2][currentX] != pathc) 
					continue; 
				
				// if not, move 1 cell down
				array[currentY+1][currentX] = solvc; 
				
				// recurse deeper
				if(dfs_solve(currentX, currentY+2, 3))
					return 1; 
				
				array[currentY+1][currentX] = pathc; 
				array[currentY+2][currentX] = pathc; 

				break;
		}
	}
	return 0; 
}

