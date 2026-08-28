idf_component_register(
    SRCS
        "app_hf_msg_set.c"
        "bt_app_core.c"
        "bt_app_hf.c"
        "gpio_pcm_config.c"
        "bt_app_pbac.c"
        "hfp_audio.c"
        "call_buttons.c"

        "display.c"
        "i2c_bus.c"
        "rtc.c"
        "face.c"
        "call_display.c"

        "main.c"

    PRIV_REQUIRES
        bt
        nvs_flash
        esp_driver_gpio
        esp_driver_i2s
        esp_driver_i2c
        console
        esp_ringbuf

    INCLUDE_DIRS
        "."
)