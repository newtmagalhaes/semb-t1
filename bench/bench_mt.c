#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include "../src/chacha20.h"
#include "../src/chacha20_mt.h"

#define BUFFER_SIZE (8 * 1024 * 1024) // 8 Megabytes para teste Multi-thread

// Reserva de 8MB globalmente, evitando falhas de Stack e sem usar malloc, como pedido restritamente.
static uint8_t data[BUFFER_SIZE];

static double get_time_ms() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (ts.tv_sec * 1000.0) + (ts.tv_nsec / 1000000.0);
}

void run_bench(int threads, const char *label) {
    uint8_t key[32] = {0x9F};
    uint8_t nonce[12] = {0x18};
    uint32_t counter = 1;

    // Aquece
    chacha20_encrypt_mt(data, 1024, key, nonce, counter, threads);

    const int iterations = 100;
    
    double start = get_time_ms();

    for (int i = 0; i < iterations; i++) {
        chacha20_encrypt_mt(data, BUFFER_SIZE, key, nonce, counter, threads);
    }

    double end = get_time_ms();
    
    double total_time_ms = end - start;
    if (total_time_ms < 0.0001) total_time_ms = 0.0001; 
    
    double total_bytes = (double)iterations * BUFFER_SIZE;
    double throughput_mbs = (total_bytes / (1024.0 * 1024.0)) / (total_time_ms / 1000.0);
    
    double total_blocks = (double)iterations * (BUFFER_SIZE / 64.0);
    double time_per_block_ns = (total_time_ms * 1000000.0) / total_blocks;

    printf("=== %s (%d threads) ===\n", label, threads);
    printf("Total time:     %.2f ms\n", total_time_ms);
    printf("Throughput:     %.2f MB/s\n", throughput_mbs);
    printf("Time per block: %.2f ns\n\n", time_per_block_ns);
}

int main() {
    memset(data, 0, BUFFER_SIZE);

    printf("Iniciando Benchmarks com Buffer Estático de 8MB...\n\n");

    // Proporcional aos Cores Locais via sysconf POSIX 
    int logical_cores = sysconf(_SC_NPROCESSORS_ONLN);
    if (logical_cores < 1) logical_cores = 1;

    run_bench(1, "Single-Thread (Base)");
    run_bench(2, "Dual-Thread");
    run_bench(4, "Quad-Thread");
    run_bench(8, "Octa-Thread");
    
    printf(">>> System Report: Your processor reports %d logical cores.\n", logical_cores);
    run_bench(logical_cores, "Virtual Threads/Cores Fit");

    return 0;
}
