#include "TaskScheduler.h"
#include <stdexcept>

void TaskScheduler::addTask(const Task &task)
{
    if (currentTaskCount >= MAX_TASKS)
    {
        throw std::runtime_error("Maximum task count exceeded");
    }
    tasks[currentTaskCount].setNextExecutionTime(getSystemTime() + task.getExecutionTime());
    tasks[currentTaskCount] = task;
    if (task.getPriority() == true)
    {
        setHighPriorityTask(currentTaskCount);
        tasks[currentTaskCount++].setPriority(false);
    }
}

void TaskScheduler::removeAllTasks()
{
    clearHighPriorityTask();
    for (size_t i = 0; i < currentTaskCount - 1; ++i)
    {
        tasks[i].kill();
    }
}

void TaskScheduler::removeTask(int index)
{
    if (index < 0 || index >= currentTaskCount)
    {
        throw std::runtime_error("Task index out of range");
    }
    if (tasks[index].getPriority() == true)
    {
        clearHighPriorityTask();
    }
    for (size_t i = index; i < currentTaskCount - 1; i++)
    {
        tasks[i] = tasks[i + 1];
    }
    --currentTaskCount;
}

void TaskScheduler::run()
{
    this->setSchedulerStatus(true);
    while (this->getSchedulerStatus())
    {
        unsigned int start = getSystemTime();
        for (size_t index = 0; index < currentTaskCount; index++)
        {
            if (buryKilledTasks(index)) {
                index--;
                continue;
            }
            if (shiftPausedTasks(index)) continue;
            if (tasks[index].getStatus() == Task::Status::ACTIVE && getSystemTime() >= tasks[index].getNextExecutionTime())
            {
                tasks[index].execute();
                tasks[index].setNextExecutionTime(getSystemTime() + tasks[index].getPeriod());
            }
        }
        unsigned int end = getSystemTime();
        analyzeSystemTime(start, end);
    }
}

bool TaskScheduler::buryKilledTasks(int index)
{
    if (tasks[index].getStatus() == Task::Status::KILLED)
    {
        removeTask(index);
    }
}

bool TaskScheduler::shiftPausedTasks(int index)
{
    if (tasks[index].getStatus() == Task::Status::PAUSED)
    {
        tasks[index].setNextExecutionTime(tasks[index].getNextExecutionTime() + CB_1MS);    // Ugly I know, but it works
    }
}

void TaskScheduler::checkHighPriorityTask()
{
    if (getHighPriorityTaskIndex())
    {
        tasks[getHighPriorityTaskIndex()].execute();
        tasks[getHighPriorityTaskIndex()].setNextExecutionTime(getSystemTime() + tasks[HighPriorityTaskIndex].getPeriod());
        tasks[getHighPriorityTaskIndex()].setPriority(false);
        clearHighPriorityTask();
    }
}

void TaskScheduler::clearHighPriorityTask()
{
    highPriorityTask = false;
    HighPriorityTaskIndex = 0;
}

void TaskScheduler::setHighPriorityTask(int index)
{
    highPriorityTask = true;
    HighPriorityTaskIndex = index;
}

int TaskScheduler::getHighPriorityTaskIndex()
{
    if (highPriorityTask != false && HighPriorityTaskIndex != 0) return HighPriorityTaskIndex;
    return 0;
}

void TaskScheduler::stop()
{
    setSchedulerStatus(false);
}

bool TaskScheduler::getSchedulerStatus()
{
    return this->schedulerStatus;
}

void TaskScheduler::setSchedulerStatus(bool status)
{
    this->schedulerStatus = status;
}

void TaskScheduler::increaseSystemTime()
{
    systemTime += CB_1MS;
    normalizeSystemTime();
}

void TaskScheduler::normalizeSystemTime()
{
    if (getSystemTime() >= SYS_TIME_OVF_THRESHOLD + CB_1H)
    {
        systemTime -= SYS_TIME_OVF_THRESHOLD;
        for (size_t i = 0; i < currentTaskCount; ++i)
        {
            tasks[i].setNextExecutionTime(tasks[i].getNextExecutionTime() - SYS_TIME_OVF_THRESHOLD);
        }
    }
}

uint32_t TaskScheduler::getSystemTime() const
{
    return systemTime;
}

void TaskScheduler::analyzeSystemTime(const unsigned int start, const unsigned int end)
{
    unsigned long duration = end - start;
    if (duration > MAX_EXECUTION_TIME)
    {
        throw std::runtime_error("Tasks are taking too long to execute");
    }
}