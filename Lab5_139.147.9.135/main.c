#include <stdio.h>
#include <string.h>
#include "bit_manip.h"
#include "file.h"
#include "game.h"

extern int current_room;

unsigned short rooms[8] = {
    0x1001,
    0x3123,
    0x4247,
    0x838F,
    0x0C5F,
    0X553F,
    0X667F,
    0X77FF
};

int main (int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <savefile>\n", argv[0]);
        return 1;
    }
    if (strcmp(argv[1], "file") == 0) {
        if (argc < 3) {
            printf("Usage: %s file <savefile>\n", argv[0]);
            return 1;
        }
        if (!load_rooms_from_file(argv[2])) {
            return 1;
        }
    } else if (strcmp(argv[1], "default") != 0) {
        printf("Unknown option: %s\n", argv[1]);
        return 1;
    }
    init_game();
    printf("\n Fish game, collect all four fish and bring them to the pond room"
           " to win!\n\n"
        "Fish names: Bass, Carp, Trout, Salmon\n");
    
    while (1) {
        display_status();

        int choice;
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n'); // clear input buffer
            continue;
        }
        unsigned short room = rooms[current_room];
        int accessible_rooms[8];
        int num_accessible = get_accessible_rooms(room, accessible_rooms);

        if (choice < num_accessible) {
            move_to_room(accessible_rooms[choice]);
        }
        else {
            int action = choice - num_accessible;
            int count = 0;
            for (int i = 0; i < 4; i++) {
                if (has_fish(i)) {
                    if (action == count) {
                        drop_fish(i);
                        goto done;
                    }
                    count++;
                }
            }
            for (int i = 0; i < 4; i++) {
                if (fish_in_room(room, i)) {
                    if (count == action) {
                        pickup_fish(i);
                        goto done;
                    }
                    count++;
                }
            }
            printf("Invalid choice. Please try again.\n");
        }
        done:
        if (check_win()) {
            printf("Congratulations! You've collected all the fish and brought them to the pond. You win!\n");
            break;
        }
    }
    return 0;
}