#pragma once
#include "esp_err.h"
#include <stddef.h>
#include <stdint.h>

esp_err_t audio_init(void);
esp_err_t audio_read(int32_t *samples, size_t count, size_t *read);
esp_err_t audio_write(const int32_t *samples, size_t count);
float audio_rms(const int32_t *samples, size_t count);
