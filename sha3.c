
#include <common.h>
#include <sha3_common.h>

#define __debug_chunk(__r, __name, __chk) do {      \
    dprintf("%u:%s ----------", __r, __name);       \
    for (int __i = 0; __i < 25; ++__i) {            \
        if (__i % 5 == 0) {                         \
            dprintf("\n");                          \
        }                                           \
        dprintf(" %20llu", (__chk)->as_64vec[__i]); \
    }                                               \
    dprintf("\n");                                  \
} while (false)

typedef unsigned long long ptr_t;

typedef union {
    ptr_t       ptr[25];
    uint64_t    as_64vec[25];
    uint64_t    as_32vec[50];
    uint8_t     as_8vec[200];
} ALIGNED_16 chunk_t;

typedef union {
    ptr_t       ptr[8];
    uint64_t    as_64vec[8];
    uint64_t    as_32vec[16];
    uint64_t    as_8vec[64];
} ALIGNED_16 hash_t;

static void keccakF(chunk_t *chunk)
{
    static const unsigned short perm_values[24] = {
        10,  7, 11, 17, 18,
         3,  5, 16,  8, 21,
        24,  4, 15, 23, 19,
        13, 12,  2, 20, 14,
        22,  9,  6,  1
    };
    static const unsigned short rotl_values[24] = {
         1,  3,  6, 10, 15,
        21, 28, 36, 45, 55,
         2, 14, 27, 41, 56,
         8, 25, 43, 62, 18,
         39, 61, 20, 44
    };
    static const uint64_t iota_const[24] = {
        0x0000000000000001, 0x0000000000008082,
        0x800000000000808a, 0x8000000080008000,
        0x000000000000808b, 0x0000000080000001,
        0x8000000080008081, 0x8000000000008009,
        0x000000000000008a, 0x0000000000000088,
        0x0000000080008009, 0x000000008000000a,
        0x000000008000808b, 0x800000000000008b,
        0x8000000000008089, 0x8000000000008003,
        0x8000000000008002, 0x8000000000000080,
        0x000000000000800a, 0x800000008000000a,
        0x8000000080008081, 0x8000000000008080,
        0x0000000080000001, 0x8000000080008008,
    };
    uint64_t BC[5];

# define KECCAK_ROUND_NUM 24

    for (unsigned short round = 0; round < KECCAK_ROUND_NUM; ++round) {
        /* theta */
        for (int i = 0; i < 5; ++i) {
            BC[i] = chunk->as_64vec[i] ^ chunk->as_64vec[i + 5] ^
                    chunk->as_64vec[i + 10] ^ chunk->as_64vec[i + 15] ^
                    chunk->as_64vec[i + 20];
        }
        for (int i = 0; i < 5; ++i) {
            const uint64_t temp = BC[MOD5(i + 4)] ^ _roll_l64(BC[MOD5(i + 1)], 1);
            for (int j = 0; j < 25; j += 5) {
                chunk->as_64vec[i + j] ^= temp;
            }
        }
        __debug_chunk(round, "theta", chunk);

        /* rho + pi */
        uint64_t temp = chunk->as_64vec[1];
        for (int i = 0; i < 24; ++i) {
            const unsigned short perm = perm_values[i];
            const uint64_t save = chunk->as_64vec[perm];
            chunk->as_64vec[perm] = _roll_l64(temp, rotl_values[i]);
            temp = save;
        }
        __debug_chunk(round, "rho+pi", chunk);

        /* chi */
        for (int i = 0; i < 25; i += 5) {
            memcpy(BC, chunk->as_64vec + i, 40);
            for (int j = 0; j < 5; ++j) {
                chunk->as_64vec[i + j] ^= ~BC[MOD5(j + 1)] & BC[MOD5(j + 2)];
            }
        }
        __debug_chunk(round, "chi", chunk);

        /* iota */
        chunk->as_64vec[0] ^= iota_const[round];
        __debug_chunk(round, "iota", chunk);
    }
}

const unsigned char *sha3(const char *message,
                          const size_t len,
                          const unsigned short r,
                          const unsigned short d)
{
    static chunk_t chunk;
    static hash_t result;
    size_t iter = len / r;
    size_t offset = 0;
    unsigned short remaining;

    /* absorbtion */
    PANIC_ON(c + r != 200);
    memset(chunk.ptr, 0, 200);
    while (iter--) {
        for (int i = 0; i < r; ++i) {
            chunk.as_64vec[i] ^= message[offset + i];
        }
        offset += r;
        __debug_chunk(0, "start", &chunk);
        keccakF(&chunk);
    }
    remaining = len % (r * 8);
    for (int i = 0; i < remaining; ++i) {
        chunk.as_8vec[i] ^= message[offset + i];
    }
    chunk.as_8vec[remaining] ^= 0x6;
    chunk.as_8vec[r * 8 - 1] |= ~message[offset + remaining] & 0x80;
    __debug_chunk(0, "start", &chunk);
    keccakF(&chunk);

    /* squeezing */
    memcpy(result.ptr, chunk.ptr, d * 8);
    return chunk.as_8vec;
}

const unsigned char *sha3_224(const char *message, size_t len)
{
    return sha3(message, len, 18, 28);
}

const unsigned char *sha3_256(const char *message, size_t len)
{
    return sha3(message, len, 17, 32);
}

const unsigned char *sha3_384(const char *message, size_t len)
{
    return sha3(message, len, 13, 48);
}

const unsigned char *sha3_512(const char *message, size_t len)
{
    return sha3(message, len, 9, 64);
}

const char *sha3_hexdigest(const unsigned char *hash, size_t len)
{
    PANIC_ON(len != 224 && len != 256 && len != 384 && len != 512);

    static char hex[129];
    static const char *alphabet = "0123456789abcdef";

    len /= 8;
    for (unsigned short i = 0; i < len; ++i) {
        hex[i * 2] = alphabet[hash[i] / 16];
        hex[i * 2 + 1] = alphabet[hash[i] % 16];
    }
    hex[len * 2] = 0;
    return hex;
}
