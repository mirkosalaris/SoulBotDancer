/*
 * This file contains the code responsible for updating and evaluating
 * the current state of the robot. 
 */

#include <Arduino.h>
#include "Commons.h"
#include "StateManager.h"
#include "Filters.h"

unsigned long int next_beat_time;

/*
 * Change the state according to what is perceived in input.
 * The possible states are NO_MUSIC, COMPUTING, BEAT, HIGH_PITCH.
 * Look at the 'enum state' for a description of the different states.
 */
void update_state() {
  // TODO: this is just temporary, to at least test the beat detection algorithm
  STATE = BEAT;
  /*int value = ...;

  if (is_music_off(value)) {
    STATE = NO_MUSIC;
  } else if (is_computing(value)) {
    STATE = COMPUTING;
  } else if (is_high_pitch(value)) {
    STATE = HIGH_PITCH;
  } else { // then just beat!
    STATE = BEAT;
  }*/
}

/*
 * Analyze the incoming audio to detect the beat and try to predict the next beat, making it
 * available to the rest of the code, so that we always knows when the next beat will be.
 */
float sample, bass_value, envelope, beat_value; // avoid redeclaring them everytime
void update_beat() {
  static unsigned long int last_update_time = micros();
  static int i = 0; // keep track of the number of samples seen
  static int beat_dist_values[BEAT_AVG_N]; // array beat distances, to compute the average
  static unsigned long int last_beat_time = 0; // this stores the last computed beat time
  static unsigned long int beat_dist_sum = 0;
  static int beat_avg_counter = 0;
  
  // don't do anything if not enough time has passed
  if (last_update_time + SAMPLE_PERIOD < micros()) {
    return;
  }
  last_update_time = micros();
  

  // Read ADC and center so ~512
  sample = (float)analogRead(AUDIO_PIN)-519.f;
  
  bass_value = bassFilter(sample);
  
  // Take signal amplitude and filter
  if(bass_value < 0) {
    bass_value=-bass_value;
  }
  envelope = envelopeFilter(bass_value);
  
  // Every 200 samples (25hz) filter the envelope
  if(i == 200) {
          // Filter out repeating bass sounds 100 - 180bpm
          beat_value = beatFilter(envelope);
  
          // If we are above threshold, light up LED
          if(beat_value > BEAT_THRESH) {
            if (micros() - last_beat_time > MIN_BEAT_DISTANCE) {

              // micros() is the current time, in microseconds
              int distance = micros() - last_beat_time;

              // cycling sum, for the average computation
              beat_dist_sum = beat_dist_sum + distance - beat_dist_values[beat_avg_counter];
              beat_dist_values[beat_avg_counter] = distance;
              beat_avg_counter++;
              // reset the counter if it exceeds the array boundary
              if (beat_avg_counter >= BEAT_AVG_N) {
                beat_avg_counter = 0;
              }

              // before updating it, let's store the "previous" value of next_beat_time
              last_beat_time = next_beat_time;
              
              // update global variable
              next_beat_time = micros() + beat_dist_sum / BEAT_AVG_N;
            }
          }

          //Reset sample counter
          i = 0;
  }
  
}

bool is_high_pitch(int value) {
  // TODO: keep track of the last local and global values and compare the two moving averages
  // keep track of the last read values
  static float last_local_values[LOCAL_P_AVG_N];
  static float last_values[GLOBAL_P_AVG_N]; 

  static int local_sum = 0;
  static int global_sum = 0;

  // the next 5 lines trick is to avoid shifting the array
  static int j = 0;
  last_local_values[j] = value;
  j++;
  if (j >= LOCAL_P_AVG_N) { // reset if exceeds array boundary
    j = 0;
  }
  
  // the next 5 lines trick is to avoid shifting the array
  static int i = 0;
  last_values[i] = value;
  i++;
  if (i >= GLOBAL_P_AVG_N) { // reset if exceeds array boundary
    i = 0;
  }

  local_sum = local_sum + value - last_local_values[i]; // cycling sum (add the most recent, remove the last one)
  global_sum = global_sum + value - last_values[j]; // cycling sum (add the most recent, remove the last one)

  // if the pitch local average exceeds the global one, we are probably perceiving a "long high pitch"
  if (local_sum > global_sum) {
    return true;
  }
  return false;
}

bool is_computing(int value) {
  /* TODO:
   *  - if already surpassed this phase, then return false
   *  - be sure to return true for a few seconds after music is on
   *    (to guarantee a coherence in the behavior of the robot, always 'act' at least)
   *  - continue to return true until we have a defined beat
   */

}

bool is_music_off(int value) {
  /* TODO: manage both the start and the end
   *  - just check the overall volume, but avoid 'flickering'
   */
  
}
