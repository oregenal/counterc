CC=cc
CFLAGS=-Wall -Wextra -std=c11 -pedantic
BIN=dcount
SRC=key.c

.PHONY:clean

$(BIN):$(SRC)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf $(BIN)

