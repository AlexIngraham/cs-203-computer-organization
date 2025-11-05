#ifndef BIT_MANIP_H
#define BIT_MANIP_H

extern unsigned short rooms[8];

int get_room_number(unsigned short room);
int can_access_room(unsigned short room, int target);
int is_pond(unsigned short room);
int has_fish(unsigned short room, int fish_id);
void add_fish(int room_index, int fish_id);
void remove_fish(int room_index, int fish_id);
int get_accessible_rooms(unsigned short room, int* accessible);
void print_binary(unsigned short value);

#endif