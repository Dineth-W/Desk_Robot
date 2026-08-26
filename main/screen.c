#include "screen.h"

ScreenType currentScreen =
    SCREEN_SPLASH;

ScreenType previousScreen =
    SCREEN_SPLASH;


/* =========================================================
 * INITIALIZATION
 * ========================================================= */

void screen_init(void)
{
    currentScreen = SCREEN_SPLASH;

    previousScreen = SCREEN_SPLASH;
}


/* =========================================================
 * CHANGE SCREEN
 * ========================================================= */

void change_screen(
    ScreenType new_screen
)
{
    if (new_screen == currentScreen) {
        return;
    }

    previousScreen = currentScreen;

    currentScreen = new_screen;
}


/* =========================================================
 * RETURN
 * ========================================================= */

void return_to_previous_screen(void)
{
    ScreenType temp =
        currentScreen;

    currentScreen =
        previousScreen;

    previousScreen =
        temp;
}


/* =========================================================
 * GET CURRENT SCREEN
 * ========================================================= */

ScreenType get_current_screen(void)
{
    return currentScreen;
}


/* =========================================================
 * CHECK SCREEN
 * ========================================================= */

int is_screen(
    ScreenType screen
)
{
    return currentScreen == screen;
}