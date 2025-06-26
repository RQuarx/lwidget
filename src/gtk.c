#include <gtk/gtk.h>

#include "strlib.h"
#include "types.h"
#include "utils.h"
#include "gtk.h"


GtkWidget *
widget_from_class(widget_type p_type)
{
    switch (p_type) {
    case WT_COMBO_BOX_TEXT:
        return gtk_combo_box_text_new();
    case WT_COLOUR_CHOOSER:
        return gtk_color_chooser_widget_new();
    case WT_SCROLL_WINDOW:
        return gtk_scrolled_window_new(NULL, NULL);
    case WT_COLOUR_BUTTON:
        return gtk_color_button_new();
    case WT_CHECK_BUTTON:
        return gtk_check_button_new();
    case WT_PROGRESS:
        return gtk_progress_bar_new();
    case WT_TRANSFORM:
        ERR("Attempted to use an unfinished widget!");
        return NULL;
    case WT_SYSTRAY:
        ERR("Attempted to use an unfinished widget!");
        return NULL;
    case WT_EXPANDER:
        return gtk_expander_new(NULL);
    case WT_REVEALER:
        return gtk_revealer_new();
    case WT_OVERLAY:
        return gtk_overlay_new();
    case WT_BUTTON:
        return gtk_button_new();
    case WT_LABEL:
        return gtk_label_new(NULL);
    case WT_ENTRY:
        return gtk_entry_new();
    case WT_STACK:
        return gtk_stack_new();
    case WT_SCALE:
        return gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0, 100, 1);
    case WT_IMAGE:
        return gtk_image_new();
    case WT_BOX:
        return gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    case WT_NONE:
    default:
        ERR("Invalid widget");
        return NULL;
    }
}


void
populate_combo_from_string(GtkComboBoxText *restrict p_combo,
                           const char      *restrict p_str)
{
    const char *p        = p_str;
    GString    *token    = g_string_new(NULL);
    bool        in_quotes = false;

    while (*p) {
        if (*p == '"') {
            in_quotes = !in_quotes;
            g_string_append_c(token, *p);
        } else if (*p == ',' && !in_quotes) {
            g_string_append_c(token, '\0');
            char *trimmed = str_trim(token->str);
            if (*trimmed != '\0') {
                gtk_combo_box_text_append_text(p_combo, trimmed);
            }
            g_string_set_size(token, 0);
        } else {
            g_string_append_c(token, *p);
        }
        p++;
    }

    g_string_append_c(token, '\0');
    char *trimmed = str_trim(token->str);
    if (*trimmed != '\0') {
        gtk_combo_box_text_append_text(p_combo, trimmed);
    }

    g_string_free(token, TRUE);
}


bool
apply_properties_to_widget(GtkWidget        *restrict p_widget,
                           const w_property *restrict p_properties)
{
    const w_property *curr = p_properties;
    while (curr != NULL) {
        switch (curr->type) {
        case PT_ORIENTATION:
            if (GTK_IS_ORIENTABLE(p_widget)) {
                gtk_orientable_set_orientation(GTK_ORIENTABLE(p_widget),
                                               curr->val_orientation);
                break;
            }
        case PT_TRANSITION:
            if (GTK_IS_STACK(p_widget)) {
                gtk_stack_set_transition_type(
                    GTK_STACK(p_widget),
                    transition_from_string(curr->val_str));
                break;
            }
        case PT_ACTIVATED:
            if (GTK_IS_TOGGLE_BUTTON(p_widget)) {
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(p_widget),
                                             curr->val_bool);
                break;
            }
        case PT_DURATION:
            if (GTK_IS_STACK(p_widget)) {
                gtk_stack_set_transition_duration(GTK_STACK(p_widget),
                                                  curr->val_int);
                break;
            }
        case PT_ONCHANGE:
            /* Require function stuff */
            break;
        case PT_HEXPAND:
            gtk_widget_set_hexpand(p_widget, curr->val_bool);
            break;
        case PT_SPACING:
            if (GTK_IS_BOX(p_widget)) {
                gtk_box_set_spacing(GTK_BOX(p_widget), curr->val_int);
                break;
            }
        case PT_TIMEOUT:
            /* Function stuff */
            break;
        case PT_TOOLTIP:
            gtk_widget_set_tooltip_markup(p_widget, curr->val_str);
            break;
        case PT_VEXPAND:
            gtk_widget_set_vexpand(p_widget, curr->val_bool);
            break;
        case PT_VISIBLE:
            gtk_widget_set_visible(p_widget, curr->val_bool);
            break;
        case PT_HALIGN:
            gtk_widget_set_halign(p_widget, curr->val_int);
            break;
        case PT_HEIGHT: {
            int32_t w = 0;
            gtk_widget_get_size_request(p_widget, &w, NULL);
            gtk_widget_set_size_request(p_widget, w, curr->val_int);
            break;
        }
        case PT_REVEAL:
            if (GTK_IS_REVEALER(p_widget)) {
                gtk_revealer_set_reveal_child(GTK_REVEALER(p_widget),
                                              curr->val_bool);
                break;
            }
        case PT_VALIGN:
            gtk_widget_set_valign(p_widget, curr->val_int);
            break;
        case PT_ITEMS:
            if (GTK_IS_COMBO_BOX_TEXT(p_widget)) {
                populate_combo_from_string(GTK_COMBO_BOX_TEXT(p_widget),
                                           curr->val_str);
                break;
            }
        case PT_LABEL:
            if (GTK_IS_LABEL(p_widget)) {
                gtk_label_set_markup(GTK_LABEL(p_widget), curr->val_str);
                break;
            }
        case PT_WIDTH: {
            int32_t h = 0;
            gtk_widget_get_size_request(p_widget, NULL, &h);
            gtk_widget_set_size_request(p_widget, curr->val_int, h);
            break;
        }
        case PT_NONE:
            break;
        }

        curr = curr->next;
    }
    return true;
}


GtkWidget *
widget_to_gtk(const widget *restrict p_widget)
{
    GtkWidget *widget = widget_from_class(p_widget->type);

    if (p_widget->name != NULL)
        gtk_widget_set_name(widget, p_widget->name);

    apply_properties_to_widget(widget, p_widget->properties);

    return widget;
}


GtkWidget *
root_to_gtk(const widget *restrict p_root)
{
    GtkWidget *root = widget_to_gtk(p_root);

    for (GList *l = p_root->children; l != NULL; l = l->next) {
        gtk_container_add(GTK_CONTAINER(root),
                          root_to_gtk(l->data));
    }

    return root;
}


GtkWindow *
window_to_gtk(const window *restrict p_window)
{
    GtkWindow *window = GTK_WINDOW(gtk_window_new(p_window->type));

    /* gtk_window_set_default_size(window, p_window->w, p_window->h); */

    gtk_widget_set_visible(GTK_WIDGET(window), p_window->visible);

    gtk_container_add(GTK_CONTAINER(window), root_to_gtk(p_window->child));

    return window;
}
