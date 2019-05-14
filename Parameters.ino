const int MAX_ANGLE = 120;  // we can adjust this for aesthetic reasons
const int MIN_ANGLE = 0;    // we can adjust this for aesthetic reasons
const int RP_N = 50;        // number of values to consider to compute the average for the Rising Pitch calculation
const int PREV_P_AV_N = 3;  // number of previous averages we keep to compare with the current Pitch average
const int PREV_P_AV_COEFF = 0.99; // coefficient of reduction, multiplied by the previous averages in rising Pitch calculation
