
//
//	file: source/helpers.c
//

#include "helpers.h"

// initializaes maze with default arguments
void init_maze() {
	mz.rows = 21; 
	mz.cols = 41; 
	mz.delay = 0; 
	mz.solv_delay = 0; 
	mz.solving = 0; 
	mz.wallc = "░"; 	
	mz.solvc = "▓";
	mz.pathc = " ";
	mz.fileout = 0; 
	mz.array = 0; 
	mz.verbose = 0; 
	mz.endx = 0; 
	mz.endy = 0; 
	mz.startx = 0; 
	mz.starty = 0; 
}

// finds the manhatten distance between two points
// benefit: do not use square root which is computationally expensive
float man_distance(int x, int y, int x2, int y2) 
{
	return abs(x - x2) + abs(y - y2);   
//	return sqrt(pow(x-x2, 2) + pow(y-y2, 2));
}

// prints maze to screen 
void print_maze()
{
	// for performance reasons, must print maze to buffer in memory, 
	// then output buffer to screen in one go
	static char* buffer;

	// make sure to only allocate the buffer once to avoid memory overflow
	static char a = 0; 
	if (!a) 
	{
		buffer = (char*) malloc(mz.cols * mz.rows * 4); 	// allocate enough memory; 1 unicode char = 4 bytes
		a = 1; 
	}

	int p = 0;
	for (int i = 0; i < mz.rows; i++) 
	{
		for (int j = 0; j < mz.cols + 1; j++)			// print newline char +1 
		{
			// write unicode char to buffer and increment offset pointer
			// bar # of characters written (for next char) 
			p += sprintf(buffer + p, "%s",  mz.array[i][j]);
		}
	}

	if (!mz.fileout)
		printf("\033[;H%s", buffer);	// move cursor to top of screen instead of clearing 
	else
		printf("%s", buffer); 			// just print regularlry
}

// gets random number inclusively. 
int rand_rng(int min, int max) 
{
	return rand() % (max+1) + min; 
}

void show_usage()
{
	const char* usg = "Mazegen 1.0 by Travis Ziegler\n\
Program to generate and solve ASCII/Unicode mazes\n\n\
Usage:  mazegen [-d width height] [-r delay] [-s delay] [-p char] [-w char] [-s char] [-v iter] [-o]\n\n\
Options: \n\
  -d <width> <height>	specify the maze dimensions\n\
  -r			use the default realtime generation delay\n\
  -r <delay>  		specify generation delay in milliseconds per iteration\n\
  -s			instantly solve the maze after generation\n\
  -s <delay>		specify solve draw delay in milliseconds per iteration\n\
  -p <char>   		specify the path characer\n\
  -w <char>   		specify the wall (background) character\n\
  -f <char>		specify the solved path character after maze finished generation\n\
  -o 			file output mode; output is formatted for file. Use &> operator\n\
  -v <iter>		verbose mode; program is run <iter> times to calculate avg elapsed time\n\n\
Example: \n\
  mazegen -d 40 20 -r -s 50 -w █ -p . \n\n";
	printf("%s", usg); 
	exit(1); 
}

void handle_args(int argc, char* argv[])
{
	if (argc == 1)
		mz.solving = 1; 

	// loop through each arg
	for (int i = 1; i < argc; i++) 
	{
		// switch on second character (after the '-')
		switch (*(argv[i]+1)) 
		{
			// set mz.cols to next arg, and mz.rows to next next arg (skip those next time by ++i) 
			case 'd': 
				mz.cols = atoi(argv[++i]);
				mz.rows = atoi(argv[++i]);

				// if dimensions are odd or below zero 
				if ((mz.cols % 2 == 0 || mz.rows % 2 == 0) || (mz.cols <= 0 || mz.rows <= 0))
				{
					printf("Dimensions mustn't be even!"); 
					fflush(stdout); 
					_exit(-1); 
				}
				break;
			
			// set delay to argv + 1 if it's number and if it exists, else do default delay
			case 'r':
				if (argc >= i+2 && *argv[i+1] >= '0' && *argv[i+1] <= '9')
					mz.delay = atoi(argv[++i]) * 1000;
				else
					mz.delay = 60 * 1000; 
				break;
			
			case 's':
				if (argc >= i+2 && *argv[i+1] >= '0' && *argv[i+1] <= '9')
				{
					mz.solv_delay = atoi(argv[++i]) * 1000;
					mz.solving = 1;
				}
				else
				{
					mz.solv_delay = 0; 
					mz.solving = 1; 
				}
				break;
			

			// set wallc to arg[++i]
			case 'w':
				mz.wallc = argv[++i]; 
				break;

			// set pathc to next arg
			case 'p':
				mz.pathc = argv[++i]; 
				break;

			case 'v':
				if (argc >= i+2 && *argv[i+1] >= '0' && *argv[i+1] <= '9')
					mz.verbose = atoi(argv[++i]); 
				else
					mz.verbose = 1; 
				break;

			case 'f':
				mz.solvc = argv[++i];
				break;

			case 'o':
				mz.fileout = 1;
				break;

			// else, argument mismatch, show usage and abort
			default: 
				show_usage();
		}
	}
	
}

