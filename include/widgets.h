#pragma once
#ifndef __widgets__h
#define __widgets__h
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "types.h"

#define __vec_growth_factor 1.5


__attribute__((__nonnull__(1, 2), __malloc__, __warn_unused_result__))
widget *widget_new(const char *restrict p_class,
                   const char *restrict p_name);


__attribute__((__nonnull__(1), __malloc__, __warn_unused_result__))
window *window_new(const char *p_name);


__attribute__((__nonnull__(1, 2, 3)))
void widget_add_property_str(widget     *restrict p_widget,
                             const char *restrict p_name,
                             const char *restrict p_value);


__attribute__((__nonnull__(1, 2)))
void widget_add_property_int(widget     *restrict p_widget,
                             const char *restrict p_name,
                             int32_t              p_value);


__attribute__((__nonnull__(1, 2)))
void widget_add_property_bool(widget     *restrict p_widget,
                              const char *restrict p_name,
                              bool                 p_value);


__attribute__((__nonnull__(1, 2)))
void widget_add_child(widget *restrict p_root,
                      widget *restrict p_child);


__attribute__((__nonnull__(1)))
void widget_free    (widget *p_widget);
__attribute__((__nonnull__(1)))
void widget_free_all(widget *p_widget);

typedef struct lua_State lua_State;
__attribute__((__nonnull__(1), __returns_nonnull__))
widget *lua_get_widget_recursive(lua_State *restrict p_lua,
                                 int32_t             p_idx);


#endif /* __widgets__h */
