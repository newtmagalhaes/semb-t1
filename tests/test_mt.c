#include <stdio.h>
#include <string.h>
#include "../src/chacha20.h"
#include "../src/chacha20_mt.h"

int main() {
    // Usaremos um tamanho esquisito no par pra forçar erro se o algoritmo picotar mal o bloco residual MT.
    size_t buffersize = 8100;
    uint8_t data_st[buffersize];
    uint8_t data_mt[buffersize];
    uint8_t original[buffersize];

    uint8_t key[32] = {0x1A, 0x2B, 0x3C};
    uint8_t nonce[12] = {0x01, 0x02, 0x03};
    uint32_t counter = 1;

    for (size_t i = 0; i < buffersize; i++) {
        original[i] = i % 256;
        data_st[i] = original[i];
        data_mt[i] = original[i];
    }
    
    // Executa a Single-Thread
    chacha20_encrypt(data_st, buffersize, key, nonce, counter);

    // Testar se os valores de cada variação Pthread geram o mesmíssimo hex match Single-Thread 
    int thread_counts[] = {2, 3, 4, 7, 8, 16};
    
    for (int t = 0; t < 6; t++) {
        int nt = thread_counts[t];
        
        // Reset state MT e perform MT ciphering
        memcpy(data_mt, original, buffersize);
        chacha20_encrypt_mt(data_mt, buffersize, key, nonce, counter, nt);
        
        for (size_t i = 0; i < buffersize; i++) {
            if (data_st[i] != data_mt[i]) {
                printf("[FAIL] MT mismatch with %d threads at index: %zu\n", nt, i);
                return 1;
            }
        }
    }

    printf("[OK] Multi-Thread encrypt validates perfectly bit-by-bit against Single-Thread mode.\n");
    return 0;
}
