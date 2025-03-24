#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include "date.h"

typedef struct {
    uint32_t id;
    char name[51];
    date birthday;
    float salary;
} employee;

employee* create_employee(uint32_t id, const char* name, date birthday, float salary);

// fired :D
void free_employee(employee* emp_ptr);

void print_employee(const employee* emp_ptr);

#endif
