#include "todo.h"
#include "clock.h"
#include "display.h"
#include "buzzer.h"
#include "esp_timer.h"
#include <stdio.h>
#include <string.h>

static task_t tasks[MAX_TASKS];
static int count;

void todo_init(void)
{
    strcpy(tasks[0].title,"Study DSP"); tasks[0].hour=20; tasks[0].minute=30; tasks[0].enabled=true;
    strcpy(tasks[1].title,"Drink Water"); tasks[1].hour=21; tasks[1].minute=0; tasks[1].enabled=true;
    strcpy(tasks[2].title,"Project Meeting"); tasks[2].hour=21; tasks[2].minute=30; tasks[2].enabled=true;
    count=3;
}

void todo_draw(void)
{
    display_clear();
    display_center_text("TO-DO LIST",0,1,true);
    for(int i=0;i<count && i<5;i++){
        char line[32];
        snprintf(line,sizeof(line),"%c %s",
                 tasks[i].completed?'X':'-',tasks[i].title);
        display_text(0,12+i*10,line,1,true);
    }
    display_update();
}

void todo_update(void)
{
    static int last_min=-1;
    struct tm t;
    if(!clock_get(&t)) return;
    int m=t.tm_hour*60+t.tm_min;
    if(m==last_min) return;
    last_min=m;

    for(int i=0;i<count;i++){
        if(!tasks[i].enabled || tasks[i].completed) continue;
        int rm=tasks[i].hour*60+tasks[i].minute-10;
        if(rm<0) rm+=1440;
        if(m==rm) buzzer_beep(150);
    }
}
