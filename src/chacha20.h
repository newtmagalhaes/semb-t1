#ifndef CHACHA20_H
#define CHACHA20_H

#include <stdint.h>
#include <stddef.h>

void quarter_round(uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d);
uint32_t rotl(uint32_t x, int n);
void chacha20_block(uint32_t state[16], uint8_t keystream[64]);
void chacha20_encrypt(uint8_t *data, size_t len, uint8_t key[32], uint8_t nonce[12], uint32_t counter);

#endif // CHACHA20_H
