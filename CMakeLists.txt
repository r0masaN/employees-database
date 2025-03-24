cmake_minimum_required(VERSION 3.28)
project(employees_database C)

set(CMAKE_C_STANDARD 23)

add_executable(employees_database main.c
        include/employee.h
        include/employee_database.h
        include/date.h
        src/date.c
        src/employee.c
        src/employee_database.c
        include/cli.h
        src/cli.c)
