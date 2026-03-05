#include <Arduino.h>

#include "blink.hpp"

#include "config.hpp"
#include "confighelpers.hpp"
#include "state.hpp"


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


