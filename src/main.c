#include "glib.h"
#include <stdint.h>

#include <gtk-layer-shell.h>
#include <gtk/gtk.h>

#include <lauxlib.h>
#include <lualib.h>
#include <lua.h>

#if !__release /* If the meson buildtype is release, we don't test. */
#   define __test
#else
#endif

#include "widgets.h"
#include "memory.h"
#include "strlib.h"
#include "utils.h"
#include "arg.h"
#include "gtk.h"


__attribute__((__nonnull__(2), __malloc__))
static void *lua_alloc(void  * /* p_user_defined */,
                       void  *p_ptr,
                       size_t  /* p_original_size */,
                       size_t p_new_size)
{
    alloc(&p_ptr, sizeof(typeof(p_ptr)), p_new_size);
    return p_ptr;
}


__attribute__((__nonnull__(1)))
bool handle_css_context(lua_State *p_lua)
{
    if (lua_getglobal(p_lua, "css_path") != LUA_TNIL) {
        GtkCssProvider *css = gtk_css_provider_new();
        GError *err = NULL;
        if (!gtk_css_provider_load_from_path(css,
                                             lua_tostring(p_lua, -1),
                                             &err)) {
            ERR("Failed to load CSS from path %s: %s",
                lua_tostring(p_lua, -1),
                err->message);
            return false;
        }
        gtk_style_context_add_provider_for_screen(
            gdk_screen_get_default(),
            GTK_STYLE_PROVIDER(css),
            GTK_STYLE_PROVIDER_PRIORITY_USER
        );

        INFO("Load CSS from path: %s", lua_tostring(p_lua, -1));
        lua_pop(p_lua, 1);
    } else if (lua_getglobal(p_lua, "css_text") != LUA_TNIL) {
        size_t      len  = 0;
        const char *data = lua_tolstring(p_lua, -1, &len);

        GtkCssProvider *css = gtk_css_provider_new();
        GError *err = NULL;
        if (!gtk_css_provider_load_from_data(css, data, len, &err)) {
            ERR("Failed to load CSS from data: %s", err->message);
            return false;
        }
        gtk_style_context_add_provider_for_screen(
            gdk_screen_get_default(),
            GTK_STYLE_PROVIDER(css),
            GTK_STYLE_PROVIDER_PRIORITY_USER
        );

        INFO("Load CSS from data/text");
        lua_pop(p_lua, 2);
    }
    return true;
}


__attribute__((__nonnull__(1)))
bool get_config_file(char *restrict p_path)
{
    if (p_path[0] == '\0') {
        if (file_exist("config.lua")) {
            str_copy(p_path, "config.lua");
            return true;
        }

        if (file_exist("lwidget/config.lua")) {
            str_copy(p_path, "lwidget/config.lua");
            return true;
        }

        /* $HOME/.config/lwidget/config.lua */
        const char *home = getenv("HOME");
        if (home == NULL) {
            ERR("$HOME is not set!");
            exit(EXIT_FAILURE);
        }

        snprintf(p_path, PATH_MAX, "%s/.config/lwidget/config.lua", home);
        if (file_exist(p_path)) return true;
    }

    if (!file_exist(p_path)) {
        ERR("Config file does not exist!");
        return false;
    }
    return true;
}


__attribute__((__nonnull__(1)))
int32_t parse_dimention(const char *restrict p_str,
                        int32_t              p_max)
{
    const size_t len = str_len(p_str);

    if (g_str_has_suffix(p_str, "px")) {
        char   *copy = g_strndup(p_str, len - 2);
        int32_t val  = atoi(copy);
        g_free(copy);
        return val;
    }

    if (g_str_has_suffix(p_str, "%")) {
        char   *copy    = g_strndup(p_str, len - 1);
        int32_t percent = atoi(copy);
        g_free(copy);
        return (p_max * percent) / 100;
    }

    return atoi(p_str);
}


