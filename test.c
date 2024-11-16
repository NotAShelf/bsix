#include "base64.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  const char *test_str = "hello, world!";
  printf("original: %s\n", test_str);

  char *encoded =
      base64_encode((const unsigned char *)test_str, strlen(test_str));
  if (encoded) {
    printf("encoded: %s\n", encoded);

    size_t decoded_len;
    unsigned char *decoded = base64_decode(encoded, &decoded_len);
    if (decoded) {
      printf("decoded: %.*s\n", (int)decoded_len, decoded);
      free(decoded);
    } else {
      printf("decoding failed\n");
    }
    free(encoded);
  } else {
    printf("encoding failed\n");
  }

  return 0;
}
