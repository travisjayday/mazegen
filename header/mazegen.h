
//
//	file: header/mazegen.h
//

#ifndef MAZEGEN_H_
#define MAZEGEN_H_

#include "maze.h"
#include "helpers.h"

// needed vars for mazegen
int pathd; 			// distance between end and start (needed for finding best/longest end position) 

// generates numbers from 1 - 4 at random positions in array of size 4
// used for randomly getting next direction 
void get_dir(int dir[4]); 

// recursively populates maze array. main dfs algorithm is here
void next_depth(int currentX, int currentY);

// allocates depth first search maze and starts dfs algorithm given from global vars
void create_dfs_maze();

#endif
