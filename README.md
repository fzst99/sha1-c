# SHA-1 Implementation in C

A lightweight, standalone C implementation of the **SHA-1 (Secure Hash Algorithm 1)** cryptographic hash algorithm, developed from scratch without external cryptographic libraries.

## Technical Overview
This project focuses on low-level binary operations, bitwise manipulations, and memory layout techniques according to the RFC 3174 / FIPS PUB 180-1 specification.

* **Language:** Pure C (C99 compliant)
* **Scope:** Single-block message padding & hash computation (inputs up to 55 bytes)
* **Core Logic:** Implementation of logical functions ($f_0 \dots f_{79}$), bitwise left rotations, and standard SHA-1 constant schedules ($K_0 \dots K_3$)

##  Building & Running

### Prerequisites
* GCC or Clang compiler
* Standard C C-runtime library

### Compilation
```bash
gcc -O2 main.c -o sha1_test
