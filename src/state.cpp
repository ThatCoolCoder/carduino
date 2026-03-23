#include <Arduino.h>

#include "state.hpp"

bool unlocked = false;
bool locked_out = false;

bool no_lift_active = false;

int global_limiter_level_idx = 0;
bool global_limiter_cut_mode = GLOBAL_LIMITER_DEFAULT_CUT;

bool two_step_active = false;
int two_step_level_idx = 0;
bool two_step_cut_mode = TWO_STEP_DEFAULT_CUT;

bool rolling_cut_mode = ROLLING_DEFAULT_CUT;
int rolling_cut_target_rpm = 0; // if 0 means not active

bool accel_pressed = false;
bool clutch_pressed = false;

int rpm = 3000;
int rpm_pulse_count = 0;
unsigned long last_rpm_query = 0;

unsigned long last_hard_cut = 0;
unsigned long last_soft_cut_switch = 0;
bool soft_cut_was_on_coil_1 = false;

void safen()
{
    // reset the outputs and constantly changing variables, but not the config stuff
    // also not rpm cuz rpm determines if is safe

    if (STATUS_LED_ENABLED) digitalWrite(OUT_STATUS_LED, LOW);
    if (SPARK_CUT_ENABLED) digitalWrite(OUT_COIL_1_CUT, LOW);
    if (SPARK_CUT_ENABLED) digitalWrite(OUT_COIL_2_CUT, LOW);

    no_lift_active = false;
    two_step_active = false;
    rolling_cut_target_rpm = 0; // if 0 means not active

    accel_pressed = false;
    clutch_pressed = false;

    last_hard_cut = 0;
    last_soft_cut_switch = 0;
    soft_cut_was_on_coil_1 = false;
}


void resetSecurity()
{
    REQUIRE_ENABLED(SECURITY_ENABLED);

    locked_out = false;

    if (SECURITY_ENABLED == START_UNLOCKED)
    {
        unlocked = true;
        digitalWrite(OUT_FUEL_PUMP, HIGH);
        digitalWrite(OUT_HORN, LOW);
    }
    else
    {
        unlocked = false;
        digitalWrite(OUT_FUEL_PUMP, LOW);
        digitalWrite(OUT_HORN, LOW);
    }
}

void resetNonSecurity()
{

    // use safen then reset everything else - ie config stuff and RPM
    safen();


    rpm_pulse_count = 0;
    last_rpm_query = millis();


    two_step_level_idx = 0;
    two_step_cut_mode = TWO_STEP_DEFAULT_CUT;
    global_limiter_level_idx = 0;
    global_limiter_cut_mode = GLOBAL_LIMITER_DEFAULT_CUT;
    rolling_cut_mode = ROLLING_DEFAULT_CUT;

}


