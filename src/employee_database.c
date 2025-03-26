#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "..\include\employee_database.h"

// bit masks for comparisons
static const uint8_t LESS_OR_EQUAL = 0b1, // <=
LESS = 0b10, // <
MORE_OR_EQUAL = 0b100, // >=
MORE = 0b1000, // >
EQUAL = 0b10000; // ==

employee_database* create_database(const size_t capacity, const size_t max_capacity) {
    employee_database* const db = malloc(sizeof(employee_database));
    if (!db) {
        printf("Error creating employees database of capacity %zu!\n", capacity);
        return NULL;
    }
    memset(db, 0, sizeof(employee_database)); // clean memory before using!

    employee* const new_table = calloc(capacity, sizeof(employee));
    if (!new_table) {
        memset(db, 0, sizeof(employee_database)); // clean memory after using!
        free(db);
        printf("Error creating employees database of capacity %zu!\n", capacity);
        return NULL;
    }
    memset(new_table, 0, capacity * sizeof(employee)); // clean memory before using!

    db->table = new_table;
    db->capacity = capacity;
    db->start_capacity = capacity;
    db->max_capacity = max_capacity;
    db->size = 0;

    printf("Employees database with capacity of %zu has been successfully created!\n", capacity);

    return db;
}

void free_database(employee_database* const db_ptr) {
    employee* const table = db_ptr->table;

    memset(table, 0, db_ptr->size * sizeof(employee)); // clean memory after using!
    free(table);
    memset(db_ptr, 0, sizeof(employee_database)); // clean memory after using!
    free(db_ptr);

    printf("Employees database has been successfully deleted!\n"); // moodle edition :D
}

// moodle simulator =)
void drop_database(employee_database* const db_ptr) {
    const size_t start_capacity = db_ptr->start_capacity;

    // first try to create a new table instead of just replacing old with it right away
    employee* const new_table = calloc(start_capacity, sizeof(employee));
    if (new_table) {
        memset(new_table, 0, start_capacity * sizeof(employee)); // clean memory before using!

        // bye-bye, old table!
        employee* old_table = db_ptr->table;
        memset(old_table, 0, db_ptr->size * sizeof(employee)); // clean memory after using!
        free(old_table);

        db_ptr->table = new_table;
        old_table = NULL; // just in case

        // full reset to factory settings
        db_ptr->capacity = start_capacity;
        db_ptr->size = 0;

        printf("Employees database has been successfully dropped!\n");

    } else {
        // oh no! 😨
        printf("Error dropping employees database!\n");
    }
}

void add_employee(employee_database* const db_ptr, const employee* const emp_ptr) {
    const size_t old_capacity = db_ptr->capacity, max_capacity = db_ptr->max_capacity, size = db_ptr->size;
    bool can_add = true; // by default, we can add a new employee, but...

    // if database already is full
    if (size == old_capacity) {
        // if we can increase it
        if (old_capacity < max_capacity) {
            const size_t new_capacity = (size_t) fmin((double) old_capacity * 1.5, (double) max_capacity);

            employee* const new_table = calloc(new_capacity, sizeof(employee));
            if (new_table) {
                memset(new_table, 0, new_capacity * sizeof(employee)); // clean memory before using!

                printf("Successfully increased capacity from %zu to %zu\n", old_capacity, new_capacity);
                // copy data from the old table to the new one
                employee* old_table = db_ptr->table;
                memcpy(new_table, old_table, size * sizeof(employee));

                memset(old_table, 0, size * sizeof(employee));  // clean memory after using!
                free(old_table);
                old_table = NULL; // just in case

                db_ptr->table = new_table;
                db_ptr->capacity = new_capacity;

            } else {
                can_add = false; // oops :(
                printf("Error increasing employees database\n");
            }

            // otherwise, suck 😋
        } else {
            can_add = false; // oops :(
            printf("Capacity limit is reached!\n");
        }
    }

    if (can_add) {
        // copy data from dynamic memory to... dynamic memory
        // (mb better have a dynamic array with pointers instead of just an array with data?..)
        // pointer to a database with pointer to a dynamic array full of pointers to actual employees
        // hmm...
        db_ptr->table[db_ptr->size++] = *emp_ptr;

        printf("Added: ");
        print_employee(emp_ptr);
    }
}

