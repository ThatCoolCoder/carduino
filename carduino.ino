#include "src/config.hpp"

#include "src/blink.hpp"
#include "src/buttons.hpp"
#include "src/confighelpers.hpp"
#include "src/pedals.hpp"
#include "src/security.hpp"
#include "src/sparkcut.hpp"
#include "src/state.hpp"

String s;

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

    blinkCode(2, 200, 50);
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