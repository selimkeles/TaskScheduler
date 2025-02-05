#include "unity.h"
#include "Task.h"

void setUp(void) {
    // Called before each test
}

void tearDown(void) {
    // Called after each test
}

static bool taskExecuted = false;
static void* savedUserInput = nullptr;

static void testCallback(void* user_input) {
    taskExecuted = true;
    savedUserInput = user_input;
}

void test_task_default_constructor(void) {
    Task task;
    TEST_ASSERT_EQUAL(Task::Status::INACTIVE, task.getStatus());
    TEST_ASSERT_EQUAL(static_cast<uint32_t>(CallbackTime::CB_INSTANT), static_cast<uint32_t>(task.getExecutionTime()));
    TEST_ASSERT_EQUAL(static_cast<uint32_t>(CallbackTime::CB_1MS), static_cast<uint32_t>(task.getPeriod()));
    TEST_ASSERT_FALSE(task.getPriority());
    TEST_ASSERT_NULL(task.getUserInput());
    TEST_ASSERT_NULL(task.getTaskFunction());
}

void test_task_execution(void) {
    taskExecuted = false;
    int userData = 42;
    Task task(CallbackTime::CB_1MS, CallbackTime::CB_10MS, &userData, testCallback);
    
    TEST_ASSERT_EQUAL(Task::Status::ACTIVE, task.getStatus());
    task.execute();
    TEST_ASSERT_TRUE(taskExecuted);
    TEST_ASSERT_EQUAL_PTR(&userData, savedUserInput);
}

void test_task_state_transitions(void) {
    Task task(CallbackTime::CB_1MS, testCallback);
    
    // Test initial state
    TEST_ASSERT_EQUAL(Task::Status::ACTIVE, task.getStatus());
    
    // Test pause
    task.pause();
    TEST_ASSERT_EQUAL(Task::Status::PAUSED, task.getStatus());
    
    // Test resume
    task.resume();
    TEST_ASSERT_EQUAL(Task::Status::ACTIVE, task.getStatus());
    
    // Test kill
    task.kill();
    TEST_ASSERT_EQUAL(Task::Status::KILLED, task.getStatus());
}

void test_task_timing_properties(void) {
    Task task(CallbackTime::CB_100MS, CallbackTime::CB_1S, nullptr, testCallback);
    
    TEST_ASSERT_EQUAL(static_cast<uint32_t>(CallbackTime::CB_100MS), 
                     static_cast<uint32_t>(task.getExecutionTime()));
    TEST_ASSERT_EQUAL(static_cast<uint32_t>(CallbackTime::CB_1S), 
                     static_cast<uint32_t>(task.getPeriod()));
    
    task.setNextExecutionTime(1000);
    TEST_ASSERT_EQUAL(1000, task.getNextExecutionTime());
}

void test_task_priority(void) {
    Task task(CallbackTime::CB_1MS, testCallback);
    
    TEST_ASSERT_FALSE(task.getPriority());
    task.setPriority(true);
    TEST_ASSERT_TRUE(task.getPriority());
}

int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_task_default_constructor);
    RUN_TEST(test_task_execution);
    RUN_TEST(test_task_state_transitions);
    RUN_TEST(test_task_timing_properties);
    RUN_TEST(test_task_priority);
    
    return UNITY_END();
} 