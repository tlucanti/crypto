#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "errors.h"

/*
 * Define STATIC as an empty string to turn certain static functions public
 */
#ifndef STATIC
#define STATIC static inline
#endif

#define TRUE    1
#define FALSE   0

#ifndef MAX
#define MAX(a,b) ((a)>(b)?(a):(b))
#endif

#ifndef MIN
#define MIN(a,b) ((a)<(b)?(a):(b))
#endif

#define _PASTE(x,y) x##y
#define _PASTE2(x,y) _PASTE(x,y)

#include <stdint.h>


#ifndef UINT32_MAX
#define UINT32_MAX 0xFFFFFFFFUL
#endif

#endif /* HAVE_STDINT_H */

#ifdef _MSC_VER

#define inline _inline
#define RESTRICT __restrict

#include <malloc.h>

#else /** Not MSC **/

#if __STDC_VERSION__ >= 199901L
#define RESTRICT restrict
#else
#ifdef __GNUC__
#define RESTRICT __restrict
#define inline __inline
#else
#define RESTRICT
#define inline
#endif
#endif

#endif

/** Force checking of assertions **/
#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>

/*
 * On Windows, distutils expects that a CPython module always exports the symbol init${MODNAME}
 */
#if defined(_MSC_VER) || defined(__MINGW32__)
#include <Python.h>
#if PY_MAJOR_VERSION >= 3
#define FAKE_INIT(x) PyMODINIT_FUNC _PASTE2(PyInit__,x) (void) { return NULL; }
#else
#define FAKE_INIT(x) PyMODINIT_FUNC _PASTE2(init_,x) (void) { return; }
#endif
#else
#define FAKE_INIT(x)
#endif

/*
 * On Windows, functions must be explicitly marked for export.
 */
#if defined(_MSC_VER) || defined(__MINGW32__)
#define EXPORT_SYM __declspec(dllexport)
#else
#define EXPORT_SYM
#endif

/*
 * Platform specific routine for aligned allocation
 */
#if defined(_MSC_VER) || defined(__MINGW32__)

static inline void* align_alloc(size_t size, unsigned boundary)
{
    return _aligned_malloc(size, boundary);
}

static inline void align_free(void *mem)
{
    if (mem) {
        _aligned_free(mem);
    }
}

#elif defined(HAVE_POSIX_MEMALIGN)

static inline void* align_alloc(size_t size, unsigned boundary)
{
    int result;
    void *new_mem;
    result = posix_memalign((void**)&new_mem, boundary, size);
    return result ? NULL : new_mem;
}

static inline void align_free(void *mem)
{
    free(mem);
}




/*
 * Find first character in a string which is not c.
 */
static inline const uint8_t* memchr_not(const uint8_t* s, int c, size_t n)
{
    size_t i;

    for (i=0; i<n; i++, s++)
        if (*s != c)
            return s;
    return NULL;
}

#endif
