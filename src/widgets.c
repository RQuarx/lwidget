#include <string.h>

#include <lua.h>

#include "widgets.h"
#include "memory.h"
#include "strlib.h"
#include "utils.h"


widget *
widget_new(const char *restrict p_class,
           const char *restrict p_name)
{
    widget *w = NULL;
    NEW(&w, widget, 1);

    w->name       = strdup(p_name);
    w->type       = widget_type_from_class(p_class);
    w->children   = NULL;
    w->properties = NULL;

    return w;
}


void
widget_add_property_str(widget     *restrict p_widget,
                        const char *restrict p_name,
                        const char *restrict p_value)
{
    w_property *prop = NULL;
    NEW(&prop, w_property, 1);

    bool dup_string = false;

    if (file_exist(p_name)) {
        prop->val_type = PV_PATH;
        dup_string = true;
    } else if (orientation_from_string(p_value, &prop->val_orientation)) {
        prop->val_type = PV_ORIENTATION;
    } else if (alignment_from_string  (p_value, &prop->val_align)) {
        prop->val_type = PV_ALIGN;
    } else if (str_isnum(p_value)) {
        prop->val_type = PV_INT;
        prop->val_int  = atoi(p_value);
    } else {
        prop->val_type = PV_STRING;
        dup_string = true;
    }

    if (dup_string) prop->val_str = strdup(p_value);
    prop->type     = property_type_from_name(p_name);
    prop->next     = NULL;

    if (p_widget->properties == NULL) {
        p_widget->properties = prop;
    } else {
        w_property *list = p_widget->properties;
        while (list->next != NULL) list = list->next;
        list->next = prop;
    }

    INFO("Added a property to %s: %s: %s",
         p_widget->name, p_name, p_value);
}


void
widget_add_property_int(widget     *restrict p_widget,
                        const char *restrict p_name,
                        int32_t              p_value)
{
    w_property *prop = NULL;
    NEW(&prop, w_property, 1);

    prop->val_int  = p_value;
    prop->val_type = PV_INT;
    prop->type     = property_type_from_name(p_name);
    prop->next     = NULL;

    if (p_widget->properties == NULL) {
        p_widget->properties = prop;
    } else {
        w_property *list = p_widget->properties;
        while (list->next != NULL) list = list->next;
        list->next = prop;
    }

    INFO("Added a property to %s: %s: %d",
         p_widget->name, p_name, p_value);
}


void
widget_add_property_bool(widget     *restrict p_widget,
                         const char *restrict p_name,
                         bool                 p_value)
{
    w_property *prop = NULL;
    NEW(&prop, w_property, 1);

    prop->val_bool = p_value;
    prop->val_type = PV_BOOL;
    prop->type     = property_type_from_name(p_name);
    prop->next     = NULL;

    if (p_widget->properties == NULL) {
        p_widget->properties = prop;
    } else {
        w_property *list = p_widget->properties;
        while (list->next != NULL) list = list->next;
        list->next = prop;
    }

    INFO("Added a property to %s: %s: %s",
         p_widget->name, p_name, p_value ? "true" : "false");
}


void
widget_add_child(widget *restrict p_root,
                 widget *restrict p_child)
{
    p_root->children = g_list_append(p_root->children, p_child);
}


void
widget_free(widget *p_widget)
{
    w_property *curr = p_widget->properties;
    while (curr != NULL) {
        w_property *next = curr->next;

        if (curr->val_type & (PV_PATH | PV_STRING)) {
            DELETE(&curr->val_str);
        }

        DELETE(&curr);
        curr = next;
    }

    DELETE(&p_widget->properties);
    DELETE(&p_widget->children);
    DELETE(&p_widget);
}


static void
g_widget_free(void *p_widget)
{ widget_free(p_widget); }


void
widget_free_all(widget *p_widget)
{
    g_list_free_full(p_widget->children, g_widget_free);
    widget_free(p_widget);
}


__attribute__((__nonnull__(1, 2)))
static void
lua_widget_apply_properties(widget    **restrict p_widget,
                            lua_State  *restrict p_lua,
                            int32_t              p_idx)
{
    lua_pushnil(p_lua);
    while (lua_next(p_lua, p_idx)) {
        /* -1 would be the value, -2 would be the name. */
        switch (lua_type(p_lua, -1)) {
        case LUA_TSTRING:
            widget_add_property_str(*p_widget,
                                    lua_tostring(p_lua, -2),
                                    lua_tostring(p_lua, -1));
            break;
        case LUA_TNUMBER:
            if (lua_isinteger(p_lua, -1)) {
                widget_add_property_int(*p_widget,
                                        lua_tostring(p_lua, -2),
                                        lua_tointeger(p_lua, -1));
                break;
            }
        case LUA_TBOOLEAN:
            widget_add_property_bool(*p_widget,
                                        lua_tostring(p_lua, -2),
                                        lua_toboolean(p_lua, -1));
            break;
        default:
            ERR("Invalid type as value to %s", lua_tostring(p_lua, -2));
        }

        lua_pop(p_lua, 1);
    }
}


widget *
lua_get_widget_recursive(lua_State *restrict p_lua,
                         int32_t             p_idx)
{
    widget *w = NULL;

    lua_pushnil(p_lua);
    while (lua_next(p_lua, p_idx)) {
        /* p_idx + 1: table */
        lua_getfield(p_lua, p_idx + 2, "class");
        lua_getfield(p_lua, p_idx + 2, "name");

        w = widget_new(lua_tostring(p_lua, p_idx + 3),
                       lua_tostring(p_lua, p_idx + 4));
        lua_pop(p_lua, 2);

        lua_getfield(p_lua, p_idx + 2, "properties");

        if (!lua_isnoneornil(p_lua, p_idx + 3)) {
            lua_widget_apply_properties(&w, p_lua, p_idx + 3);
        }
        lua_pop(p_lua, 1);

        if (lua_gettop(p_lua) >= p_idx + 2) {
            lua_getfield(p_lua, p_idx + 2, "children");

            if (!lua_isnoneornil(p_lua, p_idx + 3)) {
                widget_add_child(w, lua_get_widget_recursive(p_lua, p_idx + 3));
            }
            lua_pop(p_lua, 1);
        }

        lua_pop(p_lua, 1);
    }

    return w;
}
