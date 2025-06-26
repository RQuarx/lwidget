#pragma once
#ifndef __utils__h
#define __utils__h
#include <stdint.h>
#include <stdio.h>
#include <errno.h>


#define THROW(p_fmt, ...) \
{ \
    fprintf(stderr, \
        "Program throwed at %s(...), in %s:%d\n", \
        __func__, \
        __FILE__, \
        __LINE__); \
    \
    fprintf(stderr, "    what(): " p_fmt __VA_OPT__(,) __VA_ARGS__); \
    fputc('\n', stderr); \
    fflush(stderr); \
    exit(1); \
}


#ifndef __fatal_err
#   define ERR(p_fmt, ...) \
    { \
        fprint_time(stderr); \
        fprintf(stderr, \
            "[\033[1;31merror\033[0;0m at %s( %s:%d )]: ", \
            __func__, \
            __FILE__, \
            __LINE__); \
        fprintf(stderr, p_fmt __VA_OPT__(,) __VA_ARGS__); \
        fputc('\n', stderr); \
        fflush(stderr); \
    }
#else
#   define ERR(p_fmt, ...) THROW(p_fmt __VA_OPT__(,) __VA_ARGS__)
#endif

#define INFO(p_fmt, ...) \
{ \
    fprint_time(stdout);  \
    printf("[\033[1;32minfo\033[0;0m at %s(...)]: ", __func__); \
    printf(p_fmt __VA_OPT__(,) __VA_ARGS__); \
    putchar('\n'); \
    fflush(stdout); \
}

#define ASSERT(p_condition) \
    if (p_condition == 0) {THROW("Assertion '%s' failed.", #p_condition); }

#define ERRNO strerror(errno)
#define LERRNO(p_lua) lua_tostring(p_lua, -1) lua_pop(p_lua, 1)


void clean_exit();

typedef struct _IO_FILE FILE;
__attribute__((__nonnull__(1)))
void fprint_time(FILE *restrict p_stream);

typedef struct lua_State lua_State;
__attribute__((__nonnull__(1)))
const char *lerrno(lua_State *p_lua);

__attribute__((__nonnull__(1)))
int32_t l_atpanic(lua_State *p_lua);

__attribute__((__nonnull__(1)))
bool file_exist(const char *restrict p_file);

#endif /* __utils__h */
