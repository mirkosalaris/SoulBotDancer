#ifndef STATEMANAGER
#define STATEMANAGER

#include "Commons.h"

void update_state();

void update_beat(float envelope);

bool is_music_off(float value, boolean no_update);

bool is_interacting();

#endif
