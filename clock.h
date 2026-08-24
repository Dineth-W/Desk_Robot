#ifndef CLOCK_H
#define CLOCK_H

#include <time.h>

void clock_init(void);

void update_clock(void);

void draw_clock(void);

struct tm get_current_time(void);

#endif