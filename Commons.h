#ifndef COMMONS
#define COMMONS

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

/*** Sampling ***/
const int SAMPLE_RATE = 5000; 

/*** Beat Calculation ***/

/*** Rising Pitch calculation ***/
const int RP_N = 50;        // number of values to consider to compute the average for the Rising Pitch calculation
const int PREV_P_AV_N = 3;  // number of previous averages we keep to compare with the current Pitch average
const int PREV_P_AV_COEFF = 0.99; // coefficient of reduction, multiplied by the previous averages in rising Pitch calculation

/*** Movement parameters ***/ 
const int MAX_ANGLE = 120;  // we can adjust this for aesthetic reasons
const int MIN_ANGLE = 0;    // we can adjust this for aesthetic reasons

#endif