__attribute__((__nonnull__(1, 2)))
void set_layer_shell_position(GtkWindow  *restrict p_window,
                              const char *restrict p_position)
{
    gtk_layer_set_anchor(p_window, GTK_LAYER_SHELL_EDGE_TOP, FALSE);
    gtk_layer_set_anchor(p_window, GTK_LAYER_SHELL_EDGE_BOTTOM, FALSE);
    gtk_layer_set_anchor(p_window, GTK_LAYER_SHELL_EDGE_LEFT, FALSE);
    gtk_layer_set_anchor(p_window, GTK_LAYER_SHELL_EDGE_RIGHT, FALSE);

    char pos[64];
    strncpy(pos, p_position, sizeof(pos) - 1);
    pos[sizeof(pos) - 1] = '\0';

    for (char *p = pos; *p; ++p)
        *p = g_ascii_tolower(*p);

    gboolean top = strstr(pos, "top") != NULL;
    gboolean bottom = strstr(pos, "bottom") != NULL;
    gboolean left = strstr(pos, "left") != NULL;
    gboolean right = strstr(pos, "right") != NULL;
    gboolean center_h = strstr(pos, "center") != NULL && !left && !right;
    gboolean center_v = strstr(pos, "center") != NULL && !top && !bottom;

    if (top)
        gtk_layer_set_anchor(p_window, GTK_LAYER_SHELL_EDGE_TOP, TRUE);
    if (bottom)
        gtk_layer_set_anchor(p_window, GTK_LAYER_SHELL_EDGE_BOTTOM, TRUE);
    if (left)
        gtk_layer_set_anchor(p_window, GTK_LAYER_SHELL_EDGE_LEFT, TRUE);
    if (right)
        gtk_layer_set_anchor(p_window, GTK_LAYER_SHELL_EDGE_RIGHT, TRUE);

    if (center_h) {
        gtk_layer_set_anchor(p_window, GTK_LAYER_SHELL_EDGE_LEFT, TRUE);
        gtk_layer_set_anchor(p_window, GTK_LAYER_SHELL_EDGE_RIGHT, TRUE);
        gtk_layer_set_margin(p_window, GTK_LAYER_SHELL_EDGE_LEFT, -1);
        gtk_layer_set_margin(p_window, GTK_LAYER_SHELL_EDGE_RIGHT, -1);
    }

    if (center_v) {
        gtk_layer_set_anchor(p_window, GTK_LAYER_SHELL_EDGE_TOP, TRUE);
        gtk_layer_set_anchor(p_window, GTK_LAYER_SHELL_EDGE_BOTTOM, TRUE);
        gtk_layer_set_margin(p_window, GTK_LAYER_SHELL_EDGE_TOP, -1);
        gtk_layer_set_margin(p_window, GTK_LAYER_SHELL_EDGE_BOTTOM, -1);
    }
}



__attribute__((__nonnull__(1)))
void print_lua_stack(lua_State *L) {
    int32_t top = lua_gettop(L);
    printf("Lua stack (top -> bottom):\n");
    for (int i = top; i >= 1; i--) {
        int t = lua_type(L, i);
        switch (t) {
            case LUA_TSTRING:
                printf("  [%d]: string \"%s\"\n", i, lua_tostring(L, i));
                break;
            case LUA_TBOOLEAN:
                printf("  [%d]: boolean %s\n", i, lua_toboolean(L, i) ? "true" : "false");
                break;
            case LUA_TNUMBER:
                printf("  [%d]: number %g\n", i, lua_tonumber(L, i));
                break;
            case LUA_TNIL:
                printf("  [%d]: nil\n", i);
                break;
            default:
                printf("  [%d]: %s\n", i, lua_typename(L, t));
                break;
        }
    }
}


