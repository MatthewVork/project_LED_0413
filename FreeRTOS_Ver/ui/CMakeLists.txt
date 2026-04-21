SET(SOURCES screens/ui_Screen_Init.c
    screens/ui_Screen_menu.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c
    ui_events.c
    fonts/ui_font_Font1.c)

add_library(ui ${SOURCES})
