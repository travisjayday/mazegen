// include source files
#include "helpers.h"		// for printing maze and general stuff
#include "mazegen.h"		// for generating a maze
#include "mazesolve.h"	// for solving a maze

// start end times for verbose
struct timeval star_t, fin_t; 

// entry point
int main(int argc, char* argv[])
{
	init_maze(); 				// populates global maze struct with default arguments
	handle_args(argc, argv);	// read user arguments into global vars

	if (!mz.fileout)				// clear screen if not outputting to file
		system("clear"); 

	// handle verbose logic; if verbose is set, run maze algo multiple times and get avg runtime
	if (mz.verbose > 0)
	{
		gettimeofday(&star_t, NULL); 			// get start time
		for (int i = 0; i < mz.verbose; i++)	// run algo multiple times
			create_dfs_maze();					// write maze to screen 
		gettimeofday(&fin_t, NULL); 			// get end time

		int elapsed = (((fin_t.tv_sec - star_t.tv_sec) * 1000000) + (fin_t.tv_usec - star_t.tv_usec)) / mz.verbose;
		printf("Results:\nTotal Elapsed Time for %d runs: %d microseconds\nAverage Time per run: %.0f mircroseconds\n\n", 
				mz.verbose, elapsed, (float)elapsed / mz.verbose);
	}
	else 
	{
		create_dfs_maze();
		// if user specified -s arg, solve maze with depth first search algorithm
		if (mz.solving != 0) 
			solve_maze(); 		
		print_maze(); 
	}
	printf("\n\e[0m\e[0m");	
	free(mz.array);	
	return 0;
}
