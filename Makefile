CC=cc
CFLAGS=-Wall -Wextra -std=c11 -pedantic
BIN=dcount
SRC=key.c

$(BIN):$(SRC)
	$(CC) $(CFLAGS) -o $@ $^
