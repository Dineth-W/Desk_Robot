#include "call.h"
#include "buttons.h"
#include "display.h"
#include "face.h"
#include "buzzer.h"
#include "esp_timer.h"
#include <stdio.h>

static call_state_t state=CALL_IDLE;
static int64_t next_demo;
static int64_t last_ring;

void call_init(void)
{
    next_demo=esp_timer_get_time()/1000+60000;
}

static void incoming_draw(void)
{
    display_clear();
    display_center_text("INCOMING CALL",0,1,true);
    display_center_text("John Smith",20,1,true);
    display_center_text("ANSWER",40,1,true);
    display_center_text("END",54,1,true);
    display_update();
}

static void active_draw(void)
{
    display_clear();
    display_center_text("CALL ACTIVE",0,1,true);
    display_center_text("John Smith",20,1,true);
    display_center_text("TALKING",38,1,true);
    display_center_text("END",54,1,true);
    display_update();
}

call_state_t call_state(void){return state;}

void call_update(void)
{
    int64_t now=esp_timer_get_time()/1000;

    /* Development simulation only. Replace this state machine with HFP later. */
    if(state==CALL_IDLE && now>=next_demo){
        state=CALL_INCOMING;
        last_ring=0;
        incoming_draw();
    }

    if(state==CALL_INCOMING){
        if(now-last_ring>=600){
            last_ring=now;
            buzzer_beep(100);
        }
        if(button_answer_pressed()){
            state=CALL_ACTIVE;
            buzzer_off();
            face_set(FACE_TALKING);
            active_draw();
        } else if(button_reject_pressed()){
            state=CALL_IDLE;
            buzzer_off();
            face_set(FACE_NORMAL);
            next_demo=now+60000;
        }
    } else if(state==CALL_ACTIVE && button_reject_pressed()){
        state=CALL_IDLE;
        buzzer_off();
        face_set(FACE_NORMAL);
        next_demo=now+60000;
    }
}
