cmake_minimum_required(VERSION 3.14)
project(TaskScheduler)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable testing and CTest
include(CTest)
enable_testing()

# Add include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Create the main library
set(LIB_NAME "TaskScheduler")

add_library(${LIB_NAME} STATIC
    src/Task.cpp
    src/TaskScheduler.cpp
)

target_include_directories(${LIB_NAME} 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Only build tests if this is the main project
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    add_subdirectory(tests)
endif()