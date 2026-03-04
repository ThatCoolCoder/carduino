
#include "src/config.hpp"
#include "src/buttons.cpp"
#include "src/state.cpp"
#include "src/blink.cpp"
#include "src/config_helpers.hpp"




void setup()
{
    Serial.begin(9600);

    INPUT_IF(MASTER_SWITCH_ENABLED, IN_MASTER);
    INPUT_IF(SECURITY_ENABLED, IN_UNLOCK);
    INPUT_IF(SECURITY_ENABLED, IN_STARTER);
    INPUT_IF(RPM_ENABLED, IN_RPM_SIGNAL); // TODO: figure how rpm works
    INPUT_IF(GLOBAL_LIMITER_ENABLED, IN_GLOBAL_LIMITER_SETTING);
    INPUT_IF(PEDALS_ENABLED, IN_CLUTCH);
    INPUT_IF(PEDALS_ENABLED, IN_ACCEL);
    INPUT_IF(NO_LIFT_ENABLED, IN_NO_LIFT_ENABLE_SWITCH);
    INPUT_IF(MANUAL_CUT_ENABLED, IN_SPARK_CUT_MANUAL);
    INPUT_IF(TWO_STEP_ENABLED, IN_TWO_STEP_SETTING);
    INPUT_IF(TWO_STEP_ENABLED, IN_TWO_STEP_ACTIVE);
    INPUT_IF(ROLLING_CUT_ENABLED, IN_ROLLING_CUT_ACTIVE);
    INPUT_IF(ROLLING_CUT_ENABLED, IN_ROLLING_CUT_SETTING);

    OUTPUT_IF(SECURITY_ENABLED, OUT_HORN);
    OUTPUT_IF(SECURITY_ENABLED, OUT_FUEL_PUMP);
    OUTPUT_IF(SPARK_CUT_ENABLED, OUT_COIL_1_CUT);
    OUTPUT_IF(SPARK_CUT_ENABLED, OUT_COIL_2_CUT);
    OUTPUT_IF(STATUS_LED_ENABLED, OUT_STATUS_LED);

    resetSecurity();
    resetNonSecurity();

    blinkCode(1, 100, 0);
}


void checkSecurity()
{
    REQUIRE_ENABLED(SECURITY_ENABLED);
    if (unlocked || locked_out) return;

    bool starting = digitalRead(IN_STARTER) == LOW;
    bool unlock_button = digitalRead(IN_UNLOCK) == LOW;

    if (starting && unlock_button)
    {
        unlocked = true;
        digitalWrite(OUT_FUEL_PUMP, HIGH);
        digitalWrite(OUT_HORN, LOW);
    }

    if (starting && ! unlock_button)
    {
        locked_out = true;
        digitalWrite(OUT_FUEL_PUMP, LOW);
        digitalWrite(OUT_HORN, HIGH);
    }
}

void loop()
{
    if (SECURITY_ENABLED && ! unlocked)
    {
        checkSecurity();
        delay(10);
        return;
    }

    if (MASTER_SWITCH_ENABLED && digitalRead(IN_MASTER) == HIGH)
    {
        resetNonSecurity();
        delay(100);
        return;
    }


    if (RPM_ENABLED)
    {
        // todo: read rpm

        if (rpm < MIN_SAFE_RPM || rpm > MAX_SAFE_RPM)
        {
            safen();
            delay(100);
            return;
        }
    }

    readPedals();
    updateButtons();
    manageSparkCut();


    delay(10);
}

void readPedals()
{
    REQUIRE_ENABLED(PEDALS_ENABLED);

    clutch_pressed = digitalRead(IN_CLUTCH) == LOW;
    accel_pressed = digitalRead(IN_ACCEL) == LOW;
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

bool checkManualCut()
{
    if (! MANUAL_CUT_ENABLED) return false;

    // This one returns its result since there is no state to save etc

    return digitalRead(IN_SPARK_CUT_MANUAL) == LOW;
}

void checkNoLift()
{
    if (! NO_LIFT_ENABLED || digitalRead(IN_NO_LIFT_ENABLE_SWITCH) == HIGH)
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

    bool button_pressed = digitalRead(IN_TWO_STEP_BUTTON) == LOW;

    if (button_pressed) two_step_active = true;
    // have chosen to make this stay on even if you let off accel,
    // so that you can let off accel if unhappy without starting revinations
    else if (! clutch_pressed) two_step_active = false;
}

void hardLimiter(int target_rpm, int cut_time, bool* coil_1_cut, bool* coil_2_cut)
{
    // Cut both coils whenever rpm > target, and don't uncut for at least cut_time

    bool in_window = (millis() - last_hard_cut) < cut_time;

    if (rpm > target_rpm)
    {
        if (! in_window) last_hard_cut = millis();

        *coil_1_cut = true;
        *coil_1_cut = true;
    }
    else if (in_window)
    {
        *coil_1_cut = true;
        *coil_1_cut = true;
    }
    else
    {
        *coil_1_cut = false;
        *coil_1_cut = false;
    }
}

void softLimiter(int target_rpm, int soft_cut_region, bool* coil_1_cut, bool* coil_2_cut)
{
    // If is within soft_cut_region below the limit, only cut 1 coil
    // else cut 2 but only as short as needed

    if (rpm > target_rpm)
    {
            *coil_1_cut = true;
            *coil_2_cut = true;
    }
    else if (rpm > target_rpm - soft_cut_region)
    {
        if (soft_cut_was_on_coil_1)
        {
            *coil_1_cut = false;
            *coil_2_cut = true;
        }
        else
        {
            *coil_1_cut = true;
            *coil_2_cut = false;
        }

        soft_cut_was_on_coil_1 = ! soft_cut_was_on_coil_1;
    }
    else
    {
        *coil_1_cut = false;
        *coil_2_cut = false;
    }
}