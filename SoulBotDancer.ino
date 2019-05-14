#include "Move.h"
#include "StateManager.h"

state STATE;

/* ***** SETUP ***** */
void setup() {
  STATE = NO_MUSIC;
}

/* ***** LOOP ***** */
void loop() {
  update_state();
  
  if (STATE == HIGH_PITCH) {
    // we still have to keep track of the beat, even if our movement ignores it at the moment
    update_beat();
    
    high_pitch_action();
    
  } else if (STATE == BEAT) {
    update_beat();
    beat_action();
    
  } else if (STATE == NO_MUSIC) {
    no_music_action();
    
  } else { // == COMPUTING
    computing_action();
  }
}
