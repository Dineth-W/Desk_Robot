#ifndef ALARM_H
#define ALARM_H

#include <stdbool.h>


#define MAX_ALARMS 10


typedef struct
{
    int hour;
    int minute;

    bool enabled;
    bool triggered;
    bool repeat;

} Alarm;


extern Alarm alarms[MAX_ALARMS];

extern int alarmCount;


void alarm_init(void);

void update_alarm(void);

void draw_alarm_screen(
    const Alarm *alarm
);


bool add_alarm(
    int hour,
    int minute,
    bool repeat
);


void remove_alarm(int index);

Alarm *get_alarm(int index);


/* =========================================================
 * BUZZER
 * ========================================================= */

void buzzer_init(void);

void buzzer_on(void);

void buzzer_off(void);

void buzzer_beep(
    unsigned long duration_ms
);


#endif