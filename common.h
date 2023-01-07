
#ifndef COMMON_H
# define COMMON_H

# include <stdio.h>

# include <string.h>
# include <stdint.h>
# include <stdbool.h>

# define ALIGNED_16        __attribute__((__aligned__(16)))

const unsigned char *md5(const char *message, size_t len);
const char *md5_to_hex(const unsigned char hash[static 16]);

static inline uint32_t cyclic_shift_left(uint32_t v, unsigned short s)
{
    return v << s | v >> (32 - s);
}

#endif /* COMMON_H */
