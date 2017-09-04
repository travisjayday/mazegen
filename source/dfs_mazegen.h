#ifndef DFS_MAZEGEN_H_
#define DFS_MAZEGEN_H_

// generates numbers from 1 - 4 at random positions in array of size 4
// used for randomly getting next direction 
void get_dir(int dir[4]); 

// recursively populates maze array. main dfs algorithm is here
void next_depth(int currentX, int currentY);

// allocates depth first search maze and starts dfs algorithm given from global vars
void create_dfs_maze();

#include "dfs_mazegen.c"

#endif
