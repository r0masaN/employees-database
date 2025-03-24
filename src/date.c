#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "../include/date.h"

inline date create_date(const uint8_t day, const uint8_t month, const uint16_t year) {
    const date d = {day, month, year};
    return d;
}

date parse_from_string(const char str[11]) {
    // TODO validation (how?)
    // dd.mm.yyyy format
    const date d = {strtol(str, NULL, 10),
                    strtol(str + 3, NULL, 10),
                    strtol(str + 6, NULL, 10)};
    return d;
}

inline void print_date(const date d) {
    printf("%02d.%02d.%04d", d.day, d.month, d.year);
}

// returns bit mask, like local const vars in employee_database.c
uint8_t compare_date(const date d1, const date d2) {
    // <
    if (d1.year < d2.year ||
        d1.year == d2.year && d1.month < d2.month ||
        d1.year == d2.year && d1.month == d2.month && d1.day < d2.day)
        return 0b10;

    // >
    if (d1.year > d2.year ||
        d1.year == d2.year && d1.month > d2.month ||
        d1.year == d2.year && d1.month == d2.month && d1.day > d2.day)
        return 0b1000;

    // ==
    if (d1.year == d2.year && d1.month == d2.month && d1.day == d2.day)
        return 0b10000;

    return 0b0;
}

// just full years, integer number
uint8_t calculate_age(const date birthday) {
    const time_t t = time(NULL);
    const struct tm tm = *localtime(&t);
    uint8_t age = (tm.tm_year + 1900 - 1) - birthday.year;
    if (tm.tm_mon + 1 > birthday.month || tm.tm_mon + 1 == birthday.month && tm.tm_mday >= birthday.day) age++;

    return age;
}

// is date of birth later than actual date
bool is_my_man_is_from_the_future(const date birthday) {
    const time_t t = time(NULL);
    const struct tm tm = *localtime(&t);

    return 0b1000 & compare_date(birthday, create_date(tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900));
}
