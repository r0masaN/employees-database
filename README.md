# Employees Database

A simple employee database management system written in C. Currently, the database exists only in memory during execution. Future updates will include serialization to a file for persistent storage.

## Features

- Add new employees
- Search for employees by various criteria
- Display all employees

## Upcoming Features

- Update employee information
- Delete employees

## Build and Run

### Prerequisites

- CMake (version 3.28.1 or higher)
- GCC or compatible C compiler

### Instructions

1. Clone the repository:

    ```bash
    git clone https://github.com/r0masaN/employees-database.git
    cd employees-database
    ```

2. Build the project:

    ```bash
    cmake -B cmake-build-debug
    cmake --build cmake-build-debug
    ```

3. Run the executable:

    ```bash
    ./cmake-build-debug/employees_database
    ```

## Future Improvements

- Persistent storage (save/load from file)
- Enhanced search capabilities
- Command-line arguments support
