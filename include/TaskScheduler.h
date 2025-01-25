#pragma once

#include "Task.h"
#include <array>
#include <cstdint>

#define MAX_EXECUTION_TIME 50             // In milliseconds (for all tasks)
#define SYS_TIME_OVF_THRESHOLD 0x80000000 // Max signed 32-bit integer value

class TaskScheduler
{
public:
    void addTask(const Task &task);
    void removeAllTasks();
    void removeTask(int index);
    void run();
    void stop();
    bool getSchedulerStatus() const;
    void setSchedulerStatus(bool status);
    void increaseSystemTime();

private:
    bool schedulerStatus = false;
    bool highPriorityTask = false;
    int highPriorityTaskIndex = 0;
    std::array<Task, MAX_TASKS> tasks;
    size_t currentTaskCount = 0;
    uint32_t systemTime = 0;

    void analyzeSystemTime(uint32_t start, uint32_t end);
    bool buryKilledTasks(int index);
    bool shiftPausedTasks(int index);
    void checkHighPriorityTask();
    void clearHighPriorityTask();
    void setHighPriorityTask(int index);
    int getHighPriorityTaskIndex() const;
    void normalizeSystemTime();
    uint32_t getSystemTime() const;
};