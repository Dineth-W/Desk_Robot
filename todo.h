#ifndef TODO_H
#define TODO_H

#include <Arduino.h>

#define MAX_TASKS 20

// Task Priority
enum TaskPriority
{
    PRIORITY_LOW,
    PRIORITY_MEDIUM,
    PRIORITY_HIGH
};


// Task Structure
struct Task
{
    String title;

    int hour;
    int minute;

    bool enabled;
    bool completed;
    bool reminderShown;

    TaskPriority priority;
};

// Global Variables
extern Task tasks[MAX_TASKS];
extern int taskCount;

// Used by sketch.ino for reminder timeout
extern unsigned long reminderStartTime;

// Initialization
void todoInit();

// Update
void updateTodo();

// Drawing
void drawTodoList();
void drawReminder(Task task);

// Task Management
bool addTask(
    String title,
    int hour,
    int minute,
    TaskPriority priority
);

void completeTask(int index);
void removeTask(int index);
Task* getTask(int index);

#endif
