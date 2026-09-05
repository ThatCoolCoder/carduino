#include <Arduino.h>

#include "state.hpp"

#include <cppQueue.h>

#include "confighelpers.hpp"

bool unlocked = false;
bool locked_out = false;

bool no_lift_active = false;

int global_limiter_level_idx = 0;
int global_limiter_cut_type = 0;

bool two_step_active = false;
int two_step_level_idx = 0;
int two_step_cut_type = 0;

int rolling_cut_type = 0;
int rolling_cut_target_rpm = 0; // 0 means not active

bool accel_pressed = false;
unsigned long accel_last_pressed = 0;
bool clutch_pressed = false;
unsigned long clutch_last_pressed = 0;

int rpm = 3000;
long rpm_pulse_times[RPM_SMOOTHNESS + 1];
int rpm_pulse_idx = 0;
long last_valid_rpm_time = 0;

unsigned long last_cut_time = 0;
bool hysteresis_cut_active = false;

unsigned long soft_cut_switch_time = 0;
bool soft_cut_coil_2 = false;

cppQueue queue(sizeof(QueuedBlink), 20, FIFO, false);
unsigned long last_blink_started = 0;

void safenOutputs()
{
    // reset the outputs and variables that immediately determine them, but not the config stuff

    resetLimiters();

    if (STATUS_LED_ENABLED) digitalWrite(OUT_STATUS_LED, LOW);
    if (SPARK_CUT_ENABLED) digitalWrite(OUT_COIL_1_CUT, LOW);
    if (SPARK_CUT_ENABLED) digitalWrite(OUT_COIL_2_CUT, LOW);

    no_lift_active = false;
    two_step_active = false;
    rolling_cut_target_rpm = 0;

    accel_pressed = false;
    clutch_pressed = false;
}

void resetLimiters()
{
    last_cut_time = 0;
    hysteresis_cut_active = false;

    soft_cut_switch_time = 0;
    soft_cut_coil_2 = false;
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
    safenOutputs();

    queue.clean();

    two_step_level_idx = 0;
    two_step_cut_type = 0;
    global_limiter_level_idx = 0;
    global_limiter_cut_type = 0;
    rolling_cut_type = 0;

}


