#include <string.h>
#include <ctype.h>

#include "memory.h"
#include "strlib.h"


size_t
str_len(const char *restrict p_str)
{
    size_t len = 0;
    while (p_str[len] != '\0') len++;
    return len;
}


void
str_copy(char       *restrict p_dst,
         const char *restrict p_src)
{ strcpy(p_dst, p_src); }


char *
str_dup(const char *restrict p_str)
{
    char *new = NULL;
    NEW(&new, char, str_len(p_str) + 1);

    if (new == NULL) return NULL;

    str_copy(new, p_str);
    return new;
}


bool
str_eq(const char *restrict p_str1,
       const char *restrict p_str2)
{ return strcmp(p_str1, p_str2) == 0; }


bool
str_neq(const char *restrict p_str1,
        const char *restrict p_str2,
        size_t               p_n)
{ return strncmp(p_str1, p_str2, p_n) == 0; }


size_t
str_findc(const char *restrict p_str,
          const char           p_char)
{
    size_t idx = 0;
    for (; *p_str != '\0'; p_str++, idx++) {
        if (*p_str == p_char) return idx;
    }
    return idx;
}


char *
str_trim(char *restrict p_str)
{
    char *end = NULL;

    while (isspace(*p_str)) p_str++;
    if (*p_str == 0) return p_str;
    while (end > p_str && isspace(*end)) end--;
    *(end + 1) = '\0';
    return p_str;
}


bool
str_isnum(const char *restrict p_str)
{
    const char *ptr = p_str;
    while (*ptr != '\0') {
        if (isdigit(*ptr) == 0) return false;
        ptr++;
    }
    return true;
}
