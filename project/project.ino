/*
 * This is the main file for the SoulBotDancer project.
 * 
 * The setup function initializes the electronical part and set a few parameters.
 * The loop function continuosly update the state of the robot and triggers the correspondent action.
 */

#include <Arduino.h>
#include "Commons.h"
#include "Move.h"
#include "StateManager.h"

state STATE; // overall state of the application: see Commons.h for details
Servo franklin;   // franklin body servo motor
Servo aretha;     // aretha body servo motor
Servo franklin_arms;  // franklin arms servo motor
Servo aretha_arms;    // aretha arms servo motor


// defines for setting and clearing register bits
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

/* ***** SETUP ***** */
void setup() {
  // When we start we assume there is no music.
  // As soon as there is some music the state will change automatically
  STATE = NO_MUSIC;

  // Set ADC to 77khz, max for 10bit
  sbi(ADCSRA,ADPS2);
  cbi(ADCSRA,ADPS1);
  cbi(ADCSRA,ADPS0);

  Serial.begin(9600);

  franklin.attach(FRANKLIN_PIN);
  franklin_arms.attach(FRANKLIN_ARMS_PIN);
  aretha.attach(ARETHA_PIN);
  aretha_arms.attach(ARETHA_ARMS_PIN);
  franklin.write(90);
  franklin_arms.write(0);
  aretha.write(90);
  aretha_arms.write(0); 
}

/* ***** LOOP ***** */
void loop() {
  update_state(); // let's check what we have to do (wait? follow the beat? ...)

  // act according to the state
  if (STATE == HIGH_PITCH) {
    // we still have to keep track of the beat, even if our movement ignores it at the moment
    update_beat();

    high_pitch_action();

    /* Notice: do not change the order of the calls in here.
     * Let's call the update_beat() first, so that it is always called
     * as the first function (independently of the state), to have a regular timing of the update.
     */
     
  } else if (STATE == BEAT) {
    update_beat();
    beat_action();
    
  } else if (STATE == NO_MUSIC) {
    no_music_action();
    
  } else { // == COMPUTING
    computing_action();
  }
}
