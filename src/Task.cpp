#include "Task.h"

void Task::execute()
{
    if (status == Status::ACTIVE)
    {
        if (taskFunction != nullptr)
        {
            if (getUserInput() != nullptr)
            {
                taskFunction(getUserInput());
            }
            else
            {
                taskFunction(nullptr);
            }
        }
    }
}

void Task::pause()
{
    if (status == Status::ACTIVE)
    {
        status = Status::PAUSED;
    }
}

void Task::resume()
{
    if (status == Status::PAUSED)
    {
        status = Status::ACTIVE;
    }
}

void Task::kill()
{
    status = Status::KILLED;
}

Task::Status Task::getStatus() const
{
    return status;
}

int Task::getExecutionTime() const
{
    return executionTime;
}

int Task::getPeriod() const
{
    return period;
}

unsigned long Task::getNextExecutionTime() const
{
    return nextExecutionTime;
}

void Task::setNextExecutionTime(unsigned long time)
{
    nextExecutionTime = time;
}

bool Task::getPriority() const
{
    return priority;
}

void Task::setPriority(bool value)
{
    this->priority = value;
}

void *Task::getUserInput() const
{
    return user_input;
}
