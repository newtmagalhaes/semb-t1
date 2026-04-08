CC = gcc
CFLAGS = -Wall -Wextra
OPT_CFLAGS = $(CFLAGS) -O3
LDFLAGS = -lpthread

# Targets
TARGETS = test_rfc test_8kb test_mt bench bench_mt

all: $(TARGETS)

test_rfc: tests/test_rfc.c src/chacha20.c
	$(CC) $(CFLAGS) $^ -o tests/test_rfc

test_8kb: tests/test_8kb.c src/chacha20.c
	$(CC) $(CFLAGS) $^ -o tests/test_8kb

test_mt: tests/test_mt.c src/chacha20_mt.c src/chacha20.c
	$(CC) $(CFLAGS) $^ -o tests/test_mt $(LDFLAGS)

bench: bench/bench_chacha20.c src/chacha20.c
	$(CC) $(OPT_CFLAGS) $^ -o bench/bench_chacha20

bench_mt: bench/bench_mt.c src/chacha20_mt.c src/chacha20.c
	$(CC) $(OPT_CFLAGS) $^ -o bench/bench_mt $(LDFLAGS)

run-tests: test_rfc test_8kb test_mt
	./tests/test_rfc
	./tests/test_8kb
	./tests/test_mt

run-bench: bench bench_mt
	./bench/bench_chacha20
	./bench/bench_mt

clean:
	rm -f tests/test_rfc tests/test_8kb tests/test_mt bench/bench_chacha20 bench/bench_mt

.PHONY: all test_rfc test_8kb bench run-tests run-bench clean
