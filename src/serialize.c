#include <stdlib.h>
#include <memory.h>
#include <string.h>

#include "..\include\serialize.h"

// TODO doesn't work correctly
static int create_folder_if_needed(const char *folder_name) {
    FILE *const restrict folder = fopen(folder_name, "r");
    if (!folder) { // folder doesn't exist so we need to create it

        FILE *created_folder = fopen(folder_name, "w");
        if (!created_folder) {
            fprintf(stderr, "Error creating folder \"%s\"!\n", folder_name);
            return 1;
        }

        fclose(created_folder);
    }

    fclose(folder);

    return 0;
}

int serialize(const char *const restrict filename, const employee_database *const restrict db_ptr) {
    // TODO fix
//    if (create_folder_if_needed("saves\\") != 0) return 1;
//
//    const size_t length = strlen(filename) + 6;
//    char file_path[length + 1] = {}; // + 1 symbol for '\0'
//    sprintf(file_path, "saves\\%s", filename); // actual path "saves\filename"
//    file_path[length] = '\0';

    // trying to open file
    FILE *const restrict file = fopen(filename, "wb");
    if (!file) { // failure
        fprintf(stderr, "Error opening file \"%s\" for writing!\n", filename);
        return 1;
    }

    // serialization
    fwrite(&db_ptr->capacity, sizeof(db_ptr->capacity), 1, file);
    fwrite(&db_ptr->start_capacity, sizeof(db_ptr->start_capacity), 1, file);
    fwrite(&db_ptr->max_capacity, sizeof(db_ptr->max_capacity), 1, file);
    fwrite(&db_ptr->size, sizeof(db_ptr->size), 1, file);

    employee *emp_ptr;
    for (size_t i = 0; i < db_ptr->size; i++) {
        emp_ptr = db_ptr->table + i;

        // serialization
        fwrite(&emp_ptr->id, sizeof(emp_ptr->id), 1, file);
        fwrite(emp_ptr->name, sizeof(emp_ptr->name), 1, file);
        fwrite(&emp_ptr->birthday, sizeof(emp_ptr->birthday), 1, file);
        fwrite(&emp_ptr->salary, sizeof(emp_ptr->salary), 1, file);
    }

    printf("Database has been successfully serialized into file \"%s\"!\n", filename);
    fclose(file); // important!

    return 0;
}

int deserialize(const char *const restrict filename, employee_database *const restrict db_ptr) {
    // TODO fix
//    if (create_folder_if_needed("saves\\") != 0) return 1;
//
//    const size_t length = strlen(filename) + 6;
//    char file_path[length + 1] = {}; // + 1 symbol for '\0'
//    sprintf(file_path, "saves\\%s", filename); // actual path "saves\filename"
//    file_path[length] = '\0';

    // trying to open file
    FILE *const restrict file = fopen(filename, "rb");
    if (!file) { // failure
        fprintf(stderr, "Error opening file \"%s\" for reading!\n", filename);
        return 1;
    }

    // deserialization
    fread(&db_ptr->capacity, sizeof(db_ptr->capacity), 1, file);
    fread(&db_ptr->start_capacity, sizeof(db_ptr->start_capacity), 1, file);
    fread(&db_ptr->max_capacity, sizeof(db_ptr->max_capacity), 1, file);
    fread(&db_ptr->size, sizeof(db_ptr->size), 1, file);

    // creating new table with needed capacity
    employee *const new_table = calloc(db_ptr->capacity, sizeof(employee));
    if (!new_table) {
        printf("Error creating employees database of capacity %zu!\n", db_ptr->capacity);
        return 1;
    }
    memset(new_table, 0, db_ptr->capacity * sizeof(employee)); // clean memory before using!

    // full destroying old table that we don't need anymore
    memset(db_ptr->table, 0, db_ptr->capacity * sizeof(employee)); // clean old table before freeing
    free(db_ptr->table);
    db_ptr->table = new_table;

    employee *emp_ptr;
    for (size_t i = 0; i < db_ptr->size; i++) {
        emp_ptr = db_ptr->table + i;

        // deserialization
        fread(&emp_ptr->id, sizeof(emp_ptr->id), 1, file);
        fread(emp_ptr->name, sizeof(emp_ptr->name), 1, file);
        fread(&emp_ptr->birthday, sizeof(emp_ptr->birthday), 1, file);
        fread(&emp_ptr->salary, sizeof(emp_ptr->salary), 1, file);
    }

    printf("Database has been successfully deserialized from file \"%s\"!\n", filename);
    fclose(file); // important!

    return 0;
}
