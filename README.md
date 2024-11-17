# bsix

Lightweight, efficient, and self-contained Base 64 encoding/decoding library
suitable for embedding in small projects, or systems with limited resources.

**Why?**

I need a microscopic Base64 encoding/decoding library to use on my embedded
systems. This is that library, built with my own needs in mind.

Cross-platform in theory, should work on embedded systems (in my case, Risc-V)
pretty well. If it doesn't, open an issue.

## Benchmarks

The speed of bsix depends mostly on your implementation. Below is the benchmark
for my test implementation with little to no focus on performance.

| Command     |     Mean [µs] | Min [µs] | Max [µs] | Relative |
| :---------- | ------------: | -------: | -------: | -------: |
| `bsix-test` | 699.1 ± 143.6 |    556.3 |   6300.3 |     1.00 |

## Example Usage

```c
#include "base64.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    const unsigned char data[] = "Hello, World!";
    size_t data_len = sizeof(data) - 1;

    char *encoded = base64_encode(data, data_len);
    if (!encoded) {
        fprintf(stderr, "Failed to encode data\n");
        return 1;
    }

    printf("Base64 Encoded: %s\n", encoded);

    free(encoded);
    return 0;
}
```
