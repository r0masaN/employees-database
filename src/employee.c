#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "..\include\employee.h"

employee *create_employee(const uint32_t id, const char name[], const date birthday, const float salary) {
    employee *const emp = malloc(sizeof(employee));

    emp->id = id;
    strncpy(emp->name, name, 50);
    emp->birthday = birthday;
    emp->salary = salary;

    return emp;
}

// fired :D
inline void free_employee(employee *const emp_ptr) {
    memset(emp_ptr, 0, sizeof(employee));
    free(emp_ptr);
}

void print_employee(const employee *const emp_ptr) {
    const employee emp = *emp_ptr;
    printf("id: %d, name: %s, birthday: ", emp.id, emp.name);
    print_date(emp.birthday);
    printf(", salary: %.2f$\n", emp.salary);
}
