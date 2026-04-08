#include <stdio.h>
#include <string.h>
#include "../src/chacha20.h"

int main() {
    uint8_t data[8192];
    uint8_t original[8192];
    uint8_t key[32] = {0x00};
    uint8_t nonce[12] = {0x00};
    uint32_t counter = 1;

    // Inicializa com padrão
    for (int i = 0; i < 8192; i++) {
        data[i] = i % 256;
        original[i] = data[i];
    }
    
    // Altera a chave para não ser toda zero
    key[0] = 0x55; 
    nonce[0] = 0xAA;

    // Cifrar
    chacha20_encrypt(data, 8192, key, nonce, counter);

    // Confirma que os dados foram modificados
    int changed = 0;
    for (int i = 0; i < 8192; i++) {
        if (data[i] != original[i]) {
            changed = 1;
            break;
        }
    }
    if (!changed) {
        printf("[FAIL] Data was not modified by encryption\n");
        return 1;
    }

    // Decifrar (em ChaCha20, aplicar novamente tem o mesmo efeito)
    chacha20_encrypt(data, 8192, key, nonce, counter);

    // Checar reversão correta
    for (int i = 0; i < 8192; i++) {
        if (data[i] != original[i]) {
            printf("[FAIL] 8KB test mismatched at byte %d\n", i);
            return 1;
        }
    }
    
    printf("[OK] 8KB encrypt/decrypt test passed\n");
    return 0;
}
