#ifndef GAME_H
#define GAME_H

extern int current_room;
extern int player_fish;

void init_game();
void display_room();
void move_to_room(int room);
void pickup_fish(int fish_id);
void drop_fish(int fish_id);
int has_fish(int room, int fish_id);
int check_win();
int player_has_fish(int fish_id);
const char* get_fish_name(int fish_id);


#endif