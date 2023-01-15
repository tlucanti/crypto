
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

static void md5_step(chunk_t *chunk, hash_t *hash)
{
    static const uint32_t T[64] = MD5_T_TABLE;
    static const unsigned short K1[16] = MD5_K1_TABLE;
    static const unsigned short K2[16] = MD5_K2_TABLE;
    static const unsigned short K3[16] = MD5_K3_TABLE;
    static const unsigned short K4[16] = MD5_K4_TABLE;

    static const unsigned short S1[16] = MD5_S1_TABLE;
    static const unsigned short S2[16] = MD5_S2_TABLE;
    static const unsigned short S3[16] =
    static const unsigned short S4[16] =

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
    static const hash_t init = \
    { .as_32vec = { 0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476 } };
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
