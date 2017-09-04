#include "helpers.h"
#include "locale.h"

float man_distance(int x, int y, int x2, int y2) 
{
	// d = sqrt((x-x2)^2 + (y-y2)^2)
	return abs(x - x2) + abs(y - y2);   
//	return sqrt(pow(x-x2, 2) + pow(y-y2, 2));
}

// prints maze
void print_maze()
{
	static char* buffer;
	static char a = 0; 
	if (!a) 
	{
		buffer = (char*) malloc(cols * rows * 4); 
		a = 1; 
	}
	int p = 0;
	
	for (int i = 0; i < rows+1; i++) 
	{
		for (int j = 0; j < cols+2; j++)	// print newline char +1 
		{
			// write unicode char to buffer and increment offset pointer
			// bar # of characters written (for next char) 
			p+= sprintf(buffer+p, "%s",  array[i][j]);
		}
	}
//	if (!fileout)
		printf("%s", buffer);	// move cursor to top of screen instead of clearing 
//	else
//		printf("jj%s", buffer); 
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
		solving = 1; 

	// loop through each arg
	for (int i = 1; i < argc; i++) 
	{
		// switch on second character (after the '-')
		switch (*(argv[i]+1)) 
		{
			// set cols to next arg, and rows to next next arg (skip those next time by ++i) 
			case 'd': 
				cols = atoi(argv[++i]);
				rows = atoi(argv[++i]);

				if (cols <= 0 || rows <= 0)		
					show_usage(); 
				break;
			
			// set delay to argv + 1 if it's number and if it exists, else do default delay
			case 'r':
				if (argc >= i+2 && *argv[i+1] >= '0' && *argv[i+1] <= '9')
					delay = atoi(argv[++i]) * 1000;
				else
					delay = 60 * 1000; 
				break;
			
			case 's':
				if (argc >= i+2 && *argv[i+1] >= '0' && *argv[i+1] <= '9')
				{
					solv_delay = atoi(argv[++i]) * 1000;
					solving = 1;
				}
				else
				{
					solv_delay = 0; 
					solving = 1; 
				}
				break;
			

			// set wallc to arg[++i]
			case 'w':
				wallc = argv[++i]; 
				break;

			// set pathc to next arg
			case 'p':
				pathc = argv[++i]; 
				break;

			case 'v':
				if (argc >= i+2 && *argv[i+1] >= '0' && *argv[i+1] <= '9')
					verbose = atoi(argv[++i]); 
				else
					verbose = 1; 
				break;

			case 'f':
				solvc = argv[++i];
				break;

			case 'o':
				fileout = 1;
				break;

			// else, argument mismatch, show usage and abort
			default: 
				show_usage();
		}
	}
	
}

