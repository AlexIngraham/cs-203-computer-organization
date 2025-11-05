#include "file.h"
#include <stdio.h>
#include "bit_manip.h"

unsigned short rooms[8];

int load_rooms_from_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open file %s for reading.\n", filename);
        return 0;
    }
    for (int i = 0; i < 8; i++) {
        if (fscan(file, "%hx", &rooms[i]) != 1) {
            printf("Error: Could not read room data from file %s.\n", i);
            fclose(file);
            return 0;
        }
    }
    fclose(file);
    return 1;
}