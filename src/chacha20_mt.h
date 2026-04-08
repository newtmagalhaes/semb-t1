#ifndef CHACHA20_MT_H
#define CHACHA20_MT_H

#include <stdint.h>
#include <stddef.h>

// Efetua a cifra paralelizada utilizando um máximo fixo de 16 threads, permitindo particionamento do bloco
void chacha20_encrypt_mt(uint8_t *data, size_t len, uint8_t key[32], uint8_t nonce[12], uint32_t counter, int num_threads);

#endif // CHACHA20_MT_H
