CC = gcc
CFLAGS = -Wall -Wextra
OPT_CFLAGS = $(CFLAGS) -O3

# Targets
TARGETS = test_rfc test_8kb bench

all: $(TARGETS)

test_rfc: tests/test_rfc.c src/chacha20.c
	$(CC) $(CFLAGS) $^ -o tests/test_rfc

test_8kb: tests/test_8kb.c src/chacha20.c
	$(CC) $(CFLAGS) $^ -o tests/test_8kb

bench: bench/bench_chacha20.c src/chacha20.c
	$(CC) $(OPT_CFLAGS) $^ -o bench/bench_chacha20

run-tests: test_rfc test_8kb
	./tests/test_rfc
	./tests/test_8kb

run-bench: bench
	./bench/bench_chacha20

clean:
	rm -f tests/test_rfc tests/test_8kb bench/bench_chacha20

.PHONY: all test_rfc test_8kb bench run-tests run-bench clean
