#include "base64.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_base64(const char *input) {
  printf("original: %s\n", input);

  char *encoded = base64_encode((const unsigned char *)input, strlen(input));
  if (!encoded) {
    fprintf(stderr, "error: base64 encoding failed\n");
    return;
  }
  printf("encoded: %s\n", encoded);

  size_t decoded_len;
  unsigned char *decoded = base64_decode(encoded, &decoded_len);
  if (!decoded) {
    fprintf(stderr, "error: base64 decoding failed\n");
    free(encoded);
    return;
  }

  if (strncmp(input, (const char *)decoded, decoded_len) == 0) {
    printf("decoded: %.*s\n", (int)decoded_len, decoded);
    printf("test passed: decoded output matches original input\n");
  } else {
    fprintf(stderr,
            "test failed: decoded output does not match original input\n");
  }

  free(encoded);
  free(decoded);
}

// run tests for predefined test cases
int main() {
  const char *test_cases[] = {
      "hello, world!", "", "special chars: !@#$%^&*()_+-=[]{}|;:'\",.<>?/`~",
      "a longer line that spans over two lines to test if the decoding "
      "functionality works"
      "properly",
      NULL // sentinel value
  };

  for (int i = 0; test_cases[i] != NULL; i++) {
    printf("\n=== Test Case %d ===\n", i + 1);
    test_base64(test_cases[i]);
  }

  return 0;
}
