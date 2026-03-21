#ifndef CONFIGHELPERS
#define CONFIGHELPERS

#include <Arduino.h>

#define INPUT_IF(condition, pin) if(condition) pinMode(pin, INPUT_PULLUP)
#define OUTPUT_IF(condition, pin)  if(condition) pinMode(pin, OUTPUT)

#define REQUIRE_ENABLED(condition) if (!condition) return


#endif
