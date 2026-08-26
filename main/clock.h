#pragma once
#include <stdbool.h>

void clock_init(void);
bool clock_get(struct tm *t);
void clock_draw(void);
void clock_update(void);
