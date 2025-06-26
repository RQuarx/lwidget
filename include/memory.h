#pragma once
#ifndef __memory__h
#define __memory__h
#include <stdbool.h>

#include <stddef.h>


/** @brief Allocates, frees, or reallocates @p p_ptr .
 * If @p p_obj_amount or @p p_obj_size is zero, frees the memory at @p *p_ptr.
 * Otherwise, reallocates if @p *p_ptr is non-NULL, or allocates new memory.
 *
 * @note On failure, frees @p *p_ptr and sets it to NULL.
 *
 * @param p_ptr        Pointer to the memory pointer to manage.
 * @param p_obj_size   Size of each object.
 * @param p_obj_amount Number of objects.
 * @return true on success, or false on allocation failure or overflow.
 */
bool alloc(void **p_ptr,
           size_t p_obj_size,
           size_t p_obj_amount);

#define NEW(p_ptr, p_type, p_amount) \
    alloc((void **)(p_ptr), sizeof(p_type), p_amount)

#define RENEW(p_ptr, p_new_size) \
    alloc((void **)(p_ptr), sizeof(p_ptr), p_new_size)

#define DELETE(p_ptr) \
    alloc((void **)(p_ptr), 0, 0)

#endif /* __memory__h */
