
#ifndef __SHA3_COMMON_H__
# define __SHA3_COMMON_H__


const unsigned char *sha3(const char *__restrict message_ptr,
                          const size_t len,
                          const unsigned short r,
                          const unsigned short d);

# define MOD5(__val) ((__val) % 5)

# define SHA3_PERMUTATION_TABLE {           \
    10,  7, 11, 17, 18,                     \
     3,  5, 16,  8, 21,                     \
    24,  4, 15, 23, 19,                     \
    13, 12,  2, 20, 14,                     \
    22,  9,  6,  1                          \
}

# define SHA3_ROTATION_TABLE {              \
     1,  3,  6, 10, 15,                     \
    21, 28, 36, 45, 55,                     \
     2, 14, 27, 41, 56,                     \
     8, 25, 43, 62, 18,                     \
     39, 61, 20, 44                         \
}

# define SHA3_IOTA_TABLE {                  \
    0x0000000000000001, 0x0000000000008082, \
    0x800000000000808a, 0x8000000080008000, \
    0x000000000000808b, 0x0000000080000001, \
    0x8000000080008081, 0x8000000000008009, \
    0x000000000000008a, 0x0000000000000088, \
    0x0000000080008009, 0x000000008000000a, \
    0x000000008000808b, 0x800000000000008b, \
    0x8000000000008089, 0x8000000000008003, \
    0x8000000000008002, 0x8000000000000080, \
    0x000000000000800a, 0x800000008000000a, \
    0x8000000080008081, 0x8000000000008080, \
    0x0000000080000001, 0x8000000080008008, \
}

#endif /* __SHA3_COMMON_H__ */

