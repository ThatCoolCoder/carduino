#include "rpm.hpp"

#include "config.hpp"
#include "confighelpers.hpp"
#include "limiters.hpp"
#include "state.hpp"


void logRpmPulse()
{
    rpm_pulse_count ++;
}

void queryRpm()
{
    unsigned long now = millis(); // save this and reuse when setting last query so we don't end up not counting the cycles taken up by this function

    float rpm_pulse_hz = (float) rpm_pulse_count / (float) (now - last_rpm_query);



    last_rpm_query = now;
    rpm_pulse_count = 0;
}