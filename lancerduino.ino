#include <debounce.h>

#include "config.hpp"

#define RPM_ENABLED (TWO_STEP_ENABLED || ROLLING_CUT_ENABLED || GLOBAL_LIMITER_ENABLED)
#define SPARK_CUT_ENABLED (TWO_STEP_ENABLED || ROLLING_CUT_ENABLED || GLOBAL_LIMITER_ENABLED || MANUAL_CUT_ENABLED || NO_LIFT_ENABLED)

#define INPUT_IF(condition, pin) if(condition) pinMode(pin, INPUT_PULLUP)
#define OUTPUT_IF(condition, pin)  if(condition) pinMode(pin, OUTPUT)

#define REQUIRE_ENABLED(condition) if (!condition) return

bool unlocked = false;
bool locked_out = false;

bool no_lift_active = false;
int two_step_level_idx = 0;
bool two_step_active = false;
bool two_step_cut_mode = TWO_STEP_DEFAULT_CUT;
bool rolling_cut_mode = ROLLING_DEFAULT_CUT;
bool rolling_cut_target_rpm = 0; // if 0 means not active

bool accel_pressed = false;
bool clutch_pressed = false;

bool soft_cut_was_on_coil_1 = false;

long last_hard_cut = 0;

int rpm = 3000;

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

void blinkCode(int blinks, int blink_duration, int pause_duration)
{
    REQUIRE_ENABLED(STATUS_LED_ENABLED);

    // since this blocks, we need to safen first just so we don't kill engine if in middle of cut or something lol
    safen();

    for (int i = 0; i < blinks; i ++)
    {
        digitalWrite(OUT_STATUS_LED, HIGH);
        delay(blink_duration);
        digitalWrite(OUT_STATUS_LED, LOW);
        if (i != blinks - 1) delay(pause_duration);
    }
}


void resetSecurity()
{
    REQUIRE_ENABLED(SECURITY_ENABLED);

    unlocked = false;
    digitalWrite(OUT_FUEL_PUMP, LOW);
    digitalWrite(OUT_HORN, LOW);
}

void resetNonSecurity()
{
    safen();

    two_step_level_idx = 0;
    two_step_cut_mode = TWO_STEP_DEFAULT_CUT;
    rolling_cut_mode = ROLLING_DEFAULT_CUT;

    soft_cut_was_on_coil_1 = false;
}

void safen()
{
    // reset the outputs and constantly changing variables, but not the config stuff

    if (STATUS_LED_ENABLED) digitalWrite(OUT_STATUS_LED, LOW);
    if (SPARK_CUT_ENABLED) digitalWrite(OUT_COIL_1_CUT, LOW);
    if (SPARK_CUT_ENABLED) (OUT_COIL_2_CUT, LOW);

    no_lift_active = false;
    two_step_active = false;
    rolling_cut_target_rpm = 0; // if 0 means not active

    accel_pressed = false;
    clutch_pressed = false;

    last_hard_cut = 0;
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

void two_step_level_cycle_handler(uint8_t btnId, uint8_t btnState)
{
    // for some reason all these are 
    if (btnState == BTN_PRESSED)
    {
        two_step_level_idx ++;
        if (two_step_level_idx >= TWO_STEP_LEVEL_COUNT) two_step_level_idx = 0;
        blinkCode(two_step_level_idx + 1, 100, 100);
    }
}

void two_step_cut_mode_handler(uint8_t btnId, uint8_t btnState)
{
    if (btnState == BTN_PRESSED)
    {
        two_step_cut_mode = !two_step_cut_mode;
        blinkCode(two_step_cut_mode + 1, 100, 100);
    }
}

void rolling_cut_mode_handler(uint8_t btnId, uint8_t btnState)
{
    if (btnState == BTN_PRESSED)
    {
        rolling_cut_mode = !rolling_cut_mode;
        blinkCode(rolling_cut_mode + 1, 100, 100);
    }
}

void rolling_cut_handler(uint8_t btnId, uint8_t btnState)
{

    if (btnState == BTN_PRESSED)
    {
        rolling_cut_target_rpm = rpm;
    }
    else
    {
        rolling_cut_target_rpm = 0;
    }
}

Button two_step_level_cycle_button(0, two_step_level_cycle_handler);
Button two_step_cut_mode_button(0, two_step_cut_mode_handler);
Button rolling_cut_mode_button(0, rolling_cut_mode_handler);
Button rolling_cut_button(0, rolling_cut_handler);

void updateButtons()
{
    if (TWO_STEP_ENABLED)
    {
        two_step_level_cycle_button.update(digitalRead(IN_TWO_STEP_LEVEL_CYCLE));
        two_step_cut_mode_button.update(digitalRead(IN_TWO_STEP_CUT_MODE));
    }
    if (ROLLING_CUT_ENABLED)
    {
        rolling_cut_mode_button.update(digitalRead(IN_ROLLING_CUT_MODE));
        rolling_cut_button.update(digitalRead(IN_ROLLING_CUT_BUTTON));
    }
    if (GLOBAL_LIMITER_ENABLED)
    {
        // todo: read global limi button
    }
}
