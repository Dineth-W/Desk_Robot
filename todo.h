#ifndef TODO_H
#define TODO_H

struct Task
{
    String title;

    int hour;
    int minute;

    bool completed;

    bool reminded;
};

extern Task tasks[];

extern const int taskCount;

void todoInit();

void updateTodo();

void showTodoList();

#endif