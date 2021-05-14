CC=gcc
CFLAGS=-fsanitize=address -Wall -Werror -std=gnu11 -g -lm

tests: tests.c virtual_alloc.c
	$(CC) $^ -o $@ $(CFLAGS)

run_tests: virtual_sbrk.c
	$(CC) $^ -c -o virtual_sbrk $(CFLAGS)
	bash tests.sh

clean:
	rm -f virtual_sbrk
	rm -f tests
