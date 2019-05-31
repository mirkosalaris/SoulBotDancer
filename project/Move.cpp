/*
   This file contains all the function that define the robot movements
   and that enables the interaction with the user.
*/

#include <Arduino.h>
#include "Move.h"
#include "Commons.h"
#include "StateManager.h"

enum direction {up, down};
direction dir = up;

//Values for the servos delay
float coeff = 255.f / float(255);
float delay1 = 1000 * coeff;
float delay2 = 500 * coeff;


/**
   Returns the speed of the movements based on the distance, which is
   the distance between the previous beat and the current one.
*/
int get_speed(unsigned long int distance) {
  if (distance <= MIN_BEAT_DISTANCE) {
    return 255;
  } else if (distance > MIN_BEAT_DISTANCE && distance <= MAX_BEAT_DISTANCE) {
    return 175;
  } else {
    return 100;
  }
}

void beat_action(unsigned long int beat_time, int arms_speed) {
  // keep track of which is the last "next_beat_time", the one we were following
  static unsigned long int last_next_beat_time = beat_time;
  static direction last_dir = dir;

  // If the next_beat_time has changed, it means we surpassed the current beat
  // It's time to change direction
  if (beat_time != last_next_beat_time) {
    // if we were going up, now go down, and viceversa
    if (dir == up) {
      dir = down;
    } else {
      dir = up;
    }

    // update the last seen "next_beat_time"
    last_next_beat_time = beat_time;
  }

  // start to move ONLY when it is time. The goal is to reach the maximum extension ON the beat.
  if (last_dir != dir) {
    last_dir = dir;

    // body speed is 2/3 of the arms speed
    int body_speed = arms_speed * 0.6;

    // Move the right and left (body)
    if (dir == up) {
      franklin_arms.write(MID_ANGLE, arms_speed);
      aretha_arms.write(MID_ANGLE, arms_speed, true);
      franklin_arms.write(0, arms_speed);
      aretha_arms.write(0, arms_speed);
      franklin.write(MAX_ANGLE, body_speed);
      // wait until the end of the movement.
      aretha.write(MAX_ANGLE, body_speed, true);
    } else { // move the arms down
      franklin_arms.write(MID_ANGLE, arms_speed);
      aretha_arms.write(MID_ANGLE, arms_speed, true);
      franklin_arms.write(0, arms_speed);
      aretha_arms.write(0, arms_speed);
      franklin.write(MIN_ANGLE, body_speed);
      // wait until the end of the movement.
      aretha.write(MIN_ANGLE, body_speed, true);
    }
  }
}

/**
   get distance from the ultrasonic sensor: this should be called only when there is no music
*/
float get_user_distance() {
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  float duration = pulseIn(ECHO_PIN, HIGH);
  // meassured in cm.
  return (duration * 0.0340 / 2);
}

/**
   Do something when there is no music.
    - If there is no one in front of the robot (distance > 2 mts), it should move its arms
    - If there is someone in between 2 mts and 30 cm, the robot should talk
    - If there is someone in front of the robot, Let's play the music!
*/
void no_music_action() {
  unsigned long start_time = millis();
  unsigned long current_time = millis();

  // the maximum range of the HC-SR04 is 4 meters
  float distance = get_user_distance();

  if (distance > 200) {
    //Serial.println("distance > 200");
    emmit_sound(SPEAK_PIN_1);
    // wait 3 seconds to do any interaction again
  } else if (distance > 30) {
    //Serial.println("distance > 30");
    // come closer to me sound
    emmit_sound(SPEAK_PIN_2);
    // move the arms up and down.
    move_arms_up_down();
  } else if (distance <= 30) {
    //Serial.println("distance <= 30");
    // let the music play sound
    emmit_sound(SPEAK_PIN_3);
    // move the body left and right
    move_body_left_right();
  }

  // (active) wait 5 seconds before continue.
  while (current_time - start_time < 5000) {
    current_time = millis();
  }
}

void emmit_sound(int pin_number) {
  digitalWrite(pin_number, LOW);
  delay(250);
  digitalWrite(pin_number, HIGH);
}

void move_arms_up_down() {
  int angle = 0;
  for (int i = 0; i < 4; i++) {
    franklin_arms.write(angle, 255);
    aretha_arms.write(angle, 255);
    angle = (angle == 0) ? 90 : 0;
    delay(200);
  }
}

void move_body_left_right() {
  int angle = 45;
  for (int i = 0; i < 4; i++) {
    franklin.write(angle, 200);
    aretha.write(angle, 200);
    angle = (angle == 45) ? 135 : 0;
    delay(200);
  }
}

/*******************************************************/
/**
   @deprecated
*/
void high_pitch_action() {
  // TODO: raise the arms
}
