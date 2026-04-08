#include "chacha20_mt.h"
#include "chacha20.h"
#include <pthread.h>
#include <string.h>

// Capacidade máxima do buffer estático limitados ao não uso de malloc (restrição de embarcados).
#define MAX_THREADS 16

typedef struct {
    uint8_t *data;
    size_t len;
    uint8_t key[32];
    uint8_t nonce[12];
    uint32_t counter;
} chacha_thread_args_t;

static void *thread_worker(void *arg) {
    chacha_thread_args_t *args = (chacha_thread_args_t *)arg;
    
    // Cada thread fará chamadas single-thread em seus próprios pedaços da memória,
    // garantindo zero contenção de cache/estado visto que operam buffers separados.
    chacha20_encrypt(args->data, args->len, args->key, args->nonce, args->counter);
    return NULL;
}

void chacha20_encrypt_mt(uint8_t *data, size_t len, uint8_t key[32], uint8_t nonce[12], uint32_t counter, int num_threads) {
    if (num_threads <= 1 || len == 0) {
        chacha20_encrypt(data, len, key, nonce, counter);
        return;
    }

    if (num_threads > MAX_THREADS) {
        num_threads = MAX_THREADS;
    }

    pthread_t threads[MAX_THREADS];
    chacha_thread_args_t args[MAX_THREADS];

    // O particionamento é garantido por blocos inteiros.
    size_t total_blocks = (len + 63) / 64;
    
    // Tratando cenários onde o usuário passou mais threads do que tem de blocos disponíveis
    if ((size_t)num_threads > total_blocks) {
        num_threads = (int)total_blocks;
    }

    size_t blocks_per_thread = total_blocks / num_threads;
    size_t current_offset = 0;
    uint32_t current_counter = counter;

    for (int i = 0; i < num_threads; i++) {
        size_t t_blocks = blocks_per_thread;
        if (i == num_threads - 1) {
            // A última thread deve ficar no encargo de varrer todos os pedaços residuais de blocos
            t_blocks = total_blocks - (i * blocks_per_thread);
        }

        size_t t_len = t_blocks * 64;
        
        // Acerta o caso na qual the length do bloco é parcial final (Ex: byte 13 de 64 no ultimo bloco)
        if (current_offset + t_len > len) {
            t_len = len - current_offset;
        }

        args[i].data = data + current_offset;
        args[i].len = t_len;
        args[i].counter = current_counter;
        
        memcpy(args[i].key, key, 32);
        memcpy(args[i].nonce, nonce, 12);

        // Despacha as threads para calcular as porções
        pthread_create(&threads[i], NULL, thread_worker, &args[i]);

        current_offset += t_len;
        current_counter += t_blocks;
    }

    // Block the function loop tracking via joiners until all tasks are executed successfully
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
}
