#include <Arduino.h>

#include <cppQueue.h>

#include "blink.hpp"

#include "config.hpp"
#include "confighelpers.hpp"
#include "state.hpp"

void queueLedBlink(int duration, bool led_on)
{
    REQUIRE_ENABLED(STATUS_LED_ENABLED);

    if (queue.isEmpty()) last_blink_started = millis();

    QueuedBlink blink = {duration, led_on};
    queue.push(&blink);
}


void queueBlinkCode(int blinks, int blink_duration, int pause_duration)
{
    REQUIRE_ENABLED(STATUS_LED_ENABLED);

    for (int i = 0; i < blinks; i ++)
    {
        queueLedBlink(blink_duration, true);
        if (i != blinks - 1) queueLedBlink(blink_duration, false);
    }
}

void manageLedBlinks()
{
    REQUIRE_ENABLED(STATUS_LED_ENABLED);

    if (! queue.isEmpty())
    {
        QueuedBlink curr;
        queue.peek(&curr);
        digitalWrite(OUT_STATUS_LED, curr.led_on);

        unsigned long now = millis();

        if (now - last_blink_started > 100)
        {
            // delete and move to next
            queue.pop(&curr);

            if (queue.isEmpty())
            {
                digitalWrite(OUT_STATUS_LED, LOW);
                return;
            }
            last_blink_started = now;

            queue.peek(&curr);
        }
    }

}


