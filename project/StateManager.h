#ifndef STATEMANAGER
#define STATEMANAGER

#include "Commons.h"

void update_state();

void update_beat();

bool is_high_pitch(int value);

bool is_computing(int value);

bool is_music_off(int value);

#endif