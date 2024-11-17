#include "base64.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// charset
static const char base64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                   "abcdefghijklmnopqrstuvwxyz"
                                   "0123456789+/";

// reverse lookup table for decoding
static const unsigned char base64_lookup[256] = {
    [0 ... 255] = 0xFF, // default invalid
    ['A'] = 0,          ['B'] = 1,  ['C'] = 2,  ['D'] = 3,  ['E'] = 4,
    ['F'] = 5,          ['G'] = 6,  ['H'] = 7,  ['I'] = 8,  ['J'] = 9,
    ['K'] = 10,         ['L'] = 11, ['M'] = 12, ['N'] = 13, ['O'] = 14,
    ['P'] = 15,         ['Q'] = 16, ['R'] = 17, ['S'] = 18, ['T'] = 19,
    ['U'] = 20,         ['V'] = 21, ['W'] = 22, ['X'] = 23, ['Y'] = 24,
    ['Z'] = 25,         ['a'] = 26, ['b'] = 27, ['c'] = 28, ['d'] = 29,
    ['e'] = 30,         ['f'] = 31, ['g'] = 32, ['h'] = 33, ['i'] = 34,
    ['j'] = 35,         ['k'] = 36, ['l'] = 37, ['m'] = 38, ['n'] = 39,
    ['o'] = 40,         ['p'] = 41, ['q'] = 42, ['r'] = 43, ['s'] = 44,
    ['t'] = 45,         ['u'] = 46, ['v'] = 47, ['w'] = 48, ['x'] = 49,
    ['y'] = 50,         ['z'] = 51, ['0'] = 52, ['1'] = 53, ['2'] = 54,
    ['3'] = 55,         ['4'] = 56, ['5'] = 57, ['6'] = 58, ['7'] = 59,
    ['8'] = 60,         ['9'] = 61, ['+'] = 62, ['/'] = 63, ['='] = 0 // padding
};

static void base64_encode_triplet(uint32_t triple, char *encoded, size_t *j) {
  const char *base64_chars_ptr = base64_chars;

  encoded[(*j)++] = base64_chars_ptr[(triple >> 18) & 0x3F];
  encoded[(*j)++] = base64_chars_ptr[(triple >> 12) & 0x3F];
  encoded[(*j)++] = base64_chars_ptr[(triple >> 6) & 0x3F];
  encoded[(*j)++] = base64_chars_ptr[triple & 0x3F];
}

static int base64_decode_sextets(const char *encoded, size_t *i,
                                 uint32_t *sextet_a, uint32_t *sextet_b,
                                 uint32_t *sextet_c, uint32_t *sextet_d) {
  *sextet_a = base64_lookup[(unsigned char)encoded[(*i)++]];
  *sextet_b = base64_lookup[(unsigned char)encoded[(*i)++]];
  *sextet_c = base64_lookup[(unsigned char)encoded[(*i)++]];
  *sextet_d = base64_lookup[(unsigned char)encoded[(*i)++]];

  if ((*sextet_a | *sextet_b | *sextet_c | *sextet_d) == 0xFF) {
    return 1; // invalid character encountered
  }
  return 0;
}

// encode
char *base64_encode(const unsigned char *data, size_t len) {
  if (!data)
    return NULL;

  size_t out_len = 4 * ((len + 2) / 3); // rounding up for padding
  char *encoded = malloc(out_len + 1);  // allocate memory for encoded string
  if (!encoded)
    return NULL;

  size_t i, j = 0;
  for (i = 0; i < len;) {
    uint32_t octet_a = i < len ? data[i++] : 0;
    uint32_t octet_b = i < len ? data[i++] : 0;
    uint32_t octet_c = i < len ? data[i++] : 0;
    uint32_t triple = (octet_a << 16) | (octet_b << 8) | octet_c;

    base64_encode_triplet(triple, encoded, &j);

    if (i > len)
      encoded[j - 1] = '=';
    if (i > len + 1)
      encoded[j - 2] = '=';
  }

  encoded[out_len] = '\0'; // null-terminate the string
  return encoded;
}

// decode
unsigned char *base64_decode(const char *encoded, size_t *out_len) {
  if (!encoded)
    return NULL;

  size_t len = strlen(encoded);
  if (len % 4 != 0)
    return NULL;

  *out_len = len / 4 * 3;
  if (encoded[len - 1] == '=')
    (*out_len)--;
  if (encoded[len - 2] == '=')
    (*out_len)--;

  unsigned char *decoded =
      malloc(*out_len); // allocate memory for the decoded result
  if (!decoded)
    return NULL;

  size_t i, j = 0;
  uint32_t sextet_a, sextet_b, sextet_c, sextet_d;
  for (i = 0; i < len;) {
    if (base64_decode_sextets(encoded, &i, &sextet_a, &sextet_b, &sextet_c,
                              &sextet_d)) {
      free(decoded); // free if error occurs
      return NULL;
    }

    uint32_t triple =
        (sextet_a << 18) | (sextet_b << 12) | (sextet_c << 6) | sextet_d;

    // writing decoded bytes directly into the output
    if (j < *out_len)
      decoded[j++] = (triple >> 16) & 0xFF;
    if (j < *out_len)
      decoded[j++] = (triple >> 8) & 0xFF;
    if (j < *out_len)
      decoded[j++] = triple & 0xFF;
  }

  return decoded;
}