// just print actually, like others
void find_by_id(const employee_database* const db, const uint32_t id, const uint8_t mask) {
    // if mask idk from where, must be <=, <, >=, > or ==
    if (!(mask & (LESS_OR_EQUAL | LESS | MORE_OR_EQUAL | MORE | EQUAL))) {
        printf("Employees not found!\n");

    } else {
        const employee* const table = db->table;
        size_t counter = 0;

        for (size_t i = 0; i < db->size; i++) {
            const uint32_t emp_id = table[i].id;

            if (mask & LESS_OR_EQUAL && emp_id <= id ||
                mask & LESS && emp_id < id ||
                mask & MORE_OR_EQUAL && emp_id >= id ||
                mask & MORE && emp_id > id ||
                mask & EQUAL && emp_id == id) {

                counter++;
                printf("Employee found: ");
                print_employee(&table[i]);
            }
        }

        // if found 0 suitable employees
        if (!counter) printf("Employees not found!\n");
    }
}

void find_by_name(const employee_database* const db, const char name[]) {
    const employee* const table = db->table;
    size_t counter = 0;

    for (size_t i = 0; i < db->size; i++) {
        // if employee's name contains name passed by user in cli
        if (strstr(table[i].name, name)) {
            counter++;
            printf("Employee \"%s\" found: ", name);
            print_employee(&table[i]);
        }
    }

    if (!counter) printf("Employee \"%s\" not found!\n", name);
}

void find_by_birthday(const employee_database* const db, const date birthday, const uint8_t mask) {
    if (!(mask & (LESS_OR_EQUAL | LESS | MORE_OR_EQUAL | MORE | EQUAL))) {
        printf("Employees not found!\n");

    } else {
        const employee* const table = db->table;
        size_t counter = 0;

        for (size_t i = 0; i < db->size; i++) {
            // comparison mask (birthday from user and actual employee's one)
            const uint8_t m = compare_date(table[i].birthday, birthday);

            // if mask are equal (<, > or ==) or <= (< or ==) and >= (> or ==)
            if (mask & m ||
                mask & LESS_OR_EQUAL && (m & LESS || m & EQUAL) ||
                mask & MORE_OR_EQUAL && (m & MORE || m & EQUAL)) {

                counter++;
                printf("Employee found: ");
                print_employee(&table[i]);
            }
        }

        if (!counter) printf("Employees not found!\n");
    }
}

void find_by_age(const employee_database* const db, const uint8_t age, const uint8_t mask) {
    if (!(mask & (LESS_OR_EQUAL | LESS | MORE_OR_EQUAL | MORE | EQUAL))) {
        printf("Employees not found!\n");

    } else {
        const employee* const table = db->table;
        size_t counter = 0;

        for (size_t i = 0; i < db->size; i++) {
            const date emp_birthday = table[i].birthday;
            const uint8_t emp_age = calculate_age(emp_birthday);

            if (!is_my_man_is_from_the_future(emp_birthday) &&
                (mask & LESS_OR_EQUAL && emp_age <= age ||
                 mask & LESS && emp_age < age ||
                 mask & MORE_OR_EQUAL && emp_age >= age ||
                 mask & MORE && emp_age > age ||
                 mask & EQUAL && emp_age == age)) {

                counter++;
                printf("Employee found: ");
                print_employee(&table[i]);
            }
        }

        if (!counter) printf("Employees not found!\n");
    }
}

void find_by_salary(const employee_database* const db, const float salary, const uint8_t mask) {
    if (!(mask & (LESS_OR_EQUAL | LESS | MORE_OR_EQUAL | MORE | EQUAL))) {
        printf("Employees not found!\n");

    } else {
        const employee* const table = db->table;
        size_t counter = 0;

        for (size_t i = 0; i < db->size; i++) {
            const float emp_salary = table[i].salary;

            if (mask & LESS_OR_EQUAL && emp_salary <= salary ||
                mask & LESS && emp_salary < salary ||
                mask & MORE_OR_EQUAL && emp_salary >= salary ||
                mask & MORE && emp_salary > salary ||
                mask & EQUAL && emp_salary == salary) {

                counter++;
                printf("Employee found: ");
                print_employee(&table[i]);
            }
        }

        if (!counter) printf("Employees not found!\n");
    }
}

void print_database(const employee_database* const db) {
    const size_t db_size = db->size;
    if (db_size == 0) {
        printf("Employees database is empty.\n");

    } else {
        printf("Employees database (%zu records):\n", db_size);
        const employee* table = db->table;

        for (size_t i = 0; i < db_size; i++) print_employee(&table[i]);
    }
}
