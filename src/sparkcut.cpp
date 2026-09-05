#include <Arduino.h>

#include "sparkcut.hpp"

#include "config.hpp"
#include "confighelpers.hpp"
#include "limiters.hpp"
#include "limiter_definitions.hpp"
#include "state.hpp"

bool checkManualCut()
{
    if (! MANUAL_CUT_ENABLED) return false;

    // This one returns its result since there is no state to save etc

    return digitalRead(IN_SPARK_CUT_MANUAL) == LOW;
}

void checkNoLift()
{
    if (! NO_LIFT_ENABLED || digitalRead(IN_NO_LIFT_SELECT_SWITCH) == HIGH || two_step_active)
    {
        no_lift_active = false;
        return;
    }


    if (no_lift_active)
    {
        // stay on until clutch is released (means that if driver releases accelerator out of habit, it doesn't start firing)
        if (! clutch_pressed) no_lift_active = false;
    }
    else
    {
        // trigger on
        if (clutch_pressed && millis() - accel_last_pressed < NO_LIFT_ACCEL_TIMEOUT) no_lift_active = true;
    }

}

void check2Step()
{
    REQUIRE_ENABLED(TWO_STEP_ENABLED);

    if (digitalRead(IN_TWO_STEP_ACTIVE) == LOW)
    {
        if (! two_step_active) resetLimiters();
        two_step_active = true;
    }
    // have chosen to make this stay on even if you let off accel,
    // so that you can let off accel if unhappy without starting revinations
    else if (! PEDALS_ENABLED || ! clutch_pressed) two_step_active = false;
}

void manageSparkCut()
{
    REQUIRE_ENABLED(SPARK_CUT_ENABLED);

    bool manual_cut_active = checkManualCut();

    check2Step(); // check first as no lift needs to not activate if 2 step is on
    checkNoLift();

    bool c1 = false;
    bool c2 = false;

    if (GLOBAL_LIMITER_ENABLED && global_limiter_levels[global_limiter_level_idx] > 0)
    {
        delegateLimiter(global_limiter_levels[global_limiter_level_idx],
            &global_limiter_presets[global_limiter_cut_type], &c1, &c2);
    }

    // Prevent other limiter overriding global limiter if that is already active
    if (! (c1 || c2))
    {
        if (manual_cut_active || no_lift_active)
        {
            c1 = true;
            c2 = true;
        }
        else if (two_step_active && two_step_levels[two_step_level_idx] > 0)
        {
            delegateLimiter(two_step_levels[two_step_level_idx],
                &two_step_presets[two_step_cut_type], &c1, &c2);
        }
        else if (rolling_cut_target_rpm > 0)
        {	
            delegateLimiter(rolling_cut_target_rpm,
                &rolling_cut_presets[rolling_cut_type], &c1, &c2);
        }
    }

    if (c1) digitalWrite(OUT_COIL_1_CUT, HIGH);
    else digitalWrite(OUT_COIL_1_CUT, LOW);

    if (c2) digitalWrite(OUT_COIL_2_CUT, HIGH);
    else digitalWrite(OUT_COIL_2_CUT, LOW);


    if (FLASH_LED_ON_CUT_ENABLED)
    {
        digitalWrite(OUT_STATUS_LED, c1 || c2);
    }
}


