#include "todo.h"
#include "display.h"
#include "config.h"

Task tasks[] =
{
    {"Study DSP",20,30,false,false},
    {"Team Meeting",21,00,false,false},
    {"Drink Water",21,15,false,false},
    {"Go to Sleep",22,00,false,false}
};

const int taskCount = sizeof(tasks)/sizeof(tasks[0]);

void todoInit()
{

}

void showTodoList()
{

    tft.fillScreen(ILI9341_BLACK);

    tft.setTextColor(ILI9341_YELLOW);
    tft.setTextSize(3);

    tft.setCursor(50,10);
    tft.println("TO-DO LIST");

    tft.drawLine(0,45,320,45,ILI9341_WHITE);

    int y=60;

    for(int i=0;i<taskCount;i++)
    {

        tft.setCursor(10,y);

        if(tasks[i].completed)
        {
            tft.setTextColor(ILI9341_GREEN);
            tft.print("[X] ");
        }
        else
        {
            tft.setTextColor(ILI9341_WHITE);
            tft.print("[ ] ");
        }

        tft.print(tasks[i].title);

        y+=35;

    }

}

void reminderPopup(String taskName)
{

    tft.fillScreen(ILI9341_RED);

    tft.setTextColor(ILI9341_WHITE);

    tft.setTextSize(3);

    tft.setCursor(45,40);
    tft.println("REMINDER");

    tft.setTextSize(2);

    tft.setCursor(20,110);
    tft.println(taskName);

    tft.setCursor(20,140);
    tft.println("Starts in 10 min");

    tone(BUZZER_PIN,1000);

    delay(800);

    noTone(BUZZER_PIN);

    delay(3000);

}

void updateTodo()
{
    struct tm timeinfo;

    if(!getLocalTime(&timeinfo))
        return;

    int nowMinutes = timeinfo.tm_hour * 60 + timeinfo.tm_min;

    for(int i = 0; i < taskCount; i++)
    {
        int taskMinutes = tasks[i].hour * 60 + tasks[i].minute;

        if(!tasks[i].reminded &&
           nowMinutes == taskMinutes - 10)
        {
            tasks[i].reminded = true;

            reminderPopup(tasks[i].title);
        }
    }
}