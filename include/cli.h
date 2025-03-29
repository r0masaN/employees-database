#ifndef CLI_H
#define CLI_H

#include <stdint.h>

typedef struct {
    const char* ptr;
    const uint8_t mask;
} ptr_and_mask;

void process_command(const char command[]);

#endif
