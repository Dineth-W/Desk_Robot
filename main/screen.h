#ifndef SCREEN_H
#define SCREEN_H

typedef enum
{
    SCREEN_SPLASH,
    SCREEN_CLOCK,
    SCREEN_FACE,
    SCREEN_TODO,
    SCREEN_REMINDER,
    SCREEN_ALARM,
    SCREEN_INCOMING_CALL,
    SCREEN_ACTIVE_CALL

} ScreenType;


extern ScreenType currentScreen;

extern ScreenType previousScreen;


void screen_init(void);

void change_screen(
    ScreenType new_screen
);

void return_to_previous_screen(void);

ScreenType get_current_screen(void);

int is_screen(
    ScreenType screen
);


#endif