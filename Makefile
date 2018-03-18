debug: textmaze.c
	@gcc -std=c99 textmaze.c -o build/mazegen -lm
	@./build/mazegen
