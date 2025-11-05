#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <stdio.h>

typedef enum {
    R0 = 0,
    R1 = 1,
    R2 = 2,
    R3 = 3,
    AC = 4,
    SP = 5,
    BP = 6,
    PC = 7,
    REGCNT = 8
} regnames;

void controller_init(unsigned short pc_start, unsigned short sp_start, unsigned short bp_start);

void controller_run_until_halt(FILE *log_file);

int controller_step(FILE *log_file);

int controller_is_halted(void);

unsigned short controller_get_register(regnames reg);

void controller_display(void);

#endif // CONTROLLER_H