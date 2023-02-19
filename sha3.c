
#include <crypto.h>
#include <common.h>
#include <sha3_common.h>

#define CHUNK_LEN_BYTES     200
#define CHUNK_LEN_QWORDS    CHUNK_LEN_BYTES / 8
#define HASH_LEN_BYTES      64

#define __debug_chunk(__r, __name, __chk) do {              \
    __debug_print("%u:%s ----------", __r, __name);         \
    for (int __i = 0; __i < 25; ++__i) {                    \
        if (__i % 5 == 0) {                                 \
            __debug_print("\n");                            \
        }                                                   \
        __debug_print(" %20llu", (__chk)->as_64vec[__i]);   \
    }                                                       \
    __debug_print("\n");                                    \
} while (false)

typedef unsigned long long ptr_t;

typedef union {
    ptr_t       ptr[CHUNK_LEN_QWORDS];
    uint64_t    as_64vec[CHUNK_LEN_QWORDS];
    uint32_t    as_32vec[CHUNK_LEN_QWORDS * 2];
    uint8_t     as_8vec[CHUNK_LEN_BYTES];
} ALIGNED_8 chunk_t;

typedef union {
    ptr_t       ptr[HASH_LEN_BYTES / 8];
    uint64_t    as_64vec[HASH_LEN_BYTES / 8];
    uint32_t    as_32vec[HASH_LEN_BYTES / 4];
    uint64_t    as_8vec[HASH_LEN_BYTES];
} ALIGNED_16 hash_t;

static void keccakF(chunk_t *chunk)
{
    static const unsigned short perm_values[24] = SHA3_PERMUTATION_TABLE;
    static const unsigned short rotl_values[24] = SHA3_ROTATION_TABLE;
    static const uint64_t iota_const[24] = SHA3_IOTA_TABLE;

    uint64_t BC[5];
    uint64_t theta_temp[5];
    uint64_t temp;

# define KECCAK_ROUND_NUM 24

    for (unsigned short round = 0; round < KECCAK_ROUND_NUM; ++round) {
        /* theta */

        memset(BC, 0, 5 * 8);
        _mm_ixor_64x5(BC, chunk->as_64vec +  0);
        _mm_ixor_64x5(BC, chunk->as_64vec +  5);
        _mm_ixor_64x5(BC, chunk->as_64vec + 10);
        _mm_ixor_64x5(BC, chunk->as_64vec + 15);
        _mm_ixor_64x5(BC, chunk->as_64vec + 20);

        theta_temp[0] = BC[MOD5(0 + 4)] ^ _roll_l64(BC[MOD5(0 + 1)], 1);
        theta_temp[1] = BC[MOD5(1 + 4)] ^ _roll_l64(BC[MOD5(1 + 1)], 1);
        theta_temp[2] = BC[MOD5(2 + 4)] ^ _roll_l64(BC[MOD5(2 + 1)], 1);
        theta_temp[3] = BC[MOD5(3 + 4)] ^ _roll_l64(BC[MOD5(3 + 1)], 1);
        theta_temp[4] = BC[MOD5(4 + 4)] ^ _roll_l64(BC[MOD5(4 + 1)], 1);

        _mm_ixor_64x5(chunk->as_64vec +  0, theta_temp);
        _mm_ixor_64x5(chunk->as_64vec +  5, theta_temp);
        _mm_ixor_64x5(chunk->as_64vec + 10, theta_temp);
        _mm_ixor_64x5(chunk->as_64vec + 15, theta_temp);
        _mm_ixor_64x5(chunk->as_64vec + 20, theta_temp);
        __debug_chunk(round, "theta", chunk);

        /* rho + pi */
        temp = chunk->as_64vec[1];
        for (int i = 0; i < CHUNK_LEN_QWORDS - 1; ++i) {
            const unsigned short perm = perm_values[i];
            const uint64_t save = chunk->as_64vec[perm];
            chunk->as_64vec[perm] = _roll_l64(temp, rotl_values[i]);
            temp = save;
        }
        __debug_chunk(round, "rho+pi", chunk);

        /* chi */
        for (int i = 0; i < CHUNK_LEN_QWORDS; i += 5) {
            memcpy(BC, chunk->as_64vec + i, 40);
            chunk->as_64vec[i + 0] ^= ~BC[MOD5(0 + 1)] & BC[MOD5(0 + 2)];
            chunk->as_64vec[i + 1] ^= ~BC[MOD5(1 + 1)] & BC[MOD5(1 + 2)];
            chunk->as_64vec[i + 2] ^= ~BC[MOD5(2 + 1)] & BC[MOD5(2 + 2)];
            chunk->as_64vec[i + 3] ^= ~BC[MOD5(3 + 1)] & BC[MOD5(3 + 2)];
            chunk->as_64vec[i + 4] ^= ~BC[MOD5(4 + 1)] & BC[MOD5(4 + 2)];
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
    size_t iter = len / (r * 8);
    unsigned short remaining;

    /* absorbtion */
    memset(chunk.ptr, 0, CHUNK_LEN_BYTES);
    while (iter--) {
        _mm_ixor_64x8(chunk.as_64vec, (const void *)message);
        message += 64;
        for (int i = 8; i < r; ++i) {
            chunk.as_64vec[i] ^= LOAD_64(message);
            message += 8;
        }
        __debug_chunk(0, "start", &chunk);
        keccakF(&chunk);
    }
    remaining = len % (r * 8);
    memxor(chunk.as_64vec, chunk.as_64vec, message, len % (r * 8));
    if (UNLIKELY(remaining == r * 8 - 1)) {
        chunk.as_8vec[r * 8 - 1] ^= 0x06 | 0x80;
    } else {
        chunk.as_8vec[remaining] ^= 0x6;
        chunk.as_8vec[r * 8 - 1] ^= 0x80;
    }
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
    static char hex[129];
    static const char *alphabet = "0123456789abcdef";

    PANIC_ON(len != 224 && len != 256 && len != 384 && len != 512,
            "SHA3 len argument should be one of 224, 256, 384 or 512");

    len /= 8;
    for (unsigned short i = 0; i < len; ++i) {
        hex[i * 2] = alphabet[hash[i] / 16];
        hex[i * 2 + 1] = alphabet[hash[i] % 16];
    }
    hex[len * 2] = 0;
    return hex;
}
