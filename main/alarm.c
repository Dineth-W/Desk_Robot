#include "alarm.h"
#include "clock.h"
#include "display.h"
#include "buzzer.h"

typedef struct { int h,m; bool repeat; bool triggered; } alarm_t;
static alarm_t a[4]={{6,30,true,false},{8,0,false,false}};
static bool active;

void alarm_init(void){}

bool alarm_active(void){return active;}

void alarm_stop(void)
{
    active=false;
    buzzer_off();
    for(int i=0;i<4;i++) if(a[i].repeat) a[i].triggered=false;
}

void alarm_update(void)
{
    struct tm t;
    if(!clock_get(&t)) return;
    int m=t.tm_hour*60+t.tm_min;

    for(int i=0;i<2;i++){
        if(a[i].triggered) continue;
        if(m==a[i].h*60+a[i].m){
            a[i].triggered=true;
            active=true;
            display_clear();
            display_center_text("ALARM",0,1,true);
            char s[8];
            snprintf(s,sizeof(s),"%02d:%02d",a[i].h,a[i].m);
            display_center_text(s,20,3,true);
            display_center_text("PRESS END",54,1,true);
            display_update();
            buzzer_on();
        }
    }
}
