#include <gtest/gtest.h>
#include "TaskScheduler.h"
#include "Task.h"

class TaskSchedulerTest : public ::testing::Test {
protected:
    TaskScheduler scheduler;
    Task* task;

    void SetUp() override {
        task = new Task(1, 1000, true, [](void*){});
        scheduler.addTask(*task);
    }

    void TearDown() override {
        delete task;
    }
};

TEST_F(TaskSchedulerTest, AddTask) {
    EXPECT_NO_THROW(scheduler.addTask(*task));
}

TEST_F(TaskSchedulerTest, RemoveTask) {
    EXPECT_NO_THROW(scheduler.removeTask(0));
}

TEST_F(TaskSchedulerTest, RunScheduler) {
    EXPECT_NO_THROW(scheduler.run());
}

TEST_F(TaskSchedulerTest, StopScheduler) {
    scheduler.run();
    EXPECT_NO_THROW(scheduler.stop());
}

TEST_F(TaskSchedulerTest, SchedulerStatus) {
    scheduler.setSchedulerStatus(true);
    EXPECT_TRUE(scheduler.getSchedulerStatus());
    scheduler.setSchedulerStatus(false);
    EXPECT_FALSE(scheduler.getSchedulerStatus());
}