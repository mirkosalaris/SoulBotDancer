/*
   This file contains the code responsible for updating and evaluating
   the current state of the robot.
*/

#include <Arduino.h>
#include "Commons.h"
#include "StateManager.h"
#include "Filters.h"
#include "Move.h"

/**
 *** VARIABLES ***
   Avoid redeclaring them everytime
*/
float sample, bass_value, envelope, beat_value;
/*
   Change the state according to what is perceived in input.
   The possible states are NO_MUSIC, BEAT.
   Look at the 'enum state' for a description of the different states.
*/
void update_state() {
  // Read ADC and center so ~81
  sample = (float)analogRead(AUDIO_PIN) - 81.f;

  bass_value = bassFilter(sample);
  
  // Take signal amplitude and filter
  if (bass_value < 0) {
    bass_value = -bass_value;
  }
  envelope = envelopeFilter(bass_value);

  if (is_music_off(sample, true)) {
    STATE = NO_MUSIC;
  } else if(is_high_pitch(envelope)){
//    Serial.println("HIGH PITCH DO SOMETHING!");
  } else { // if the state is not interacting, we can analyze the music.
    STATE = BEAT;
  }
}

/**
   Analyse the audio in order to undestand whether there is a beat or not.
   In case we detect a beat, we make the robot move.
*/
void update_beat(float envelope) {

  static unsigned long last_update_time = micros();
  static int i = 0; // keep track of the number of samples seen
  static unsigned long last_beat_time = 0; // this stores the last computed beat time

  // don't do anything if not enough time has passed
  if (last_update_time + SAMPLE_PERIOD > micros()) {
    return;
  }
  last_update_time = micros();

  // Every 200 samples (25hz) filter the envelope
  if (i == 200) {
    // Filter out repeating bass sounds 100 - 180bpm
    beat_value = beatFilter(envelope);
    // If we are above threshold, we are in a beat!
    if (beat_value > BEAT_THRESH) {
      unsigned long int current = micros();
      unsigned long int distance = current - last_beat_time;

      // Get the velocity of the movements based on the distance between beats.
      int velocity = get_speed(distance);

      // the first condition is to avoid underflow
      // Avoid counting several beats that are almost together.
      if (current > last_beat_time && distance > MIN_BEAT_DISTANCE) {
        // this funciton is blocking, so we'll miss the next beats.
        // But that's not a problem, since we may do that in real life.
        beat_action(current, velocity);
        // update the last beat with the current beat.
        last_beat_time = current;
      }
    }
    i = 0;
  }

  i++;
}

/**
   Check whether the music is OFF or ON.
   Parameters:
     value: the sound value
     b_update: true implies that calculation or update on the state will be performed

   Return:
     true if the music is off
     false if the music is on
*/
bool is_music_off(float value, bool b_update) {
  // vector with last values from the analog input.
  static int last_values[NO_MUSIC_AVG_N];
  static int last_index = 0;
  static float sum = 0;
  static unsigned long int last_time = micros();

  // remove the negative part of the sound wave
  // and return the current state. This will
  // avoid summing and substracting the value every time.
  // if that is the case, just return the current state:
  // if the music is on, returns FALSE, if not, TRUE.
  if (value < 0) {
    return (STATE == NO_MUSIC);
  }
  
  // do computation only if requested
  if (b_update) {
    unsigned long int current_time = micros();

    // if not enough time has passed, just assume nothing changed
    if (current_time < last_time + NO_MUSIC_SAMPLE_PERIOD) {
      return (STATE == NO_MUSIC);
    }

    // otherwise, update 'last_time' and do the calculation
    last_time = current_time;

    // moving average of the positive sound values.
    sum = sum + value - last_values[last_index];
    last_values[last_index] = value;
    last_index++;
    // reset the counter.
    if (last_index >= NO_MUSIC_AVG_N) {
      last_index = 0;
    }
  }

  // if the average is over the threashold, it means that the music is ON.
  // NOTE that the function is called is_music_OFF.
    //Serial.println(sum/NO_MUSIC_AVG_N);
  if (sum / NO_MUSIC_AVG_N >= NO_MUSIC_THRESH) {
    return false;
  } else {
    // if the average is above the threshold, it means that the music is OFF.
    return true;
  }
}

bool is_high_pitch(int value) {
  return false;
  // TODO: keep track of the last local and global values and compare the two moving averages
  // keep track of the last read values
  static float last_local_values[LOCAL_P_AVG_N];
  static float last_values[GLOBAL_P_AVG_N];

  static int local_sum = 0;
  static int global_sum = 0;

  // the next 5 lines trick is to avoid shifting the array
  static int lv_counter = 0;
  last_local_values[lv_counter] = value;
  lv_counter++;
  if (lv_counter >= LOCAL_P_AVG_N) { // reset if exceeds array boundary
    lv_counter = 0;
  }

  // the next 5 lines trick is to avoid shifting the array
  static int gv_counter = 0;
  last_values[gv_counter] = value;
  gv_counter++;
  if (gv_counter >= GLOBAL_P_AVG_N) { // reset if exceeds array boundary
    gv_counter = 0;
  }

  local_sum = local_sum + value - last_local_values[lv_counter]; // cycling sum (add the most recent, remove the last one)
  global_sum = global_sum + value - last_values[gv_counter]; // cycling sum (add the most recent, remove the last one)

  float avg_difference = ((float)local_sum / LOCAL_P_AVG_N) - ((float)global_sum / GLOBAL_P_AVG_N);
  
  // if the pitch local average exceeds the global one, we are probably perceiving a "long high pitch"
  if (avg_difference > HIGH_PITCH_THRESHOLD) {
    return true;
  }
  return false;
}
