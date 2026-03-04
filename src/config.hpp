#pragma once

#include "./softhard.hpp"


// readings outside of this range will be considered nonsense and all spark-cut systems will be disabled
#define MIN_SAFE_RPM 2000
#define MAX_SAFE_RPM 7500

#define MASTER_SWITCH_ENABLED true // if false, will just assume is safe instead of using master switch
#define STATUS_LED_ENABLED true
#define SECURITY_ENABLED true
#define PEDALS_ENABLED true // whether pedal-related functionality for two-step and no lift is available
#define MANUAL_CUT_ENABLED true
#define NO_LIFT_ENABLED true

#define GLOBAL_LIMITER_ENABLED true
#define GLOBAL_LIMITER_DEFAULT_CUT HARD
#define GLOBAL_LIMITER_HARD_CUT_DURATION 500
#define GLOBAL_LIMITER_SOFT_CUT_REGION 300
#define GLOBAL_LIMITER_LEVEL_COUNT 4
const int global_limiter_levels[] = {
    5000,
    6000,
    6500,
    0, // 0 means no limiter
};

#define TWO_STEP_ENABLED true
#define TWO_STEP_DEFAULT_CUT HARD
#define TWO_STEP_HARD_CUT_DURATION 300
#define TWO_STEP_SOFT_CUT_REGION 300
#define TWO_STEP_LEVEL_COUNT 4
const int two_step_levels[] = {
    0, // 0 means not active
    3000,
    4000,
    5000
};

#define ROLLING_CUT_ENABLED true
#define ROLLING_DEFAULT_CUT SOFT
#define ROLLING_HARD_CUT_DURATION 300
#define ROLLING_SOFT_CUT_REGION 300



// Pins configuration
// Stuff that isn't ENABLED above doesn't need to be wired or configured below
// Use common sense to determine what pins are needed for what function
#define IN_MASTER A0
#define IN_UNLOCK A1
#define IN_STARTER A2
#define IN_RPM_SIGNAL A3
#define IN_GLOBAL_LIMITER_SETTING A4
#define IN_ROLLING_CUT_SETTING A5

#define IN_CLUTCH 2
#define IN_ACCEL 3
#define IN_NO_LIFT_ENABLE_SWITCH 4
#define IN_SPARK_CUT_MANUAL 5
#define IN_TWO_STEP_SETTING 6
#define IN_TWO_STEP_ACTIVE 7
#define IN_ROLLING_CUT_ACTIVE 8

#define OUT_HORN 9
#define OUT_FUEL_PUMP 10
#define OUT_COIL_1_CUT 11
#define OUT_COIL_2_CUT 12
#define OUT_STATUS_LED 13