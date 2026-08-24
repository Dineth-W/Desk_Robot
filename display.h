#ifndef DISPLAY_H
#define DISPLAY_H

#include "esp_err.h"
#include "ssd1306.h"

extern ssd1306_handle_t oled;


/* =========================================================
 * INITIALIZATION
 * ========================================================= */

esp_err_t display_init(void);


/* =========================================================
 * BASIC DISPLAY
 * ========================================================= */

void display_clear(void);
void display_refresh(void);


/* =========================================================
 * TEXT
 * ========================================================= */

void display_text(
    int x,
    int y,
    const char *text,
    int size
);

void display_center_text(
    const char *text,
    int y,
    int size
);


/* =========================================================
 * SCREENS
 * ========================================================= */

void display_show_test(void);
void display_show_splash(void);

void display_show_clock(
    const char *time_string,
    const char *date_string
);


#endif