
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
	mz.solvc = "░"; 	
	//mz.wallc = "▓";
	mz.wallc = "█\0";
//	mz.wallc = "k\0";
	mz.pathc = " ";
	mz.fileout = 0; 
	mz.array = 0; 
	mz.verbose = 0; 
	mz.endx = 0; 
	mz.endy = 0; 
	mz.startx = 0; 
	mz.starty = 0; 
	mz.usecolor = 0; 
	memcpy(mz.colors[0], NRM, 6); 
	memcpy(mz.colors[1], BLK, 6); 
	memcpy(mz.colors[2], NRM, 6); 
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
		buffer = (char*) malloc(mz.cols * mz.rows * 4 * 4); 	// allocate enough memory; 1 unicode char = 4 bytes
		a = 1; 
	}

	int p = 0;
	for (int i = 0; i < mz.rows; i++) 
	{
		for (int j = 0; j < mz.cols + 1; j++)			// print newline char +1 
		{
			// write unicode char to buffer and increment offset pointer
			// bar # of characters written (for next char) 
			const char* c = mz.array[i][j]; 
			if (mz.usecolor == 0) {
				p += sprintf(buffer + p, "%s", c);
				continue;
			}

			if (c == mz.wallc)
				p += sprintf(buffer + p, "%s%s", mz.colors[0], c);
			else if (c == mz.pathc)
				p += sprintf(buffer + p, "%s%s", mz.colors[1], c);
//			else if (c == mz.solvc)
//				p += sprintf(buffer + p, "%s%s", mz.colors[2], c);
			else
				p += sprintf(buffer + p, "%s%s", mz.colors[2], c);
		}
	}

	if (mz.fileout == 0)
		printf("\033[;H%s", buffer);	// move cursor to top of screen instead of clearing 
	else {
		FILE* fil = fopen(mz.fileout, "w"); 
		fprintf(fil, "%s", buffer); 	// just print regularlry
		printf("Successfully wrote to %s!\n", mz.fileout); 
		fclose(fil); 

		if (mz.printmaze) 
		{
			static char cmd[20]; 
			sprintf(cmd, "leafpad %s", mz.fileout); 
			system(cmd); 
		}
	}
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
Usage:  mazegen [option] <value> ... \n\n\
Options: \n\
  -d  <width> <height>	specify the maze dimensions\n\n\
  -r			use the default realtime generation delay\n\
  -r  <delay>  		generation delay in milliseconds per iteration\n\n\
  -s			solve the maze after generation\n\
  -s  <delay>		solve draw delay in milliseconds per iteration\n\
  -sf <style> <delay>	just like -s but with a styled path \n\
  -w  <char>   		specify the wall (background) character\n\
  -p  <char>   		specify the path characer\n\
  -f  <char>		specify the finished (solved) path character \n\
  -cw <color>		specify color of wall character\n\
  -cp <color>		specify color of path character\n\
  -cf <color>		specify color of finished path character\n\n\
  -o  <fname> <print>	file output mode. If print = 1, will send to printer\n\
  -v  <iter>		verbose mode; benchmark performance\n\
  -l 			list unicode characters for maze\n\n\
Styles: \n\
  <style> = [n]ormal, [t]hin, [d]ouble \n\n\
Colors: \n\
  <color> = [r]ed, [g]reen, [y]ellow, [b]lue, [m]agenta, [c]yan, [w]hite \n\n\
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

				if (mz.cols % 2 == 0)
					printf("[Warning] Not Odd! Truncated column dimension to %d\n", --mz.cols); 

				if (mz.rows % 2 == 0)
					printf("[Warning] Not Odd! Truncated row dimension to %d\n", --mz.rows); 

				// if dimensions are odd or below zero 
				if (mz.cols <= 1 || mz.rows <= 1)
				{
					printf("[Error] Dimensions too small!\n"); 
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
				if (*(argv[i] + 2) == 'f') 
				{
					mz.fancyp = 1; // default
					if (argc >= i+2 && *argv[i+1] != '-')
					{
						if (*argv[++i] == 'n')
							mz.fancyp = 1; 
						else if (*argv[i] == 't')
							mz.fancyp = 2; 
						else if (*argv[i] == 'd') 
							mz.fancyp = 3; 
						else 
							printf("[Warning] invalid path style: defaulting to normal\n");
					}
				}

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

			case 'c': 
				if (argc >= i+2) {
					mz.usecolor = 1; 
					switch (*argv[i + 1]) {
						case 'r': 
							if (*(argv[i] + 2) == 'w') memcpy(mz.colors[0], RED, 6); 
							else if (*(argv[i] + 2) == 'p') printf("\e[0m\e[41m");	// change terminal backgruond to red 
							else if (*(argv[i] + 2) == 'f') memcpy(mz.colors[2], RED, 6); 
							break;
						case 'g': 
							if (*(argv[i] + 2) == 'w') memcpy(mz.colors[0], GRN, 6); 
							else if (*(argv[i] + 2) == 'p') printf("\e[0m\e[42m");	// change terminal backgruond to grn
							else if (*(argv[i] + 2) == 'f') memcpy(mz.colors[2], GRN, 6); 
							break;
						case 'y':
							if (*(argv[i] + 2) == 'w') memcpy(mz.colors[0], YEL, 6); 
							else if (*(argv[i] + 2) == 'p') printf("\e[0m\e[43m");	// change terminal backgruond to red 
							else if (*(argv[i] + 2) == 'f') memcpy(mz.colors[2], YEL, 6); 
							break; 
						case 'b': 
							if (*(argv[i] + 2) == 'w') memcpy(mz.colors[0], BLU, 6); 
							else if (*(argv[i] + 2) == 'p') printf("\e[0m\e[44m");	
							else if (*(argv[i] + 2) == 'f') memcpy(mz.colors[2], BLU, 6); 
							break;
						case 'm': 
							if (*(argv[i] + 2) == 'w') memcpy(mz.colors[0], MAG, 6); 
							else if (*(argv[i] + 2) == 'p') printf("\e[0m\e[45m");	
							else if (*(argv[i] + 2) == 'f') memcpy(mz.colors[2], MAG, 6); 
							break;
						case 'c':
							if (*(argv[i] + 2) == 'w') memcpy(mz.colors[0], CYN, 6); 
							else if (*(argv[i] + 2) == 'p') printf("\e[0m\e[46m");	
							else if (*(argv[i] + 2) == 'f') memcpy(mz.colors[2], CYN, 6); 
							break;
					}
					i++; 
				}
				else 
					show_usage();
				break;

			case 'o':
				mz.fileout = argv[++i];
				if (argc >= i+2 && *argv[i+1] != '-')
				{
					mz.printmaze = 1; 
					i++; 
				}
				break;
			
			case 'l': 
				printf(
"╳  ⁛ \n\
░  ▒ \n\
▓  █ \n");
				_exit(0); 
				break;

			// else, argument mismatch, show usage and abort
			default: 
				show_usage();
		}
	}
	
}

