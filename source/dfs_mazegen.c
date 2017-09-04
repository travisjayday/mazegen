#include "dfs_mazegen.h" 

// generates numbers from 1 - 4 at random positions in array of size 4
// used for randomly getting next direction 
void get_dir(int dir[4]) 
{
	// randomly select position and populate it 
	for (int i = 4; i > 0; i--)
	{
		int r = rand() % i;	// not inclusive, random range from 0 to number needed
		while (dir[r] != 0) 	// if number needed is already populated, 
			if (r < 4) 	// increment to next space if number needed < 4
				r++;
			else 
				r = 0; 	// if r > 4, set r back to zero
		dir[r] = i; 		// asign to dir array
	}
}



// recursively randomly, populates maze array
void next_depth(int currentX, int currentY)
{
	char served = 0;			// if this iteration block has moves onward, it has served
						// if it is a dead end, served will stay 0 

	array[currentY][currentX] = pathc; 	// set current pos to path
	int dir[4] = {0};			// create dir array
	get_dir(dir);				// init dir array to random directions

	// if there's a delay, clear screen and print updated maze
	if (delay > 0)
	{
		print_maze();
		usleep(delay); 
	}

	// loop through each possible move left, right, top, down 
	// moves are ordered randomly in array dir
	for (int d = 0; d < 4; d++) 
	{
		// switch on random dir 
		switch (dir[d]) 
		{
			// go left 
			case 1: 
				// check left bounds
				if (currentX - 2 <= 0) 
					continue; 
				
				// check if path already taken
				if (array[currentY][currentX-2] != wallc) 
					continue; 
				
				// if not, move 1 cell left
				array[currentY][currentX-1] = pathc; 
				
				// recurse deeper
				next_depth(currentX-2, currentY);
				served = 1;
				break;

			// go right
			case 2:
				// check right bounds
				if (currentX + 2 > cols-1) 
					continue; 
				
				// check if path already taken
				if (array[currentY][currentX+2] != wallc) 
					continue; 
				
				// if not, move 1 cell right
				array[currentY][currentX+1] = pathc; 
				
				// recurse deeper
				next_depth(currentX+2, currentY); 
				served = 1;
				break;

			// go up
			case 3:
				// check top bounds
				if (currentY - 2 < 0) 
					continue; 
				
				// check if path already taken
				if (array[currentY-2][currentX] != wallc) 
					continue; 
				
				// if not, move 1 cell up
				array[currentY-1][currentX] = pathc; 
				
				// recurse deeper
				next_depth(currentX, currentY-2); 
				served = 1;
				break;

			// go down
			case 4: 
				// check down bounds
				if (currentY + 2 > rows-1) 
					continue; 
				
				// check if path already taken
				if (array[currentY+2][currentX] != wallc) 
					continue; 
				
				// if not, move 1 cell down
				array[currentY+1][currentX] = pathc; 
				
				// recurse deeper
				next_depth(currentX, currentY+2); 
				served = 1; 
				break;
		}
	}

	// cell was a dead end
	if (!served)
	{
		int d = man_distance(startx, starty, currentX, currentY);	
		
		// if current position is on the edge and the distance to the 
		// start is gretaer than the last distance, mark this cell as the 
		// best end cell (cell with most distance) 
		if (currentY >= rows -2 && d > pathd)
		{
			endx = currentX;
			endy = currentY+1; 
			pathd = d; 
		}
		if (currentX >= cols -2 && d > pathd) 
		{
			endx = currentX+1;
			endy = currentY;
		       	pathd = d; 	
		}
	}
}

// generates depth first search maze given from global vars
void create_dfs_maze() 
{
	// prime random 
	srand(time(0)); 	

	// populate array with given dimensions. array[rows][cols]
	
	// allocate rows * pointer to pointer of const char pointers
	// in other words, rows * column pointers 

	array = (const char***) malloc((rows+1) * sizeof(const char**)); // new const char**[rows]; 

	// loop through each row
	for (int i = 0; i < rows+1; i++)
	{
		// for each row pointer, allocate cols+2 (because of padding + newline) * const char pointers
		// in other words, allocate the column pointers
		array[i] = (const char**) malloc((cols+2) * sizeof(const char*)); // const char*[cols+1];
		
		// initialize all const char* in column to wallc, then add \n as final char
		int j = 0; 
		for (; j < cols+1; j++) 
			array[i][j] = wallc;
		array[i][j] = "\n";
	}

	// start recursion maze at point either on top or left border
	if (rand_rng(0, 1)) 
	{
		// start topside
		startx = rand_rng(0, cols-1);

		// should be odd number if rows and cols are even
		if (startx % 2 == 0) 
			startx++; 

		array[0][startx] = pathc;
		next_depth(startx, 1); 
	}
	else
	{
		// start left side
		starty = rand_rng(0, rows-1);

		// should be odd number if rows and cols are even
		if (starty % 2 == 0) 
			starty++; 

		array[starty][0] = pathc; 
		next_depth(1, starty);
	}

	// if no end was found, artificially generate one 	
	if (endx == 0 && endy == 0) 
	{
		endx = cols - 1; 
		endy = rows; 
	}
	array[endy][endx] = pathc; 	
	
	// if user specified -s arg, solve maze with depth first search algorithm
	if (solving != 0) 
		solve_maze(); 		
	
	print_maze(); 
	free (array);	
}
