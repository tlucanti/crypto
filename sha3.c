
#include <common.h>

typedef union {
    uint64_t    as_64vec[25];
    uint64_t    as_32vec[50];
    uint8_t     as_8vec[200];
} ALIGNED16 chunk_t;

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

    for (unsigned short round = 0; round; ++round) {

    }
}

const unsigned char *sha3(const char *message, const size_t len, const unsigned short r, const unsigned short d)
{
    static chunk_t result;
    const unsigned short c = d * 2;

    PANIC_ON(c + r != 200);
}

const unsigned char *sha3_224(const char *message, size_t len)
{
    return sha3(message, len, 144, 28);
}

const unsigned char *sha3_256(const char *message, size_t len)
{
    return sha3(message, len, 136, 32);
}

const unsigned char *sha3_384(const char *message, size_t len)
{
    return sha3(message, len, 104, 48);
}

const unsigned char *sha3_512(const char *message, size_t len)
{
    return sha3(message, len, 72, 64);
}