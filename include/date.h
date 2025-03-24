#ifndef DATE_H
#define DATE_H

#include <stdint.h>

typedef struct {
    uint8_t day, month;
    uint16_t year;
} date;

date create_date(uint8_t day, uint8_t month, uint16_t year);

date parse_from_string(const char str[11]);

void print_date(date d);

// returns bit mask, like local const vars in employee_database.c
uint8_t compare_date(date d1, date d2);

// just full years, integer number
uint8_t calculate_age(date birthday);

// is date of birth later than actual date
bool is_my_man_is_from_the_future(date birthday);

#endif