int32_t
main(int32_t p_argc,
     char  **p_argv)
{
    char config_file[PATH_MAX] = { '\0' };

    for (int32_t i = 1; i < p_argc; i++) {
        arg_result *res = get_arg(p_argv[i], p_argv[i + 1], "-c", "--config");
        if (res != NULL) {
            if (res->option == NULL) {
                ERR("Trying to specify a NULL config file!");
                exit(EXIT_FAILURE);
            }

            str_copy(config_file, res->option);
            DELETE(&res->option);
            DELETE(&res);
            continue;
        }

        res = get_arg(p_argv[i], p_argv[i + 1], "-h", "--help");
        if (res != NULL) {
            print_help_message(stdout);
            DELETE(&res);
            continue;
        }

        res = get_arg(p_argv[i], p_argv[i + 1], "-V", "--version");
        if (res != NULL) {
            fprintf(stdout, "%s %s\n", __app_name, __app_version);
            DELETE(&res);
        }
    }

    gtk_init(&p_argc, &p_argv);

    if (!get_config_file(config_file)) return EXIT_FAILURE;

    lua_State *lua = lua_newstate(lua_alloc, NULL);
    lua_atpanic(lua, l_atpanic);
    luaL_openlibs(lua);

    if (luaL_dofile(lua, config_file)) {
        THROW("Lua failed to read file\n"
              "       err: %s", lerrno(lua));
    }

    if (!handle_css_context(lua)) return EXIT_FAILURE;

    INFO("Fetching windows from %s", config_file);
    size_t window_amount = 0;
    lua_pushnil(lua);

    while (lua_next(lua, 1)) {
        if (lua_istable(lua, 3)) window_amount++;

        lua_pop(lua, 1);
    }

    INFO("Found %zu windows, creating widgets", window_amount);

    window **windows = calloc(window_amount, sizeof(window *));

    /* Getting depth-0 (window) widgets */
    lua_pushnil(lua);

    size_t i = 0;
    while (lua_next(lua, 1)) {
        window *w = NULL;
        NEW(&w, window, 1);

        /* We need to get the type here, so we go one level deeper.
           Index 2 would be the name, and 3 would be the table.
           The depth-1 table contains the
           type, properties, and child of the window.
        */

        /* Get the key */
        w->name = str_dup(lua_tostring(lua, 2));

        /* Search for the window type. */
        lua_getfield(lua, 3, "type");
        const char *type = lua_tostring(lua, lua_gettop(lua));

        if (str_eq(type, "popup")) {
            w->type = GTK_WINDOW_POPUP;
        } else w->type = GTK_WINDOW_TOPLEVEL;
        lua_pop(lua, 1);

        /* Now, we apply the properties. */
        lua_getfield(lua, 3, "properties");

        uint8_t starting_idx = 5;

        /* With us using all of these, it would mean that
           each window would require all of the properties to be present
        */
        lua_getfield(lua, 4, "x");
        lua_getfield(lua, 4, "y");
        lua_getfield(lua, 4, "w");
        lua_getfield(lua, 4, "h");
        lua_getfield(lua, 4, "anchor");
        lua_getfield(lua, 4, "stacking");
        lua_getfield(lua, 4, "exclusive");
        lua_getfield(lua, 4, "visible");

        w->x         = str_dup(lua_tostring(lua, starting_idx++));
        w->y         = str_dup(lua_tostring(lua, starting_idx++));
        w->w         = str_dup(lua_tostring(lua, starting_idx++));
        w->h         = str_dup(lua_tostring(lua, starting_idx++));
        w->anchor    = str_dup(lua_tostring(lua, starting_idx++));
        w->stacking  = str_dup(lua_tostring(lua, starting_idx++));
        w->exclusive = lua_toboolean(lua, starting_idx++);
        w->visible   = lua_toboolean(lua, starting_idx++);

        lua_pop(lua, 9);

        INFO("window '%s': { %s, %s, %s, %s, %s, %s, %s, %s }",
             w->name,   w->x, w->y, w->w, w->h,
             w->anchor, w->stacking,
             w->exclusive ? "true" : "false",
             w->visible   ? "true" : "false");

        /* Now we need to somehow parse the child and
           the children of their children.
        */
        lua_getfield(lua, 3, "child");
        w->child = lua_get_widget_recursive(lua, 4);

        windows[i++] = w;
        lua_pop(lua, 2);
        print_lua_stack(lua);
    }

    INFO("%s", windows[0]->type == GTK_WINDOW_TOPLEVEL ? "toplevel" : "popup");
    // GtkWindow *wd = GTK_WINDOW(gtk_window_new(windows[0]->type));
    // gtk_container_add(GTK_CONTAINER(wd), root_to_gtk(windows[0]->child));
    GtkWindow *wd = window_to_gtk(windows[0]);
    gtk_widget_show_all(GTK_WIDGET(wd));


    // INFO("%s", lua_istable(lua, -1) ? "true" : "false")
    // print_table(lua, 1, 0);

    // lua_pushnil(lua);
    // while (lua_next(lua, -2)) {
    //     const char *key = lua_tostring(lua, -2);
    //     INFO("key: %s", key);

    //     lua_pop(lua, 1);
    // }

    // widget *widgets = widget_new("window", "main-window");
    // widget *box     = widget_new("box", "main-box");
    // widget *label   = widget_new("label", "text-label");

    // widget_add_property_bool(widgets, "visible", true);
    // widget_add_property_bool(box, "visible", true);
    // widget_add_property_bool(label, "visible", true);
    // widget_add_property_str(label, "label", "Hello, World!");

    // widget_add_child(box, label);
    // widget_add_child(widgets, box);

    // GtkWindow *window = root_to_gtk(widgets);

    gtk_main();



    return 0;
}
