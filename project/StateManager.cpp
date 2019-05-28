/*
 * This file contains the code responsible for updating and evaluating
 * the current state of the robot. 
 */

#include <Arduino.h>
#include "Commons.h"
#include "StateManager.h"
#include "Filters.h"
#include "Move.h"

/*
 * Change the state according to what is perceived in input.
 * The possible states are NO_MUSIC, COMPUTING, BEAT, HIGH_PITCH.
 * Look at the 'enum state' for a description of the different states.
 */
void update_state() {
  float value = (float)analogRead(AUDIO_PIN) - 81.f;

  if (is_music_off(value, true)) {
    STATE = NO_MUSIC;
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
  sample = (float)analogRead(AUDIO_PIN) - 81.f;
  
  bass_value = bassFilter(sample);
  
  // Take signal amplitude and filter
  if(bass_value < 0) {
    bass_value=-bass_value;
  }
  envelope = envelopeFilter(bass_value);

  
  // Every 200 samples (25hz) filter the envelope
  if(i % 200 == 0) { //  && !is_music_off(sample, false)
          // Filter out repeating bass sounds 100 - 180bpm
          beat_value = beatFilter(envelope);
          // If we are above threshold, we are in a beat!
          if(beat_value > BEAT_THRESH) {
            unsigned long int current = micros();
            unsigned long int distance = current - last_beat_time;
            
            int velocity = get_speed(distance);
            //Serial.println(velocity);
            // the first condition is to avoid underflow
            // Avoid counting several beats that are almost together.
            if (current > last_beat_time && distance > MIN_BEAT_DISTANCE) {
                beat_action(current, velocity);
                last_beat_time = current;
            }
          }
    // avoid going to MAX_INT
    i = 0;
  }
  i++;
  
}

bool is_high_pitch(int value) {
  return false; // TODO remove this
}

bool is_computing(int value) {
  /* TODO:
   *  - continue to return true until we have a defined beat
   *
  static boolean started = false;

  static unsigned long start_time = 0;

  // if the music is NOT off and we have NOT already noticed it
  if (!is_music_off(value, false) && !started) {
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
  } */
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
bool is_music_off(float value, boolean b_update) {
  /* TODO: manage both the start and the end
   *  - just check the overall volume, but avoid 'flickering'
   */
  static int last_values[NO_MUSIC_AVG_N];
  static int last_index = 0;
  static float sum = 0;
  static unsigned long int last_time = micros();

  // remove the negative part of the sound wave
  // and return the current state. This will 
  // avoid summing and substracting the value every time.
  if(value < 0){
    return (STATE == NO_MUSIC);
  }

  // do computation only if requested
  if (b_update) {
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
  
  if (sum / NO_MUSIC_AVG_N >= NO_MUSIC_THRESH) {
    return false;
  } else {
    return true;
  }
  
}
