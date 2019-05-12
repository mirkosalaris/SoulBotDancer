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

const int MAX_ANGLE = 120;  // we can adjust this for aesthetic reasons
const int MIN_ANGLE = 0;    // we can adjust this for aesthetic reasons

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

/*
 * Change the state according to what is perceived in input.
 * The possible states are NO_MUSIC, COMPUTING, BEAT, HIGH_PITCH.
 * Look at the 'enum state' for a description of the different states.
 */
void update_state() {
  /*value = ...

  if (is_rising_pitch(value)) {
    STATE = HIGH_PITCH;
    return;
  }

  if (...) {
    STATE = BEAT;
    return; 
  }

  ...
  */
}

/*
 * Analyze the incoming audio to detect the beat and try to predict the next beat, making it
 * available to the rest of the code, so that we always knows when the next beat will be.
 */
void update_beat() {
  /* TODO: 
   *  - if not enough time has passed, don't do anything, otherwise
   *  - various filtering operations
   *  - identify if we are currently in a beat (notice, keep track of it only if it is a different beat w.r.t. the previous iteration)
   *  - keep track of the few previous beats and the distance among them
   *  - predict the next beat (?in terms of actual time we need to wait?) using the info from the previous ones
   */
  
}

void high_pitch() {
  // TODO: raise the arms
  
}

void beat() {
  // TODO: follow the beat
  // NB: the beat computation is NOT done here. Here we just 'follow' the beat
  
}

void no_music() {
  // TODO: interaction with the user
 
}

void computing() {
  // TODO: maybe implement a few strategies to fill the delay
  
}
