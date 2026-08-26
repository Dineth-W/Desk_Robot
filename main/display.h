#pragma once

#include "esp_err.h"
#include <stdbool.h>
#include <stdint.h>

esp_err_t display_init(void);
bool display_is_ready(void);
void display_clear(void);
void display_update(void);

void display_draw_pixel(int x, int y, bool on);
void display_draw_rect(int x, int y, int w, int h, bool on);
void display_fill_rect(int x, int y, int w, int h, bool on);
void display_draw_hline(int x, int y, int w, bool on);
void display_draw_vline(int x, int y, int h, bool on);

void display_text(int x, int y, const char *text, int scale, bool on);
void display_center_text(const char *text, int y, int scale, bool on);
