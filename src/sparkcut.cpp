#include <Arduino.h>

#include "config.hpp"
#include "confighelpers.hpp"
#include "limiters.cpp"
#include "state.cpp"

bool checkManualCut()
{
    if (! MANUAL_CUT_ENABLED) return false;

    // This one returns its result since there is no state to save etc

    return digitalRead(IN_SPARK_CUT_MANUAL) == LOW;
}

void checkNoLift()
{
    if (! NO_LIFT_ENABLED || digitalRead(IN_NO_LIFT_SELECT_SWITCH) == HIGH)
    {
        no_lift_active = false;
        return;
    }


    if (no_lift_active)
    {
        // stay on until clutch is released (means that if accel is instinctively released, doesn't drive)
        if (! clutch_pressed) no_lift_active = false;
    }
    else
    {
        // trigger on
        if (clutch_pressed && accel_pressed) no_lift_active = true;
    }

}

void check2Step()
{
    REQUIRE_ENABLED(TWO_STEP_ENABLED);

    bool button_pressed = digitalRead(IN_TWO_STEP_ACTIVE) == LOW;

    if (button_pressed) two_step_active = true;
    // have chosen to make this stay on even if you let off accel,
    // so that you can let off accel if unhappy without starting revinations
    else if (! clutch_pressed) two_step_active = false;
}

void manageSparkCut()
{
    REQUIRE_ENABLED(SPARK_CUT_ENABLED);

    bool manual_cut_active = checkManualCut();

    checkNoLift();
    check2Step();

    bool coil_1_cut = false;
    bool coil_2_cut = false;

    if (manual_cut_active || no_lift_active)
    {
        coil_1_cut = true;
        coil_2_cut = true;
    }
    else if (two_step_active)
    {
        if (two_step_cut_mode == HARD) hardLimiter(two_step_levels[two_step_level_idx], TWO_STEP_HARD_CUT_DURATION, &coil_1_cut, &coil_2_cut);
        else softLimiter(two_step_levels[two_step_level_idx], TWO_STEP_SOFT_CUT_REGION, &coil_1_cut, &coil_2_cut);
    }
    else if (rolling_cut_target_rpm > 0)
    {
        if (rolling_cut_mode == HARD) hardLimiter(rolling_cut_target_rpm, ROLLING_HARD_CUT_DURATION, &coil_1_cut, &coil_2_cut);
        else softLimiter(rolling_cut_target_rpm, ROLLING_SOFT_CUT_REGION, &coil_1_cut, &coil_2_cut);
    }

    if (coil_1_cut) digitalWrite(OUT_COIL_1_CUT, HIGH);
    else digitalWrite(OUT_COIL_1_CUT, LOW);

    if (coil_2_cut) digitalWrite(OUT_COIL_2_CUT, HIGH);
    else digitalWrite(OUT_COIL_2_CUT, LOW);
}