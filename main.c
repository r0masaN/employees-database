#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "include\cli.h"
#include "include\employee_database.h"

bool log_mode;
employee_database* db_ptr;

int main() {
    log_mode = true;
    db_ptr = create_database(5, 100);

    if (db_ptr) {
        while (1) {
            char command[81];
            fgets(command, 81, stdin);
            command[strlen(command) - 1] = '\0'; // cuts off unnecessary '\n' at the end of the line

            if (strlen(command) > 0) process_command(command);
            printf("\n");
        }
    }

    return 0;
}
