#pragma once
#ifndef __arg__h
#define __arg__h

typedef struct _IO_FILE FILE;
typedef struct _arg_result { char *option; } arg_result;


/**
 * @brief Checks wheter @p p_current_arg is @p p_short_arg or @p p_long_arg .
 *
 * @param current_arg The argument string to check, comes from argv.
 * @param next_arg    The argument string after @p p_current_arg .
 * @param short_arg   The short-form of the option (can be NULL).
 * @param long_arg    The long-form of the option (can be NULL).
 *
 * @return A non NULL arg_result pointer if the arg exist,
 *         a non NULL arg_result pointer with a set option if an option exists,
 *         or a NULL arg_result pointer.
 */
arg_result *get_arg(const char *restrict p_current_arg,
                    const char *restrict p_next_arg,
                    const char *restrict p_short_arg,
                    const char *restrict p_long_arg);


/** @brief Retrieves the parameter value for a given command-line option. */
const char *get_param(const char *restrict p_current_arg,
                      const char *restrict p_next_arg,
                      const char *restrict p_short_arg);


void print_help_message(FILE *stream);


#endif /* __arg__h */
