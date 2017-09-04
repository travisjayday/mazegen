debug: textmaze.c
	@gcc textmaze.c -o build/mazegen -lm
	@build/./mazegen
