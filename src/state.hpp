#ifndef STATE
#define STATE


#include "config.hpp"
#include "confighelpers.hpp"

extern bool unlocked;
extern bool locked_out;

extern bool no_lift_active;

extern int global_limiter_level_idx;
extern bool global_limiter_cut_mode;

extern bool two_step_active;
extern int two_step_level_idx;
extern bool two_step_cut_mode;

extern bool rolling_cut_mode;
extern int rolling_cut_target_rpm; // if 0 means not active

extern bool accel_pressed;
extern bool clutch_pressed;

extern int rpm;
extern int rpm_pulse_count;
extern unsigned long last_rpm_query;

extern unsigned long last_hard_cut;
extern unsigned long last_soft_cut_switch;
extern bool soft_cut_was_on_coil_1;

void safen();

void resetNonSecurity();

void resetSecurity();

#endif
