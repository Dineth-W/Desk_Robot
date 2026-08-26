#pragma once
typedef enum { CALL_IDLE, CALL_INCOMING, CALL_ACTIVE } call_state_t;

void call_init(void);
void call_update(void);
call_state_t call_state(void);
