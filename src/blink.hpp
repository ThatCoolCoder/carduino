#ifndef BLINK
#define BLINK

void queueLedBlink(int duration, bool led_on);

void queueBlinkCode(int blinks, int blink_duration, int pause_duration);

void manageLedBlinks();

#endif
