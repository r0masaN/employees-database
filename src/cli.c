#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "..\include\cli.h"
#include "..\include\employee_database.h"

extern employee_database* db_ptr;
extern bool log_mode;

// to confirm important actions
static bool are_you_sure() {
    printf("Are you sure? [y/n]\n");
    char line[81] = {0};
    char first_word[11] = {0};

    while (true) {
        fgets(line, 81, stdin);
        line[strlen(line) - 1] = '\0';
        sscanf(line, "%10s", first_word);

        if (strncmp(first_word, "y", 10) == 0) {
            return true;
        } else if (strncmp(first_word, "n", 10) == 0) {
            return false;
        } else {
            if (log_mode) printf("Please, answer!\n");
        }
    }
}

static void process_command_quit() {
    if (are_you_sure()) {
        if (log_mode) printf("Shutting down...\n");
        exit(0);
    } else if (log_mode) {
        printf("Shutting down is canceled.\n");
    }
}

static void process_command_log() {
    if (!log_mode) {
        log_mode = true;
        printf("Log mode is enabled.\n");
    } else {
        printf("Log mode is already enabled.\n");
    }
}

static void process_command_nolog() {
    if (log_mode) {
        log_mode = false;
        printf("Log mode is disabled.\n");
    } else {
        printf("Log mode is already disabled.\n");
    }
}

static void process_command_help() {
    printf("help - shows all possible commands;\n");
    printf("log - enables log messages;\n");
    printf("nolog - disables log messages;\n");
    printf("show - shows all database records;\n");
    printf("quit - turns off system;\n");
    printf("drop - drops database out;\n");
    printf("add \"[name]\" [dd.mm.yyyy] [salary] - adds an employee with these name, birthday and salary;\n");
    printf("find name=[value] - finds an employees which names contains given string;\n");
    printf("find [id/birthday/age/salary][<=/</>=/=][value] - finds an employees satisfying given condition.\n");
    printf("delete id=[id] - deletes an employee with given id.\n");
}

static inline void process_command_show() {
    print_database(db_ptr);
}

static void process_command_add(const char command[]) {
    char name[51] = {0};
    uint8_t day = 0, month = 0;
    uint16_t year = 0;
    float salary = 0.0f;

    const size_t params_count = sscanf(command, "add \"%50[^\"]\" %hhu.%hhu.%hu %f",
                                       name, &day, &month, &year, &salary);
    if (params_count != 5) {
        printf("Incorrect \"add\" usage!\n");
        return;
    }

    const date birthday = {day, month, year};
    const uint32_t id = db_ptr->size == 0 ? 0 : db_ptr->table[db_ptr->size - 1].id + 1;
    const employee* const emp_ptr = create_employee(id, name, birthday, salary);
    add_employee(db_ptr, emp_ptr);
}

static ptr_and_mask get_ptr_and_mask(const char command[]) {
    const char* const ptr1 = strstr(command, "<=");
    const char* const ptr2 = strstr(command, "<");
    const char* const ptr3 = strstr(command, ">=");
    const char* const ptr4 = strstr(command, ">");
    const char* const ptr5 = strstr(command, "=");
    uint8_t mask = 0b0;
    const char* f_ptr = NULL;

    if (ptr1) {
        f_ptr = ptr1 + 2;
        mask = 0b1;
    } else if (ptr2) {
        f_ptr = ptr2 + 1;
        mask = 0b10;
    } else if (ptr3) {
        f_ptr = ptr3 + 2;
        mask = 0b100;
    } else if (ptr4) {
        f_ptr = ptr4 + 1;
        mask = 0b1000;
    } else if (ptr5) {
        f_ptr = ptr5 + 1;
        mask = 0b10000;
    }

    const ptr_and_mask res = {.ptr = f_ptr, .mask = mask};
    return res;
}

