#include "buzzer.h"
#include "config.h"
#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void buzzer_init(void)
{
    ledc_timer_config_t t={
        .speed_mode=LEDC_LOW_SPEED_MODE,.timer_num=LEDC_TIMER_0,
        .duty_resolution=LEDC_TIMER_8_BIT,.freq_hz=BUZZER_FREQ_HZ,
        .clk_cfg=LEDC_AUTO_CLK};
    ledc_timer_config(&t);

    ledc_channel_config_t c={
        .gpio_num=BUZZER_GPIO,.speed_mode=LEDC_LOW_SPEED_MODE,
        .channel=LEDC_CHANNEL_0,.intr_type=LEDC_INTR_DISABLE,
        .timer_sel=LEDC_TIMER_0,.duty=0,.hpoint=0};
    ledc_channel_config(&c);
}

void buzzer_on(void){ ledc_set_duty(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0,BUZZER_DUTY); ledc_update_duty(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0); }
void buzzer_off(void){ ledc_set_duty(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0,0); ledc_update_duty(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0); }
void buzzer_beep(int ms){ buzzer_on(); vTaskDelay(pdMS_TO_TICKS(ms)); buzzer_off(); }
