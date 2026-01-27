#ifndef PRGL_COMMON_MACROS_H
#define PRGL_COMMON_MACROS_H

#ifdef __GNUC__
#ifndef UNUSED
#define UNUSED(x) UNUSED_##x __attribute__((__unused__))
#endif
#else
#ifndef UNUSED
#define UNUSED(x) UNUSED_##x
#endif
#endif

/**
 * Can be used to get the length of an array when used in the same scope it is
 * declared in. This version has some protections from passing pointers as
 * arguments, but it isn't completely foolproof.
 *
 * @param[in] x The array to get the length of
 * @return size_t The length of the array
 */
#ifndef ARR_LEN
#define ARR_LEN(x)                                                             \
    ((size_t)(sizeof(x) / sizeof(0 [x]))                                       \
     / ((size_t)(!(sizeof(x) % sizeof(0 [x])))))
#endif

#ifndef STRINGIFY
#define STRINGIFY_STAGE1(x) #x
#define STRINGIFY(x) STRINGIFY_STAGE1(x)
#endif

#endif
