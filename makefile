CC=gcc

all:
	make tests

tests:
	$(CC) tests.c art.c -std=c89 -pedantic -O3 -o art

clean:
	rm art
