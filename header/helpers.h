#ifndef HELPERS_H_
#define HELPERS_H_

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
