#ifndef __GLOBALS_H_
#define __GLOBALS_H_

typedef struct {
	// default arguments
	int rows = 21; 			// default vertical size of maze in characters
	int cols = 41;			// default horizontal size of maze in characters
	int delay = 0; 			// delay between iterations of generating algorithm
	int solv_delay = 0;		// delay between iterations of solving algorithm
	char solving = 0;		// set to 1 if no args were provided 
	const char* wallc = "▓";// character for wall, ex: "█\0";// "█"; //" "; 
	const char* pathc = " ";// character for path, ex: "░\0";
	const char* solvc = "█";// character for solving path, ex: "░";
	char fileout = 0; 		// true if writing to file. If writing to file, skip formatting (clearing) output
	const char*** array; 	// structure for holding maze
	char verbose = 0;		// if true, runs program v iterations and gets average time per run
} Args; 

Args args; 					// definition of args

#endif
