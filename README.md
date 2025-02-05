# C++ Task Scheduler

## Overview
The C++ Task Scheduler is a lightweight, clock-driven task scheduling library designed for embedded systems. It allows users to manage tasks efficiently with features such as prioritization, delayed execution, and task state management.

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
│   ├── CMakeLists.txt
│   ├── test_task.cpp
│   ├── test_taskscheduler.cpp
│   └── Unity
│       ├── src
│       │   ├── unity.c
│       │   ├── unity.h
│       │   └── unity_internals.h
├── CMakeLists.txt
├── README.md
└── LICENSE

```

## Setup Instructions
1. Clone the repository:
   ```
   git clone <repository-url>
   ```
2. Navigate to the project directory:
   ```
   cd TaskScheduler
   ```
3. Create a build directory and navigate to it:
   ```
   mkdir build && cd build
   ```
4. Configure and build the project:
   ```
   cmake ..
   cmake --build . --config Debug
   ```

## Testing
The project uses Unity for unit testing. To run the tests:
```
cd build
ctest -C Debug --output-on-failure
```

Or to run specific test:
```
./tests/test_task
./tests/test_taskscheduler
```

## Usage Example
To use the Task Scheduler, include the header file in your project:
```cpp
#include "TaskScheduler.h"
```
Create an instance of `TaskScheduler` and manage your tasks as needed.

## License
This project is licensed under the MIT License. See the LICENSE file for more details.