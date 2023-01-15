
#ifndef COMMON_H
# define COMMON_H

# include <stddef.h>
# include <string.h>
# include <stdint.h>
# include <stdbool.h>

# ifdef __verbose
#  include <stdio.h>
#  define dprintf(...) printf(__VA_ARGS__)
# else /* no verbose */
#  define dprintf(...) /* empty */
# endif
# ifdef __debug
#  include <stdio.h>
#  define PANIC_ON(__expr) do {                                     \
    if ((__expr) == 0) {                                            \
        printf("PANIC ON %s:%d\n", __FILE__, __LINE__);             \
        abort();                                                    \
    }                                                               \
} while (false)

# define ALIGNED_16        __attribute__((__aligned__(16)))

# if defined(__x86_64__) || defined(__i386)
#  include <immintrin.h>
#  include <x86intrin.h>
#  define ___mm_iadd_32x4_impl(__a, __b) ___mm_iadd_32x4_impl_x86(__a, __b)
# else /* not x86 */
#  define ___mm_iadd_32x4_impl(__a, __b) ___mm_iadd_32x4_impl_noX86(__a, __b)
# endif

# if defined(__clang__) || defined(__GNUC__)
#  define ___roll_l32_impl(__x, __s) ___roll_l32_impl_builtin(__x, __s)
# else /* not clang or gcc */
#  define ___roll_l32_impl(__x, __s) ___roll_l32_impl_rough(__x, __s)
# endif

# define ___mm_iadd_32x4_impl_x86(__a, __b) do {                    \
    __m128i __v1 = _mm_set_epi32(__a[3], __a[2], __a[1], __a[0]);   \
    __m128i __v2 = _mm_set_epi32(__b[3], __b[2], __b[1], __b[0]);   \
    __v2 = _mm_add_epi32(__v1, __v2);                               \
    _mm_store_si128((__m128i *)__a, __v2);                          \
} while (false)

# define  ___mm_iadd_32x4_impl_noX86(__a, __b) do {                 \
    __a[0] += __b[0];                                               \
    __a[1] += __b[1];                                               \
    __a[2] += __b[2];                                               \
    __a[3] += __b[3];                                               \
} while (false)

# define ___roll_l32_impl_builtin(__x, __s) __rold(__x, __s)
# define ___roll_l32_impl_rough(__x, __s) ((__x) << (__s) | (__x) >> (32 - (__s)))

static inline void __mm_iadd_32x4(unsigned int __a[static 4], const unsigned int __b[static 4]) {
    ___mm_iadd_32x4_impl(__a, __b);
}

static inline uint32_t __roll_l32(unsigned int __x, unsigned short __s) {
    return ___roll_l32_impl(__x, __s);
}

#endif /* COMMON_H */
