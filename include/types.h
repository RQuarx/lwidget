#pragma once
#ifndef __types__h
#define __types__h
#include <stddef.h>
#include <stdint.h>
#include <gtk/gtk.h>

#undef TRUE
#undef FALSE
#define TRUE true
#define FALSE false


typedef enum _widget_type : uint8_t
{
    WT_COLOUR_CHOOSER,
    WT_COMBO_BOX_TEXT,
    WT_COLOUR_BUTTON,
    WT_SCROLL_WINDOW,
    WT_CHECK_BUTTON,
    WT_TRANSFORM,
    WT_EXPANDER,
    WT_PROGRESS,
    WT_REVEALER,
    WT_OVERLAY,
    WT_SYSTRAY,
    WT_BUTTON,
    WT_WINDOW,
    WT_ENTRY,
    WT_IMAGE,
    WT_LABEL,
    WT_STACK,
    WT_SCALE,
    WT_BOX,
    WT_NONE,
} widget_type;

typedef enum _property_type : uint8_t
{
    PT_ORIENTATION,
    PT_TRANSITION,
    PT_ACTIVATED,
    PT_DURATION,
    PT_ONCHANGE,
    PT_HEXPAND,
    PT_SPACING,
    PT_TIMEOUT,
    PT_TOOLTIP,
    PT_VEXPAND,
    PT_VISIBLE,
    PT_HALIGN,
    PT_HEIGHT,
    PT_REVEAL,
    PT_VALIGN,
    PT_ITEMS,
    PT_LABEL,
    PT_WIDTH,
    PT_NONE
} property_type;

typedef enum _property_valtype : uint8_t
{
    PV_ORIENTATION = 1 << 0,
    PV_STRING      = 1 << 1,
    PV_ALIGN       = 1 << 2,
    PV_BOOL        = 1 << 3,
    PV_PATH        = 1 << 4,
    PV_INT         = 1 << 5,
} property_valtype;

typedef struct _w_property
{
    struct _w_property *next;

    property_valtype val_type;
    property_type    type;

    union
    {
        char          *val_str;
        int32_t        val_int;
        bool           val_bool;
        GtkAlign       val_align;
        GtkOrientation val_orientation;
    };
} w_property;

typedef struct _window
{
    GtkWindowType type;
    char
        *stacking,
        *anchor,
        *name,
        *x,
        *y,
        *w,
        *h;
    bool
        exclusive,
        visible;

    struct _widget *child;
} window;

typedef struct _widget
{
    widget_type type;
    char       *name;

    w_property *properties;

    GList *children;
} widget;

struct w_type_str
{
    widget_type type;
    char       *class;
};

struct p_type_str
{
    property_type type;
    char         *name;
};

static const struct w_type_str WIDGET_TYPES[] = {
    { WT_COLOUR_CHOOSER, "colour-chooser" },
    { WT_COMBO_BOX_TEXT, "combo-box-text" },
    { WT_COLOUR_BUTTON,  "colour-button"  },
    { WT_SCROLL_WINDOW,  "scroll-window"  },
    { WT_CHECK_BUTTON,   "check-button"   },
    { WT_TRANSFORM,      "transform"      },
    { WT_EXPANDER,       "expander"       },
    { WT_PROGRESS,       "progress"       },
    { WT_REVEALER,       "revealer"       },
    { WT_OVERLAY,        "overlay"        },
    { WT_SYSTRAY,        "systray"        },
    { WT_BUTTON,         "button"         },
    { WT_ENTRY,          "entry"          },
    { WT_IMAGE,          "image"          },
    { WT_LABEL,          "label"          },
    { WT_STACK,          "stack"          },
    { WT_SCALE,          "scale"          },
    { WT_BOX,            "box"            },
    { WT_NONE,            NULL            }, /* Sentinel. */
};

static const struct p_type_str PROPERTY_TYPES[] = {
    { PT_ORIENTATION, "orientation" },
    { PT_TRANSITION,  "transition"  },
    { PT_ACTIVATED,   "activated"   },
    { PT_DURATION,    "duration"    },
    { PT_ONCHANGE,    "on-change"   },
    { PT_HEXPAND,     "hexpand"     },
    { PT_SPACING,     "spacing"     },
    { PT_TIMEOUT,     "timeout"     },
    { PT_TOOLTIP,     "tooltip"     },
    { PT_VEXPAND,     "vexpand"     },
    { PT_VISIBLE,     "visible"     },
    { PT_HALIGN,      "halign"      },
    { PT_HEIGHT,      "height"      },
    { PT_REVEAL,      "reveal"      },
    { PT_VALIGN,      "valign"      },
    { PT_ITEMS,       "items"       },
    { PT_LABEL,       "label"       },
    { PT_WIDTH,       "width"       },
    { PT_NONE,         NULL         }, /* Sentinel */
};


__attribute__((__nonnull__(1)))
GtkStackTransitionType transition_from_string(const char *restrict p_type);
__attribute__((__nonnull__(1, 2)))
bool orientation_from_string(const char     *restrict p_orientation,
                             GtkOrientation *restrict p_out);
__attribute__((__nonnull__(1, 2)))
bool alignment_from_string(const char *restrict p_align,
                           GtkAlign            *p_out);
__attribute__((__nonnull__(1)))
property_type property_type_from_name(const char *restrict p_name );
__attribute__((__nonnull__(1)))
widget_type   widget_type_from_class (const char *restrict p_class);

#endif /* __types__h */
