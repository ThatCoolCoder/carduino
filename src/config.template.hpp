#ifndef CONFIG
#define CONFIG

#include <Arduino.h>

#include "enums.hpp"
#include "limiter_configs.hpp"


// PINS CONFIG
// -----------

// (set values to 0 for features that aren't being used)

#define IN_MASTER 6
#define IN_UNLOCK A0
#define IN_STARTER A1
#define IN_RPM_SIGNAL 2
#define IN_GLOBAL_LIMITER_SETTING 7
#define IN_ROLLING_CUT_SETTING 11

#define IN_CLUTCH A4
#define IN_ACCEL A5
#define IN_NO_LIFT_SELECT_SWITCH 9
#define IN_SPARK_CUT_MANUAL 0
#define IN_TWO_STEP_SETTING 10
#define IN_TWO_STEP_ACTIVE 8
#define IN_ROLLING_CUT_ACTIVE 12

#define OUT_HORN 5
#define OUT_FUEL_PUMP A3
#define OUT_COIL_1_CUT 3
#define OUT_COIL_2_CUT 4
#define OUT_STATUS_LED 13


// GENERAL SETTINGS
// ----------------

#define LONG_PRESS_DURATION_MS 300 // determiner of long/short press on buttons

#define SECURITY_ENABLED true // can set to START_UNLOCKED if security is wired up but you don't want to use it
#define MASTER_SWITCH_ENABLED true // if false, will act as if master switch is always pressed
#define STATUS_LED_ENABLED true

#define SPARK_CUT_ENABLED true
#define FLASH_LED_ON_CUT_ENABLED true
#define MANUAL_CUT_ENABLED true

#define PEDALS_ENABLED true // whether pedal-related functionality for two-step and no lift is available. optional for two-step, required for no lift
#define NO_LIFT_ENABLED true
#define NO_LIFT_ACCEL_TIMEOUT 500 // allow accelerator to be released for up to x ms before clutch is pressed and still trigger no lift

#define RPM_ENABLED true
#define RPM_SMOOTHNESS 1 // higher values give a less noisy reading but also more latency
#define RPM_PULSES_PER_REVOLUTION 1 // may be either 1, your number of cylinders, or half your number of cylinders (use LOG_RPM to find correct value)
#define RPM_MAX_JUMP 500 // if rpm jumps by more than this much between readings, it becomes invalid and the old "stale" reading remains
#define RPM_MAX_STALE_DURATION_MS 2500 // if the old reading has been stale for this long (due to new ones being out of range), it gives up and uses the new one 

#define RPM_MIN 2000 // sensible/safe rpm range - beyond this, spark cut will be disbled
#define RPM_MAX 10000
#define MAX_CUT_DURATION_MS 3000
#define COUNT_SOFT_CUT_FOR_MAX_CUT_DURATION false


// LOGGING/DEBUG
// -------------

#define LOG_RPM false
#define LOG_CUT_STATUS false
#define LOG_CUT_STATUS_WHEN_NO_CUT false
#define LOG_CUT_REASON false
#define LOG_SECURITY_STATUS false


// LIMITER PARAMETERS
// ------------------

#define GLOBAL_LIMITER_ENABLED true
#define GLOBAL_LIMITER_LEVEL_COUNT 3
const int global_limiter_levels[GLOBAL_LIMITER_LEVEL_COUNT] = {
    4000,
    5500,
    6000,
};
#define GLOBAL_LIMITER_PRESET_COUNT 3
const LimiterPreset global_limiter_presets[GLOBAL_LIMITER_PRESET_COUNT] = {
    HARD_TIME(25, true),
    SOFT_TIME(250, 250, 150, true),
    HARD_TIME(250, true),
};

#define TWO_STEP_ENABLED true
#define TWO_STEP_LEVEL_COUNT 3
const int two_step_levels[TWO_STEP_LEVEL_COUNT] = {
    3000, // 0 means not active
    4000,
    5000,
};
#define TWO_STEP_PRESET_COUNT 4
const LimiterPreset two_step_presets[TWO_STEP_PRESET_COUNT] = {
    HARD_SIMPLE,
    SOFT_SIMPLE(250, 250),
    SOFT_TIME(250, 500, 250, true),
    HARD_TIME(250, true),
};

#define ROLLING_CUT_ENABLED true
#define ROLLING_CUT_PRESET_COUNT 2
const LimiterPreset rolling_cut_presets[ROLLING_CUT_PRESET_COUNT] = {
    SOFT_SIMPLE(250, 250),
    HARD_TIME(10, true)
};


#endif
