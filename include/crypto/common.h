
#ifndef COMMON_H
# define COMMON_H

# include <stddef.h>
# include <string.h>
# include <stdint.h>
# include <stdbool.h>

# ifdef __verbose
#  include <stdio.h>
#  define __debug_print printf
# else /* no verbose */
#  define __debug_print(...) /* empty */
# endif /* __verbose */

# ifdef __debug
#  include <stdio.h>
#  define PANIC_ON(__expr, __message) do {                          \
    if (!!(__expr)) {                                               \
        printf("PANIC ON %s:%d\n", __FILE__, __LINE__);             \
        printf("reason: %s\n", __message);                          \
        abort();                                                    \
    }                                                               \
} while (false)
# else /* no debug */
#  define PANIC_ON(__expr, __message) /* empty */
# endif /* __debug */

# define ATTRIBUTE(__attr)          __attribute__((__attr))
# define ALIGNED(__size)            ATTRIBUTE(__aligned__(__size))
# define ALIGNED_8                  ALIGNED(8)
# define ALIGNED_16                 ALIGNED(16)
# define ALIGNED_PTR                ALIGNED(sizeof(void *))

# define IS_ALIGNED_8(__ptr)        (((uint64_t)(__ptr) & 0x7) == 0)
# define IS_ALIGNED_16(__ptr)       (((uint64_t)(__ptr) & 0xF) == 0)
# define IS_ALIGNED_32(__ptr)       (((uint64_t)(__ptr) & 0x1F) == 0)
# define IS_ALIGNED_64(__ptr)       (((uint64_t)(__ptr) & 0x3F) == 0)

# define LOAD_8(__ptr)              (*(const uint8_t *)(const void *)(__ptr))
# define LOAD_16(__ptr)             (*(const uint16_t *)(const void *)(__ptr))
# define LOAD_32(__ptr)             (*(const uint32_t *)(const void *)(__ptr))
# define LOAD_64(__ptr)             (*(const uint64_t *)(const void *)(__ptr))

# define STORE_8(__ptr, __value)    (*(uint8_t *)(void *)(__ptr) = __value)
# define STORE_16(__ptr, __value)   (*(uint16_t *)(void *)(__ptr) = __value)
# define STORE_32(__ptr, __value)   (*(uint32_t *)(void *)(__ptr) = __value)
# define STORE_64(__ptr, __value)   (*(uint64_t *)(void *)(__ptr) = __value)

# define UNLIKELY(__expr)           __builtin_expect(__expr, 0)
# define LIKELY(__expr)             __builtin_expect(__expr, 1)

# if defined(__x86_64__) || defined(__i386)
#  include <immintrin.h>
#  include <x86intrin.h>
#  define ___mm_iadd_32x4_impl(__a, __b) ___mm_iadd_32x4_impl_x86(__a, __b)
#  define ___mm_ixor_64x5_impl(__a, __b) ___mm_ixor_64x5_impl_no_x86(__a, __b)
#  define ___mm_ixor_64x8_impl(__a, __b) ___mm_ixor_64x8_impl_no_x86(__a, __b)
# else /* not x86 */
#  define ___mm_iadd_32x4_impl(__a, __b) ___mm_iadd_32x4_impl_no_x86(__a, __b)
#  define ___mm_ixor_64x5_impl(__a, __b) ___mm_ixor_64x5_impl_no_x86(__a, __b)
#  define ___mm_ixor_64x8_impl(__a, __b) ___mm_ixor_64x8_impl_no_x86(__a, __b)
# endif

# if defined(__clang__) || defined(__GNUC__)
#  define ___roll_l32_impl(__x, __s) ___roll_l32_impl_builtin(__x, __s)
#  define ___roll_l64_impl(__x, __s) ___roll_l64_impl_rough(__x, __s)
# else /* not clang or gcc */
#  define ___roll_l32_impl(__x, __s) ___roll_l32_impl_rough(__x, __s)
#  define ___roll_l64_impl(__x, __s) ___roll_l64_impl_rough(__x, __s)
# endif

# define ___x4_access_ofs(__x, __ofs)                                       \
    __x[0 + __ofs], __x[1 + __ofs], __x[2 + __ofs], __x[3 + __ofs]
# define ___x8_access_ofs(__x, __ofs)                                       \
    ___x4_access_ofs(__x, 0 + __ofs), ___x4_access_ofs(__x, 1 + __ofs),     \
    ___x4_access_ofs(__x, 2 + __ofs), ___x4_access_ofs(__x, 3 + __ofs)
# define ___x16_access_ofs(__x, __ofs)                                      \
    ___x8_access_ofs(__x, 0 + __ofs), ___x8_access_ofs(__x, 4 + __ofs),     \
    ___x8_access_ofs(__x, 8 + __ofs), ___x8_access_ofs(__x, 12 + __ofs)

