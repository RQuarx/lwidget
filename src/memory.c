#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "memory.h"
#include "utils.h"


bool
alloc(void **p_ptr,
      size_t p_obj_size,
      size_t p_obj_amount)
{
    if (p_obj_amount == 0 || p_obj_size == 0) {
        free(*p_ptr);
        *p_ptr = NULL;
        return true;
    }

    if (p_obj_amount != 0 && p_obj_size > SIZE_MAX / p_obj_amount) {
        ERR("Attempted to allocate a size larger that SIZE_MAX: %zu, %zu",
            p_obj_size, p_obj_amount);
        return false;
    }

    if (*p_ptr != NULL) {
        void *new = realloc(*p_ptr, p_obj_amount * p_obj_size);

        if (new == NULL) {
            ERR("Failed to reallocated object: %s", ERRNO);
            free(*p_ptr);
            *p_ptr = NULL;
            return false;
        }
        *p_ptr = new;
        return true;
    }

    *p_ptr = malloc(p_obj_amount * p_obj_size);
    if (*p_ptr == NULL) {
        ERR("Failed to allocate %zu bytes of memory: %s",
            p_obj_amount * p_obj_size, ERRNO);
        return false;
    }
    return true;
}
