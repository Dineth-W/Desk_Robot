idf_component_register(
    SRCS
        "main.c"
        "display.c"
        "clock.c"
        "face.c"
        "alarm.c"
        "rtc.c"
        "screen.c"

    INCLUDE_DIRS
        "."

    REQUIRES
        espressif__ssd1306
        esp_driver_gpio
)