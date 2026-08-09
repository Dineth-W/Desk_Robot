#include "todo.h"
#include "display.h"
#include "clock.h"
#include "screen.h"

Task tasks[MAX_TASKS];
int taskCount = 0;

// Used by sketch.ino to automatically
// return from reminder screen
unsigned long reminderStartTime = 0;

// Initialize
void todoInit()
{
    addTask("Study DSP",20,30,PRIORITY_HIGH);
    addTask("Drink Water",21,00,PRIORITY_LOW);
    addTask("Project Meeting",21,30,PRIORITY_HIGH);
    addTask("Sleep",22,30,PRIORITY_MEDIUM);
}

// Add Task
bool addTask(String title,
             int hour,
             int minute,
             TaskPriority priority)
{
    if(taskCount >= MAX_TASKS)
        return false;

    tasks[taskCount].title = title;
    tasks[taskCount].hour = hour;
    tasks[taskCount].minute = minute;

    tasks[taskCount].enabled = true;
    tasks[taskCount].completed = false;
    tasks[taskCount].reminderShown = false;

    tasks[taskCount].priority = priority;

    taskCount++;

    return true;
}
// Complete Task
void completeTask(int index)
{
    if(index < 0 || index >= taskCount)
        return;

    tasks[index].completed = true;
}

// Remove Task
void removeTask(int index)
{
    if(index < 0 || index >= taskCount)
        return;

    for(int i=index;i<taskCount-1;i++)
    {
        tasks[i] = tasks[i+1];
    }

    taskCount--;
}

// Get Task
Task* getTask(int index)
{
    if(index < 0 || index >= taskCount)
        return nullptr;

    return &tasks[index];
}

// Draw Todo List
void drawTodoList()
{
    clearScreen();

    drawCenteredText(
        "TO-DO LIST",
        10,
        ILI9341_YELLOW,
        3);

    int y = 50;

    for(int i=0;i<taskCount;i++)
    {
        tft.setCursor(10,y);
        tft.setTextSize(2);

        switch(tasks[i].priority)
        {
            case PRIORITY_LOW:
                tft.setTextColor(ILI9341_GREEN);
                break;

            case PRIORITY_MEDIUM:
                tft.setTextColor(ILI9341_YELLOW);
                break;

            case PRIORITY_HIGH:
                tft.setTextColor(ILI9341_RED);
                break;
        }

        if(tasks[i].completed)
            tft.print("[X] ");
        else
            tft.print("[ ] ");

        tft.print(tasks[i].title);

        char buffer[10];

        sprintf(buffer,
                " %02d:%02d",
                tasks[i].hour,
                tasks[i].minute);

        tft.print(buffer);

        y += 25;
    }
}

// Reminder Screen
void drawReminder(Task task)
{
    clearScreen();

    drawCenteredText(
        "REMINDER",
        35,
        ILI9341_RED,
        3);

    drawCenteredText(
        task.title,
        90,
        ILI9341_WHITE,
        2);

    char buffer[10];

    sprintf(buffer,
            "%02d:%02d",
            task.hour,
            task.minute);

    drawCenteredText(
        String(buffer),
        135,
        ILI9341_CYAN,
        4);

    drawCenteredText(
        "Starts in 10 Minutes",
        200,
        ILI9341_GREEN,
        2);
}

// Update Todo
void updateTodo()
{
    struct tm now;

    if(!getLocalTime(&now))
        return;

    int currentMinutes =
        now.tm_hour * 60 +
        now.tm_min;

    for(int i=0;i<taskCount;i++)
    {
        if(!tasks[i].enabled)
            continue;

        if(tasks[i].completed)
            continue;

        int taskMinutes =
            tasks[i].hour * 60 +
            tasks[i].minute;

        // Reminder (10 minutes before)
        if(!tasks[i].reminderShown &&
           currentMinutes == taskMinutes - 10)
        {
            tasks[i].reminderShown = true;

            changeScreen(SCREEN_REMINDER);

            drawReminder(tasks[i]);

            reminderStartTime = millis();

            Serial.print("Reminder: ");
            Serial.println(tasks[i].title);
        }
    }
}
