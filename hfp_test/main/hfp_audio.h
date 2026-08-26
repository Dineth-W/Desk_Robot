#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "esp_err.h"

/*
 * Robot HFP audio bridge.
 *
 * HFP narrow-band CVSD over the HCI data path is presented to the
 * application as PCM when WBS/mSBC is disabled. The HFP callbacks use
 * 16-bit, 8 kHz, mono PCM.
 *
 * The I2S hardware is run at 8 kHz with 32-bit stereo slots so the
 * INMP441 receives the 64 x Fs clocking it requires. HFP PCM is
 * converted between 16-bit mono and 32-bit stereo at the boundary.
 */

esp_err_t hfp_audio_init(void);
void hfp_audio_start(void);
void hfp_audio_stop(void);

/* Called from the HFP incoming callback. Non-blocking. */
bool hfp_audio_put_rx_pcm(const uint8_t *data, size_t len);

/* Called from the HFP outgoing callback. Non-blocking. */
bool hfp_audio_get_tx_pcm(uint8_t *data, size_t len);
