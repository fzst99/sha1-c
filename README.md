# SHA-1 Implementation in C

A lightweight, standalone C implementation of the **SHA-1 (Secure Hash Algorithm 1)** cryptographic hash algorithm, developed from scratch without external cryptographic libraries.

## Technical Overview
This project focuses on low-level binary operations, bitwise manipulations, and memory layout techniques according to the RFC 3174 / FIPS PUB 180-1 specification.

- **Language:** Pure C (C99 compliant)
- **Scope:** Single-block message padding & hash computation (inputs up to 55 bytes)
- **Core Logic:** Implementation of logical functions ($f_0 \dots f_{79}$), bitwise left rotations, and standard SHA-1 constant schedules ($K_0 \dots K_3$)

## Building & Running

### Prerequisites
- GCC or Clang compiler
- Standard C C-runtime library

### Compilation
```bash
gcc main.c -o main.exe
```
### Usage
```bash
main.exe STRING
```
### NIST test vector Verification
To verify that your build matches the correct implementation, run the following test vector command :
| Target (Input) | Expected Result |
| :--- | :--- |
| `"abc"` | `a9993e364706816aba3e25717850c26c9cd0d89d` |
| `""` *(Empty String)* | `da39a3ee5e6b4b0d3255bfef95601890afd80709` |

## Coming Soon
Here are some upcoming features planned for future updates : 

- ➔ **Multi-Block Support:** Extending the padding mechanism to handle arbitrarily long messages (beyond the single 512-bit block limitation)
- ➔ **Interactive CLI / TUI:** Building a cleaner terminal interface to easily hash files or custom inputs on the fly
- ➔ **Graphical Interface (GUI):** A lightweight desktop companion app to visualize live hashing and byte padding steps
- ➔ **File-Based Hashing:** Adding support for hashing entire local files via file stream inputs rather than relying solely on terminal arguments
