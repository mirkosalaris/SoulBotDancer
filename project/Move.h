#ifndef MOVE
#define MOVE

#include "Commons.h"

void beat_action(unsigned long int beat_time, int s_speed);

void no_music_action();

int get_speed(unsigned long int distance);

float get_user_distance();

void emmit_sound(int pin_umber);

void move_arms_up_down();

void move_body_left_right();

#endif
