#pragma once

void call_display_init(void);

void call_display_idle(void);

void call_display_incoming(
    const char *number
);

void call_display_active(
    const char *number
);

void call_display_ended(void);