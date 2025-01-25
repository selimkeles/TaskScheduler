#include "Task.h"

void Task::execute()
{
    if (this->status == Status::ACTIVE)
    {
        if (this->taskFunction != nullptr)
        {
            this->taskFunction(this->user_input);
        }
    }
}

void Task::pause()
{
    if (this->status == Status::ACTIVE)
    {
        this->status = Status::PAUSED;
    }
}

void Task::resume()
{
    if (this->status == Status::PAUSED)
    {
        this->status = Status::ACTIVE;
    }
}

void Task::kill()
{
    this->status = Status::KILLED;
}

Task::Status Task::getStatus() const
{
    return this->status;
}

CallbackTime Task::getExecutionTime() const
{
    return this->executionTime;
}

CallbackTime  Task::getPeriod() const
{
    return this->period;
}

uint32_t Task::getNextExecutionTime() const
{
    return this->nextExecutionTime;
}

void Task::setNextExecutionTime(uint32_t time)
{
    this->nextExecutionTime = time;
}

bool Task::getPriority() const
{
    return this->priority;
}

void Task::setPriority(bool value)
{
    this->priority = value;
}

void *Task::getUserInput() const
{
    return this->user_input;
}

TaskFunction Task::getTaskFunction() const
{
    return this->taskFunction;
}