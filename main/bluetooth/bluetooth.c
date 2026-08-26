#include "bluetooth.h"
#include "hfp.h"

esp_err_t bluetooth_init(void) { return hfp_init(); }
bool bluetooth_is_connected(void) { return hfp_is_connected(); }
esp_err_t bluetooth_answer(void) { return hfp_answer_call(); }
esp_err_t bluetooth_reject(void) { return hfp_reject_call(); }
esp_err_t bluetooth_hangup(void) { return hfp_hangup_call(); }
esp_err_t bluetooth_connect_audio(void) { return hfp_connect_audio(); }
esp_err_t bluetooth_disconnect_audio(void) { return hfp_disconnect_audio(); }
