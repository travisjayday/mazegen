
//
//	file: source/mazegen.c
//

#include "mazegen.h" 
#define DEBUG

// generates numbers from 1 - 4 at random positions in mz.array of size 4
// used for randomly getting next direction 
void get_dir(int dir[4]) 
{
	// randomly select position and populate it 
	for (int i = 4; i > 0; i--)
	{
		int r = rand() % i;	// not inclusive, random range from 0 to number needed
		while (dir[r] != 0) // if number needed is already populated, 
			if (r < 4) 		// increment to next space if number needed < 4
				r++;
			else 
				r = 0; 		// if r > 4, set r back to zero
		dir[r] = i; 		// asign to dir mz.array
	}
}

// recursively randomly, populates maze mz.array
void next_depth(int currentX, int currentY)
{
	char served = 0;			// if this iteration block has moves onward, it has served
								// if it is a dead end, served will stay 0 

	mz.array[currentY][currentX] = mz.pathc; 	// set current pos to path
	int dir[4] = {0};			// create dir mz.array (allocate with all elems = 0)
	get_dir(dir);				// init dir mz.array to random directions

	// if there's a mz.delay, clear screen and print updated maze
	if (mz.delay > 0 && mz.fileout == 0)
	{
		print_maze();
		usleep(mz.delay); 
	}

	// loop through each possible move left, right, top, down 
	// moves are ordered randomly in mz.array dir
	for (int d = 0; d < 4; d++) 
	{
		// switch on random dir 
		switch (dir[d]) 
		{
			// go left 
			case 1: 
				// check left bounds
				if (currentX - 1 <= 0) 
					continue; 
				
				// check if path already taken. Cannot place path if it would create double path
				// thus must check if there is a wall 2 chars away
				if (mz.array[currentY][currentX - 2] != mz.wallc) 
					continue; 
				
				// if not, move 1 cell left
				mz.array[currentY][currentX - 1] = mz.pathc; 
				
				// recurse deeper
				next_depth(currentX - 2, currentY);
				served = 1;
				break;

			// go right
			case 2:
				// check right bounds;
				if (currentX + 1 >= mz.cols -1) 
					continue; 
				
				// check if path already taken. do not want adjacent paths
				if (mz.array[currentY][currentX + 2] != mz.wallc) 
					continue; 
				
				// if not, move 1 cell right
				mz.array[currentY][currentX + 1] = mz.pathc; 
				
				// recurse deeper
				next_depth(currentX + 2, currentY); 
				served = 1;
				break;

			// go up
			case 3:
				// check top bounds; if y = 0 -> top border (cannot go on border)
				// thus if currentY - 2 == 0, you are right under the border and cannot continue
				if (currentY - 1 <= 0) 
					continue; 
				
				// check if path already taken
				if (mz.array[currentY - 2][currentX] != mz.wallc) 
					continue; 
				
				// if not, move 1 cell up
				mz.array[currentY - 1][currentX] = mz.pathc; 
				
				// recurse deeper
				next_depth(currentX, currentY - 2); 
				served = 1;
				break;

			// go down
			case 4: 
				// check down bounds; if y = mz.rows - 1 -> bottom border (cannot go on border) 
				// thus, if currentY + 1 
				if (currentY + 2 >= mz.rows - 1) 
					continue; 
				
				// check if path already taken
				if (mz.array[currentY + 2][currentX] != mz.wallc) 
					continue; 
				
				// if not, move 1 cell down
				mz.array[currentY + 1][currentX] = mz.pathc; 
				
				// recurse deeper
				next_depth(currentX, currentY + 2); 
				served = 1; 
				break;
		}
	}

	// cell could not be served. cell is a dead end. 
	// From this cell, you cannot move left right up or down. 
	if (!served)
	{
		int d = man_distance(mz.startx, mz.starty, currentX, currentY);	
		
		// if current position is on the edge and the distance to the 
		// mz.start is gretaer than the last distance, mark this cell as the 
		// best end cell (cell with most distance) 
		if (currentY >= mz.rows -2 && d > pathd)
		{
			mz.endx = currentX;
			mz.endy = currentY+1; 
			pathd = d; 
		}
		if (currentX >= mz.cols -2 && d > pathd) 
		{
			mz.endx = currentX+1;
			mz.endy = currentY;
	    	pathd = d; 	
		}	
	}
}

// generates depth first search maze given from global vars
void create_dfs_maze() 
{
	// prime random 
	FILE* randstream = fopen("/dev/urandom", "rb"); 
	unsigned int seed; 
	fread(&seed, sizeof(unsigned int), 1, randstream); 
	fclose(randstream); 
	
	//srand(time(0)); 	
	srand(seed); 


	// populate mz.array with given dimensions. array[mz.rows][mz.cols]
	// allocate mz.rows * pointer to pointer of const char pointers
	// in other words, mz.rows * column pointers 

	mz.array = (const char***) malloc((mz.rows) * sizeof(const char**)); 	// new const char**[mz.rows]; 

	// loop through each row
	for (int i = 0; i < mz.rows; i++)
	{
		// for each row pointer, allocate mz.cols+1 (because of newline) * const char pointers
		// in other words, allocate the column pointers
		mz.array[i] = (const char**) malloc((mz.cols + 1) * sizeof(const char*)); // const char*[mz.cols+1];
		
		// initialize all const char* in column to mz.wallc, then add \n as final char
		int j = 0; 
		for (; j < mz.cols; j++) 
			mz.array[i][j] = mz.wallc;
		mz.array[i][j] = "\n";
	}

	// mz.start recursion maze at point either on top or left border
	if (rand_rng(0, 1)) 
	{
		// mz.start topside
		mz.startx = rand_rng(0, mz.cols - 1);

		// mz.start pos must be odd
		if (mz.startx % 2 == 0) 
			mz.startx -= 1; 

		// should not mz.start in corner
		if (mz.startx <= 0) 
			mz.startx = 1; 
		else if (mz.startx == mz.cols - 1)
			mz.startx = mz.rows - 1 - 2; 

		mz.array[0][mz.startx] = mz.pathc;

		// start recursive algoirhtm
		next_depth(mz.startx, 1); 
	}
	else
	{
		// mz.start left side
		mz.starty = rand_rng(0, mz.rows - 1);

		// mz.start pos must be odd 
		if (mz.starty % 2 == 0) 
			mz.starty -= 1; 

		// should not .mz.start in corner
		if (mz.starty <= 0)
			mz.starty = 1;
		else if (mz.starty == mz.rows - 1) 
			mz.starty = mz.rows - 1 - 2; 

		mz.array[mz.starty][0] = mz.pathc; 
		
		// start recursive algorithm
		next_depth(1, mz.starty);
	}

	// if no end was found, artificially generate one 	
	if (mz.endx == 0 && mz.endy == 0) 
	{
		mz.endx = mz.cols - 2; 
		mz.endy = mz.rows - 1; 
	}
	mz.array[mz.endy][mz.endx] = mz.pathc; 	
}
