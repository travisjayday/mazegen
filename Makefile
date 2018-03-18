debug: textmaze.c
	@cd source
	@gcc -std=c99 textmaze.c -o build/mazegen -lm
	@./build/mazegen
