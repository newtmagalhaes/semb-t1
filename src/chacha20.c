#include "chacha20.h"

uint32_t rotl(uint32_t x, int n) {
    return (x << n) | (x >> (32 - n));
}

void quarter_round(uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d) {
    *a += *b; *d ^= *a; *d = rotl(*d, 16);
    *c += *d; *b ^= *c; *b = rotl(*b, 12);
    *a += *b; *d ^= *a; *d = rotl(*d, 8);
    *c += *d; *b ^= *c; *b = rotl(*b, 7);
}

static inline void serialize_word(uint32_t w, uint8_t *out) {
    out[0] = w & 0xff;
    out[1] = (w >> 8) & 0xff;
    out[2] = (w >> 16) & 0xff;
    out[3] = (w >> 24) & 0xff;
}

static inline uint32_t deserialize_word(const uint8_t *in) {
    return (uint32_t)in[0] | ((uint32_t)in[1] << 8) | ((uint32_t)in[2] << 16) | ((uint32_t)in[3] << 24);
}

void chacha20_block(uint32_t state[16], uint8_t keystream[64]) {
    uint32_t working_state[16];
    for (int i = 0; i < 16; i++) {
        working_state[i] = state[i];
    }

    for (int i = 0; i < 10; i++) {
        // Column rounds
        quarter_round(&working_state[0], &working_state[4], &working_state[8],  &working_state[12]);
        quarter_round(&working_state[1], &working_state[5], &working_state[9],  &working_state[13]);
        quarter_round(&working_state[2], &working_state[6], &working_state[10], &working_state[14]);
        quarter_round(&working_state[3], &working_state[7], &working_state[11], &working_state[15]);

        // Diagonal rounds
        quarter_round(&working_state[0], &working_state[5], &working_state[10], &working_state[15]);
        quarter_round(&working_state[1], &working_state[6], &working_state[11], &working_state[12]);
        quarter_round(&working_state[2], &working_state[7], &working_state[8],  &working_state[13]);
        quarter_round(&working_state[3], &working_state[4], &working_state[9],  &working_state[14]);
    }

    for (int i = 0; i < 16; i++) {
        working_state[i] += state[i];
        serialize_word(working_state[i], &keystream[i * 4]);
    }
}

void chacha20_encrypt(uint8_t *data, size_t len, uint8_t key[32], uint8_t nonce[12], uint32_t counter) {
    uint32_t state[16];
    uint8_t keystream[64];

    // Constantes ChaCha20: "expand 32-byte k" em little-endian
    state[0] = 0x61707865;
    state[1] = 0x3320646e;
    state[2] = 0x79622d32;
    state[3] = 0x6b206574;

    // Chave
    for (int i = 0; i < 8; i++) {
        state[4 + i] = deserialize_word(&key[i * 4]);
    }

    // Contador e Nonce
    state[12] = counter;
    for (int i = 0; i < 3; i++) {
        state[13 + i] = deserialize_word(&nonce[i * 4]);
    }

    size_t offset = 0;
    while (offset < len) {
        chacha20_block(state, keystream);
        
        state[12]++; // Incrementa o contador para o próximo bloco

        size_t block_len = len - offset;
        if (block_len > 64) {
            block_len = 64;
        }

        for (size_t i = 0; i < block_len; i++) {
            data[offset + i] ^= keystream[i];
        }

        offset += block_len;
    }
}
