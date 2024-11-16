#ifndef BASE64_H
#define BASE64_H

#include <stddef.h> // for size_t

char *base64_encode(const unsigned char *data, size_t len);
unsigned char *base64_decode(const char *encoded, size_t *out_len);

#endif // BASE64_H
