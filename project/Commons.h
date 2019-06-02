/*
 * This file contains configuration parameters and variables which are globally shared
 */

#ifndef COMMONS
#define COMMONS

#include <VarSpeedServo.h>

/*** ARDUINO SCHEMATICS ***/
const int FRANKLIN_PIN = 8;
const int FRANKLIN_ARMS_PIN = 9;
const int ARETHA_PIN = 11;
const int ARETHA_ARMS_PIN = 12;

const int AUDIO_PIN = 0;

// audio fx module
const int SPEAK_PIN_0 = 22; // applauding
const int SPEAK_PIN_2 = 26; // "come to me" and "come closer to me"
const int SPEAK_PIN_4 = 30; // "come on boy, see above me" and "come on come on"
const int SPEAK_PIN_7 = 34; // "let's get it on" and "let's the music play"

// ultrasonic sensor pins
const int TRIGGER_PIN = 51;
const int ECHO_PIN = 53;

/*** SAMPLING ***/
const int SAMPLE_PERIOD = 200; // sample rate = 5000hz

/*** BEAT CALCULATION ***/
const float BEAT_THRESH = 10.f;
const unsigned long MIN_BEAT_DISTANCE = 700000; // 500ms == 500 000us
const unsigned long MAX_BEAT_DISTANCE = 1000000; // 980ms == 980 000us
const int BEAT_AVG_N = 30; // number of beats distances from which to compute the avg. distance

/*** HIGH PITCH CALCULATION ***/
const int LOCAL_P_AVG_N = 100;    // number of values to consider for the computation of the "local" pitch moving average
const int GLOBAL_P_AVG_N = 200;  // number of values to consider for the computation of the "global" pitch moving average
const float HIGH_PITCH_THRESHOLD = 5; // threshold for high pitch difference


/** NO MUSIC CALCULATION **/
const float NO_MUSIC_THRESH = 1.5;
const int NO_MUSIC_AVG_N = 10;
const unsigned long int NO_MUSIC_SAMPLE_PERIOD = 100000; //in us, so: 10hz

/*** MOVEMENT ***/
const int MAX_ANGLE = 170;  // we can adjust this for aesthetic reasons
const int MIN_ANGLE = 10;    // we can adjust this for aesthetic reasons
const int MID_ANGLE = 55; 
const unsigned long int MOVEMENT_TIME = 500000; // 500ms == 500 000us

const int TIME1 = 1000; // 1 sec
const int TIME2 = 1500;
const int TIME3 = 2000;
const int TIME4 = 3000;
const int TIME5 = 3500;
const int TIME6 = 4000;
const unsigned long int COMPUTING_TIME = 4500000; // 4.5 sec


/*** GLOBAL VARIABLES ***/
enum state {NO_MUSIC, INTERACTING, BEAT, HIGH_PITCH};
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
extern float envelope;
extern float sample;
extern float bass_value;
extern float beat_value;
extern VarSpeedServo franklin;      // franklin body servo motor
extern VarSpeedServo aretha;        // aretha body servo motor
extern VarSpeedServo franklin_arms; // franklin arms servo motor
extern VarSpeedServo aretha_arms;   // aretha arms servo motor

#endif
