/*
 * This file contains all the function that define the robot movements
 * and that enables the interaction with the user.
 */

#include <Arduino.h>
#include "Move.h"
#include "Commons.h"

enum direction {up, down};
direction dir;

void high_pitch_action() {
  // TODO: raise the arms
  
}

void beat_action() {
  unsigned long int current_time = micros();

  // keep track of which is the last "next_beat_time", the one we were following
  static unsigned long int last_next_beat_time = next_beat_time;

  // If the next_beat_time has changed, it means we surpassed the current beat
  // It's time to change direction
  if (next_beat_time != last_next_beat_time) {
    // if we were going up, now go down, and viceversa
    if (dir == up) dir = down;
    else dir = up;

    // update the last seen "next_beat_time"
    last_next_beat_time = next_beat_time;
  }

  // start to move ONLY when it is time. The goal is to reach the maximum extension ON the beat.
  if (current_time > next_beat_time - MOVEMENT_TIME) {
    if (dir == up) {
      franklin_arms.write(MAX_ANGLE);
    } else { // dir == down
      franklin_arms.write(MIN_ANGLE);
    }
  }
  
}

void no_music_action() {
  // TODO: interaction with the user
 
}

void computing_action() {
  // TODO: maybe implement a few strategies to fill the delay
  
}
