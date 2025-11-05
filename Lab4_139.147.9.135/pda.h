// For the implementation of three different PDA machines.
//
// Modified by <your-name-here>
//

#ifndef LAB02_PDA_H
#define LAB02_PDA_H

#include "stddef.h"
#include <string.h>
#include "stack.h"
#include "list.h"

enum state {
    S0 = 0,
    S1 = 1,
    S2 = 2,
    S3 = 3,
};

int PDA1 (char *str);
int PDA2 (char *str);
int PDA3 (char *str);

#endif //LAB02_PDA_H
