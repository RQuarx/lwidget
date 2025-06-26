#include "strlib.h"
#include "types.h"


GtkStackTransitionType
transition_from_string(const char *restrict p_type)
{
    if (str_eq(p_type, "none"))
        return GTK_STACK_TRANSITION_TYPE_NONE;
    else if (str_eq(p_type, "crossfade"))
        return GTK_STACK_TRANSITION_TYPE_CROSSFADE;
    else if (str_eq(p_type, "slide-left-right"))
        return GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT_RIGHT;
    else if (str_eq(p_type, "slide-up-down"))
        return GTK_STACK_TRANSITION_TYPE_SLIDE_UP_DOWN;
    else if (str_eq(p_type, "slide-left"))
        return GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT;
    else if (str_eq(p_type, "slide-right"))
        return GTK_STACK_TRANSITION_TYPE_SLIDE_RIGHT;
    else if (str_eq(p_type, "slide-up"))
        return GTK_STACK_TRANSITION_TYPE_SLIDE_UP;
    else if (str_eq(p_type, "slide-down"))
        return GTK_STACK_TRANSITION_TYPE_SLIDE_DOWN;
    else if (str_eq(p_type, "over-up"))
        return GTK_STACK_TRANSITION_TYPE_OVER_UP;
    else if (str_eq(p_type, "over-down"))
        return GTK_STACK_TRANSITION_TYPE_OVER_DOWN;
    else if (str_eq(p_type, "over-left"))
        return GTK_STACK_TRANSITION_TYPE_OVER_LEFT;
    else if (str_eq(p_type, "over-right"))
        return GTK_STACK_TRANSITION_TYPE_OVER_RIGHT;
    else if (str_eq(p_type, "under-up"))
        return GTK_STACK_TRANSITION_TYPE_UNDER_UP;
    else if (str_eq(p_type, "under-down"))
        return GTK_STACK_TRANSITION_TYPE_UNDER_DOWN;
    else if (str_eq(p_type, "under-left"))
        return GTK_STACK_TRANSITION_TYPE_UNDER_LEFT;
    else if (str_eq(p_type, "under-right"))
        return GTK_STACK_TRANSITION_TYPE_UNDER_RIGHT;
    return GTK_STACK_TRANSITION_TYPE_NONE;
}


bool
orientation_from_string(const char     *restrict p_orientation,
                        GtkOrientation *restrict p_out)
{
    if (str_eq(p_orientation, "horizontal") || p_orientation[0] == 'h') {
        *p_out = GTK_ORIENTATION_HORIZONTAL;
        return true;
    }

    if (str_eq(p_orientation, "vertical") || p_orientation[0] == 'b') {
        *p_out = GTK_ORIENTATION_VERTICAL;
        return true;
    }

    return false;
}


bool
alignment_from_string(const char *restrict p_align,
                      GtkAlign   *restrict p_out)
{
    if (str_eq(p_align, "baseline")) {
        *p_out = GTK_ALIGN_BASELINE;
        return true;
    }
    if (str_eq(p_align, "center")) {
        *p_out = GTK_ALIGN_CENTER;
        return TRUE;
    }
    if (str_eq(p_align, "start")) {
        *p_out = GTK_ALIGN_START;
        return TRUE;
    }
    if (str_eq(p_align, "fill")) {
        *p_out = GTK_ALIGN_FILL;
        return TRUE;
    }
    if (str_eq(p_align, "end")) {
        *p_out = GTK_ALIGN_END;
        return TRUE;
    }

    return false;
}


property_type
property_type_from_name(const char *restrict p_name)
{
    const struct p_type_str *entry = PROPERTY_TYPES;
    while (entry->name != NULL) {
        if (str_eq(p_name, entry->name)) return entry->type;
        entry++;
    }
    return PT_NONE;
}


widget_type
widget_type_from_class(const char *restrict p_class)
{
    const struct w_type_str *entry = WIDGET_TYPES;
    while (entry->class != NULL) {
        if (str_eq(p_class, entry->class)) return entry->type;
        entry++;
    }
    return WT_NONE;
}
