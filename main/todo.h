#pragma once
#define MAX_TASKS 10

typedef struct {
    char title[24];
    int hour;
    int minute;
    bool enabled;
    bool completed;
} task_t;

void todo_init(void);
void todo_update(void);
void todo_draw(void);
