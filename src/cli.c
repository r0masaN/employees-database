#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "..\include\cli.h"
#include "..\include\employee_database.h"

extern employee_database* db;
extern bool log_mode;

static void process_command_quit() {
    printf("Are you sure? [y/n]\n");
    char answer[2];
    bool is_answered = false;

    while (!is_answered) {
        scanf("%s", answer);

        if (strncmp(answer, "y", 1) == 0) {
            if (log_mode) printf("Shutting down...\n");
            exit(0);

        } else if (strncmp(answer, "n", 1) == 0) {
            is_answered = true;
            if (log_mode) printf("Shutting down is canceled.\n");

        } else {
            if (log_mode) printf("Please, answer!\n");
        }
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
}

static inline void process_command_show() {
    print_database(db);
}

static void process_command_add(const char command[]) {
    char name[51] = {0};
    uint8_t day = 0, month = 0;
    uint16_t year = 0;
    float salary = 0.0f;

    const size_t params_count = sscanf(command, "add \"%50[^\"]\" %hhu.%hhu.%hu %f", name, &day, &month, &year, &salary);

    if (params_count < 5) {
        printf("Incorrect \"add\" usage!\n");
        return;
    }

    const date birthday = {day, month, year};
    const employee* const emp = create_employee(db->size, name, birthday, salary);
    add_employee(db, emp);
}

static const char* get_id_from_line(const char line[]) {
    const size_t length = (size_t) fmin((double) strlen(line), 10);
    char* const id = malloc(length + 1);
    strncpy(id, line, length);
    id[length] = '\0';
    return id;
}

static void process_command_find_by_id(const char command[]) {
    const char* const ptr1 = strstr(command, "<=");
    const char* const ptr2 = strstr(command, "<");
    const char* const ptr3 = strstr(command, ">=");
    const char* const ptr4 = strstr(command, ">");
    const char* const ptr5 = strstr(command, "=");
    const char* id = NULL;
    uint8_t mask = 0b0;

    if (ptr1) {
        id = get_id_from_line(ptr1 + 2);
        mask = 0b1;
    } else if (ptr2) {
        id = get_id_from_line(ptr2 + 1);
        mask = 0b10;
    } else if (ptr3) {
        id = get_id_from_line(ptr3 + 2);
        mask = 0b100;
    } else if (ptr4) {
        id = get_id_from_line(ptr4 + 1);
        mask = 0b1000;
    } else if (ptr5) {
        id = get_id_from_line(ptr5 + 1);
        mask = 0b10000;
    }

    find_by_id(db, (size_t) strtof(id, NULL), mask);
}

static void process_command_find_by_name(const char command[]) {
    const char* const ptr1 = strstr(command, "\"");
    const char* const ptr2 = strstr(ptr1 + 1, "\"");
    char name[51];
    strncpy(name, ptr1 + 1, ptr2 - ptr1 - 1);
    name[ptr2 - ptr1 - 1] = '\0';

    find_by_name(db, name);
}

static const char* get_birthday_from_line(const char line[]) {
    char* const birthday = malloc(11);
    strncpy(birthday, line, 10);
    birthday[10] = '\0';
    return birthday;
}

static void process_command_find_by_birthday(const char command[]) {
    const char* const ptr1 = strstr(command, "<=");
    const char* const ptr2 = strstr(command, "<");
    const char* const ptr3 = strstr(command, ">=");
    const char* const ptr4 = strstr(command, ">");
    const char* const ptr5 = strstr(command, "=");
    const char* birthday = NULL;
    uint8_t mask = 0b0;

    if (ptr1) {
        birthday = get_birthday_from_line(ptr1 + 2);
        mask = 0b1;
    } else if (ptr2) {
        birthday = get_birthday_from_line(ptr2 + 1);
        mask = 0b10;
    } else if (ptr3) {
        birthday = get_birthday_from_line(ptr3 + 2);
        mask = 0b100;
    } else if (ptr4) {
        birthday = get_birthday_from_line(ptr4 + 1);
        mask = 0b1000;
    } else if (ptr5) {
        birthday = get_birthday_from_line(ptr5 + 1);
        mask = 0b10000;
    }

    find_by_birthday(db, parse_from_string(birthday), mask);
}

static const char* get_age_from_line(const char line[]) {
    const size_t length = (size_t) fmin((double) strlen(line), 4);
    char* const age = malloc(length + 1);
    strncpy(age, line, length);
    age[length] = '\0';
    return age;
}

static void process_command_find_by_age(const char command[]) {
    const char* const ptr1 = strstr(command, "<=");
    const char* const ptr2 = strstr(command, "<");
    const char* const ptr3 = strstr(command, ">=");
    const char* const ptr4 = strstr(command, ">");
    const char* const ptr5 = strstr(command, "=");
    const char* age = NULL;
    uint8_t mask = 0b0;

    if (ptr1) {
        age = get_age_from_line(ptr1 + 2);
        mask = 0b1;
    } else if (ptr2) {
        age = get_age_from_line(ptr2 + 1);
        mask = 0b10;
    } else if (ptr3) {
        age = get_age_from_line(ptr3 + 2);
        mask = 0b100;
    } else if (ptr4) {
        age = get_age_from_line(ptr4 + 1);
        mask = 0b1000;
    } else if (ptr5) {
        age = get_age_from_line(ptr5 + 1);
        mask = 0b10000;
    }

    find_by_age(db, (uint8_t) strtof(age, NULL), mask);
}

static const char* get_salary_from_line(const char line[]) {
    const size_t length = (size_t) fmin((double) strlen(line), 15);
    char* const salary = malloc(length + 1);
    strncpy(salary, line, length);
    salary[length] = '\0';
    return salary;
}

static void process_command_find_by_salary(const char command[]) {
    const char* const ptr1 = strstr(command, "<=");
    const char* const ptr2 = strstr(command, "<");
    const char* const ptr3 = strstr(command, ">=");
    const char* const ptr4 = strstr(command, ">");
    const char* const ptr5 = strstr(command, "=");
    const char* salary = NULL;
    uint8_t mask = 0b0;

    if (ptr1) {
        salary = get_salary_from_line(ptr1 + 2);
        mask = 0b1;
    } else if (ptr2) {
        salary = get_salary_from_line(ptr2 + 1);
        mask = 0b10;
    } else if (ptr3) {
        salary = get_salary_from_line(ptr3 + 2);
        mask = 0b100;
    } else if (ptr4) {
        salary = get_salary_from_line(ptr4 + 1);
        mask = 0b1000;
    } else if (ptr5) {
        salary = get_salary_from_line(ptr5 + 1);
        mask = 0b10000;
    }


    find_by_salary(db, strtof(salary, NULL), mask);
}

static void process_command_drop() {
    printf("Are you sure? [y/n]\n");
    char answer[2];
    bool is_answered = false;

    while (!is_answered) {
        scanf("%s", answer);

        if (strncmp(answer, "y", 1) == 0) {
            is_answered = true;
            if (log_mode) printf("Dropping database...\n");
            drop_database(db);

        } else if (strncmp(answer, "n", 1) == 0) {
            is_answered = true;
            if (log_mode) printf("Dropping database is canceled.\n");

        } else {
            if (log_mode) printf("Please, answer!\n");
        }
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

    } else if (strncmp(command, "drop", 4) == 0) {
        process_command_drop();
    } else {
        process_unknown_command();
    }
}
