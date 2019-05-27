/*
 * This file contains the code responsible for updating and evaluating
 * the current state of the robot. 
 */

#include <Arduino.h>
#include "Commons.h"
#include "StateManager.h"
#include "Filters.h"
#include "Move.h"

//unsigned long int next_beat_time = 0;

/*
 * Change the state according to what is perceived in input.
 * The possible states are NO_MUSIC, COMPUTING, BEAT, HIGH_PITCH.
 * Look at the 'enum state' for a description of the different states.
 */
void update_state() {
  int value = (float)analogRead(AUDIO_PIN) - 519.f;

  if (is_music_off(value, false)) {
    STATE = NO_MUSIC;
  } else if (is_computing(value)) {
    STATE = COMPUTING;
  } else if (is_high_pitch(value)) {
    STATE = HIGH_PITCH;
  } else { // then just beat!
    STATE = BEAT;
  }
}

/*
 * Analyze the incoming audio to detect the beat and try to predict the next beat, making it
 * available to the rest of the code, so that we always knows when the next beat will be.
 */
float sample, bass_value, envelope, beat_value; // avoid redeclaring them everytime
void update_beat() {
  
  static unsigned long last_update_time = micros();
  static int i = 0; // keep track of the number of samples seen
  static int beat_dist_values[BEAT_AVG_N]; // array beat distances, to compute the average
  static unsigned long last_beat_time = 0; // this stores the last computed beat time
  static unsigned long int beat_dist_sum = 0;
  static int beat_avg_counter = 0;
  
  // don't do anything if not enough time has passed
  if (last_update_time + SAMPLE_PERIOD > micros()) {
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
  
          // If we are above threshold, we are in a beat!
          if(beat_value > BEAT_THRESH) {
            unsigned long int current = micros();
            unsigned long int distance = current - last_beat_time;

            // the first condition is to avoid underflow
            // Avoid counting several beats that are almost together.
            if (current > last_beat_time && distance > MIN_BEAT_DISTANCE) {

                // DO SOMETHING IN THE BEAT --> we can add delay here, it does not matter whether we miss info.
                // We can use beat_action with a speed parameter in order to move faster if the beats are near enough

                beat_action(current);

//              // cycling sum, for the average computation
//              beat_dist_sum = beat_dist_sum + distance - beat_dist_values[beat_avg_counter];
//              beat_dist_values[beat_avg_counter] = distance;
//              beat_avg_counter++;
//              // reset the counter if it exceeds the array boundary
//              if (beat_avg_counter >= BEAT_AVG_N) {
//                beat_avg_counter = 0;
//              }
//
//              // before updating it, let's store the "previous" value of next_beat_time
//              last_beat_time = current;
//
//              Serial.println("update");
//              // update global variable
//              next_beat_time = current + beat_dist_sum / BEAT_AVG_N;
            }
          }

          //Reset sample counter
          i = 0;
  }
  i=i+1;
  
}

bool is_high_pitch(int value) {
  return false; // TODO remove this
}

bool is_computing(int value) {
  /* TODO:
   *  - continue to return true until we have a defined beat
   */
  static boolean started = false;

  static unsigned long start_time = 0;

  // if the music is NOT off and we have NOT already noticed it
  if (!is_music_off(value, true) && !started) {
    started = true;
    start_time = micros();
  }

// the time has passed (4.5 sec)
  if (micros() - start_time > COMPUTING_TIME) {
    // reset
    started = false;
    return false;
  } else {
    return true;
  }
}

/**
 * 
 * Parameters:
 *   value: the sound value
 *   no_update: true implies that no calculation or update on the state will be performed
 *   
 * Return:
 *   true if the music is off
 *   false if the music is on
 */
bool is_music_off(int value, boolean no_update) {
  /* TODO: manage both the start and the end
   *  - just check the overall volume, but avoid 'flickering'
   */
  static int last_values[NO_MUSIC_AVG_N];
  static int last_index = 0;
  static int sum = 0;
  static unsigned long int last_time = micros();

  // do computation only if requested
  if (!no_update) {
    unsigned long int current_time = micros();
    if (current_time < last_time + NO_MUSIC_SAMPLE_PERIOD) {
      // if not enough time has passed, just assume nothing changed
      return (STATE == NO_MUSIC);
    }
    // otherwise, update 'last_time' and do the calculation
    last_time = current_time;
  
    sum = sum + value - last_values[last_index];
    last_values[last_index] = value;
    last_index++;
    if (last_index >= NO_MUSIC_AVG_N) {
      last_index = 0;
    }
  }
    
  if (sum / NO_MUSIC_AVG_N > NO_MUSIC_THRESH) {
    return false;
  } else {
    return true;
  }
  
}
