
#include <common.h>

#define debug_chunk(__round_num, ti) do {                                       \
    dprintf("%u %2u (%10u %10u %10u %10u)\n", __round_num - 1, i,               \
        hash->as_32vec[0], hash->as_32vec[1],                                   \
        hash->as_32vec[2], hash->as_32vec[3]);                                  \
    dprintf("K(%u) S(%u) I(%u) T(%u) X(%u)\n",                                  \
        K##__round_num[i], S##__round_num[i], ti, T[ti],                        \
        chunk->as_32vec[K##__round_num[i]]);                                    \
} while (false)

#define fun1(x, y, z) ((x & y) | (~(x) & z))
#define fun2(x, y, z) ((x & z) | (~(z) & y))
#define fun3(x, y, z) (x ^ y ^ z)
#define fun4(x, y, z) (y ^ (~(z) | x))

#define ROUND(__round_num, ti) do {                                             \
    uint32_t fun_val, to_shift, new_b;                                          \
                                                                                \
    debug_chunk(__round_num, ti);                                               \
    fun_val = fun##__round_num(hash->b, hash->c, hash->d);                      \
    to_shift = hash->a + fun_val + chunk->as_32vec[K##__round_num[i]] + T[ti];  \
    new_b = hash->b + __roll_l32(to_shift, S##__round_num[i]);                  \
    hash->a = hash->d;                                                          \
    hash->d = hash->c;                                                          \
    hash->c = hash->b;                                                          \
    hash->b = new_b;                                                            \
} while (false)

typedef unsigned long long ptr_t;

typedef union {
    ptr_t           ptr[8];
    uint64_t        as_64vec[8];
    uint32_t        as_32vec[16];
    uint8_t         as_8vec[64];
} ALIGNED_16 chunk_t;

typedef union {
    ptr_t           ptr[2];
    uint64_t        as_64vec[2];
    uint32_t        as_32vec[4];
    unsigned char   as_str[16];
    struct      {
        uint32_t a;
        uint32_t b;
        uint32_t c;
        uint32_t d;
    };
} ALIGNED_16 hash_t;

void md5_step(chunk_t *chunk, hash_t *hash)
{
    static const uint32_t T[64] = {
        0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
        0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
        0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
        0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
        0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
        0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
        0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
        0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
        0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
        0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
        0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
        0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
        0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
        0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
        0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
        0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
    };
    static const unsigned short K1[16] = { 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15};
    static const unsigned short K2[16] = { 1,  6, 11,  0,  5, 10, 15,  4,  9, 14,  3,  8, 13,  2,  7, 12};
    static const unsigned short K3[16] = { 5,  8, 11, 14,  1,  4,  7, 10, 13,  0,  3,  6,  9, 12, 15,  2};
    static const unsigned short K4[16] = { 0,  7, 14,  5, 12,  3, 10,  1,  8, 15,  6, 13,  4, 11,  2,  9};

    static const unsigned short S1[16] = { 7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22};
    static const unsigned short S2[16] = { 5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20};
    static const unsigned short S3[16] = { 4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23};
    static const unsigned short S4[16] = { 6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21};

    for (unsigned short i = 0; i < 16; ++i) {
        ROUND(1, i);
    }
    for (unsigned short i = 0; i < 16; ++i) {
        ROUND(2, i + 16);
    }
    for (unsigned short i = 0; i < 16; ++i) {
        ROUND(3, i + 32);
    }
    for (unsigned short i = 0; i < 16; ++i) {
        ROUND(4, i + 48);
    }
    dprintf("(%u, %u, %u, %u)\n",
        hash->as_32vec[0], hash->as_32vec[1],
        hash->as_32vec[2], hash->as_32vec[3]);
}

const unsigned char *md5(const char *message, size_t len)
{
    static const hash_t init = { .as_32vec = { 0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476 } };
    chunk_t chunk;
    unsigned short remaining;
    hash_t hash;
    size_t offset = 0;
    static hash_t result;
    size_t iter = len / 64;

    memcpy(result.ptr, init.ptr, 16);
    while (iter--) {
        memcpy(chunk.ptr, message + offset, 64);
        memcpy(hash.ptr, result.ptr, 16);
        md5_step(&chunk, &hash);
        __mm_iadd_32x4(result.as_32vec, hash.as_32vec);
        offset += 64;
    }
    remaining = len % 64;
    if (remaining <= 55) {
        memset(chunk.ptr, 0, 64);
        memcpy(chunk.ptr, message + offset, remaining);
        chunk.as_8vec[remaining] = 0x80;
        chunk.as_64vec[7] = len * 8;
        memcpy(hash.ptr, result.ptr, 16);
        md5_step(&chunk, &hash);
        __mm_iadd_32x4(result.as_32vec, hash.as_32vec);
    } else {
        chunk.as_64vec[7] = 0;
        memcpy(chunk.ptr, message + offset, remaining);
        chunk.as_8vec[remaining] = 0x80;
        memcpy(hash.ptr, result.ptr, 16);
        md5_step(&chunk, &hash);
        __mm_iadd_32x4(result.as_32vec, hash.as_32vec);
        memset(chunk.ptr, 0, 64);
        chunk.as_64vec[7] = len * 8;
        memcpy(hash.ptr, result.ptr, 16);
        md5_step(&chunk, &hash);
        __mm_iadd_32x4(result.as_32vec, hash.as_32vec);
    }
    dprintf("[%u %u %u %u]\n",
        result.as_32vec[0], result.as_32vec[1],
        result.as_32vec[2], result.as_32vec[3]);
    return result.as_str;
}

const char *md5_hexdigest(const unsigned char hash[static 16])
{
    static char hex[33];
    static const char *alphabet = "0123456789abcdef";

    for (unsigned short i = 0; i < 16; ++i) {
        hex[i * 2] = alphabet[hash[i] / 16];
        hex[i * 2 + 1] = alphabet[hash[i] % 16];
    }
    return hex;
}
