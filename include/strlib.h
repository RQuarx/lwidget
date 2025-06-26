#pragma once
#ifndef __strlib__h
#define __strlib__h
#include <stddef.h>


__attribute__((__nonnull__(1)))
size_t str_len(const char *restrict p_str);

__attribute__((__nonnull__(1, 2)))
void str_copy(char       *restrict p_dst,
              const char *restrict p_src);

__attribute__((__nonnull__(1), __malloc__))
char *str_dup(const char *restrict p_str);

__attribute__((__nonnull__(1, 2)))
bool str_eq(const char *restrict p_str1,
            const char *restrict p_str2);

__attribute__((__nonnull__(1, 2)))
bool str_neq(const char *restrict p_str1,
             const char *restrict p_str2,
             size_t               p_n);

__attribute__((__nonnull__(1)))
size_t str_findc(const char *restrict p_str,
                 const char           p_char);


__attribute__((__nonnull__(1), __returns_nonnull__))
char *str_trim(char *restrict p_str);

__attribute__((__nonnull__(1)))
bool str_isnum(const char *restrict p_str);

#endif /* __strlib__h */
