#include "clock.h"
#include "rtc.h"
#include "display.h"
#include <time.h>
#include <stdio.h>

static struct tm current;

void clock_init(void) {}

bool clock_get(struct tm *t)
{
    if (rtc_read(t)) return true;
    time_t now=time(NULL);
    if(now < 100000) return false;
    localtime_r(&now,t);
    return true;
}

void clock_draw(void)
{
    char s[32];
    display_clear();
    if(clock_get(&current)){
        snprintf(s,sizeof(s),"%02d:%02d:%02d",
                 current.tm_hour,current.tm_min,current.tm_sec);
        display_center_text(s,18,2,true);
        snprintf(s,sizeof(s),"%02d/%02d/%04d",
                 current.tm_mday,current.tm_mon+1,current.tm_year+1900);
        display_center_text(s,48,1,true);
    }else{
        display_center_text("--:--:--",18,2,true);
        display_center_text("RTC NOT READY",50,1,true);
    }
    display_update();
}

void clock_update(void)
{
    static int last_sec=-1;
    struct tm t;
    if(!clock_get(&t)) return;
    if(t.tm_sec!=last_sec){
        last_sec=t.tm_sec;
        clock_draw();
    }
}
