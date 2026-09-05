#include <Arduino.h>

#include "pedals.hpp"

#include "config.hpp"
#include "confighelpers.hpp"
#include "state.hpp"

void readPedals()
{
    REQUIRE_ENABLED(PEDALS_ENABLED);

    clutch_pressed = digitalRead(IN_CLUTCH) == LOW;
    if (clutch_pressed) clutch_last_pressed = millis();
    accel_pressed = digitalRead(IN_ACCEL) == LOW;
    if (accel_pressed) accel_last_pressed = millis();
}


