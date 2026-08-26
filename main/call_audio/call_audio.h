#pragma once

#include "esp_err.h"
#include <stddef.h>
#include <stdint.h>

esp_err_t call_audio_init(void);

esp_err_t call_audio_start(void);
esp_err_t call_audio_stop(void);

esp_err_t call_audio_read(
    int32_t *buffer,
    size_t samples,
    size_t *bytes_read
);

esp_err_t call_audio_write(
    const int32_t *buffer,
    size_t samples,
    size_t *bytes_written
);