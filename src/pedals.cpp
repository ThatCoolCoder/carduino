#include <Arduino.h>

#include "pedals.hpp"

#include "config.hpp"
#include "confighelpers.hpp"
#include "state.hpp"

void readPedals()
{
    REQUIRE_ENABLED(PEDALS_ENABLED);

    clutch_pressed = digitalRead(IN_CLUTCH) == LOW;
    accel_pressed = digitalRead(IN_ACCEL) == LOW;
}


