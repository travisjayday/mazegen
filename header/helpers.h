
//
//	file: header/helpers.h
//

#ifndef HELPERS_H_
#define HELPERS_H_

#include "maze.h"
#include "locale.h"

// Linux system includes
#include <stdlib.h>		// for rand and malloc
#include <time.h> 		// for rand seed
#include <sys/time.h> 	// needed for verbose gettimeofday microseconds
#include <unistd.h> 	// for usleep
#include <stdio.h>		// for i/o

// initializes default values of maze struct
void init_maze(); 

// calculates the manhatten distance between two points
float man_distance(int x, int y, int x2, int y2);

// prints the global variable array (that holds maze information) to stdout
void print_maze();

// returns a random number from within the _inclusive_ range of min to max
int rand_rng(int min, int max);

// prints the usage of mazegen
void show_usage();

// evaluates given arguments from main and sets global variables according to the args
void handle_args(int argc, char* argv[]);

#endif
