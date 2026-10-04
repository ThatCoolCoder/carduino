#include <Arduino.h>

#include "security.hpp"

#include "config.hpp"
#include "confighelpers.hpp"
#include "state.hpp"

void doSecurity()
{
    REQUIRE_ENABLED(SECURITY_ENABLED);
    
    if (locked_out)
    {
        if (LOG_SECURITY_STATUS) Serial.println("security: locked out");

        digitalWrite(OUT_FUEL_PUMP, LOW);
        digitalWrite(OUT_HORN, HIGH);

        return;
    }

    if (unlocked)
    {
        digitalWrite(OUT_FUEL_PUMP, HIGH); // keep this enabled in case relay module has brief power interrupt and forgets fuel pump on
        return;
    }
    else
    {
        Serial.println("security: not unlocked");
    }

    bool starting = digitalRead(IN_STARTER) == LOW;
    bool unlock_button = digitalRead(IN_UNLOCK) == LOW;

    if (starting && unlock_button)
    {
        unlocked = true;
        digitalWrite(OUT_FUEL_PUMP, HIGH);
        digitalWrite(OUT_HORN, LOW);
        if (LOG_SECURITY_STATUS) Serial.println("security: logged in successfully");
    }

    if (starting && ! unlock_button)
    {
        locked_out = true;
    }
}


