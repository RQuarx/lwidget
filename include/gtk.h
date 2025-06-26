#pragma once
#ifndef __gtk__h
#define __gtk__h
#include <gtk/gtk.h>
#include "types.h"


GtkWidget *widget_from_class(widget_type p_type);

__attribute__((__nonnull__(1, 2)))
void populate_combo_from_string(GtkComboBoxText *restrict p_combo,
                                const char      *restrict p_str);


__attribute__((__nonnull__(1)))
GtkWidget *widget_to_gtk(const widget *restrict p_widget);

__attribute__((__nonnull__(1, 2)))
bool apply_properties_to_widget(GtkWidget        *restrict p_widget,
                                const w_property *restrict p_properties);


__attribute__((__nonnull__(1)))
GtkWidget *root_to_gtk(const widget *restrict p_root);



__attribute__((__nonnull__(1)))
GtkWindow *window_to_gtk(const window *restrict p_window);


#endif /* __gtk__h */
