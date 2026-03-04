# Wiring

Give the arduino power and ground obviously, wire arduino to relay board common pins. Todo: give basic description of wiring everything else, it's mostly up to the user how they do it in the car

These are all of the pins used

#define IN_MASTER A0
#define IN_UNLOCK A1
#define IN_STARTER A2
#define IN_RPM_SIGNAL A3
#define IN_TWO_STEP_CUT_MODE A4
#define IN_ROLLING_CUT_MODE A5

#define IN_CLUTCH 2
#define IN_ACCEL 3
#define IN_NO_LIFT_ENABLE_SWITCH 4
#define IN_SPARK_CUT_MANUAL 5
#define IN_TWO_STEP_LEVEL_CYCLE 6
#define IN_TWO_STEP_BUTTON 7
#define IN_ROLLING_CUT_BUTTON 8

#define OUT_HORN 9
#define OUT_FUEL_PUMP 10
#define OUT_COIL_1_CUT 11
#define OUT_COIL_2_CUT 12
#define OUT_STATUS_LED 13

Try to put RPM on an analog input and put less-used switches next to it to avoid noise