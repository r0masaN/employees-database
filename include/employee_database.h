#ifndef EMPLOYEE_DATABASE_H
#define EMPLOYEE_DATABASE_H

#include "employee.h"

// dynamic array, literally
// it can resize if doesn't have enough space
typedef struct {
    employee* table;
    size_t capacity, start_capacity, max_capacity, size;
} employee_database;

employee_database* create_database(size_t capacity, size_t max_capacity);

void free_database(employee_database* db_ptr);

// :D
void drop_database(employee_database* db_ptr);

void add_employee(employee_database* db_ptr, employee* emp_ptr);

// just print actually, like others
void find_by_id(employee_database* db, size_t id, uint8_t mask);

void find_by_name(employee_database* db, const char* name);

void find_by_birthday(employee_database* db, date birthday, uint8_t mask);

void find_by_age(employee_database* db, uint8_t age, uint8_t mask);

void find_by_salary(employee_database* db, float salary, uint8_t mask);

void print_database(const employee_database* db);

#endif
