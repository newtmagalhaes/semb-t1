# ChaCha20 Didactic Implementation (RFC 8439)

This project provides a clean, well-documented, and didactic implementation of the ChaCha20 stream cipher, as defined in **RFC 8439**.

## Overview

ChaCha20 is a high-speed stream cipher developed by Daniel J. Bernstein. It is designed for both software and hardware efficiency and is widely used in modern protocols like TLS 1.3 and SSH.

### How it Works

1.  **State Matrix**: ChaCha20 operates on a 4x4 matrix of 32-bit words (64 bytes total).
2.  **Quarter Round**: The core operation (ARX: Addition-Rotation-XOR) is applied to columns and diagonals of the matrix.
3.  **Keystream**: After 20 rounds, the matrix is added to its initial state to produce a block of keystream.
4.  **Encryption**: The keystream is XORed with the plaintext to produce ciphertext.

## Project Structure

- `include/chacha20.h`: Public API (Encapsulation).
- `src/chacha20.c`: Core logic (Implementation).
- `tests/test_vectors.c`: Verification against RFC 8439 test vectors.
- `tests/test_nonce_reuse.c`: Demonstration of nonce reuse vulnerability.
- `main.c`: Simple demonstration of the library usage.

## The API

```c
// Symmetric XOR for encryption/decryption
void chacha20_xor(uint8_t *out, const uint8_t *in, size_t len, const uint8_t key[32], const uint8_t nonce[12], uint32_t counter);

// Generate raw keystream (Didactic)
void chacha20_generate_keystream(uint8_t keystream[64], const uint8_t key[32], const uint8_t nonce[12], uint32_t counter);
```

## Compilation and Usage

A `Makefile` is provided for convenience.

### Build all targets:
```bash
make
```

### Run TUI:
```bash
./bin/chacha20_tui
```

## Compilation Flags

The `Makefile` uses the following `gcc` flags to ensure quality and performance:

- `-Wall`: Enables all common compiler warnings. Essential for catching potential bugs.
- `-Wextra`: Enables additional warnings not covered by `-Wall`, such as signed/unsigned comparisons.
- `-O3`: Highest level of optimization for performance. It performs aggressive loop unrolling and other optimizations to maximize encryption throughput.
- `-Iinclude`: Tells the compiler to look in the `include/` directory for header files, allowing for clean `#include "chacha20.h"` in source files.

## Security Warning: Nonce Reuse

**NEVER REUSE A NONCE** with the same key. As demonstrated in `test_nonce_reuse`, if a (Key, Nonce) pair is reused for two different messages:
1.  The XOR of the two ciphertexts equals the XOR of the two plaintexts.
2.  If one plaintext is known, the other is immediately revealed.
3.  Structural information is leaked even if neither is known.

## Compliance

This implementation strictly follows the little-endian conventions and ARX operations specified in RFC 8439.
