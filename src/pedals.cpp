#include <Arduino.h>

#include "config.hpp"
#include "confighelpers.hpp"
#include "state.cpp"

void readPedals()
{
    REQUIRE_ENABLED(PEDALS_ENABLED);

    clutch_pressed = digitalRead(IN_CLUTCH) == LOW;
    accel_pressed = digitalRead(IN_ACCEL) == LOW;
}
