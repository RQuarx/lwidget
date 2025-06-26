--[[
To change the CSS file path, you must specify it BEFORE the main UI table.
]]
-- css_path = "style.css"
css_text = "#this { color: red; }"

--[[
Main UI configuration table.

This file defines a hierarchical structure for describing GUI components,
such as windows and nested widgets, using Lua tables. The outermost
structure is a keyed table where each key is a unique window identifier.
Each window can contain properties and child widgets.

Returned structure:
{
    [window_id] = {
        type = "toplevel" | "popup",
        properties = { ... },
        children = { ... }
    }
}
]]
tree = {
    --[[ "main-window":
         Unique identifier for this top-level window.
         Used in stylesheets (e.g., via `#main-window` in CSS).
    ]]
    ["main-window"] = {
        --[[ Type of the window.
             Determines how it is managed and rendered.

             Allowed values:
             - "toplevel": A standard window managed by the compositor.
             - "popup": A temporary or overlay window (e.g., tooltips, context menus).
        ]]
        ---@type string
        type = "toplevel",

        --[[ Window-specific properties defining layout and behavior ]]
        properties = {
            --[[ X coordinate (horizontal position).
                 Can be:
                 - A number (e.g., 100)
                 - A percentage string (e.g., "50%")
                 - "px" suffix is optional

                 Relative to the screen or parent container.
            ]]
            ---@type string|integer
            x = "0%",

            --[[ Y coordinate (vertical position).
                 Same rules as `x`.
            ]]
            ---@type string|integer
            y = 0,

            --[[ Width of the window.
                 Can be an integer (pixels) or a string percentage.
            ]]
            ---@type string|integer
            w = "100%",

            --[[ Height of the window.
                 Can be an integer (pixels) or a percentage string.
            ]]
            ---@type string|integer
            h = 21,

            --[[ Anchor point for positioning.
                 Defines how the window aligns relative to (x, y).

                 Examples:
                 - "center"
                 - "top left"
                 - "bottom center"
            ]]
            ---@type string
            anchor = "bottom center",

            --[[ Stacking layer (z-order).
                 Defines rendering order relative to other windows.

                 Options:
                 - "bottom": Behind all other elements
                 - "bg": Background layer
                 - "fg": Foreground layer (default content)
                 - "overlay": Above all others (tooltips, modals)
            ]]
            ---@type string
            stacking = "fg",

            --[[ Whether the window should reserve space in the layout.
                 This is useful for docks, panels, or bars.

                 true: Window affects tiling layout.
                 false: Treated as a floating/overlay element.

                 If true, then anchor must include "center"
            ]]
            ---@type boolean
            exclusive = true,

            --[[ Initial visibility of the window.
                 true: window is shown on startup
                 false: window is hidden
            ]]
            ---@type boolean
            visible = true
        },

        --[[ Children:
             A table of named widgets that are direct children of this window.
             Each child is indexed by a unique key used for styling and reference.
        ]]
        child = {
            --[[ "main-container":
                 Main UI container, typically a box layout widget.
            ]]
            {
                name = "main-container",
                --[[ Class of the widget.
                     Defines its type and layout behavior.

                     Common classes:
                     - "box": Horizontal or vertical container
                     - "label": Text display
                     - "button": Clickable button
                     - "entry": Text input
                     - etc.
                ]]
                ---@type string
                class = "box",

                --[[ Optional name/ID used for styling and debugging.
                     Also serves as the key in the children table.
                ]]

                --[[ Widget properties define its appearance and behavior.
                     Most properties are class-dependent.
                ]]
                properties = {
                    --[[ Horizontal alignment inside the parent container.

                         Options:
                         - "fill": Stretch to fill width
                         - "start": Align to left
                         - "center": Center horizontally
                         - "end": Align to right
                         - "baseline": Align with text baseline
                    ]]
                    ---@type string
                    halign = "start",

                    --[[ Tooltip displayed on mouse hover.
                         Can include Pango markup for formatting (e.g., <i>italic</i>).
                    ]]
                    ---@type string
                    tooltip = "<i>Hello, World!</i>",

                    visible = true
                },

                --[[ Nested children inside "main-container".
                     This allows for deeply nested UI trees.
                ]]
                children = {
                    --[[ "hello-label":
                         A text label displaying a message.
                    ]]
                    {
                        name = "hello-label",
                        class = "label",
                        properties = {
                            --[[ Text shown on the label.
                                 Supports Pango markup.
                            ]]
                            label = "<i>Hello, World!</i>",
                            visible = true

                        }
                    },

                    --[[ "other-box":
                         Another container, potentially for future content.
                         Currently minimal.
                    ]]
                    {
                        name = "other-box",
                        class = "box"
                        -- properties and children can go here
                    }
                }
            }
        }
    }
    --[[ "popup-window":
         A secondary window that acts as a popup or overlay.

         This window is logically associated with the "main-window",
         meaning it may position itself relative to or be owned by it.
    ["popup-window"] = {
        type = "popup",

        properties = {
          x = 10,
          y = 50,
          w = 100,
             ...
        }
     } ]]
}

return tree
