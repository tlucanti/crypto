
#include <common.h>
#include <md5_common.h>

#define CHUNK_LEN_BYTES 64
#define HASH_LEN_BYTES  16
#define PADDING_BYTE    0x80

#define __debug_chunk(__round_num, ti) do {                                     \
    Dprintf("%u %2u (%10u %10u %10u %10u)\n", __round_num - 1, i,               \
        hash->as_32vec[0], hash->as_32vec[1],                                   \
        hash->as_32vec[2], hash->as_32vec[3]);                                  \
    Dprintf("K(%u) S(%u) I(%u) T(%u) X(%u)\n",                                  \
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
    __debug_chunk(__round_num, ti);                                             \
    fun_val = fun##__round_num(hash->b, hash->c, hash->d);                      \
    to_shift = hash->a + fun_val + chunk->as_32vec[K##__round_num[i]] + T[ti];  \
    new_b = hash->b + _roll_l32(to_shift, S##__round_num[i]);                   \
    hash->a = hash->d;                                                          \
    hash->d = hash->c;                                                          \
    hash->c = hash->b;                                                          \
    hash->b = new_b;                                                            \
} while (false)

typedef unsigned long long ptr_t;

typedef union {
    ptr_t           ptr[CHUNK_LEN_BYTES / 8];
    uint64_t        as_64vec[CHUNK_LEN_BYTES / 8];
    uint32_t        as_32vec[CHUNK_LEN_BYTES / 4];
    uint8_t         as_8vec[CHUNK_LEN_BYTES];
} ALIGNED_16 chunk_t;


typedef union {
    ptr_t           ptr[HASH_LEN_BYTES / 8];
    uint64_t        as_64vec[HASH_LEN_BYTES / 8];
    uint32_t        as_32vec[HASH_LEN_BYTES / 4];
    unsigned char   as_str[HASH_LEN_BYTES];
    struct      {
        uint32_t a;
        uint32_t b;
        uint32_t c;
        uint32_t d;
    };
} ALIGNED_16 hash_t;

static void md5_step(chunk_t *__restrict chunk, hash_t *__restrict hash)
{
    static const uint32_t T[64] = MD5_T_TABLE;
    static const unsigned short K1[16] = MD5_K1_TABLE;
    static const unsigned short K2[16] = MD5_K2_TABLE;
    static const unsigned short K3[16] = MD5_K3_TABLE;
    static const unsigned short K4[16] = MD5_K4_TABLE;

    static const unsigned short S1[16] = MD5_S1_TABLE;
    static const unsigned short S2[16] = MD5_S2_TABLE;
    static const unsigned short S3[16] = MD5_S3_TABLE;
    static const unsigned short S4[16] = MD5_S4_TABLE;

# define STAGE_NUM  16

    for (unsigned short i = 0; i < STAGE_NUM; ++i) {
        ROUND(1, i + HASH_LEN_BYTES * 0);
    }
    for (unsigned short i = 0; i < STAGE_NUM; ++i) {
        ROUND(2, i + HASH_LEN_BYTES * 1);
    }
    for (unsigned short i = 0; i < STAGE_NUM; ++i) {
        ROUND(3, i + HASH_LEN_BYTES * 2);
    }
    for (unsigned short i = 0; i < STAGE_NUM; ++i) {
        ROUND(4, i + HASH_LEN_BYTES * 3);
    }
    Dprintf("(%u, %u, %u, %u)\n",
        hash->as_32vec[0], hash->as_32vec[1],
        hash->as_32vec[2], hash->as_32vec[3]);
}

const unsigned char *md5(const char *__restrict message, size_t len)
{
    static const hash_t init = { .as_32vec = HASH_INIT_VALUE };
    chunk_t chunk;
    unsigned short remaining;
    hash_t hash;
    size_t offset = 0;
    static hash_t result;
    size_t iter = len / CHUNK_LEN_BYTES;

    memcpy(result.ptr, init.ptr, HASH_LEN_BYTES);
    while (iter--) {
        memcpy(chunk.ptr, message + offset, CHUNK_LEN_BYTES);
        memcpy(hash.ptr, result.ptr, HASH_LEN_BYTES);
        md5_step(&chunk, &hash);
        _mm_iadd_32x4(result.as_32vec, hash.as_32vec);
        offset += CHUNK_LEN_BYTES;
    }
    remaining = len % CHUNK_LEN_BYTES;
    if (remaining < CHUNK_LEN_BYTES - 8) {
        memset(chunk.ptr, 0, CHUNK_LEN_BYTES);
        memcpy(chunk.ptr, message + offset, remaining);
        chunk.as_8vec[remaining] = PADDING_BYTE;
        chunk.as_64vec[7] = len * 8;
        memcpy(hash.ptr, result.ptr, HASH_LEN_BYTES);
        md5_step(&chunk, &hash);
        _mm_iadd_32x4(result.as_32vec, hash.as_32vec);
    } else {
        chunk.as_64vec[7] = 0;
        memcpy(chunk.ptr, message + offset, remaining);
        chunk.as_8vec[remaining] = PADDING_BYTE;
        memcpy(hash.ptr, result.ptr, HASH_LEN_BYTES);
        md5_step(&chunk, &hash);
        _mm_iadd_32x4(result.as_32vec, hash.as_32vec);
        memset(chunk.ptr, 0, CHUNK_LEN_BYTES);
        chunk.as_64vec[7] = len * 8;
        memcpy(hash.ptr, result.ptr, HASH_LEN_BYTES);
        md5_step(&chunk, &hash);
        _mm_iadd_32x4(result.as_32vec, hash.as_32vec);
    }
    Dprintf("[%u %u %u %u]\n",
        result.as_32vec[0], result.as_32vec[1],
        result.as_32vec[2], result.as_32vec[3]);
    return result.as_str;
}

const char *md5_hexdigest(const unsigned char hash[static HASH_LEN_BYTES])
{
# define HASH_LEN_DIGITS    HASH_LEN_BYTES * 2
# define HEX_BASE           16

    static char hex[HASH_LEN_DIGITS + 1];
    static const char *alphabet = "0123456789abcdef";

    for (unsigned short i = 0; i < HASH_LEN_DIGITS; ++i) {
        hex[i * 2] = alphabet[hash[i] / HEX_BASE];
        hex[i * 2 + 1] = alphabet[hash[i] % HEX_BASE];
    }
    hex[HASH_LEN_DIGITS] = 0;
    return hex;
}
