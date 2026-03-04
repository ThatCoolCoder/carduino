#include <Arduino.h>

#define RPM_ENABLED (TWO_STEP_ENABLED || ROLLING_CUT_ENABLED || GLOBAL_LIMITER_ENABLED)
#define SPARK_CUT_ENABLED (TWO_STEP_ENABLED || ROLLING_CUT_ENABLED || GLOBAL_LIMITER_ENABLED || MANUAL_CUT_ENABLED || NO_LIFT_ENABLED)

#define INPUT_IF(condition, pin) if(condition) pinMode(pin, INPUT_PULLUP)
#define OUTPUT_IF(condition, pin)  if(condition) pinMode(pin, OUTPUT)

#define REQUIRE_ENABLED(condition) if (!condition) return