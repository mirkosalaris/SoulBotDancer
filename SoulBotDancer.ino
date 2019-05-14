enum state {NO_MUSIC, COMPUTING, BEAT, HIGH_PITCH};
/* the state in which we are at the current moment. 
 * - NO_MUSIC:  when the music is off (and we have to interact with the user)
 * - COMPUTING: not mandatory to use. When the music has been turned on and
 *              we are waiting to understand what to do.
 * - BEAT:      the state in which the robot will be for most of the time.
 *              In this state the robot is just following the rhythm with its arms/body
 * - HIGH_PITCH: when we detect an high pitch and we change the robot behavior,
 *               suspending the usual "beat following" behavior
 */

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
    
    high_pitch();
    
  } else if (STATE == BEAT) {
    update_beat();
    beat();
    
  } else if (STATE == NO_MUSIC) {
    no_music();
    
  } else { // == COMPUTING
    computing();
  }
}
