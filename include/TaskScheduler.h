#include "Task.h"

#define MAX_EXECUTION_TIME 50             //  In milliseconds (for all tasks)
#define SYS_TIME_OVF_THRESHOLD 0x80000000 // Max signed 32-bit integer value
class TaskScheduler
{
public:
    void addTask(const Task &task);
    void removeAllTasks();
    void removeTask(int index);
    void run();
    void stop();
    bool getSchedulerStatus();
    void setSchedulerStatus(bool status);
    void increaseSystemTime();

private:
    bool schedulerStatus = false;
    bool highPriorityTask = false;
    int HighPriorityTaskIndex = 0;
    Task tasks[MAX_TASKS]; // Static array to avoid dynamic allocation
    size_t currentTaskCount = 0;
    uint32_t systemTime = 0;
    void analyzeSystemTime(const unsigned int start, const unsigned int end);
    bool buryKilledTasks(int index);
    void checkHighPriorityTask();
    void clearHighPriorityTask();
    void setHighPriorityTask(int index);
    int getHighPriorityTaskIndex();
    void normalizeSystemTime();
    bool shiftPausedTasks(int index);
    uint32_t getSystemTime() const;
};