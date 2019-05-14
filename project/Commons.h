/*
 * This file contains configuration parameters and variables which are globally shared
 */

#ifndef COMMONS
#define COMMONS

#include <Servo.h>

/*** ARDUINO SCHEMATICS ***/
const int FRANKLIN_PIN = 9;
const int FRANKLIN_ARMS_PIN = 10;
const int ARETHA_PIN = 11;
const int ARETHA_ARMS_PIN = 12;

const int AUDIO_PIN = 0;

/*** SAMPLING ***/
const int SAMPLE_PERIOD = 200; // sample rate = 5000hz

/*** BEAT CALCULATION ***/
const float BEAT_THRESH = 10.f;
const int MIN_BEAT_DISTANCE = 100*1000; // 100ms == 100 000us
const int BEAT_AVG_N = 30; // number of beats distances from which to compute the avg. distance

/*** HIGH PITCH CALCULATION ***/
const int LOCAL_P_AVG_N = 100;    // number of values to consider for the computation of the "local" pitch moving average
const int GLOBAL_P_AVG_N = 5000;  // number of values to consider for the computation of the "global" pitch moving average


/*** MOVEMENT ***/
const int MAX_ANGLE = 90;  // we can adjust this for aesthetic reasons
const int MIN_ANGLE = 0;    // we can adjust this for aesthetic reasons
const unsigned long int MOVEMENT_TIME = 500*1000; // 500ms == 500 000us


/*** GLOBAL VARIABLES ***/
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

extern state STATE;
extern unsigned long int next_beat_time;
extern Servo franklin;      // franklin body servo motor
extern Servo aretha;        // aretha body servo motor
extern Servo franklin_arms; // franklin arms servo motor
extern Servo aretha_arms;   // aretha arms servo motor

#endif