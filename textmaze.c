#include <stdlib.h>		// for rand and malloc
#include <time.h> 		// for rand seed
#include <sys/time.h> 		// needed for verbose gettimeofday microseconds
#include <unistd.h> 		// for usleep
#include <stdio.h>		// for i/o

// default arguments
int rows = 20; 			// default vertical size of maze in characters
int cols = 40;			// default horizontal size of maze in characters
int delay = 0; 			// delay between iterations of generating algorithm
int solv_delay = 0;		// delay between iterations of solving algorithm
char solving = 0;		// set to 1 if no args were provided 
const char* wallc = "▓";	// character for wall, ex: "█\0";// "█"; //" "; 
const char* pathc =  " ";	// character for path, ex: "░\0";
const char* solvc = "█";	// character for solving path, ex: "░";
char fileout = 0; 		// true if writing to file. If writing to file, skip formatting (clearing) output
const char*** array; 		// structure for holding maze
char verbose = 0;		// if true, runs program v iterations and gets average time per run

// needed vars for mazegen
int endx = 0;			// end position (for solving)			
int endy = 0;			
int startx = 0;			// start position (generated randomly, needed for solving) 
int starty = 0; 
int pathd = 0; 			// distance between end and start (needed for finding best/longest end position) 


// start end times for verbose
struct timeval star_t, fin_t; 

// include source
#include "source/helpers.h"
#include "source/dfs_mazesolve.h"
#include "source/dfs_mazegen.h"

// entry point
int main(int argc, char* argv[])
{
	handle_args(argc, argv);	// read arguments into global vars

	if (!fileout)
	{
		system("clear"); 
	}

	// handle verbose logic	
	if (verbose > 0 )
	{
		gettimeofday(&star_t, NULL); 
		for (int i = 0; i < verbose; i++)
			create_dfs_maze();		// write maze to screen 
		gettimeofday(&fin_t, NULL); 
		int elapsed = (((fin_t.tv_sec - star_t.tv_sec) * 1000000) + (fin_t.tv_usec - star_t.tv_usec)) / verbose;
		printf("Results:\nTotal Elapsed Time for %d runs: %d microseconds\nAverage Time per run: %.0f mircroseconds\n\n", 
				verbose, elapsed, (float)elapsed / verbose);
	}
	else 
		create_dfs_maze();
	return 0;
}
