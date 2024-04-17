CC = gcc
CFLAGS = -Wall -Wextra

all: 2019038068

2019038068: 2019038068.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f 2019038068