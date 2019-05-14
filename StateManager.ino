extern state STATE;

/*
 * Change the state according to what is perceived in input.
 * The possible states are NO_MUSIC, COMPUTING, BEAT, HIGH_PITCH.
 * Look at the 'enum state' for a description of the different states.
 */
void update_state() {
  /* TODO
  int value = ...;

  if (is_music_off(value)) {
    STATE = NO_MUSIC;
  } else if (is_computing(value)) {
    STATE = COMPUTING;
  } else if (is_rising_pitch(value)) {
    STATE = HIGH_PITCH;
  } else { // then just beat!
    STATE = BEAT;
  }
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

boolean is_rising_pitch(int value) {
  /* TODO:
   *  - keep track of previous values and check if the pitch is rising
   *  - avoid flickering using a moving average or counting number of increasing values in the last tot samples
   */
}

boolean is_computing(int value) {
  /* TODO:
   *  - if already surpassed this phase, then return false
   *  - be sure to return true for a few seconds after music is on
   *    (to guarantee a coherence in the behavior of the robot, always 'act' at least)
   *  - continue to return true until we have a defined beat
   */

}

boolean is_music_off(int value) {
  /* TODO: manage both the start and the end
   *  - just check the overall volume, but avoid 'flickering'
   */
  
}