# define __x4_array_access(__x) ___x4_access_ofs(__x, 0)
# define __x8_array_access(__x) ___x8_access_ofs(__x, 0)
# define __x16_array_access(__x) __x16_access_ofs(__x, 0)

# define ___mm_iadd_32x4_impl_x86(__a, __b) do {                    \
    __m128i __v1 = _mm_set_epi32(__x4_array_access(__a));           \
    __m128i __v2 = _mm_set_epi32(__x4_array_access(__b));           \
    __v2 = _mm_add_epi32(__v1, __v2);                               \
    _mm_store_si128((__m128i *)__a, __v2);                          \
} while (false)

# define  ___mm_iadd_32x4_impl_no_x86(__a, __b) do {                \
    __a[0] += __b[0];                                               \
    __a[1] += __b[1];                                               \
    __a[2] += __b[2];                                               \
    __a[3] += __b[3];                                               \
} while (false)

# define ___mm_ixor_64x5_impl_x86(__a, __b) do {                    \
    __m256i __v1 = _mm256_set_epi64(__x4_array_access(__a));        \
    __m256i __v2 = _mm256_set_epi64(__x4_array_access(__b));        \
    __v2 = _mm256_xor_epi64(__v1, __v2);                            \
    _mm256_store_epi64((__m256i *)__a, __v2);                       \
    __a[4] ^= __b[4];                                               \
} while (false)

# define ___mm_ixor_64x5_impl_no_x86(__a, __b) do {                 \
    __a[0] ^= __b[0];                                               \
    __a[1] ^= __b[1];                                               \
    __a[2] ^= __b[2];                                               \
    __a[3] ^= __b[3];                                               \
    __a[4] ^= __b[4];                                               \
} while (false)

# define ___mm_ixor_64x8_impl_x86(__a, __b) do {                    \
    __m512i __v1 = _mm512_set_epi64(__x8_array_acess(__a));         \
    __m512i __v2 = _mm512_set_epi64(__x8_array_access(__b));        \
    __v2 = _mm512_xor_epi64(__v1, __v2);                            \
    _mm512_store_epi64((__m512 *)__a, __v2);                        \
}

# define ___mm_ixor_64x8_impl_no_x86(__a, __b) do {                 \
    __a[0] ^= __b[0];                                               \
    __a[1] ^= __b[1];                                               \
    __a[2] ^= __b[2];                                               \
    __a[3] ^= __b[3];                                               \
    __a[4] ^= __b[4];                                               \
    __a[5] ^= __b[5];                                               \
    __a[6] ^= __b[6];                                               \
    __a[7] ^= __b[7];                                               \
} while (false)

# define ___roll_l32_impl_builtin(__x, __s) __rold(__x, __s)
# define ___roll_l32_impl_rough(__x, __s) ((__x) << (__s) | (__x) >> (32 - (__s)))
# define ___roll_l64_impl_rough(__x, __s) ((__x) << (__s) | (__x) >> (64 - (__s)))

static inline void _mm_iadd_32x4(uint32_t __a[static 4],
                                 const uint32_t __b[static 4])
{
    //___mm_iadd_32x4_impl(a, b);
    ___mm_iadd_32x4_impl_no_x86(__a, __b);
}

static inline void _mm_ixor_64x5(uint64_t __a[static 5],
                                  const uint64_t __b[static 5])
{
    ___mm_ixor_64x5_impl(__a, __b);
}

static inline void _mm_ixor_64x8(uint64_t __a[static 8],
                                 const uint64_t __b[static 8])
{
    ___mm_ixor_64x8_impl_no_x86(__a, __b);
    //___mm_ixor_64x8_impl(__a, __b);
}

static inline uint32_t _roll_l32(uint32_t __x, unsigned short __s)
{
    return ___roll_l32_impl(__x, __s);
}

static inline uint64_t _roll_l64(uint64_t __x, unsigned short __s)
{
    return ___roll_l64_impl(__x, __s);
}

static inline void memxor(void *dest_ptr,
                          const void *src1_ptr,
                          const void *src2_ptr,
                          size_t size)
{
    uintptr_t dest, src1, src2;

    dest = (uintptr_t)dest_ptr;
    src1 = (uintptr_t)src1_ptr;
    src2 = (uintptr_t)src2_ptr;
    while (size >= 8) {
        STORE_64(dest, LOAD_64(src1) ^ LOAD_64(src2));
        src1 += 8;
        src2 += 8;
        dest += 8;
        size -= 8;
    }
    while (size) {
        STORE_8(dest, LOAD_8(src1) ^ LOAD_8(src2));
        src1 += 1;
        src2 += 1;
        dest += 1;
        --size;
    }
}

#endif /* COMMON_H */
