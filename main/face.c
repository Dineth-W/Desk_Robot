#include "face.h"
#include "display.h"
#include "esp_timer.h"

static face_expression_t expr=FACE_NORMAL;
static bool blink=false;
static bool mouth=false;
static int64_t last_blink=0,last_anim=0;

void face_init(void){ face_draw(); }

void face_set(face_expression_t e){ expr=e; face_draw(); }

static void eye(int x,int y)
{
    if(blink){ display_draw_hline(x-7,y,14,true); return; }
    display_fill_rect(x-7,y-7,15,15,true);
    display_fill_rect(x-3,y-3,7,7,false);
}

void face_draw(void)
{
    display_clear();
    eye(38,27);
    eye(90,27);

    if(expr==FACE_HAPPY){
        display_draw_hline(56,49,16,true);
        display_draw_hline(58,47,12,true);
    } else if(expr==FACE_SAD){
        display_draw_hline(56,47,16,true);
        display_draw_hline(58,49,12,true);
    } else if(expr==FACE_SURPRISED){
        display_draw_rect(59,42,10,12,true);
    } else if(expr==FACE_SLEEP){
        display_draw_hline(56,48,16,true);
    } else if(expr==FACE_TALKING && mouth){
        display_fill_rect(58,43,12,10,true);
    } else {
        display_draw_hline(56,48,16,true);
    }
    display_update();
}

void face_update(void)
{
    int64_t now=esp_timer_get_time()/1000;
    if(now-last_blink>3500){
        blink=true; face_draw();
        vTaskDelay(pdMS_TO_TICKS(120));
        blink=false; last_blink=now; face_draw();
    }
    if(expr==FACE_TALKING && now-last_anim>300){
        mouth=!mouth; last_anim=now; face_draw();
    }
}
