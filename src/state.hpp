#ifndef STATE
#define STATE

#include <cppQueue.h>

#include "config.hpp"

extern bool unlocked;
extern bool locked_out;

extern bool no_lift_active;

extern int global_limiter_level_idx;
extern int global_limiter_cut_type;

extern bool two_step_active;
extern int two_step_level_idx;
extern int two_step_cut_type;

extern int rolling_cut_type;
extern int rolling_cut_target_rpm; // if 0 means not active

extern bool accel_pressed;
extern unsigned long accel_last_pressed;
extern bool clutch_pressed;
extern unsigned long clutch_last_pressed;

extern int rpm;
extern long rpm_pulse_times[RPM_SMOOTHNESS + 1];
extern int rpm_pulse_idx;
extern long last_valid_rpm_time;

extern unsigned long last_cut_time;
extern bool hysteresis_cut_active;

extern unsigned long soft_cut_switch_time;
extern bool soft_cut_coil_2;

typedef struct QueuedBlink
{
    int duration;
    bool led_on;
} QueuedBlink;

extern cppQueue queue;
extern unsigned long last_blink_started;

void safenOutputs();

void resetLimiters();

void resetNonSecurity();

void resetSecurity();

#endif
