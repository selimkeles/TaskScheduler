#include "unity.h"
#include "TaskScheduler.h"
#include <thread>
#include <chrono>
#include <atomic>

#ifdef _WIN32
#include <windows.h>
#else
#include <signal.h>
#include <time.h>
#endif

static bool taskExecuted = false;
static int executionCount = 0;
static std::atomic<bool> timerRunning(false);
static std::thread timerThread;

static void startSystemTimer(TaskScheduler& scheduler) {
    timerRunning = true;
    timerThread = std::thread([&scheduler]() {
        #ifdef _WIN32
            HANDLE timer = CreateWaitableTimer(NULL, FALSE, NULL);
            if (timer != NULL) {
                LARGE_INTEGER li;
                li.QuadPart = -10000; // 1ms intervals
                
                if (SetWaitableTimer(timer, &li, 1, NULL, NULL, FALSE)) {
                    while (timerRunning) {
                        WaitForSingleObject(timer, INFINITE);
                        scheduler.increaseSystemTime();
                    }
                }
                CloseHandle(timer);
            }
        #else
            while (timerRunning) {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                scheduler.increaseSystemTime();
            }
        #endif
    });
}

static void stopSystemTimer() {
    timerRunning = false;
    if (timerThread.joinable()) {
        timerThread.join();
    }
}

void setUp(void) {
    taskExecuted = false;
    executionCount = 0;
}

void tearDown(void) {
    if (timerRunning) {
        stopSystemTimer();
    }
}

static void testCallback(void* user_input) {
    taskExecuted = true;
    executionCount++;
}

void test_scheduler_initial_state(void) {
    TaskScheduler scheduler;
    TEST_ASSERT_FALSE(scheduler.getSchedulerStatus());
}

void test_scheduler_add_task(void) {
    TaskScheduler scheduler;
    Task task(CallbackTime::CB_1MS, testCallback);
    
    scheduler.addTask(task);
    
    // Start scheduler first
    std::thread schedulerThread([&scheduler]() {
        scheduler.run();
    });
    
    // Then start timer
    startSystemTimer(scheduler);
    
    // Wait for task execution or timeout
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    // Stop in correct order
    scheduler.stop();
    stopSystemTimer();  // Stop timer before joining thread
    schedulerThread.join();
    
    TEST_ASSERT_TRUE(taskExecuted);
}

void test_scheduler_delete_task(void) {
    TaskScheduler scheduler;
    Task task(CallbackTime::CB_1MS, testCallback);
    
    scheduler.addTask(task);
    scheduler.deleteTask(static_cast<size_t>(0));
    
    taskExecuted = false;
    
    std::thread schedulerThread([&scheduler]() {
        scheduler.run();
    });
    
    startSystemTimer(scheduler);
    
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    scheduler.stop();
    stopSystemTimer();
    schedulerThread.join();
    
    TEST_ASSERT_FALSE(taskExecuted);
}

void test_scheduler_delete_all_tasks(void) {
    TaskScheduler scheduler;
    Task task1(CallbackTime::CB_1MS, testCallback);
    Task task2(CallbackTime::CB_1MS, testCallback);
    
    scheduler.addTask(task1);
    scheduler.addTask(task2);
    scheduler.deleteAllTasks();
    
    taskExecuted = false;
    
    std::thread schedulerThread([&scheduler]() {
        scheduler.run();
    });
    
    startSystemTimer(scheduler);
    
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    scheduler.stop();
    stopSystemTimer();
    schedulerThread.join();
    
    TEST_ASSERT_FALSE(taskExecuted);
}

void test_scheduler_high_priority_task(void) {
    TaskScheduler scheduler;
    Task highPriorityTask(CallbackTime::CB_1MS, CallbackTime::CB_10MS, nullptr, testCallback);
    highPriorityTask.setPriority(true);
    
    scheduler.addTask(highPriorityTask);
    
    std::thread schedulerThread([&scheduler]() {
        scheduler.run();
    });
    
    startSystemTimer(scheduler);
    
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    scheduler.stop();
    stopSystemTimer();
    schedulerThread.join();
    
    TEST_ASSERT_TRUE(taskExecuted);
}

int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_scheduler_initial_state);
    RUN_TEST(test_scheduler_add_task);
    RUN_TEST(test_scheduler_delete_task);
    RUN_TEST(test_scheduler_delete_all_tasks);
    RUN_TEST(test_scheduler_high_priority_task);
    
    return UNITY_END();
} 