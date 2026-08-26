#pragma once

#include <stdbool.h>
#include "esp_err.h"
#include "esp_bt_defs.h"
#include "esp_hf_client_api.h"

#ifdef __cplusplus
extern "C" {
#endif

esp_err_t hfp_init(void);
bool hfp_is_connected(void);
bool hfp_is_audio_connected(void);

esp_err_t hfp_connect(const esp_bd_addr_t remote_addr);
esp_err_t hfp_disconnect(void);

esp_err_t hfp_answer_call(void);
esp_err_t hfp_reject_call(void);
esp_err_t hfp_hangup_call(void);

esp_err_t hfp_connect_audio(void);
esp_err_t hfp_disconnect_audio(void);

#ifdef __cplusplus
}
#endif
