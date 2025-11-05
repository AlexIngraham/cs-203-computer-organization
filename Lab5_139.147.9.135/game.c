#include "game.h"
#include "bit_manip.h"
#include <stdio.h>

extern unsigned short rooms[];
int current_room = 0;
int player_fish = 0;

void init_game() {
    current_room = 0;
    player_fish = 0;
}

const char* get_fish_name(int fish_id) {
    switch(fish_id) {
        case 0: return "one fish";
        case 1: return "two fish";
        case 2: return "red fish";
        case 3: return "blue fish";
        default: return "unknown fish";
    }
}

int player_has_fish(int fish_id) {
    return (player_fish >> fish_id) & 1;
}

void display_room() {
    unsigned short room = rooms[current_room];
    int accessible[8];
    int count = get_accessible_rooms(room, accessible);
    
    printf("Room Number: %d\n", current_room);
    
    if (is_pond(room)) {
        printf("*** THIS IS THE POND ROOM ***\n");
    }
    
    printf("\nRoom Movement Selection:\n");
    for (int i = 0; i < count; i++) {
        if (accessible[i] == current_room) {
            printf("%d - stay in room %d\n", i, accessible[i]);
        } else {
            printf("%d - move to room %d\n", i, accessible[i]);
        }
    }
    
    printf("\nFish you have: ");
    int has_any = 0;
    for (int i = 0; i < 4; i++) {
        if (player_has_fish(i)) {
            if (has_any) printf(", ");
            printf("%s", get_fish_name(i));
            has_any = 1;
        }
    }
    if (!has_any) printf("none");
    printf("\n");
    
    printf("Available Fish: ");
    has_any = 0;
    for (int i = 0; i < 4; i++) {
        if (has_fish(room, i)) {
            if (has_any) printf(", ");
            printf("%s", get_fish_name(i));
            has_any = 1;
        }
    }
    if (!has_any) printf("none");
    printf("\n");
    
    printf("\nFish Options:\n");
    int option = count;
    for (int i = 0; i < 4; i++) {
        if (player_has_fish(i)) {
            printf("%d - put down %s\n", option++, get_fish_name(i));
        }
    }
    for (int i = 0; i < 4; i++) {
        if (has_fish(room, i)) {
            printf("%d - pick up %s\n", option++, get_fish_name(i));
        }
    }
    
    printf("\nEnter your choice: ");
}

void move_to_room(int target) {
    current_room = target;
    printf("Moving to room %d...\n", target);
}

void pickup_fish(int fish_id) {
    player_fish |= (1 << fish_id);
    remove_fish(current_room, fish_id);
    printf("Picked up %s!\n", get_fish_name(fish_id));
}

void drop_fish(int fish_id) {
    player_fish &= ~(1 << fish_id);
    add_fish(current_room, fish_id);
    printf("Put down %s!\n", get_fish_name(fish_id));
}

int check_win() {
    for (int i = 0; i < 8; i++) {
        if (is_pond(rooms[i])) {
            int all_fish = 1;
            for (int f = 0; f < 4; f++) {
                if (!has_fish(rooms[i], f)) {
                    all_fish = 0;
                    break;
                }
            }
            return all_fish;
        }
    }
    return 0;
}
