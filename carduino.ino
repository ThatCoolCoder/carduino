#include "src/config.hpp"

#include "src/blink.hpp"
#include "src/buttons.hpp"
#include "src/confighelpers.hpp"
#include "src/pedals.hpp"
#include "src/rpm.hpp"
#include "src/security.hpp"
#include "src/sparkcut.hpp"
#include "src/state.hpp"

void setup()
{
    Serial.begin(9600);

    INPUT_IF(MASTER_SWITCH_ENABLED, IN_MASTER);
    INPUT_IF(SECURITY_ENABLED, IN_UNLOCK);
    INPUT_IF(SECURITY_ENABLED, IN_STARTER);
    INPUT_IF(RPM_ENABLED, IN_RPM_SIGNAL);
    if (RPM_ENABLED) attachInterrupt(digitalPinToInterrupt(IN_RPM_SIGNAL), logRpmPulse, FALLING);
    INPUT_IF(GLOBAL_LIMITER_ENABLED, IN_GLOBAL_LIMITER_SETTING);
    INPUT_IF(PEDALS_ENABLED, IN_CLUTCH);
    INPUT_IF(PEDALS_ENABLED, IN_ACCEL);
    INPUT_IF(NO_LIFT_ENABLED, IN_NO_LIFT_SELECT_SWITCH);
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

    buttonConfig();
    resetSecurity();
    resetNonSecurity();

    queueBlinkCode(2, 200, 50);
    // todo: manual blink code and make pre-security slower loop

    Serial.println("Booted!");

}

void loop()
{
    manageLedBlinks();

    delay(5); // has to be here so that led blink has precedence over spark cut flash
    // should really just move ALL led control to the blink module so it can prioritise as needed

    if (SECURITY_ENABLED)
    {
        doSecurity();
        if (! unlocked)
        {
            delay(10);
            return;
        }
    }

    if (MASTER_SWITCH_ENABLED && digitalRead(IN_MASTER) == HIGH)
    {
        resetNonSecurity();
        delay(10);
        return;
    }


    readPedals();
    updateButtons();

    if (RPM_ENABLED)
    {
        updateRpm();

        if (rpm < MIN_ACTIVE_RPM || rpm > MAX_ACTIVE_RPM)
        {
            safenOutputs();
            return;
        }
    }

    manageSparkCut();
}