static void process_command_find_by_id(const char command[]) {
    const ptr_and_mask v = get_ptr_and_mask(command);

    uint32_t id = 0;
    const size_t params_count = sscanf(v.ptr, "%u", &id);

    if (params_count != 1) {
        printf("Incorrect \"find id\" usage!\n");
        return;
    }

    find_by_id(db_ptr, id, v.mask);
}

static void process_command_find_by_name(const char command[]) {
    char name[51] = {0};
    const size_t params_count = sscanf(command, "find name=\"%50[^\"]\"", name);

    if (params_count != 1) {
        printf("Incorrect \"find name\" usage!\n");
        return;
    }

    find_by_name(db_ptr, name);
}

static void process_command_find_by_birthday(const char command[]) {
    const ptr_and_mask v = get_ptr_and_mask(command);

    date birthday = {0, 0, 0};
    const size_t params_count = sscanf(v.ptr, "%hhu.%hhu.%hu", &birthday.day, &birthday.month, &birthday.year);

    if (params_count != 3) {
        printf("Incorrect \"find birthday\" usage!\n");
        return;
    }

    find_by_birthday(db_ptr, birthday, v.mask);
}

static void process_command_find_by_age(const char command[]) {
    const ptr_and_mask v = get_ptr_and_mask(command);

    uint8_t age = 0;
    const size_t params_count = sscanf(v.ptr, "%hhu", &age);

    if (params_count != 1) {
        printf("Incorrect \"find age\" usage!\n");
        return;
    }

    find_by_age(db_ptr, age, v.mask);
}

static void process_command_find_by_salary(const char command[]) {
    const ptr_and_mask v = get_ptr_and_mask(command);

    float salary = 0.0f;
    const size_t params_count = sscanf(v.ptr, "%f", &salary);

    if (params_count != 1) {
        printf("Incorrect \"find salary\" usage!\n");
        return;
    }

    find_by_salary(db_ptr, salary, v.mask);
}

static void process_command_delete_by_id(const char command[]) {
    uint32_t id = 0;
    const size_t params_count = sscanf(command, "delete id=%u", &id);

    if (params_count != 1) {
        printf("Incorrect \"delete\" usage!\n");
        return;
    }

    if (are_you_sure()) {
        if (log_mode) printf("Deleting employee...\n");
        delete_by_id(db_ptr, id);
    } else if (log_mode) {
        printf("Deleting employee is canceled.\n");
    }
}

static void process_command_drop() {
    if (are_you_sure()) {
        if (log_mode) printf("Dropping database...\n");
        drop_database(db_ptr);
    } else if (log_mode) {
        printf("Dropping database is canceled.\n");
    }
}

static inline void process_unknown_command() {
    if (log_mode) printf("Unknown command. Try \"help\" to see all possible commands.\n");
}

void process_command(const char command[]) {
    if (strncmp(command, "quit", 4) == 0) {
        process_command_quit();
    } else if (strncmp(command, "nolog", 5) == 0) {
        process_command_nolog();
    } else if (strncmp(command, "log", 3) == 0) {
        process_command_log();
    } else if (strncmp(command, "help", 4) == 0) {
        process_command_help();
    } else if (strncmp(command, "show", 4) == 0) {
        process_command_show();

    } else if (strstr(command, "add")) {
        process_command_add(command);
    } else if (strstr(command, "find id")) {
        process_command_find_by_id(command);
    } else if (strstr(command, "find name=")) {
        process_command_find_by_name(command);
    } else if (strstr(command, "find birthday")) {
        process_command_find_by_birthday(command);
    } else if (strstr(command, "find age")) {
        process_command_find_by_age(command);
    } else if (strstr(command, "find salary")) {
        process_command_find_by_salary(command);
    } else if (strstr(command, "delete id=")) {
        process_command_delete_by_id(command);

    } else if (strncmp(command, "drop", 4) == 0) {
        process_command_drop();
    } else {
        process_unknown_command();
    }
}
