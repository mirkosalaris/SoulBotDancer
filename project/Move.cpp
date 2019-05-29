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


/** 
 * Returns the speed of the movements based on the distance, which is 
 * the distance between the previous beat and the current one. 
 */
int get_speed(unsigned long int distance){
  if(distance <= MIN_BEAT_DISTANCE){
    return 255;
  } else if(distance > MIN_BEAT_DISTANCE && distance <= MAX_BEAT_DISTANCE){
    return 175;
  }else {
    return 100;
  }
}

void beat_action(unsigned long int beat_time, int arms_speed) {
  unsigned long int current_time = micros();
  // keep track of which is the last "next_beat_time", the one we were following
  static unsigned long int last_next_beat_time = beat_time;
  static direction last_dir = dir;
  static unsigned long int last_command_time = 0;
  
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

    // time when we sent the move instruction => NOW!
    last_command_time = current_time;
    
    // body speed is 2/3 of the arms speed
    int body_speed= arms_speed*0.6;

    // Move the arms up
    if (dir == up) {
      franklin_arms.write(MID_ANGLE, arms_speed);
      aretha_arms.write(MID_ANGLE,arms_speed);
      franklin.write(MAX_ANGLE, body_speed);
      // wait until the end of the movement.
      aretha.write(MAX_ANGLE, body_speed,true);
    } else { // move the arms down
      franklin_arms.write(MIN_ANGLE, arms_speed);
      aretha_arms.write(MIN_ANGLE,arms_speed);
      franklin.write(MIN_ANGLE, body_speed);
      // wait until the end of the movement.
      aretha.write(MIN_ANGLE, body_speed,true);

    }
  }
}

/**
 * get distance from the ultrasonic sensor: this should be called only when there is no music 
 */
float get_user_distance(){
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
 * Do something when there is no music.
 *  - If there is no one in front of the robot (distance > 2 mts), it should move its arms
 *  - If there is someone in between 2 mts and 30 cm, the robot should talk
 *  - If there is someone in front of the robot, Let's play the music!
 */
void no_music_action() {
  float distance = get_user_distance();
  
  if(distance > 200){
    // no one in front of the stage
  } else if(distance > 30){
    // come closer to me sound
  } else if (distance <= 30){
    // let the music play sound
  }
 
}

/*******************************************************/
/**
 * @deprecated
 */
void high_pitch_action() {
  // TODO: raise the arms
}

/**
 * @deprecated
 */
void computing_action() {
  // time in which the movement started
  // only calculated on the first call
  static unsigned long int start_time = millis(); 

  // time from when the movement started
  unsigned long int mov_time = millis() - start_time;

  //Stretching movements for robot aretha
  //The robot rotates  0-90-0 and then 180-90-180 with rising and lowering hands

  if (mov_time < TIME1)
   franklin.write(0);
  else if (mov_time < TIME2)
    franklin.write(90);
  else if (mov_time < TIME3)
    franklin.write(0);
  else if (mov_time < TIME4)
  {
    franklin_arms.write(120); //rising hands
    franklin.write(180);
  }
  else if (mov_time < TIME5)
    franklin.write(90);
  else if (mov_time < TIME6)
    franklin.write(180);
  else
  {
    franklin_arms.write(0); //lowering hands
    franklin.write(90);
    // start the movement again
    start_time = millis();
  }
}
