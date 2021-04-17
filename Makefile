# make example

CC=gcc

target:
	$(CC) -s -Wall -O2 example.cpp -o example

.PHONY: clean

clean:
	-rm example

