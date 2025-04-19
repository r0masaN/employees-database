#ifndef SERIALIZE_H
#define SERIALIZE_H

#include <stdio.h>

#include "employee_database.h."

int serialize(const char *restrict filename, const employee_database *restrict db_ptr);

int deserialize(const char *restrict filename, employee_database *restrict db_ptr);

#endif
