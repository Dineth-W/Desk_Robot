#include "display.h"
#include "config.h"

#include "driver/i2c.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <stdint.h>

static const char *TAG = "DISPLAY";

ssd1306_handle_t oled = NULL;


/* =========================================================
 * I2C INITIALIZATION
 * ========================================================= */

static esp_err_t i2c_init(void)
{
    i2c_config_t config = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = OLED_SDA,
        .scl_io_num = OLED_SCL,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_FREQUENCY,
        .clk_flags = 0
    };

    esp_err_t err = i2c_param_config(
        I2C_PORT,
        &config
    );

    if (err != ESP_OK) {
        return err;
    }

    err = i2c_driver_install(
        I2C_PORT,
        I2C_MODE_MASTER,
        0,
        0,
        0
    );

    /*
     * The driver may already have been installed.
     */
    if (err == ESP_ERR_INVALID_STATE) {
        return ESP_OK;
    }

    return err;
}


/* =========================================================
 * I2C SCAN
 * ========================================================= */

static void i2c_scan(void)
{
    ESP_LOGI(TAG, "Scanning I2C bus...");

    int found = 0;

    for (uint8_t address = 1; address < 127; address++) {

        i2c_cmd_handle_t cmd = i2c_cmd_link_create();

        if (cmd == NULL) {
            continue;
        }

        i2c_master_start(cmd);

        i2c_master_write_byte(
            cmd,
            (address << 1) | I2C_MASTER_WRITE,
            true
        );

        i2c_master_stop(cmd);

        esp_err_t result = i2c_master_cmd_begin(
            I2C_PORT,
            cmd,
            pdMS_TO_TICKS(100)
        );

        i2c_cmd_link_delete(cmd);

        if (result == ESP_OK) {

            ESP_LOGI(
                TAG,
                "I2C device found at 0x%02X",
                address
            );

            found++;
        }
    }

    ESP_LOGI(
        TAG,
        "I2C scan complete. Found %d device(s).",
        found
    );
}


/* =========================================================
 * DISPLAY INITIALIZATION
 * ========================================================= */

esp_err_t display_init(void)
{
    esp_err_t err;

    ESP_LOGI(TAG, "Initializing I2C...");

    err = i2c_init();

    if (err != ESP_OK) {

        ESP_LOGE(
            TAG,
            "I2C initialization failed: %s",
            esp_err_to_name(err)
        );

        return err;
    }

    ESP_LOGI(TAG, "I2C initialized successfully.");

    i2c_scan();

    ESP_LOGI(
        TAG,
        "Creating SSD1306 at address 0x%02X...",
        OLED_ADDRESS
    );

    oled = ssd1306_create(
        I2C_PORT,
        OLED_ADDRESS
    );

    if (oled == NULL) {

        ESP_LOGE(
            TAG,
            "SSD1306 creation failed."
        );

        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "SSD1306 object created.");

    err = ssd1306_init(oled);

    if (err != ESP_OK) {

        ESP_LOGE(
            TAG,
            "SSD1306 initialization failed: %s",
            esp_err_to_name(err)
        );

        oled = NULL;

        return err;
    }

    display_clear();

    ESP_LOGI(TAG, "OLED initialized successfully.");

    return ESP_OK;
}


/* =========================================================
 * CLEAR
 * ========================================================= */

void display_clear(void)
{
    if (oled == NULL) {
        return;
    }

    ssd1306_clear_screen(
        oled,
        0x00
    );

    ssd1306_refresh_gram(oled);
}


/* =========================================================
 * REFRESH
 * ========================================================= */

void display_refresh(void)
{
    if (oled == NULL) {
        return;
    }

    ssd1306_refresh_gram(oled);
}


/* =========================================================
 * TEXT
 * ========================================================= */

void display_text(
    int x,
    int y,
    const char *text,
    int size
)
{
    if (oled == NULL || text == NULL) {
        return;
    }

    ssd1306_draw_string(
        oled,
        x,
        y,
        (const uint8_t *)text,
        16,
        size
    );
}


/* =========================================================
 * CENTERED TEXT
 * ========================================================= */

void display_center_text(
    const char *text,
    int y,
    int size
)
{
    if (oled == NULL || text == NULL) {
        return;
    }

    int char_width = 8 * size;
    int length = 0;

    while (text[length] != '\0') {
        length++;
    }

    int width = length * char_width;

    int x = (OLED_WIDTH - width) / 2;

    if (x < 0) {
        x = 0;
    }

    display_text(
        x,
        y,
        text,
        size
    );
}


/* =========================================================
 * SPLASH SCREEN
 * ========================================================= */

void display_show_splash(void)
{
    display_clear();

    display_center_text(
        "DESK",
        10,
        2
    );

    display_center_text(
        "ROBOT",
        32,
        2
    );

    display_refresh();
}


/* =========================================================
 * TEST SCREEN
 * ========================================================= */

void display_show_test(void)
{
    display_clear();

    display_center_text(
        "DESK ROBOT",
        0,
        1
    );

    display_center_text(
        "OLED WORKING",
        20,
        1
    );

    display_center_text(
        "I2C OK",
        40,
        1
    );

    display_refresh();
}


/* =========================================================
 * CLOCK SCREEN
 * ========================================================= */

void display_show_clock(
    const char *time_string,
    const char *date_string
)
{
    if (oled == NULL) {
        return;
    }

    display_clear();

    if (time_string != NULL) {

        display_center_text(
            time_string,
            8,
            2
        );
    }

    if (date_string != NULL) {

        display_center_text(
            date_string,
            40,
            1
        );
    }

    display_refresh();
}