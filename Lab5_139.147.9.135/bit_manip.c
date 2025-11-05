#include "bit_manip.h"
#include <stdio.h>

int get_room_number(unsigned short room) {
    return (room >> 8) & 0xF;
}

int can_access_room(unsigned short room, int target) {
    return (room >> target) & 1;
}

int is_pond(unsigned short room) {
    return (room >> 11) & 1;
}

int has_fish(unsigned short room, int fish_id) {
    return (room >> (12 + fish_id)) & 1;
}

void add_fish (int room_index, int fish_id) {
    extern unsigned short rooms[];
    rooms[room_index] |= (1 << (12 + fish_id));
}

void remove_fish (int room_index, int fish_id) {
    extern unsigned short rooms[];
    rooms[room_index] &= ~(1 << (12 + fish_id));
}

int get_accessible_rooms(unsigned short room, int* accessible) {
    int count = 0;
    for (int i = 0; i < 8; i++) {
        if (can_access_room(room, i)) {
            accessible[count++] = i;
        }
    }
    return count;
}

void print_binary(unsigned short value) {
    for (int i = 15; i >= 0; i--) {
        putchar((value & (1 << i)) ? '1' : '0');
        if (i % 4 == 0) putchar(' ');
    }
    putchar('\n');
}