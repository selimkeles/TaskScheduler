#include <gtest/gtest.h>
#include "../include/Task.h"

class TaskTest : public ::testing::Test {
protected:
    Task* task;

    void SetUp() override {
        task = new Task(1, 1000); // Task ID 1, execution time 1000 ms
    }

    void TearDown() override {
        delete task;
    }
};

TEST_F(TaskTest, ExecuteTask) {
    EXPECT_NO_THROW(task->execute());
    EXPECT_EQ(task->getStatus(), Task::Status::ACTIVE);
}

TEST_F(TaskTest, PauseTask) {
    task->execute();
    task->pause();
    EXPECT_EQ(task->getStatus(), Task::Status::PAUSED);
}

TEST_F(TaskTest, ResumeTask) {
    task->execute();
    task->pause();
    task->resume();
    EXPECT_EQ(task->getStatus(), Task::Status::ACTIVE);
}

TEST_F(TaskTest, KillTask) {
    task->execute();
    task->kill();
    EXPECT_EQ(task->getStatus(), Task::Status::KILLED);
}

TEST_F(TaskTest, TaskExecutionTime) {
    task->execute();
    EXPECT_GE(task->getExecutionTime(), 0);
}