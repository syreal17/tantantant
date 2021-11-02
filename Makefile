.PHONY: main
main: clean game


game: game.c
	gcc -o game game.c -lcurses
	strip -s game


game.c:
	touch game.c


game-dev:
	gcc -g -o game game.c -lcurses

clean:
	rm -f game
