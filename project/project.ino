/*
   This is the main file for the SoulBotDancer project.

   The setup function initializes the electronical part and set a few parameters.
   The loop function continuosly update the state of the robot and triggers the correspondent action.
*/

#include <Arduino.h>
#include "Commons.h"
#include "Move.h"
#include "StateManager.h"

state STATE; // overall state of the application: see Commons.h for details
VarSpeedServo franklin;   // franklin body servo motor
VarSpeedServo aretha;     // aretha body servo motor
VarSpeedServo franklin_arms;  // franklin arms servo motor
VarSpeedServo aretha_arms;    // aretha arms servo motor


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
  sbi(ADCSRA, ADPS2);
  cbi(ADCSRA, ADPS1);
  cbi(ADCSRA, ADPS0);

  Serial.begin(9600);

  // Set-up audio module. All pins should be High except when the emmit sound.
  pinMode(SPEAK_PIN_0, OUTPUT);
  pinMode(SPEAK_PIN_2, OUTPUT);
  pinMode(SPEAK_PIN_4, OUTPUT);
  pinMode(SPEAK_PIN_7, OUTPUT);

  digitalWrite(SPEAK_PIN_0, HIGH);
  digitalWrite(SPEAK_PIN_2, HIGH);
  digitalWrite(SPEAK_PIN_4, HIGH);
  digitalWrite(SPEAK_PIN_7, HIGH);


  // Set-up the movement motors
  franklin.attach(FRANKLIN_PIN);
  franklin_arms.attach(FRANKLIN_ARMS_PIN);
  aretha.attach(ARETHA_PIN);
  aretha_arms.attach(ARETHA_ARMS_PIN);
  franklin.write(90);
  franklin_arms.write(0);
  aretha.write(90);
  aretha_arms.write(0);

  delay(500);

  // everything is set-up.
  emmit_sound(SPEAK_PIN_0);
}

/* ***** LOOP ***** */
void loop() {
  update_state(); // let's check what we have to do (wait? follow the beat? ...)

  if (STATE == BEAT) {
    update_beat(envelope);

  } else if (STATE == NO_MUSIC) {
    no_music_action();
  }

}
