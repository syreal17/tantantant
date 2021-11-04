.PHONY: main
main: game


game: game.c
	gcc -o game game.c -lcurses -lpthread


game.c:
	touch game.c


game-dev:
	gcc -g -o game game.c -lcurses -lpthread

clean:
	rm -f game
