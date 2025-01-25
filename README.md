# C++ Task Scheduler

## Overview
The C++ Task Scheduler is a lightweight, clock-driven task scheduling library designed for embedded systems. It allows users to manage tasks efficiently with features such as prioritization, delayed execution, and task state management.

## ToDo
- **Error Handler** Directory and subclass  will be added
- Add Unity C tests

## Features
- **High Priority Tasks**: Temporarily elevate the priority of a task using external interrupts.
- **Delayed Task Execution**: Start tasks after a specified delay.
- **System Time Analysis**: Analyze the time taken to execute all tasks.
- **Task Management**: Add, remove, pause, resume, and kill tasks.
- **Error Handling**: Manage errors related to maximum task limits and long execution times.

## Project Structure
```
cpp-task-scheduler
├── include
│   ├── TaskScheduler.h
│   └── Task.h
├── src
│   ├── TaskScheduler.cpp
│   └── Task.cpp
├── tests
│   ├── TaskSchedulerTest.cpp
│   └── TaskTest.cpp
├── CMakeLists.txt
└── README.md
```

## Setup Instructions
1. Clone the repository:
   ```
   git clone <repository-url>
   ```
2. Navigate to the project directory:
   ```
   cd cpp-task-scheduler
   ```
3. Create a build directory:
   ```
   mkdir build && cd build
   ```
4. Run CMake to configure the project:
   ```
   cmake ..
   ```
5. Build the project:
   ```
   make
   ```

## Usage Example
To use the Task Scheduler, include the header file in your project:
```cpp
#include "TaskScheduler.h"
```
Create an instance of `TaskScheduler` and manage your tasks as needed.

## Testing
Unit tests are provided for both the `TaskScheduler` and `Task` classes. To run the tests, ensure you have built the project and then execute the test binaries located in the build directory.

## License
This project is licensed under the MIT License. See the LICENSE file for more details.