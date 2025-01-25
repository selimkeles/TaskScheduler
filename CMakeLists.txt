project(cpp-task-scheduler)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)

file(GLOB SOURCES
    src/*.cpp
)

add_library(TaskScheduler ${SOURCES})

enable_testing()

file(GLOB TEST_SOURCES
    tests/*.cpp
)

add_executable(TaskSchedulerTest ${TEST_SOURCES})
target_link_libraries(TaskSchedulerTest TaskScheduler)

add_test(NAME TaskSchedulerTest COMMAND TaskSchedulerTest)