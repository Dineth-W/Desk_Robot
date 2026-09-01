#pragma once
bool call_display_is_active(void);
void call_display_update(void);

#include <stdbool.h>

void call_display_init(void);

void call_display_idle(void);

void call_display_incoming(const char *number);

void call_display_active(const char *number);

void call_display_ended(void);

/*
 * Call animation update.
 *
 * Call this periodically while an incoming/active call
 * is being displayed.
 */
void call_display_update(void);

/*
 * Returns true while the call UI owns the display.
 */
bool call_display_is_active(void);