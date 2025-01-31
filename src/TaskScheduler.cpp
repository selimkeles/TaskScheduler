#include "TaskScheduler.h"

void TaskScheduler::addTask(const Task &task)
{
    if (this->currentTaskCount >= MAX_TASKS)
    {
        // Call Error Handler
        return;
    }
    this->tasks[this->currentTaskCount].setNextExecutionTime(this->getSystemTime() + static_cast<uint32_t>(task.getExecutionTime()));
    this->tasks[this->currentTaskCount++] = task;
    if (task.getPriority())
    {
        this->setHighPriorityTask(this->currentTaskCount-1);
        this->tasks[this->currentTaskCount-1].setPriority(false);
    }
}

void TaskScheduler::deleteAllTasks()
{
    this->clearHighPriorityTask();
    for (size_t i = 0; i < this->currentTaskCount; ++i)
    {
        this->tasks[i].kill();
    }
    this->currentTaskCount = 0;
}

void TaskScheduler::deleteTask(size_t index)
{
    if (index < 0 || index >= static_cast<int>(this->currentTaskCount))
    {
        // Call Error Handler
        return;
    }
    if (this->tasks[index].getPriority())
    {
        this->clearHighPriorityTask();
    }
    for (size_t i = index; i < this->currentTaskCount; ++i)
    {
        this->tasks[i] = this->tasks[i + 1];
    }
    --this->currentTaskCount;
}

void TaskScheduler::deleteTask(TaskFunction matchFunction)
{
    for (size_t i = 0; i < this->currentTaskCount; ++i)
    {
        if (matchFunction == this->tasks[i].getTaskFunction())
        {
            this->deleteTask(static_cast<int>(i));
            return;
        }
    }
    // Call Error Handler if task not found
}

void TaskScheduler::run()
{
    this->setSchedulerStatus(true);
    while (this->getSchedulerStatus())
    {
        uint32_t start = this->getSystemTime();
        for (size_t index = 0; index < this->currentTaskCount; ++index)
        {
            if (this->buryKilledTasks(index))
            {
                --index;
                continue;
            }
            if (this->shiftPausedTasks(index))
            {
                continue;
            }
            if (this->tasks[index].getStatus() == Task::Status::ACTIVE && this->getSystemTime() >= this->tasks[index].getNextExecutionTime())
            {
                this->tasks[index].execute();
                this->tasks[index].setNextExecutionTime(this->getSystemTime() + static_cast<uint32_t>(this->tasks[index].getPeriod()));
            }
        }
        uint32_t end = this->getSystemTime();
        this->analyzeSystemTime(start, end);
    }
}

bool TaskScheduler::buryKilledTasks(size_t index)
{
    if (this->tasks[index].getStatus() == Task::Status::KILLED)
    {
        this->deleteTask(index);
        return true;
    }
    return false;
}

bool TaskScheduler::shiftPausedTasks(size_t index)
{
    if (this->tasks[index].getStatus() == Task::Status::PAUSED)
    {
        this->tasks[index].setNextExecutionTime(this->tasks[index].getNextExecutionTime() + static_cast<uint32_t>(CallbackTime::CB_1MS));
        return true;
    }
    return false;
}

void TaskScheduler::checkHighPriorityTask()
{
    if (this->getHighPriorityTaskIndex() != 0)
    {
        this->tasks[this->getHighPriorityTaskIndex()].execute();
        this->tasks[this->getHighPriorityTaskIndex()].setNextExecutionTime(this->getSystemTime() + static_cast<uint32_t>(this->tasks[this->highPriorityTaskIndex].getPeriod()));
        this->tasks[this->getHighPriorityTaskIndex()].setPriority(false);
        this->clearHighPriorityTask();
    }
}

void TaskScheduler::clearHighPriorityTask()
{
    this->highPriorityTask = false;
    this->highPriorityTaskIndex = 0;
}

void TaskScheduler::setHighPriorityTask(size_t index)
{
    this->highPriorityTask = true;
    this->highPriorityTaskIndex = index;
}

size_t TaskScheduler::getHighPriorityTaskIndex() const
{
    return this->highPriorityTask ? this->highPriorityTaskIndex : 0;
}

void TaskScheduler::stop()
{
    this->setSchedulerStatus(false);
}

bool TaskScheduler::getSchedulerStatus() const
{
    return this->schedulerStatus;
}

void TaskScheduler::setSchedulerStatus(bool status)
{
    this->schedulerStatus = status;
}

void TaskScheduler::increaseSystemTime()
{
    this->systemTime += static_cast<uint32_t>(CallbackTime::CB_1MS);
    this->normalizeSystemTime();
}

void TaskScheduler::normalizeSystemTime()
{
    if (this->getSystemTime() >= SYS_TIME_OVF_THRESHOLD + static_cast<uint32_t>(CallbackTime::CB_1H))
    {
        this->systemTime -= SYS_TIME_OVF_THRESHOLD;
        for (size_t i = 0; i < this->currentTaskCount; ++i)
        {
            this->tasks[i].setNextExecutionTime(this->tasks[i].getNextExecutionTime() - SYS_TIME_OVF_THRESHOLD);
        }
    }
}

uint32_t TaskScheduler::getSystemTime() const
{
    return this->systemTime;
}

void TaskScheduler::analyzeSystemTime(uint32_t start, uint32_t end)
{
    uint32_t duration = end - start;
    if (duration > MAX_EXECUTION_TIME)
    {
        // Call Error Handler
        return;
    }
}