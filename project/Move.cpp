/*
 * This file contains all the function that define the robot movements
 * and that enables the interaction with the user.
 */

#include <Arduino.h>
#include "Move.h"
#include "Commons.h"

enum direction {up, down};
direction dir = up;

//Values for the servos delay
float coeff=255.f/float(255);
float delay1=1000*coeff;
float delay2=500*coeff;


void high_pitch_action() {
  // TODO: raise the arms
  
}

void beat_action() {
  unsigned long int current_time = micros();
  

  // keep track of which is the last "next_beat_time", the one we were following
  static unsigned long int last_next_beat_time = next_beat_time;
  static direction last_dir = dir;
  static unsigned long int last_command_time = 0;
  
  // If the next_beat_time has changed, it means we surpassed the current beat
  // It's time to change direction
  if (next_beat_time != last_next_beat_time) {
    // if we were going up, now go down, and viceversa
    if (dir == up) {
      dir = down;
    } else {
      dir = up;
    }

    // update the last seen "next_beat_time"
    last_next_beat_time = next_beat_time;
  }
  
  // start to move ONLY when it is time. The goal is to reach the maximum extension ON the beat.
  if (current_time > next_beat_time - MOVEMENT_TIME && current_time > last_command_time + MOVEMENT_TIME && last_dir != dir) {
    last_dir = dir;

    last_command_time = current_time;
    
    if (dir == up) {
      franklin_arms.write(MAX_BEAT_ANGLE);
    } else { // dir == down
      franklin_arms.write(MIN_BEAT_ANGLE);
    }
  }
}

void no_music_action() {
  // TODO: interaction with the user
 
}

void computing_action() {
  // time in which the movement started
  // only calculated on the first call
  static const unsigned long int start_time = millis(); 

  // time from when the movement started
  unsigned long int mov_time = millis() - start_time;
  
  // Franklin goes 
  // Franklin movement
  if (mov_time < F_TIME1) {
    franklin.write(135);
  } else if (mov_time < F_TIME2) {
    franklin.write(45);
  } else if (mov_time < F_TIME3) {
    franklin_arms.write(120); // rising hands
    franklin.write(90);
  } else if (mov_time < F_TIME4) {
    franklin.write(0);
  } else if (mov_time < F_TIME5) {
    franklin_arms.write(MIN_BEAT_ANGLE);
    franklin.write(180);
  } else if (mov_time < F_TIME6) {
    franklin.write(45);
  } else {
    franklin_arms.write(MIN_BEAT_ANGLE); // lowering hands
    franklin.write(90);
  }

  // Aretha rotates  0-90-0 and then 180-90-180 with rising and lowering hands
  // Aretha movement
  if (mov_time < A_TIME1) {
    aretha.write(0);
  } else if (mov_time < A_TIME2) {
    aretha.write(90);
  } else if (mov_time < A_TIME3) {
    aretha.write(0);
  } else if (mov_time < A_TIME4) {
    aretha_arms.write(120); // rising hands
    aretha.write(180);
  } else if (mov_time < A_TIME5) {
    aretha.write(90);
  } else if (mov_time < A_TIME6) {
    aretha.write(180);
  } else {
    aretha_arms.write(MIN_BEAT_ANGLE); // lowering hands
    aretha.write(90);
  }
}
