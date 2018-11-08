# mazegen
An ASCII/Unicode maze generator and solver that runs in your Linux shell. See colorful, wacky, customized mazes unfold in your terminal in real time. 

# examples
![example1](/examples/default.png?raw=true "Default maze")
![example2](/examples/large.png?raw=true "Large maze")
![example3](/examples/custom.png?raw=true "Custom maze with verbose")

# usage

```
Mazegen 1.0 by Travis Ziegler
Program to generate and solve ASCII/Unicode mazes

Usage:  mazegen [-d width height] [-r delay] [-s delay] [-p char] [-w char] [-s char] [-v iter] [-o]

Options: 
  -d <width> <height>	specify the maze dimensions
  -r			use the default realtime generation delay
  -r <delay>  		specify generation delay in milliseconds per iteration
  -s			instantly solve the maze after generation
  -s <delay>		specify solve draw delay in milliseconds per iteration
  -p <char>   		specify the path characer
  -w <char>   		specify the wall (background) character
  -f <char>		specify the solved path character after maze finished generation
  -o 			file output mode; output is formatted for file. Use &> operator
  -v <iter>		verbose mode; program is run <iter> times to calculate avg elapsed time

Example: 
  mazegen -d 40 20 -r -s 50 -w █ -p . 
```
Note: the -r and -s options enable realtime generation, meaning the maze is animated as it is being generated or solved. This leads to some rather cool visuals. 

# thanks
Special thanks to the Computerphile Youtube channel for inspiring me to implement these types of depth-first-search algorithms. It's been quite fun, and the outcome is just fantastic to look at. 
