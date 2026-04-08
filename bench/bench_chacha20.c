#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <string.h>
#include "../src/chacha20.h"

// Função para retornar o tempo em milissegundos
static double get_time_ms() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (ts.tv_sec * 1000.0) + (ts.tv_nsec / 1000000.0);
}

int main() {
    uint8_t data[8192];
    uint8_t key[32] = {0xAA};
    uint8_t nonce[12] = {0xBB};
    uint32_t counter = 1;

    memset(data, 0, sizeof(data));

    const int iterations = 10000;
    const size_t buffer_size = sizeof(data);

    double start = get_time_ms();

    for (int i = 0; i < iterations; i++) {
        chacha20_encrypt(data, buffer_size, key, nonce, counter);
    }

    double end = get_time_ms();
    
    double total_time_ms = end - start;
    if (total_time_ms == 0) total_time_ms = 0.0001; // previne div por zero
    
    double total_bytes = (double)iterations * buffer_size;
    double throughput_mbs = (total_bytes / (1024.0 * 1024.0)) / (total_time_ms / 1000.0);
    
    // Cálculo do custo por bloco
    // Cada iteração roda buffer_size / 64 blocos = 128 blocos
    // Total de blocos na medição = iterations * 128
    double total_blocks = (double)iterations * (buffer_size / 64.0);
    double time_per_block_ns = (total_time_ms * 1000000.0) / total_blocks;

    printf("Total time: %.2f ms\n", total_time_ms);
    printf("Throughput: %.2f MB/s\n", throughput_mbs);
    printf("Time per block: %.2f ns\n", time_per_block_ns);

    return 0;
}
