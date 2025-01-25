#pragma once

#include "stdint.h"

typedef void (*TaskFunction)(void *user_input);

#define MAX_TASKS 20

// Callback times for periodic tasks
enum class CallbackTime : uint32_t
{
    CB_INSTANT = 0x00,
    CB_1MS = 0x02,
    CB_10MS = CB_1MS * 10,
    CB_20MS = CB_10MS * 2,
    CB_30MS = CB_10MS * 3,
    CB_50MS = CB_10MS * 5,
    CB_100MS = CB_10MS * 10,
    CB_200MS = CB_100MS * 2,
    CB_300MS = CB_100MS * 3,
    CB_500MS = CB_100MS * 5,
    CB_1S = CB_100MS * 10,
    CB_2S = CB_1S * 2,
    CB_3S = CB_1S * 3,
    CB_5S = CB_1S * 5,
    CB_10S = CB_1S * 10,
    CB_15S = CB_1S * 15,
    CB_30S = CB_1S * 30,
    CB_45S = CB_1S * 45,
    CB_1M = CB_1S * 60,
    CB_2M = CB_1M * 2,
    CB_5M = CB_1M * 5,
    CB_10M = CB_1M * 10,
    CB_15M = CB_1M * 15,
    CB_30M = CB_1M * 30,
    CB_45M = CB_1M * 45,
    CB_1H = CB_1M * 60,
    CB_2H = CB_1H * 2,
    CB_4H = CB_2H * 2,
    CB_8H = CB_4H * 2,
    CB_12H = CB_1H * 12,
    CB_1D = CB_12H * 2
};

class Task
{
public:
    enum class Status
    {
        INACTIVE, // Constructor Option
        ACTIVE,
        PAUSED,
        KILLED
    };
    Task() : executionTime(CallbackTime::CB_INSTANT), period(CallbackTime::CB_1MS), priority(false), user_input(nullptr), taskFunction(nullptr), status(Status::INACTIVE), nextExecutionTime(0) {} // Default constructor
    Task(CallbackTime period, TaskFunction func)    // Constructor with instant operation and no user input
        : executionTime(CallbackTime::CB_INSTANT), period(period), priority(false), user_input(nullptr), taskFunction(func), status(Status::ACTIVE), nextExecutionTime(0) {}
    Task(CallbackTime period, void *user_input, TaskFunction func)   // Constructor with instant operation
        : executionTime(CallbackTime::CB_INSTANT), period(period), priority(false), user_input(user_input), taskFunction(func), status(Status::ACTIVE), nextExecutionTime(0) {}
    Task(CallbackTime executionTime, CallbackTime period, void *user_input, TaskFunction func)
        : executionTime(executionTime), period(period), priority(false), user_input(user_input), taskFunction(func), status(Status::ACTIVE), nextExecutionTime(0) {}

    void execute();
    void pause();
    void resume();
    void kill();
    Status getStatus() const;
    CallbackTime getExecutionTime() const;
    bool getPriority() const;
    void setPriority(bool value);
    CallbackTime getPeriod() const;
    uint32_t getNextExecutionTime() const;
    void setNextExecutionTime(uint32_t time);
    void *getUserInput() const;
    TaskFunction getTaskFunction() const;

private:
    CallbackTime executionTime;
    CallbackTime period;
    bool priority;
    void *user_input;
    TaskFunction taskFunction;
    Status status;
    uint32_t nextExecutionTime;
};