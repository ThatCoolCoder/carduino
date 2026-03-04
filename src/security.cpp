#include <Arduino.h>

#include "config.hpp"
#include "confighelpers.hpp"
#include "state.cpp"


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