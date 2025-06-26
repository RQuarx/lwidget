#include <stdlib.h>
#include <string.h>
#include "memory.h"
#include "strlib.h"
#include "utils.h"
#include "arg.h"


arg_result *
get_arg(const char *restrict p_current_arg,
        const char *restrict p_next_arg,
        const char *restrict p_short_arg,
        const char *restrict p_long_arg)
{
    const size_t current_len = strlen(p_current_arg);

    /* Checks if the passed parameters were invalid. */
    if (p_short_arg      == NULL && p_long_arg == NULL) return NULL;
    if (p_current_arg    == NULL) return NULL;
    if (p_current_arg[0] != '-' ) return NULL;

    if (p_long_arg[0] != '-' || p_short_arg[0] != '-') {
        THROW("Arg definitions must contains dashes!");
    }

    size_t eq_idx = str_findc(p_current_arg, '=');

    /* If eq_idx is not found, then the args would be clean,
       or has no modifications done to it, like:
           - '--help'
           - '--option this'
           - '-V'
    */
    if (eq_idx >= current_len) {
        if (p_short_arg != NULL && str_eq(p_current_arg, p_short_arg)) {
            goto check_param;
        }
        if (p_long_arg  != NULL && str_eq(p_current_arg, p_long_arg )) {
            goto check_param;
        }
        goto check_short;
    }

    /* If there are an '=', then we should check until eq_idx if the
       arg matches with the definition.
    */
    if (p_long_arg  != NULL && strlen(p_long_arg)  == eq_idx)
        if (str_neq(p_current_arg, p_long_arg,  eq_idx - 1)) goto check_param;
    if (p_short_arg != NULL && strlen(p_short_arg) == eq_idx)
        if (str_neq(p_current_arg, p_short_arg, eq_idx - 1)) goto check_param;


check_short:
    /* Exit early if the current arg value is a long arg ('--arg') */
    if (p_current_arg[1] == '-') return NULL;

    /* We tries to find the short arg using str_findc, because short args
       can be combined into a single arg, like -Syu.
    */
    if (str_findc(p_current_arg, p_short_arg[1]) < current_len)
        goto check_param;
    return NULL;

check_param:;
    arg_result *retval = NULL;
    NEW(&retval, arg_result, 1);

    const char *param = get_param(p_current_arg, p_next_arg, p_short_arg);

    /* No param, shouldn't allocated memory to the retval.param */
    p_current_arg = NULL;
    if (param == NULL) return retval;

    p_next_arg = NULL;
    retval->option = str_dup(param);
    return retval;
}


const char *
get_param(const char *restrict p_current_arg,
          const char *restrict p_next_arg,
          const char *restrict p_short_arg)
{
    if (p_current_arg[0] != '-' || p_current_arg[1] == '-')
        goto next;

    size_t char_idx = str_findc(p_current_arg, p_short_arg[1]);

    /* Check for '-I=param' */
    if (p_current_arg[char_idx + 1] == '=')
        return p_current_arg + char_idx + 2;

    /* Check for '-Iparam' */
    if (p_current_arg[char_idx + 1] != '\0')
        return p_current_arg + char_idx + 1;

next:;
    size_t eq_idx = str_findc(p_current_arg, '=');

    /* Check for '--arg=param' */
    if (eq_idx != strlen(p_current_arg))
        return p_current_arg + eq_idx + 1;

    /* No params were given since the next arg
       is either NULL or another arg.
    */
    if (p_next_arg    == NULL) return NULL;
    if (p_next_arg[0] == '-' ) return NULL;

    /* With all of those checks passed,
       it'll mean that p_next_arg would be the param.
    */
    return p_next_arg;
}


void
print_help_message(FILE *stream)
{
    fprintf(stream,
    "Usage: %s <options {param}>\n"
    "\n"
    "Options:\n"
    "\t-h --help                        Shows this message.\n"
    "\t-V --version                     Shows the program's version.\n"
    "\t-c --config                      Specify the config file.\n",
    __app_name
    );
    exit(0);
}
